#include "stdinc.h"

dlink_list global_client_list;
dlink_list global_server_list;

static BlockHeap *client_heap  = NULL;

void
init_client()
{
  client_heap = BlockHeapCreate("client", sizeof(struct Client), CLIENT_HEAP_SIZE);
}

struct Client *
make_client(struct Client *from)
{
  struct Client *client = BlockHeapAlloc(client_heap);

  if(from == NULL)
    client->from = client;
  else
    client->from = from;

  client->hnext  = client;
  strlcpy(client->username, "unknown", sizeof(client->username));

  return client;
}

struct Server *
make_server(struct Client *client)
{
  if(client->server == NULL)
    client->server = MyMalloc(sizeof(struct Server));

  return client->server;
}

/* find_person()
 *
 * inputs - pointer to name
 * output - return client pointer
 * side effects - find person by (nick)name
 */
struct Client *
find_person(const struct Client *source, const char *name)
{
  struct Client *target = NULL;

  if(IsDigit(*name) && IsServer(source->from))
    target = hash_find_id(name);
  else
    target = find_client(name);

  return(target && IsClient(target)) ? target : NULL;
}

/*
 * find_chasing - find the client structure for a nick name (user)
 *      using history mechanism if necessary. If the client is not found,
 *      an error message (NO SUCH NICK) is generated. If the client was found
 *      through the history, chasing will be 1 and otherwise 0.
 */
struct Client *
find_chasing(struct Client *source_p, const char *user, int *chasing)
{
  struct Client *who = find_person(source_p, user);

  if (chasing)
    *chasing = 0;

  if (who)
    return who;

  if (IsDigit(*user))
    return NULL;

  return NULL;
}


/*
 * dead_link_on_write - report a write error if not already dead,
 *      mark it as dead then exit it
 */
void
dead_link_on_write(struct Client *client, int ierrno)
{
  if (IsDefunct(client->server))
    return;

  dbuf_clear(&client->server->buf_recvq);
  dbuf_clear(&client->server->buf_sendq);
}

