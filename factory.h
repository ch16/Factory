#define MAXCOLORS 147
#define MAX_BUF 128

/** @brief Generate number of c products, while wait
 *         if the assembly line is full or while other 
 *         assemblers are producing.
 *  @param ptr Pointer to the argument. In this function,
 *	   it's the index of colour for this assemler to produce.
 *  @return void.
 */
void *assembler_function(void *ptr);

/** @brief Generate boxes, each of which can pack up to
 *	   n products, while wait for the assemblers
 *         if the assembly line is empty or while other 
 *         packers are packing.
 *  @param ptr Pointer to the argument. In this function,
 *	   it's NULL.
 *  @return void.
 */
void *packer_function(void *ptr);

// initiate an assembly line array of products and a struct of all infomation
extern struct product *assembly_line;
extern struct infomation *info;

// initiate the mutex lock and the conditional wait
extern pthread_mutex_t the_mutex, print_mutex,colour_mutex;
extern pthread_cond_t condc, condp;

// the strcut that contains the index of colour and product number of each product
struct product{
  int colour_index;
  int product_index;
}product;

// a struct that contains the information related with assembly line, assembler and packer
struct infomation{
  int a; // the number of assembler threads
  int p; // the number of packer threads
  int l; // the size of the assembly line
  int n; // the number of products per box
  int c; // the number of products each assembler will produce
  int in; // the position on the assembly line that assembler should put products on
  int out; // the position on the assembly line that packers should get products from
  int packed; // the number of total products packed by the packers
}infomation;
