#include "base.h"

/*  Functia parseaza fisierul dns_servers.conf pentru a obtine
 *  adresele serverelor dns
 */
int serversOfDns(char msg[100][15]) {
  FILE *in = fopen("dns_servers.conf", "r");
  char aux[MAXL];
  int pos = 0;

  while(fgets(aux, MAXL, in) > 0) {
    if(aux[0] != '#') {
      aux[strlen(aux)-1] = '\0';
      strcpy(msg[pos++], aux);
    }
  }

  return pos;
}

/*  Realizeaza headerul pentru query-ul facut la serverul dns  */
dns_header_t makeHeader() {
  dns_header_t header;
  header.id = (unsigned short) htons(getpid());

  header.rd = 1;
  header.tc = 0;
  header.aa = 0;
  header.opcode = 0;
  header.qr = 0;

  header.rcode = 0;
  header.z = 0;
  header.ra = 0;

  header.qdcount = htons(1);
  header.ancount = 0;
  header.nscount = 0;
  header.arcount = 0;

  return header;
}

/*  Realizeaza intrebarea pentru serverul dns  */
dns_question_t makeQuestion(int type, char *name) {
  dns_question_t question;
  question.qname = strdup(name);
  question.qtype = (htons(type));
  question.qclass = htons(1);
  return question;
}

/*  Transforma in format DNS ex:
    www.google.com -> 0x03,www,0x06,google,0x03,com  */
void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host) {
    int ind = 0 , i;
    strcat((char*)host,".");

    for(i = 0 ; i < strlen((char*)host) ; i++)
    {
        if(host[i]=='.')
        {
            *dns++ = i-ind;
            for(;ind<i;ind++)
            {
                *dns++=host[ind];
            }
            ind=i+1;
        }
    }
    *dns++='\0';
}

/*  Transforma in format de afisat fara sa elimine punctul de la final ex:
    0x03,www,0x06,google,0x03,com -> www.google.com. */
void ChangetoHumanNameFormat(char *name) {
  int i, j;
  unsigned int p;
  for(i=0;i<(int)strlen((const char*)name);i++)
    {
        p=name[i];
        for(j=0;j<(int)p;j++)
        {
            name[i]=name[i+1];
            i=i+1;
        }
        name[i]='.';
    }
}

void makeMessage(char *buf, dns_header_t h, dns_question_t q) {

  /*  Pentru header  */
  memcpy(buf, &h.id, sizeof(short));
  buf[2] = h.qr<<7 | h.opcode<<3 | h.aa<<2 | h.tc<<1 | h.rd;
  buf[3] = h.ra<<7 | h.z<<4 | h.rcode;
  memcpy(buf+4, &h.qdcount, sizeof(short));
  memcpy(buf+6, &h.ancount, sizeof(short));
  memcpy(buf+8, &h.nscount, sizeof(short));
  memcpy(buf+10, &h.arcount, sizeof(short));

  /*  Transforma numele in format DNS  */
  char *name = (char*)calloc(strlen(q.qname)+1, sizeof(char));
  ChangetoDnsNameFormat(name, q.qname);

  /*  Pentru question  */
  strcpy(buf+12, name);
  int aux = strlen(q.qname);
  memcpy(buf + aux + 13, &q.qtype, sizeof(short));
  memcpy(buf + aux + 15, &q.qclass, sizeof(short));
}

dns_header_t remakeHeader(char *buf) {
  dns_header_t h;
  memcpy(&h.id, buf, sizeof(short));
  h.id = ntohs(h.id);
  h.qr = buf[2] >> 7;
  h.opcode = buf[2] >> 3;
  h.aa = buf[2] >> 2;
  h.tc = buf[2] >> 1;
  h.rd = buf[2];
  h.ra = buf[3] >> 7;
  h.z = buf[3] >> 4;
  h.rcode = buf[3]<<4>>4;

  memcpy(&h.qdcount, buf+4, sizeof(short));
    h.qdcount = ntohs(h.qdcount);
  memcpy(&h.ancount, buf+6, sizeof(short));
  h.ancount = ntohs(h.ancount);
  memcpy(&h.nscount, buf+8, sizeof(short));
  h.nscount = ntohs(h.nscount);
  memcpy(&h.arcount, buf+10, sizeof(short));
  h.arcount = ntohs(h.arcount);

  return h;
}

/*  Parseaza un nume folosind ppointerul (0xc0)  */
/*  Usage e folosit pentru raspunsuri care intorc mai multe nume (SOA)  */
char *makeName(int a, char *msg, int usage) {
  char *name = (char*)calloc(MAXL, sizeof(char));
  int i;
  unsigned char c;
  int pos = 0;
  int ind;
  int times = 0;

  if(usage == 1) {
    ind = a+1;
  } else {
    ind = msg[a+1];
  }

  while(1) {
    c = msg[ind];
    if(c == 0x00) {
      name[pos] = 0x00;
      break;
    } else if(c == 0xc0) {
      ind = msg[ind+1];
    } else {
      name[pos++] = msg[ind++];
    }
  }

  if(usage == 1) {
    for(ind = 0; ind < MAXL; ind++) {
      if(name[ind] == ' ') {
        ind--;
        break;
      }
    }
    ChangetoHumanNameFormat(name + ind);
  }
  ChangetoHumanNameFormat(name);

  return name;
}

dns_rr_t* makeResponse(char *msg, int res) {
  dns_rr_t *r = (dns_rr_t*)malloc(100 * sizeof(dns_rr_t));
  int i, j;
  int total = 0;
  unsigned char c, viit;
  char name[MAXL];

  for(i = 0; i < MAXLEN; i++) {
      if(total == res) {
        break;
      }
      c = msg[i];
      viit = msg[i+2];

      /*
       *  -Cazul cand nu este un pointer pentru a termina numele.
       *  -Pointerul 0xc0 poate indica offsetul de unde se gaseste numele
       *   sau offsetul de unde se continua numele; aici se trateaza doar
       *   primul caz.
       */
      if (c == 0xc0 && viit != 0xc0) {

        /*  Determin numele  */
        strcpy(name, makeName(i, msg, 0));
        r[total].name = strdup(name);

        /*  Copiez restul informatiilor din pachetul RR  */
        memcpy(&r[total].types, msg + i + 2, sizeof(short));
        r[total].types = ntohs(r[total].types);

        memcpy(&r[total].clas, msg + i + 4, sizeof(short));
        r[total].clas = ntohs(r[total].clas);

        memcpy(&r[total].ttl, msg + i + 6, sizeof(int));
        r[total].ttl = ntohl(r[total].ttl);

        memcpy(&r[total].rdlength, msg + i + 10, sizeof(short));
        r[total].rdlength = ntohs(r[total].rdlength);

        /*  Daca raspunsul este de tip A atunci intoarce un IP  */
        if(r[total].types == A) {
          memset(name, 0, MAXL);
          int aux;
          memcpy(&aux, msg + i + H_LEN, sizeof(int));
          unsigned char *p = (unsigned char*) &aux;
          sprintf(name, "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
          r[total].rdata = strdup(name);

        /*  Daca raspunsul este de tip SOA atunci intoarce un PriName  */
        } else if(r[total].types == SOA) {

          /*  Copiez numele  */
          memset(name, 0, MAXL);
          strcpy(name, makeName(i + H_LEN - 1, msg, 1));

          /*  aux_sec_name spune ca a fost salvat si al doilea string (AuthMailbox)
           *  si poate extrage si restul datelor
           */
          int aux_sec_name = 0;

          /*  Parcurg pentru a gasi AuthMailbox, urmat de restul datelor.  */
          for(j = 0; j < MAXLEN; j++) {
            c = msg[i + H_LEN + j];
            /*  Daca a fost gasit AuthMailbox  */
            if(c == 0xc0 && aux_sec_name == 0) {
              strcat(name, " ");
              strcat(name, makeName(i + H_LEN + 1 + j, msg, 1));
              aux_sec_name++;

            /*  Din cauza compresiei se va gasi si in AuthMailbox un pointer 0xc0  */
            /*  Extrag restul datelor  */
          } else if(c == 0xc0 || msg[i + H_LEN + j + 1] == 0x00 && aux_sec_name == 1) {
              int serial, refresh, retry, expiration, minimum;
              memcpy(&serial, msg + i + j + H_LEN + 2, sizeof(int));
              serial = ntohl(serial);
              memcpy(&refresh, msg + i + j + H_LEN + 6, sizeof(int));
              refresh = ntohl(refresh);
              memcpy(&retry, msg + i + j + H_LEN + 10, sizeof(int));
              retry = ntohl(retry);
              memcpy(&expiration, msg + i + j + H_LEN + 14, sizeof(int));
              expiration = ntohl(expiration);
              memcpy(&minimum, msg + i + j + H_LEN + 18, sizeof(int));
              minimum = ntohl(minimum);

              char aux[100];
              sprintf(aux, " %d %d %d %d %d", serial, refresh, retry, expiration, minimum);
              strcat(name, aux);

              /*  Parasesc for-ul pentru a realiza raspunsul SOA  */
              break;
            }
          }

          /*  Datele sunt adaugate in sectiunea data  */
          r[total].rdata = strdup(name);

          /*  Daca este de oricare alt tip  */
        } else if(r[total].types == MX){
          memset(name, 0, MAXL);
          short pref;
          memcpy(&pref, msg + i + 12, sizeof(short));
          pref = ntohs(pref);
          sprintf(name, "%d ", pref);
          strcat(name, makeName(i + 11 + sizeof(short), msg, 1));
          r[total].rdata = strdup(name);
        } else {
          memset(name, 0, MAXL);
          strcpy(name, makeName(i + 11, msg, 1));
          r[total].rdata = strdup(name);
        }

        /*  Contorizez totalul mesajelor  */
        total++;
      }
  }

  /*  Intorc toate mesajele  */
  return r;
}
