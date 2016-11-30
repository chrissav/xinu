/* ex5.c - main, prod2, cons2 */

#include <xinu.h>

void print_list();
void run_callouts();
void init_calloutq();
void callout_print(print_args);
uint32 add_callout(uint32, void *, void *);
uint32 remove_callout(uint32);
void callout_process();

struct print_args {
  uint32 num1;
  uint32 num2;
};

struct callout {
  uint32  time;
  void    (*funcaddr)(void *);
  void    *argp;
  uint32  cid;
  struct  callout *qnext;    /* Index of next callout or tail  */
  struct  callout *qprev;    /* Index of previous callout or head  */
};

struct callout *cahead = NULL;
struct callout *catail = NULL;
uint32 cid = 0;

/* extern struct callout cqueue[]; */

/*------------------------------------------------------------------------
* Main - Create processes
*------------------------------------------------------------------------
*/

process main(void) {
  printf("\nstarting main");
  //init_calloutq();

  //resume(create(callout_process, 1024, 20, "run_callouts", 0));

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

  add_callout(15, callout_print, &a);
  add_callout(8, callout_print, &b);
  add_callout(10, callout_print, &c);
  add_callout(4, callout_print, &d);

  resume(create(print_list, 1024, 20, "print_list", 0));
}


/*------------------------------------------------------------------------
* Test function and wrapper to print statements
*------------------------------------------------------------------------
*/

void callout_process() {
  while(1){
    sleep(2);
    printf("\nrunning callout");
    run_callouts();
  }
}

void callout_print(struct print_args *argp) {
  printf("num1 is %d, num2 is %d", argp->num1, argp->num2);
}

/*------------------------------------------------------------------------
* Callout functions
*------------------------------------------------------------------------
*/

// void init_calloutq(){
//   printf("\ncreating queue");
//   cahead->qnext = catail;
//   cahead->qprev = NULL;
//   catail->qprev = cahead;
//   catail->qnext = NULL;
//   printf("\nfinished creating queue");
// }

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

  /* add data to callout node */
  link.time = msdelay;
  link.funcaddr = funcaddr;
  link.argp = argp;
  link.cid = cid++;
  sleep(1);

  printf("\n update pointers. ");
  if (current == NULL){
    printf("current is null");
    cahead = &link; /* set link as first node */
  } else {
    printf("current is not null");
    printf("link time is %d", link.time);
    printf("current time is %d", current->time);
    while (current->qnext != NULL && link.time <= current->time) { /* find where to add callout */
      printf("\n incrementing current to next node");
      current = current->qnext;
      printf("\n updating link time");
      link.time -= current->time;
    }
    if (current->qnext != NULL){ /* not the last item in the list */
      printf("\n update pointers for next item");
      struct callout *next = current->qnext;
      link.qnext = next;
      next->qprev = &link;
    }
    printf("update remaining pointers");
    current->qnext = &link;
    link.qprev = current;
  }
  return OK;
}

uint32 remove_callout(uint32 cid) {
  struct callout* current = cahead;

  if(cahead->qnext == catail) {
    return SYSERR; /* no items in the list */
  }

  while(cid != current->cid) {
    if (current->qnext != catail) { /* check if it's the last msg */
      current = current->qnext;
    } else {
      return SYSERR; /* cid was not found in the callout queue */
    }
  }

  /* remove the callout node */
  struct callout *next = current->qnext; /* Following node in list */
  struct callout *prev = current->qprev; /* Previous node in list  */
  prev->qnext = next;
  next->qprev = prev;
  return OK;
}

void run_callouts(){
  struct callout* current = cahead;

  if(current->qnext != catail) { /* if calloutq is not empty */
    current = current->qnext;
    sleep(current->time);
    current->funcaddr(current->argp); /* call the function waiting */
    remove_callout(current->cid);
  }
}
