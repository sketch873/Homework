#include "base.h"

int main(int argc, char **argv) {

  /*  Pentru conexiunea udp  */
  struct sockaddr_in servaddr;
  int servlen = sizeof(struct sockaddr_in);
  int sockfd;
  fd_set read;
  int msg_len;

  /*  Parti din mesajul de trimis  */
  dns_header_t header;
  dns_question_t question;
  dns_rr_t *response;
  int responses = 0;

  /*  Numele de cautat si tipul de mesaj  */
  char qmsg[MAXL];
  int mes_type;
  struct timeval timeout;

  /*  Servere de dns  */
  char servere_dns[100][15];
  int nr_servers;

  char msg[MAXLEN];
  int i;

  /*  Verificare numarul de argumente  */
  if(argc != 3) {
    perror("Numar gresit de argumente!");
    return -1;
  }

  /*  Verificare existenta servere DNS  */
  nr_servers = serversOfDns(servere_dns);
  if(nr_servers == 0) {
    perror("Nu sunt servere DNS in fisier!");
    return -1;
  }

  /*  Deschidere socket udp  */
  if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
      perror("Eroare la  creare socket!");
      return -1;
  }

  /*  Verificare ca primul argument e IP  */
  /*  Daca primesc un IP adaug '.in-addr.arpa'  */
  strcpy(qmsg, argv[1]);
  if (inet_aton(argv[1], & servaddr.sin_addr) < 0) {
    //intorc mesajul
    // char p[4][4];
    // int pos = 0, ind = 0;
    // for(i = 0; i < strlen(qmsg); i++) {
    //   if(qmsg[i] == '.') {
    //     p[pos][ind] = '\0';
    //     ind = 0;
    //     pos++;
    //   } else {
    //     p[pos][ind++] = qmsg[i];
    //   }
    //   p[pos][ind] = '\0';
    // }
    sprintf(qmsg, "%s.%s.%s.%s", p[3], p[2], p[1], p[0]);
    // int addr = inet_addr(qmsg);
    // sprintf(qmsg, "%d", addr);
    //copiez
    // printf("ADRESA - %s\n", qmsg);
    strcat(qmsg, ".in-addr.arpa");
  }

  /*  Realizez headerul  */
  header = makeHeader();

  /*  Fac campul question pentru fiecare tip de mesaj  */
  if(strcmp(argv[2], "A") == 0) {
    mes_type = A;
    question = makeQuestion(A, qmsg);
  } else if (strcmp(argv[2], "NS") == 0) {
    mes_type = NS;
    question = makeQuestion(NS, qmsg);
  } else if (strcmp(argv[2], "CNAME") == 0) {
    mes_type = CNAME;
    question = makeQuestion(CNAME, qmsg);
  } else if (strcmp(argv[2], "MX") == 0) {
    mes_type = MX;
    question = makeQuestion(MX, qmsg);
  } else if (strcmp(argv[2], "SOA") == 0) {
    mes_type = SOA;
    question = makeQuestion(SOA, qmsg);
  } else if (strcmp(argv[2], "TXT") == 0) {
    mes_type = TXT;
    question = makeQuestion(TXT, qmsg);
  } else if(strcmp(argv[2], "PTR") == 0) {
    mes_type = PTR;
    question = makeQuestion(PTR, qmsg);
  }else {
    perror("Al doilea argument invalid!");
    return -1;
}
  /*  Afisez in 'message.log' mesajul trimis  */
  printMessageLog(header, question);

  /*  Leg 'header' de 'question' si trimit mesajul catre server  */
  makeMessage(msg , header, question);

  /*  Parcurg toate serverele dns pana gasesc unul care sa raspunda  */
  for(i = 0; i < nr_servers; i++) {
    timeout.tv_sec = SECS;
    FD_SET(sockfd, &read);

    /*  Fac destinatia cu noua adresa a serverului  */
    memset( & servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(53);
    servaddr.sin_addr.s_addr = inet_addr(servere_dns[i]);

    /*  Trimit pachet la un nou server dns  */
    sendto(sockfd, msg, H_LEN + Q_LEN + strlen(question.qname), 0,
          (struct sockaddr*) &servaddr, sizeof(servaddr));

    select(sockfd+1, &read, NULL, NULL, &timeout);

    /*  Daca vine un pachet in mai putin de SECS secunde atunci
     *  parsez, afisez si termin executia.
     */
    if(FD_ISSET(sockfd, &read)) {
      memset(msg, 0, MAXLEN);
      recvfrom(sockfd, msg, MAXLEN, 0,
            (struct sockaddr*) &servaddr, &servlen);

      /*  Refac headerul  */
      header = remakeHeader(msg);

      /*  Parsez toate raspunsurile  */
      response = makeResponse(msg, header.ancount + header.nscount + header.arcount);

      printf(";; QUERY: %d, ANSWER: %d, AUTHORITY: %d, ADDITIONAL: %d\n\n",
          header.qdcount, header.ancount, header.nscount, header.arcount);

      /*  Printez raspunsul primit de la server la stdin si in 'dns.log'  */
      printResponse(servere_dns[i], argv[1], argv[2], header, response);

      break;
    }
  } //endfor

  close(sockfd);
  return 0;
}
