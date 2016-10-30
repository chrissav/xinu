/* clkhandler.c - clkhandler */

#include <xinu.h>

/*-----------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *-----------------------------------------------------------------------
 */
void	clkhandler()
{

	static uint32 count1000 = 1000;	/* variable to count 1000ms */
	volatile struct am335x_timer1ms *csrptr =
			(struct am335x_timer1ms *)0x44E31000;
			/* Set csrptr to address of timer CSR	    */

	/* If there is no interrupt, return */

	if((csrptr->tisr & AM335X_TIMER1MS_TISR_OVF_IT_FLAG) == 0) {
		return;
	}

	/* Acknowledge the interrupt */

	csrptr->tisr = AM335X_TIMER1MS_TISR_OVF_IT_FLAG;

	/* Decrement 1000ms counter */

	count1000--;

  /* print the proctab twice a second */

	if(count1000 == 300) {
		dumpProctab(0);
	}

	if(count1000 == 700) {
		dumpProctab(0);
	}

	/* After 1 sec, increment clktime */

	if(count1000 == 0) {
		clktime++;
		count1000 = 1000;

		/* Compute the proctab priorities */
		dynamicPrio();
	}

	/* check if sleep queue is empty */

	if(!isempty(sleepq)) {

		/* sleepq nonempty, decrement the key of */
		/* topmost process on sleepq		 */

		if((--queuetab[firstid(sleepq)].qkey) == 0) {

			wakeup();
		}
	}

	/* Decrement the preemption counter */
	/* Reschedule if necessary	    */

	if((--preempt) == 0) {
		preempt = QUANTUM;

		/* process reached the end of it's time slice */
		struct procent *p;
		p = &proctab[currpid];
		if (p->cpuhog == 1) {
			p->cpuhog = 2;
		}
		if (p->cpuhog == 0) {
			p->cpuhog = 1;
		}

		resched();
	}
}
