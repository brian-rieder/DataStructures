#include <stdio.h>
#include <stdlib.h>

/**************************************************************************

ECE 368 Project 2

Name: Brian Rieder
PUID: brieder

HUFF.C:
Accepts one argument, which is the name of an input file. Compresses the
input file using Huffman coding and writes out the compressed output to a
file that has the same name as the input file with a ".huff" appended to it.

 **************************************************************************/



int main(int argc, char * * argv)
{
  //Program accepts one argument, the name of the input file
  if(argc < 2) {
    printf("Error: Must provide at least one argument for compression!\n");
    return EXIT_FAILURE;
  }
  char * filename = argv[1];

  //Load the info from the file.
}
