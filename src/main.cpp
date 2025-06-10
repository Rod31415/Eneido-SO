#include "headers/main.h"

/*
          # # #
      # #       # #
    #               #
  #        ###        #
  #      ##   ##      #
#       #       #       #
#       #       #       #
#       #       #       #
  #      #     #      #
  #      #     #      #
    #   #       #   #
      # #       # #


 */

void __main(multiboot_info *mboot)
{
  init_term(mboot);

  while (1)
  {

    loop_term();
  }
}
