#ifndef LIB
#define LIB

#define ACK "ACK"
#define NAK "NAK"

typedef struct {
    int len;
    char payload[1400];
} msg;

void init(char* remote, int remote_port);
void set_local_port(int port);
void set_remote(char* ip, int port);

int send_message(const msg* m);
int recv_message(msg* r);
msg* receive_message_timeout(int timeout); //timeout in milliseconds
unsigned short crc16_ccitt(const void *buf, int len);

msg make_S(char seq);
msg make_F(char seq, char *name);
msg make_Z(char seq);
msg make_B(char seq);
msg make_YY(char seq, char mark, msg isS);
msg make_N(char seq, char mark);
msg make_D(char seq, char *data, int len);
void print_msg(msg t);
int send_my_message(msg t);

#endif

