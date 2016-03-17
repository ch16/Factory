/* name: 		Hong Chen
 * ONE Card number: 	1386152
 * Unix id: 		chen1
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "factory.h"
#include "assemblers.c"
#include "packer.c"

int err;
void *tret1;
void *tret2;

// initiate an assembly line array of products and a struct of all infomation
struct product *assembly_line;
struct infomation *info;

// initiate the mutex lock and the conditional wait
pthread_mutex_t the_mutex, print_mutex,colour_mutex;
pthread_cond_t condc, condp;


/** @brief Get the infomation from command line
 *         and create according numbers of assembler 
 *         and packer threads.
 *  @param argc Argument count, number of arguments.
 *  @param argv[] Argumen vector.
 *  @return int.
 */
int main(int argc, char const *argv[]) {
  if (argc != 6){
    printf("Wrong number of arguments!\n");
    exit(1);
  }	

  info = (struct infomation *)malloc(sizeof(struct infomation));

  info->a = atoi(argv[1]); // the number of assembler threads
  info->p = atoi(argv[2]); // the number of packer threads
  info->l = atoi(argv[3]); // the size of the assembly line
  info->n = atoi(argv[4]); // the number of products per box
  info->c = atoi(argv[5]); // the number of products each assembler will produce
  info->in = 0; // the position on the assembly line that assembler should put products on
  info->out = 0; // the position on the assembly line that packers should get products from
  info->packed = 0; // the number of total products packed by the packers
  
  assembly_line = (struct product *)malloc((size_t)info->l*sizeof(struct product)); 

  pthread_mutex_init(&the_mutex, NULL);
  pthread_mutex_init(&print_mutex, NULL);
  pthread_cond_init(&condc, NULL);
  pthread_cond_init(&condp, NULL);

  int i;
  int c[info->a]; // array of numbers for storing the colour index of each thread

  // create number of a assembler threads
  pthread_t assembler_thread[info->a];
  for (i = 0; i < info->a; i++){
    c[i]=i;
    //printf("Enter thread %d\n",i);
    err = pthread_create(&assembler_thread[i], NULL, assembler_function, &c[i]);
    if (err != 0){
	perror("can’t create assembler thread");
    	exit(1);
    }
  }

  // create number of p packer threads
  pthread_t packer_thread[info->p];
  for (i = 0; i < info->p; i++){
    err = pthread_create(&packer_thread[i], NULL, packer_function, NULL);
    if (err != 0){
	perror("can’t create packer thread");
	exit(1);
    }
  }

  // join all the assembler threads
  for (i = 0; i < info->a; i++){
    err = pthread_join(assembler_thread[i], &tret1);
    if (err != 0){
	perror("can’t join assembler thread");
    	exit(1);
    }
  }

  // join all the packer threads
  for (i = 0; i < info->p; i++){
    err = pthread_join(packer_thread[i], &tret2);
    if (err != 0){
	perror("can’t join packer thread");
    	exit(1);
    }
  }

  free(assembly_line);
  free(info);

  // destroy the mutex locks and conditional waits
  pthread_mutex_destroy(&the_mutex);
  pthread_mutex_destroy(&print_mutex);
  pthread_cond_destroy(&condc);
  pthread_cond_destroy(&condp);
  return 0;
}

