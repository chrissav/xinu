/* ex5.c - main, prod2, cons2 */

#include <xinu.h>

void high_prio();
void low_prio();
void med_prio();
void printproc();

int32 SetMalleable(pid32);
int32 ClearMalleable(pid32);


/*------------------------------------------------------------------------
* main - Producer and consumer processes synchronized with semaphores
*------------------------------------------------------------------------
*/

process main(void) {
  /* better to print this in clkhandler to see what's going on */
  /* resume(create(printproc, 1024, 201, "printproc", 0)); */
  resume(create(low_prio, 1024, 20, "low", 0));
  resume(create(med_prio, 1024, 100, "med", 0));
  resume(create(high_prio, 1024, 200, "high", 0));
}

/*------------------------------------------------------------------------
* print the process table in a 1 second loop
*------------------------------------------------------------------------
*/
void printproc() {
  while (1) {
    dumpProctab(0);
    sleep(1);
  }
}

/*------------------------------------------------------------------------
* low_prio - lowest priority process
*------------------------------------------------------------------------
*/
void low_prio() {
  kprintf("starting low_prio\n");
  SetMalleable( getpid() );
  int32 i = 0;
  while(1){
    i++;
  }

}

/*------------------------------------------------------------------------
* med_prio - medium priority process
*------------------------------------------------------------------------
*/
void med_prio() {
  kprintf("starting med_prio");
  SetMalleable( getpid() );
  int32 i = 0;
  while(1){
    i++;
  }

}

/*------------------------------------------------------------------------
* high_prio - highest priority process
*------------------------------------------------------------------------
*/
void high_prio() {
  kprintf("starting high_prio");
  SetMalleable( getpid() );
  int32 i = 0;
  while(1){
    i++;
  }
}

/*------------------------------------------------------------------------
* Set the malleable flag on a process
*------------------------------------------------------------------------
*/
int32 SetMalleable(pid32 pid) {
  if (isbadpid(pid)) {
    return SYSERR;
  }

  struct procent *p;
  p = &proctab[pid];
  p->malleable = 1;
  return 1;
}

/*------------------------------------------------------------------------
* Clear the malleable flag on a process
*------------------------------------------------------------------------
*/
int32 ClearMalleable(pid32 pid) {
  if (isbadpid(pid)) {
    return SYSERR;
  }

  struct procent *p;
  p = &proctab[pid];
  p->malleable = 0;
  return 1;

}
