#ifndef CLIENT_H
#define CLIENT_H

extern dlink_list global_client_list;
extern dlink_list global_server_list;

#define FLAGS_PINGSENT      0x00000001UL /* Unreplied ping sent*/
#define FLAGS_DEADSOCKET    0x00000002UL /* Local socket is dead--Exiting soon*/
#define FLAGS_KILLED        0x00000004UL /* Prevents "QUIT" from being sent to this */
#define FLAGS_CLOSING       0x00000008UL /* set when closing */
#define FLAGS_CONNECTING    0x00000010UL /* Currently connecting not bursted */

#define STAT_SERVER         0x01
#define STAT_CLIENT         0x02
#define STAT_ME             0x04

#define UMODE_OPER          0x1

#define IsConnecting(x)         ((x)->flags & FLAGS_CONNECTING)
#define IsDefunct(x)            ((x)->flags & (FLAGS_DEADSOCKET|FLAGS_CLOSING))
#define IsDead(x)               ((x)->flags & FLAGS_DEADSOCKET)

#define SetConnecting(x)        ((x)->flags|= FLAGS_CONNECTING)

#define ClearConnecting(x)      ((x)->flags &= ~FLAGS_CONNECTING)

#define IsServer(x)             ((x)->status == STAT_SERVER)
#define IsClient(x)             ((x)->status == STAT_CLIENT)
#define IsMe(x)                 ((x)->status == STAT_ME)

#define SetServer(x)            ((x)->status |= STAT_SERVER)
#define SetClient(x)            ((x)->status |= STAT_CLIENT)

#define IsOper(x)               ((x)->umodes & UMODE_OPER)

#define SetOper(x)              ((x)->umodes |= UMODE_OPER)

#define ClearOper(x)            ((x)->umodes &= ~UMODE_OPER)


#define IDLEN           12 /* this is the maximum length, not the actual
                              generated length; DO NOT CHANGE! */

#define MODE_QUERY  0
#define MODE_ADD    1
#define MODE_DEL   -1

struct Server
{
  dlink_node node;
  fde_t fd;
  int flags;
  struct dbuf_queue buf_recvq;
  struct dbuf_queue buf_sendq;
  char pass[20];
};

struct Client
{
  dlink_node node;
  dlink_list channel;

  struct Client *hnext;         /* For client hash table lookups by name */
  struct Client *idhnext;       /* For SID hash table lookups by sid */
  struct Client *from;

  char          name[HOSTLEN+1];
  char          host[HOSTLEN+1];
  char          id[IDLEN + 1];      /* client ID, unique ID per client */
  char          info[REALLEN + 1];  /* Free form additional client info */
  char          username[USERLEN + 1];
    
  struct Server      *server;

  time_t        tsinfo;
  unsigned int  status;
  unsigned int  umodes;
  unsigned short hopcount;
  unsigned char handler;        /* Handler index */

  int flags;
} Client;

void init_client();
struct Client *make_client(struct Client*);
struct Server *make_server(struct Client*);
struct Client *find_person(const struct Client *, const char *);
struct Client *find_chasing(struct Client *, const char *, int *);
void dead_link_on_write(struct Client *, int);

#endif
