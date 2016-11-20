/* ex5.c - main, prod2, cons2 */

#include <xinu.h>

void sendmsg(int32, umsg32, uint16);
void sendlongmsg(int32, umsg32, uint16);
void recv(int32, uint16);


/*------------------------------------------------------------------------
* Main - Create processes
*------------------------------------------------------------------------
*/

process main(void) {
  printf("starting main");
  ptinit(100);
  int32 portnum = ptcreate(30);
  int32 portnum2 = ptcreate(30);
  int32 portnum3 = ptcreate(30);
  resume(create(sendmsg, 1024, 20, "senda", 3, portnum, (umsg32) "This is tagged with A", (uint16) 'A'));
  resume(create(sendmsg, 1024, 20, "senda2", 3, portnum, (umsg32) "This is tagged with A2", (uint16) 'A'));
  resume(create(sendmsg, 1024, 20, "senda3", 3, portnum, (umsg32) "This is tagged with A3", (uint16) 'A'));

  resume(create(sendmsg, 1024, 20, "sendb", 3, portnum, (umsg32) "This is tagged with B", (uint16) 'B'));
  resume(create(sendmsg, 1024, 20, "sendb2", 3, portnum, (umsg32) "This is tagged with B2", (uint16) 'B'));
  resume(create(sendmsg, 1024, 20, "sendb3", 3, portnum, (umsg32) "This is tagged with B3", (uint16) 'B'));

  resume(create(recv, 1024, 20, "recva", 2, portnum, (uint16) 'A'));
  resume(create(recv, 1024, 20, "recva2", 2, portnum, (uint16) 'A'));
  resume(create(recv, 1024, 20, "recva3", 2, portnum, (uint16) 'A'));
  resume(create(recv, 1024, 20, "recvb", 2, portnum, (uint16) 'B'));
  resume(create(recv, 1024, 20, "recvany", 2, portnum, (uint16) 0));

  resume(create(sendmsg, 1024, 20, "senda4", 3, portnum, (umsg32) "This is tagged with A4wait", (uint16) 'A'));
}


/*------------------------------------------------------------------------
* Send/Receive messages
*------------------------------------------------------------------------
*/

void sendmsg(int32 portnum, umsg32 msg, uint16 tag) {
  ptsend(portnum, msg, tag);
}

void recv(int32 portnum, uint16 tag) {
  sleep(1);
  char *msg;
  msg = (char *) ptrecv(portnum, tag);
  printf("\n%s", msg);
}
