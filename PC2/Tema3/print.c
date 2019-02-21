#include "base.h"

/*  Printez in 'message.log' mesajul trimis catre server  */
void printMessageLog(dns_header_t h, dns_question_t q) {
  FILE *out = fopen("message.log", "ra+");
  int i;

  /*  Ma duc la sfarsitul fisierului  */
  // fseek(out, 0, SEEK_END);

  /*  Printez campul 'header' din mesaj  */
  fprintf(out, "%04x %02x %02x %02x %02x %02x %02x %02x %02x %04x %04x %04x %04x",
    h.id, h.rd, h.tc, h.aa, h.opcode, h.qr, h.rcode,
    h.z, h.ra, h.qdcount, h.ancount, h.nscount, h.arcount);

  /*  Fac numele in format pentru DNS  */
  char *dns = calloc(MAXL, sizeof(char));
  char *host = strdup(q.qname);
  ChangetoDnsNameFormat(dns, host);

  /*  Printez numele  */
  for(i = 0; i < strlen(q.qname); i++) {
    fprintf(out, " %02x", q.qname[i]);
  }

  /*  Termin de printat si campul 'question'  */
  fprintf(out, " %04x %04x\n", q.qtype, q.qclass);

}

/*  Printez un camp din response la stdin si in 'dns.log', in functie de tip  */
void printType (FILE *out, dns_rr_t r) {
  if(r.types == A) {
    printf("%s IN A %s\n", r.name, r.rdata);
    fprintf(out, "%s IN A %s\n", r.name, r.rdata);
  } else if(r.types == NS) {
    printf("%s IN NS %s\n", r.name, r.rdata);
    fprintf(out, "%s IN NS %s\n", r.name, r.rdata);
  } else if(r.types == SOA) {
    printf("%s IN SOA %s\n", r.name, r.rdata);
    fprintf(out, "%s IN SOA %s\n", r.name, r.rdata);
  } else if(r.types == MX) {
    printf("%s IN MX %s\n", r.name, r.rdata);
    fprintf(out, "%s IN MX %s\n", r.name, r.rdata);
  } else if(r.types == TXT) {
    printf("%s IN TXT %s\n", r.name, r.rdata);
    fprintf(out, "%s IN TXT %s\n", r.name, r.rdata);
  } else if(r.types == CNAME) {
    printf("%s IN CNAME %s\n", r.name, r.rdata);
    fprintf(out, "%s IN CNAME %s\n", r.name, r.rdata);
  } else if (r.types == PTR){
    printf("%s IN PTR %s\n", r.name, r.rdata);
    fprintf(out, "%s IN PTR %s\n", r.name, r.rdata);
  } else {
    printf("R.types diferit %s\n", r.name);
  }
}

/*  Printez toate raspunsurile de la server la stdin si in 'dns.log'  */
void printResponse(char *server, char *name, char *type, dns_header_t h, dns_rr_t *r) {
  FILE *out = fopen("dns.log", "ra+");
  int i;
  fseek(out, 0, SEEK_END);

  fprintf(out, "; %s - %s %s\n\n", server, name, type);

  /*  Daca am primit ANSWER  */
  if(h.ancount > 0) {
    printf(";; ANSWER SECTION:\n");
    fprintf(out, ";; ANSWER SECTION:\n");
    for(i = 0; i < h.ancount; i++) {
      printType(out, r[i]);
    }
    printf("\n");
    fprintf(out, "\n");
  }

  /*  Daca am primit AUTHORITY  */
  if(h.nscount > 0) {
    printf(";; AUTHORITY SECTION:\n");
    fprintf(out, ";; AUTHORITY SECTION:\n");

    for(i = h.ancount; i < h.ancount + h.nscount; i++) {
      printType(out, r[i]);
    }
    printf("\n");
    fprintf(out, "\n");
  }

  /*  Daca am primit ADDITIONAL  */
  if(h.arcount > 0) {
    printf(";; ADDITIONAL SECTION:\n");
    fprintf(out, ";; ADDITIONAL SECTION:\n");

    for(i = h.ancount + h.nscount; i < h.ancount + h.nscount + h.arcount; i++) {
      printType(out, r[i]);
    }
    printf("\n");
    fprintf(out, "\n");
  }

  fclose(out);
}
