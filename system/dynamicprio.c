/* dynamicprio.c - evaluates current process priority every second */

#include <xinu.h>

void dynamicPrio() {
  /* created a single proctab struct and utilized chprio function
  * to update the priority. */
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
    /* reset all process cpuhog to 0 */
    p->cpuhog = 0;
  }
  resched();
}
