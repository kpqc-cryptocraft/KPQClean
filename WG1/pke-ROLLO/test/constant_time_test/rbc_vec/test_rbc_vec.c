/*
 * Constant-time test
 */

#include <stdlib.h>
#include "rbc.h"
#include "poison.h"
#include "random_source.h"


/*
 * Variables
 */

int INSTANCES = 10; 
int SIZE;   


rbc_vec a;

/*
 * Initialization and destruction
 */
void test_setup() {
  random_source prng;
  random_init(&prng, RANDOM_SOURCE_PRNG);
  
  // RBC initialization
  rbc_field_init();
  rbc_vec_init(&a,SIZE);

  rbc_vec_set_random(&prng,a,SIZE);
  
}

void test_clear(){
  rbc_vec_clear(a);
}


/*
 * Test functions
 */



void test_gauss() {
 
  poison(a,sizeof(rbc_elt)*SIZE);
  rbc_vec_gauss(a, SIZE, NULL, 0);
}


/*
 * Test execution
 */
void test_suite() {
  test_setup();
  test_gauss();
  test_clear();
}

int main() {
  for(int i = 0 ; i < INSTANCES ; ++i) {
    test_suite();
  }
}

