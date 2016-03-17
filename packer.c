/* name: 		Hong Chen
 * ONE Card number: 	1386152
 * Unix id: 		chen1
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


/** @brief Generate boxes, each of which can pack up to
 *	   n products, while wait for the assemblers
 *         if the assembly line is empty or while other 
 *         packers are packing.
 *  @param ptr Pointer to the argument. In this function,
 *	   it's NULL.
 *  @return void.
 */
void *packer_function(void *ptr){

  const char *COLORS[] = {"AliceBlue","AntiqueWhite","Aqua","Aquamarine","Azure","Beige","Bisque","Black","BlanchedAlmond","Blue","BlueViolet","Brown","BurlyWood","CadetBlue","Chartreuse","Chocolate","Coral","CornflowerBlue","Cornsilk","Crimson","Cyan","DarkBlue","DarkCyan","DarkGodenRod","DarkGray","DarkGrey","DarkGreen","DarkKhaki","DarkMagenta","DarkOliveGreen","Darkorange","DarkOrchid","DarkRed","DarkSalmon","DarkSeaGreen","DarkSlateBlue","DarkSlateGray","DarkSlateGrey","DarkTurquoise","DarkViolet","DeepPink","DeepSkyBlue","DimGray","DimGrey","DodgerBlue","FireBrick","FloralWhite","ForestGreen","Fuchsia","Gainsboro","GhostWhite","God","GodenRod","Gray","Grey","Green","GreenYellow","HoneyDew","HotPink","IndianRed","Indigo","Ivory","Khaki","Lavender","LavenderBlush","LawnGreen","LemonChiffon","LightBlue","LightCoral","LightCyan","LightGodenRodYellow","LightGray","LightGrey","LightGreen","LightPink","LightSalmon","LightSeaGreen","LightSkyBlue","LightSlateGray","LightSlateGrey","LightSteelBlue","LightYellow","Lime","LimeGreen","Linen","Magenta","Maroon","MediumAquaMarine","MediumBlue","MediumOrchid","MediumPurple","MediumSeaGreen","MediumSlateBlue","MediumSpringGreen","MediumTurquoise","MediumVioletRed","MidnightBlue","MintCream","MistyRose","Moccasin","NavajoWhite","Navy","OdLace","Olive","OliveDrab","Orange","OrangeRed","Orchid","PaleGodenRod","PaleGreen","PaleTurquoise","PaleVioletRed","PapayaWhip","PeachPuff","Peru","Pink","Plum","PowderBlue","Purple","Red","RosyBrown","RoyalBlue","SaddleBrown","Salmon","SandyBrown","SeaGreen","SeaShell","Sienna","Silver","SkyBlue","SlateBlue","SlateGray","SlateGrey","Snow","SpringGreen","SteelBlue","Tan","Teal","Thistle","Tomato","Turquoise","Violet","Wheat","White","WhiteSmoke","Yellow","YellowGreen"};

  // create and locate memory for a box of size n
  struct product *box;
  box = (struct product *)malloc((size_t)info->n*sizeof(struct product));

  int i; 
  int a;
  a = pthread_self(); // thread ID

  // if not all the products to be produced are packed, then keep packing
  while(info->packed < (info->a*info->c)){

    // for each box, if it has not packed n product then keep packing 
    for(i = 0; i < info->n; i++){

      pthread_mutex_lock(&the_mutex);

      // if all products are packed, then break even if the current box is not full
      if(info->packed == info->a*info->c){
        pthread_mutex_unlock(&the_mutex);
        break;
      }

      // wait if the assembly line is empty
      while(info->in == info->out && info->packed < info->a*info->c)
        pthread_cond_wait(&condc,&the_mutex);

      // after waiting, check if the other packer thread has finished packing 
      if(info->packed == info->a*info->c){
        pthread_mutex_unlock(&the_mutex);
        break;
      }

      // pack the product into the current box from position out and advance the position to get product from
      box[i] = assembly_line[info->out];
      info->out = (info->out + 1)%(info->l);
      info->packed++;

      pthread_cond_broadcast(&condp);
      pthread_mutex_unlock(&the_mutex);
    } 
     
    // if the current box has at least one product, print it out 
    if (i != 0){
      pthread_mutex_lock(&print_mutex);

      // the last four digits of current thread ID
      a = abs(a);
      while (a>=10000){
        a = a/10;
      }

      printf("[Packer %d]: ",a);
      printf("I have a box of products containing: ");
      
      // print out each product in this box
      for (int j = 0; j < i - 1; j++){
        printf("%s %d, ",COLORS[box[j].colour_index%MAXCOLORS],box[j].product_index);
      }
    	printf("%s %d",COLORS[box[i-1].colour_index%MAXCOLORS],box[i-1].product_index);
    	printf("\n\n");
      i = 0;
      pthread_mutex_unlock(&print_mutex);
    }

  }
  free(box);
  pthread_exit(0);
}
