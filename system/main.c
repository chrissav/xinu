/* ex5.c - main, prod2, cons2 */

#include <xinu.h>

void print_list();
void run_callouts();
void init_calloutq();
void callout_print(print_args);
uint32 add_callout(uint32, void *, void *);
uint32 remove_callout(uint32);
void callout_process();

/* struct for test function */
struct print_args {
  uint32 num1;
  uint32 num2;
};

/* struct for callout items to be queued */
struct callout {
  uint32  time;
  void    (*funcaddr)(void *);
  void    *argp;
  uint32  cid;
  struct  callout *qnext;    /* Index of next callout or tail  */
  struct  callout *qprev;    /* Index of previous callout or head  */
};

struct callout *cahead = NULL; /* head of the callout queue */
uint32 cid = 0; /* keep track of cid used */

/*------------------------------------------------------------------------
* Main - Create processes
*------------------------------------------------------------------------
*/

process main(void) {
  printf("\nstarting main");

  resume(create(callout_process, 1024, 20, "run_callouts", 0));

  /* create tests */
  struct print_args a;
  a.num1 = 1;
  a.num2 = 2;

  struct print_args b;
  b.num1 = 5;
  b.num2 = 6;

  struct print_args c;
  c.num1 = 9;
  c.num2 = 10;

  struct print_args d;
  d.num1 = 15;
  d.num2 = 16;

  void (*func)(struct print_args *argp);
  func = &callout_print;

  add_callout(15, func, &a);
  add_callout(8, callout_print, &b);
  add_callout(10, callout_print, &c);
  add_callout(4, callout_print, &d);

  /* run process to print current list */
  resume(create(print_list, 1024, 20, "print_list", 0));
}


/*------------------------------------------------------------------------
* Test function and wrapper to print statements
*------------------------------------------------------------------------
*/

void callout_process() {
  while(1){
    sleep(2); /* wait a little for output */
    printf("\nrunning callout");
    run_callouts();
  }
}

/*------------------------------------------------------------------------
* Callout functions
*------------------------------------------------------------------------
*/

void callout_print(struct print_args *argp) {
  printf("\nRunning the print func");
  printf("\nnum1 is %d, num2 is %d", argp->num1, argp->num2);
}

//print the callout list
void print_list() {
  while(1){
    sleep(1);
    struct callout *ptr = cahead;
    printf("\n[ ");

    //start from the beginning
    while(ptr != NULL) {
      printf("(%d,%d) ",ptr->cid,ptr->time);
      ptr = ptr->qnext;
    }

    printf(" ]");
  }
}

uint32 add_callout(uint32 msdelay, void *funcaddr, void *argp) {
  printf("\n\n adding callout");
  struct callout link;
  struct callout *current = cahead;
  struct print_args *pri = (struct print_args *) argp;

  /* add data to callout node */
  link.time = msdelay;
  link.funcaddr = funcaddr;
  link.argp = pri;
  link.cid = cid++;

  /* add link to the queue */
  if (current == NULL){
    cahead = &link; /* set link as first node */
  } else {
    while (current->qnext != NULL && link.time <= current->time) { /* find where to add callout */
      current = current->qnext;
      link.time -= current->time;
    }
    if (current->qnext != NULL){ /* not the last item in the list */
      struct callout *next = current->qnext;
      link.qnext = next;
      next->qprev = &link;
    }
    current->qnext = &link;
    link.qprev = current;
  }
  return OK;
}

uint32 remove_callout(uint32 cid) {
  printf("\n\nremoving callout");
  struct callout *current = cahead;

  if(current == NULL) {
    return SYSERR; /* no items in the list */
  }

  while(cid != current->cid) {
    if (current->qnext != NULL) { /* check if it's the last msg */
      current = current->qnext;
    } else {
      return SYSERR; /* cid was not found in the callout queue */
    }
  }

  /* remove the found callout node */
  struct callout *next = current->qnext; /* Following node in list */
  struct callout *prev = current->qprev; /* Previous node in list  */
  prev->qnext = next;
  next->qprev = prev;
  return OK;
}

void run_callouts(){
  struct callout *current = cahead;
  if(current != NULL) { /* if calloutq is not empty */
    sleep(current->time); /* sleep for function delay of first item */
    current->funcaddr(current->argp); /* call the function waiting */
    remove_callout(current->cid);
  }
}
