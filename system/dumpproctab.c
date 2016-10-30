/* dumpproctab.c - dumps the process table */

#include <xinu.h>

char *prssb[] = {
  "FREE ",
  "CURR ",
  "READY ",
  "RECV ",
  "SLEEP ",
  "SUSP ",
  "WAIT ",
  "RECTIM",
};

void dumpProctab(int32 freeflag) {
  int32 i;
  struct procent *p;
  kprintf("Process table dump:\n\n");
  kprintf("Num\tState\tPrio\tSem\tName\tMalleable\tCPUhog\n");
  kprintf("----------------------------------");
  kprintf("----------------------------------\n");
  for (i = 0; i < NPROC; i++) {
    p = &proctab[i];
    if (freeflag || (p->prstate != PR_FREE)) {
      kprintf("%d\t%6s\t%d\t%d\t%s\t%d\t%d\n", i, prssb[p->prstate],
      p->prprio, p->prsem, p->prname, p->malleable, p->cpuhog);
    }
  }
}
