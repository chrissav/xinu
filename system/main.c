/* ex5.c - main, prod2, cons2 */

#include <xinu.h>

void proc1();
void proc2();
void proc3();


/*------------------------------------------------------------------------
* Main - Create processes
*------------------------------------------------------------------------
*/

process main(void) {
  resume(create(proc1, 1024, 20, "procone", 0));
  resume(create(proc2, 1024, 20, "proctwo", 0));
  resume(create(proc3, 1024, 20, "procthree", 0));
}


/*------------------------------------------------------------------------
* processes to print first come first serve
*------------------------------------------------------------------------
*/
void proc1() {
  printf("\nStarting proc one\n");
  uint16 i = 0;
  for (i=0; i<=10; i++) {
    printf("Printing for process one.\n");
  }
}

void proc2() {
  printf("\nStarting proc one\n");
  uint16 i = 0;
  for (i=0; i<=10; i++) {
    printf("Printing for process two.\n");
  }
}

void proc3() {
  printf("\nStarting proc one\n");
  uint16 i = 0;
  for (i=0; i<=10; i++) {
    printf("Printing for process three.\n");
  }
}
