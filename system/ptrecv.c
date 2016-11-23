/* ptrecv.c - ptrecv */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  ptrecv  -  Receive a message from a port, blocking if port empty
 *------------------------------------------------------------------------
 */
uint32	ptrecv(
	  int32		portid,		/* ID of port to use		*/
		uint16 tag 				/* tag for message */
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	ptentry	*ptptr;		/* Pointer to table entry	*/
	int32	seq;			/* Local copy of sequence num.	*/
	umsg32	msg;			/* Message to return		*/
	struct	ptnode	*msgnode;	/* First node on message list	*/

	mask = disable();
	if ( isbadport(portid) ||
	     (ptptr= &porttab[portid])->ptstate != PT_ALLOC ) {
		restore(mask);
		return (uint32)SYSERR;
	}

	/* Wait for message and verify that the port is still allocated */

	seq = ptptr->ptseq;		/* Record orignal sequence	*/
	if (wait(ptptr->ptrsem) == SYSERR || ptptr->ptstate != PT_ALLOC
	    || ptptr->ptseq != seq) {
		restore(mask);
		return (uint32)SYSERR;
	}

	/* Dequeue first message that is waiting in the port */
	/* Walk the message list and look for tag number */

	msgnode = ptptr->pthead;
	while (!(tag == 0 || tag == msgnode->pttag)) {
		if (msgnode->ptnext != ptptr->pttail) { /* check if it's the last msg */
			msgnode = msgnode->ptnext;
		} else { /* msg was not found */
			/* create new semaphore */
			sid32 sem = semcreate(0);
			/* enqueue semaphore into this tag's queue, and wait */
			enqueue(sem, tag);
			wait(sem);
			/* delete semaphore when process returns (won't use it again) */
			semdelete(sem);
			/* start at the beginning of the message list */
			msgnode = ptptr->pthead;
		}
	}
	msg = msgnode->ptmsg;
	if (ptptr->pthead == ptptr->pttail)	/* Delete last item	*/
		ptptr->pthead = ptptr->pttail = NULL;
	else {
		ptptr->pthead = msgnode->ptnext;
	}
	msgnode->ptnext = ptfree;		/* Return to free list	*/
	ptfree = msgnode;
	signal(ptptr->ptssem);
	restore(mask);
	return msg;
}
