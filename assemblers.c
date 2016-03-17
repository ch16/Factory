/* name: 		Hong Chen
 * ONE Card number: 	1386152
 * Unix id: 		chen1
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


/** @brief Generate number of c products, while wait
 *         if the assembly line is full or while other 
 *         assemblers are producing.
 *  @param ptr Pointer to the argument. In this function,
 *	   it's the index of colour for this assemler to produce.
 *  @return void.
 */
void *assembler_function(void *ptr){

  int i = 0;
  
  // the index of colour the current assembler should produce the product with
  int *c;
  c = ptr;

  // if the current assembler has not finished c products, then keep producing
  while(i < info->c){

    pthread_mutex_lock(&the_mutex);

    // if the assembly line is full, then wait for the packers to consume
    while((info->in + 1)%(info->l) == info->out)
      pthread_cond_wait(&condp,&the_mutex);

    // add the new product with colour and product ID onto the position in of the assembly line
    struct product new_product;
    new_product.colour_index = *c;
    new_product.product_index = i + 1;
    assembly_line[info->in] = new_product;

    // advance the position to put product on
    info->in = (info->in + 1)%(info->l);    
    i++; 

    pthread_cond_broadcast(&condc);
    pthread_mutex_unlock(&the_mutex);

  }
  pthread_exit(0);
}

