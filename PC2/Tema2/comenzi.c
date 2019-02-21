#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server.h"
#include "erori.h"

/*  Cazul pentru login  */
int login(int n, cont* conturi, int card, int pin, int fd) {
  int i, j;

  /*  Verifica sa nu fie clientul deja logat  */
  for(i = 0; i < n; i++) {
    if(conturi[i].sockfd == fd) {
      return -6;
    }

  /*  Verifica sa nu fie blocat de un alt client  */
    for(j = 0; j < 100; j++) {
      if(conturi[i].incercari[j] == 3)
		  return -5;
    }
  }

  /* Verifica :
   *    - sa nu fie cineva logat deja
   *    - daca pinul este gresit
   *    - daca este cardul bun
   *    - daca e cardul bun intoarce 1 altfel intoarce 'card inexistent'
   */
  for(int i = 0; i < n; i++) {
    if (conturi[i].nr_card == card && conturi[i].sockfd != -1) {
      return -2;
    } else if (conturi[i].nr_card == card && conturi[i].pin != pin) {
      conturi[i].incercari[fd]++;
      if(conturi[i].incercari[fd] == 3) {
        return -5;
      } else {
        return -3;
      }
    } else if (conturi[i].nr_card == card && conturi[i].pin == pin) {
      conturi[i].sockfd = fd;
      conturi[i].incercari[fd] = 0;
      return 1;
    } else if (conturi[i].nr_card == card) {
      return -3;
    }
  }

  return -4;
}

/*  Gaseste contul pe care este logat clientul si il elibereaza  */
int logout(int n, cont* conturi, int fd) {
  int i;
  for(i = 0; i < n; i++) {
    if(conturi[i].sockfd == fd) {
      conturi[i].sockfd = -1;
      return 1;
    }
  }
  return -1;
}

/*  Gaseste contul pe care este logat clientul si afiseaza soldul  */
int listsold(int n, cont* conturi, int fd) {
  int i;
  for(i = 0; i < n; i++) {
    if(conturi[i].sockfd == fd)
      return 1;
  }

  return -1;
}

/*  Verifica cele trei cazuri <**> si intoarce 1 pentru reusita
 *  <**> nu este autentificat, card pentru transfer inexistent si
 *  fonduri insuficiente
 */
int transfer(int n, cont* conturi, int fd, int card, double sum) {
  int i;
  int valid_fd = 0;
  int valid_suma = 0;
  int valid_card = 0;
  for(i = 0; i < n; i++) {
    if(conturi[i].sockfd == fd) {
      valid_fd = 1;
    }
    if(conturi[i].sockfd == fd && conturi[i].sold - sum > 0.01) {
      valid_suma = 1;
    }

    if(conturi[i].nr_card == card) {
      valid_card = 1;
    }
  }

  if(valid_fd == 0)
    return -1;
  if(valid_suma == 0)
    return -8;
  if(valid_card == 0)
    return -4;


  return 1;
}

/*  Functie generala care ia un mesaj si aplica modificarile pe vectorul de conturi  */
char *take_message(int nr_conturi, cont* conturi, int fd, char* message) {
  char command[10];
  char buf[100];
  int card, pin;
  double sum;
  int i;

  /*  Verific prima instructiune, comanda data catre server  */
  strcpy(command, strtok(message, " "));

  /*  Daca se vrea login atunci parsez si apelez login  */
  if(strstr(command, "login") != NULL) {
    card = atoi(strtok(NULL," "));
    pin = atoi(strtok(NULL, " "));

    int n = login(nr_conturi, conturi, card, pin, fd);
    switch(n) {
      case -2: return ERR2;
      case -3: return ERR3;
      case -4: return ERR4;
      case -5: return ERR5;
      case -6: return ERR6;
      case 1:
        for(i = 0; i < nr_conturi; i++) {
          if(conturi[i].sockfd == fd) {
            strcpy(buf, "IBANK> Welcome ");
            strcat(buf, conturi[i].nume);
            strcat(buf, " ");
            strcat(buf, conturi[i].prenume);
            return strdup(buf);
          }
        }
    }

  /*  Daca se vrea logout atunci este eliminat  */
  } else if (strstr(command, "logout") != NULL) {
    int n = logout(nr_conturi, conturi, fd);
    if(n < 0) {
      return ERR1;
    } else {
      return DISCONNECTED;
    }

    /*  Daca se vrea listsold atunci se afiseaza soldul  */
  } else if (strstr(command, "listsold") != NULL) {
    int n = listsold(nr_conturi, conturi, fd);
    if(n < 0) {
      return ERR1;
    } else {
      for(i = 0; i < nr_conturi; i++) {
        if(conturi[i].sockfd == fd) {
          strcpy(buf, "IBANK> ");
          char aux[10];
          sprintf(aux, "%0.2lf", conturi[i].sold);
          strcat(buf, aux);
          return strdup(buf);
        }
      }
    }

  /*  Daca se vrea transfer atunci parsez si apelez transfer  */
  } else if (strstr(command, "transfer") != NULL) {
    char aux[10];
    card = atoi(strtok(NULL, " "));
    sum = atof(strtok(NULL, " "));
    int n = transfer(nr_conturi, conturi, fd, card, sum);
    switch(n) {
      case -1 : return ERR1;
      case -4 : return ERR4;
      case -8 : return ERR8;
      case 1 :


        strcpy(buf, "IBANK> Transfer ");
        sprintf(aux, "%0.2f", sum);
        strcat(buf, aux);
        strcat(buf, " catre ");

        for(i = 0; i < nr_conturi; i++) {
          if(conturi[i].nr_card == card) {
            strcat(buf, conturi[i].nume);
            strcat(buf, " ");
            strcat(buf, conturi[i].prenume);
          }
        }
        strcat(buf, "? [y/n]");
        send(fd, buf, strlen(buf), 0);

        /*  Daca este un mesaj diferit de 'y' se anuleaza tranzactia  */
        memset(buf, 0, 100);
        recv(fd, buf, sizeof(buf), 0);
        if(strstr(buf, "y") == NULL || strlen(buf) != 1)
          return ERR9;

        /*  Realizeaza transferul  */
        for(i = 0; i < nr_conturi; i++) {
          if(conturi[i].sockfd == fd)
            conturi[i].sold -= sum;
          if(conturi[i].nr_card == card)
            conturi[i].sold += sum;
        }

        return TRANSFER;
      } // end switch

  /*  Cazul pentru quit - am presupus ca face automat logout  */
  } else if (strstr(command, "quit") != NULL) {
      int i;
      for(i = 0; i < nr_conturi; i++) {
        if(conturi[i].sockfd == fd)
          conturi[i].sockfd = -1;
      }
      return "Deconectat";
  } else {
    return ERR6;
  }
} // end take_message

/*  Cazul in care se vrea unlock  */
char* unlock(int fd, char* buf, struct sockaddr_in client_addr,
  struct sockaddr_in udp_addr, cont* conturi, int n) {
  char buffer[BUFLEN];
  int i, j;
  char *p;
  int card;
  int len = sizeof(udp_addr);
  int existenta_card = 0;

  p = strtok(buf, " ");
  card = atoi(strtok(NULL, " "));

  strcpy(buffer, SECRET_PSSWD);

  /*  Cer parola secreta  */
  sendto(fd, buffer, strlen(buffer), 0, (struct sockaddr *) &client_addr, sizeof(udp_addr));
  memset(buffer, 0, BUFLEN);
  recvfrom(fd, buffer, BUFLEN, 0, (struct sockaddr*) &client_addr, &len);

  /*  Verific existenta cardului  */
  for(i = 0; i < n; i++) {
    if(conturi[i].nr_card == card)
      existenta_card = 1;
  }
  if(existenta_card == 0)
    return ERR4;

  /*  Daca am gasit cardul si parola secreta e buna atunci resetez vectorul de incercari  */
  for(i = 0; i < n; i++) {
    if(conturi[i].nr_card == card && strcmp(conturi[i].parola_secreta, buffer) == 0) {
      for(j = 0; j < 100; j++) {
        conturi[i].incercari[j] = 0;
      }
      return UNLOCKED;
    } else if (conturi[i].nr_card == card) {
      return ERR7;
    }
  }
}
