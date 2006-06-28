#ifndef SEND_H
#define SEND_H

void send_queued_write(struct Client *);
void send_queued_all(void);
void sendto_server(struct Client *, const char *, ...);

#define ALL_MEMBERS  0
#define NON_CHANOPS  1
#define ONLY_CHANOPS_VOICED  2
#define ONLY_CHANOPS 3
#define ONLY_SERVERS 4 /* for channel_mode.c */

#endif
