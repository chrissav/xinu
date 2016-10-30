/* dynamicprio.c - evaluates current process priority every second */

#include <xinu.h>

void dynamicPrio() {
  /* dumping the proc tab here allows you to see which process
  * is CURR running in the proctab printout, otherwise
  * this could be printed in main.c to view the priorities
  * while the current running process would always be main */
  /*dumpProctab(0); */

  /* created a single proctab struct and utilized chprio function
  * to update the priority. Saves on time and space */
  struct procent *p;

  int32 i = 0;
  for (i = 0; i < NPROC; i++) {
    p = &proctab[i];
    pri16 newprio = p->prprio;
    /* only processes that have malleable set will change priority */
    if (p->malleable == 1) {
      /* Processes that have computed to the end of their timeslice during the past second should
      have their priorities divided by 2. */
      if (p->cpuhog == 2){
        newprio /= 2;
      }
      /* Processes that have received no CPU service should have their priority incremented (to a
      max of 255). */
      if (p->cpuhog == 0){
        newprio += 1;
      }
      chprio(i, newprio);
    }

  }
  resched();
}
