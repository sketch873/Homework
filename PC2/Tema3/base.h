#ifndef __BASE
#define __BASE

/*  Input / Output  */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <ctype.h>

/*  Conexiune  */
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <errno.h>
#include <arpa/inet.h>

/*  Structurile pachetelor  */
#include "struct.h"

/*  Dimensiune buffer nume si buffer mesaj trimis/primit la serverul DNS  */
#define MAXL 256
#define MAXLEN 65536

/*  Numar de secunde in care pot primi un raspuns de la server  */
#define SECS 2

/*  Dimensiunile campurilor header si question fara variabilele alocate dinamic  */
#define H_LEN 12
#define Q_LEN 5

/*  print.c  */
void printMessageLog(dns_header_t h, dns_question_t q);
void printType (FILE *out, dns_rr_t r);
void printResponse(char *server, char *name, char *type, dns_header_t h, dns_rr_t *r);

/*  auxiliare.c  */
int serversOfDns(char msg[100][15]);
dns_header_t makeHeader();
dns_question_t makeQuestion(int type, char *name);
void ChangetoDnsNameFormat(unsigned char* dns,unsigned char* host);
void ChangetoHumanNameFormat(char *name);
void makeMessage(char *buf, dns_header_t h, dns_question_t q);
dns_header_t remakeHeader(char *buf);
char *makeName(int a, char *msg, int usage);
dns_rr_t* makeResponse(char *msg, int res);

#endif
