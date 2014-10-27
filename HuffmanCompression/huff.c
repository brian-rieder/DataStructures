#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FATAL(msg) {							\
    fprintf(stderr, "FATAL %s:%d %s\n", __FILE__, (int) __LINE__, msg); \
    exit(1);								\
  }

/**************************************************************************

ECE 368 Project 2

Name: Brian Rieder
PUID: brieder

HUFF.C:
Accepts one argument, which is the name of an input file. Compresses the
input file using Huffman coding and writes out the compressed output to a
file that has the same name as the input file with a ".huff" appended to it.

 **************************************************************************/

typedef struct node {
  char letter;
  int weight;
  struct node * left;
  struct node * right;
} Node;

typedef struct stacknode_st {
  Node * tree;
  struct stacknode_st * next;
} StackNode;

typedef struct stack_st {
  StackNode * head;
  int size;
} Stack;

Node * constructNode(char letter, int weight)
{
  Node * n = malloc(sizeof(Node));
  n->letter = letter;
  n->weight = weight;
  n->left = NULL;
  n->right = NULL;
  return(n);
}

StackNode * constructSN(Node * tree)
{
  StackNode * sn = malloc(sizeof(StackNode));
  sn->tree = tree;
  sn->next = NULL;
  return(sn);
}

Stack * constructStack(StackNode * head)
{
  Stack * s = malloc(sizeof(Stack));
  s->head = head;
  s->size = 1;
  return(s);
}

/**************************************************************************

Bitfile Manipulation -- similar to unhuff!

 **************************************************************************/

typedef struct BitFile_st
{
  FILE * fp;
  unsigned char byte; // Current byte being processed
  int offset;         // offset into current byte gives the current bit
} BitFile;

BitFile * BitFile_create(FILE * fp)
{
  BitFile * bp = malloc(sizeof(BitFile));
  bp->fp = fp;
  bp->byte = 0;
  bp->offset = 0; // The next read will trigger init of byte
  return bp;
}

void BitFile_destroy(BitFile * bp)
{
  if(bp != NULL)
    free(bp);
}

void BitFile_writeBit(char c, BitFile * bf)
{
  if(c == '1') {
    bf->byte |= (0x80 >> (bf->offset++));
  }
  else { //c == '0'
    if(bf->offset == 0)
      bf->byte &= 0x7F;
    else if(bf->offset == 1)
      bf->byte &= 0xBF;
    else if(bf->offset == 2)
      bf->byte &= 0xDF;
    else if(bf->offset == 3)
      bf->byte &= 0xEF;
    else if(bf->offset == 4)
      bf->byte &= 0xF7;
    else if(bf->offset == 5)
      bf->byte &= 0xFB;
    else if(bf->offset == 6)
      bf->byte &= 0xFD;
    else if(bf->offset == 7)
      bf->byte &= 0xFE;
    ++(bf->offset);
  }
  if(bf->offset == 8) {
    bf->offset = 0;
    fwrite(&(bf->byte), sizeof(unsigned char), 1, bf->fp);
  }
}

void BitFile_writeByte(char c, BitFile * bf)
{ 
  char bit1 = ((0x80 & c) >> 7) + '0';
  char bit2 = ((0x40 & c) >> 6) + '0';
  char bit3 = ((0x20 & c) >> 5) + '0';
  char bit4 = ((0x10 & c) >> 4) + '0';
  char bit5 = ((0x08 & c) >> 3) + '0';
  char bit6 = ((0x04 & c) >> 2) + '0';
  char bit7 = ((0x02 & c) >> 1) + '0';
  char bit8 = (0x01 & c) + '0';

  BitFile_writeBit(bit1, bf);
  BitFile_writeBit(bit2, bf);
  BitFile_writeBit(bit3, bf);
  BitFile_writeBit(bit4, bf);
  BitFile_writeBit(bit5, bf);
  BitFile_writeBit(bit6, bf);
  BitFile_writeBit(bit7, bf);
  BitFile_writeBit(bit8, bf);  
}

/**************************************************************************

Program Body Functions:

 **************************************************************************/

void weighCharacters(FILE * fp, int * asciiArray)
{
  while(!feof(fp)) {
    int ch = fgetc(fp);
    ++(asciiArray[ch]);
  }
}

Stack * genTrees(int * asciiArray)
{
  Stack * stack = NULL;
  int ind;
  StackNode * tracker = NULL;

  for(ind = 0; ind < 128; ++ind) {
    if(asciiArray[ind] != 0) {
      if(stack == NULL) {
	stack = constructStack(constructSN(constructNode(ind, asciiArray[ind])));
	tracker = stack->head;
      }
      else {
	tracker->next = constructSN(constructNode(ind, asciiArray[ind]));
	++(stack->size);
	tracker = tracker->next;
      }
    }
  }
  return stack;
}

void findTwoSmallest(Stack * stack, StackNode * * smallest, StackNode * * secondsmallest)
{
  if(stack->size == 1)
    FATAL("Calling findTwoSmallest with insufficient size!");
  if(stack->size == 2) {
    if(stack->head->tree->weight > stack->head->next->tree->weight) {
      *smallest = stack->head->next;
      *secondsmallest = stack->head;
      return;
    }
    *smallest = stack->head;
    *secondsmallest = stack->head->next;
    return;
  }
  StackNode * ptr = stack->head->next->next;
  while(ptr != NULL) {
    if(ptr->tree->weight < (*smallest)->tree->weight) {
      *secondsmallest = *smallest;
      *smallest = ptr;
    }
    else if(ptr->tree->weight < (*secondsmallest)->tree->weight)
      *secondsmallest = ptr;
    ptr = ptr->next;
  }
}

StackNode * findPrev(Stack * stack, StackNode * curr)
{
  if(stack->size == 1) {
    FATAL("Calling findPrev with insufficient stack size!");
  }
  if(curr == stack->head) {
    return NULL;
  }
  StackNode * ptr = stack->head;
  while(ptr->next != NULL) {
    if(ptr->next == curr) 
      return(ptr);
    ptr = ptr->next;
  }
  return NULL;
}

void combineAndRemoveOld(Stack * stack, StackNode * node1, StackNode * node2)
{
  if(stack->size > 2) {
    //Adjust head, if necessary
    if(node1 == stack->head || node2 == stack->head) {
      stack->head = stack->head->next;
      if(node1 == stack->head || node2 == stack->head)
	stack->head = stack->head->next;
    }

    //Fixing links to jump over the removed nodes
    StackNode * prev1 = findPrev(stack, node1);
    if(prev1 == NULL || prev1 == node2);
    else {
      prev1->next = node1->next;
      node1->next = NULL;
    }
    StackNode * prev2 = findPrev(stack, node2);
    if(prev2 == NULL || prev2 == node2);
    else {
      prev2->next = node2->next;
      node2->next = NULL;
    }
  }

  //Adjusting weights and values for the new node
  StackNode * newNode = constructSN(constructNode(node1->tree->letter + node2->tree->letter, 
						 node1->tree->weight + node2->tree->weight));
  newNode->tree->left = node1->tree;
  newNode->tree->right = node2->tree;

  //Get rid of the old node memory
  node1 = NULL;
  free(node1);
  node2 = NULL;
  free(node2);

  //Send back merged node on the front of the stack
  newNode->next = stack->head;
  stack->head = newNode;
  --(stack->size);
}

//Utilizes post-order traversal
void treeToHeaderString(char * * headerstring, Node * huffTree)
{
  if(huffTree == NULL) {
    return;
  }

  treeToHeaderString(headerstring, huffTree->left);
  treeToHeaderString(headerstring, huffTree->right);

  if(huffTree->left != NULL && huffTree->right != NULL) {
    //Non-leaf node, so put a 0 and go home
    strcat(*headerstring, "0");
  }
  else {
    //Leaf node, so put a 1 followed by the node's character
    strcat(*headerstring, "1");
    strcat(*headerstring, &(huffTree->letter));
  }
  
}

BitFile * writeHeaderToFile(FILE * fp, char * headerstring, int filelen)
{
  int i;
  BitFile * bp = BitFile_create(fp);
  for(i = 0; i < strlen(headerstring); ++i) {
    char c = headerstring[i];
    if(c == '1') {
      //write the 1 bit, then the byte
      BitFile_writeBit('1', bp);
      BitFile_writeByte(headerstring[i + 1], bp);
      ++i;
    }
    else if(c == '0') {
      //write the 0 bit, no increment necessary
      BitFile_writeBit('0', bp);
    }
    else {
      //something went wrong, kill it...
      FATAL("Invalid character in the header string.");
    }
  }
  //Clean up the last byte
  while(bp->offset != 7) {
    BitFile_writeBit('0', bp);
  }

  //Write an extra 0 and then the header length
  BitFile_writeBit('0', bp);
  
  //fputc('1', bp->fp);
  //fputc('3', bp->fp);
  char lendigit[200];
  sprintf(lendigit, "%d", filelen);
  for(i = 0; i < strlen(lendigit); ++i) {
    fputc(lendigit[i], bp->fp);
  }
  //Newline coup-de-grace!
  fputc('\n', bp->fp);

  return bp;
}

void determineLetterCode(char * bitpattern, char letter, Node * huffTree, int ind, char * * f)
{
  if(*f == NULL)
    return;
  if(huffTree == NULL)
    return;
  if((huffTree->letter == letter) && (huffTree->left == NULL) && (huffTree->right == NULL)) {
    bitpattern[ind] = '\0';
    *f = NULL;
    return;
  }

  bitpattern[ind] = '0';
  ++ind;
  determineLetterCode(bitpattern, letter, huffTree->left, ind, f);
  if(*f == NULL || bitpattern[ind] == '\0') {
    *f = NULL;
    return;
  }
  else
    --ind;
  bitpattern[ind] = '1';
  ++ind;
  determineLetterCode(bitpattern, letter, huffTree->right, ind, f);
  if(*f == NULL || bitpattern[ind] == '\0') {
    *f = NULL;
  }
}

void writeDataToFile(BitFile * compfile, FILE * fp, Node * huffTree)
 {
   char * bitpattern = malloc(sizeof(char) * 100);
   while(!feof(fp)) {
     //Get a letter from the unencoded file
     char c = fgetc(fp);

     //Clear out any potential previous bit pattern
     int i = 0;
     while(i < 100) {
       bitpattern[i++] = 'X';
     }

     //Transform character into its compressed bit pattern
     char * flag = "";
     determineLetterCode(bitpattern, c, huffTree, 0, &flag);

     //Write compressed bit pattern to compressed file
     for(i = 0; i < strlen(bitpattern); ++i) {
       BitFile_writeBit(bitpattern[i], compfile);
     }

     //Finished with this character, let's move to the next
   }
   free(bitpattern);
 }

int main(int argc, char * * argv)
{
  //Program accepts one argument, the name of the input file
  if(argc < 2) {
    printf("Error: Must provide at least one argument for compression!\n");
    return EXIT_FAILURE;
  }
  char * filename = argv[1];

  //Load the info from the file
  FILE * fp = fopen(filename, "r");

  //Determine the length of the file
  int filelen = 0;
  while(!feof(fp)) {
    fgetc(fp);
    ++filelen;
  }
  rewind(fp);

  //Count characters and assign weights in an integer array
  int asciiChars[128] = {};
  weighCharacters(fp, asciiChars);

  //Generate trees based on the array (not in order yet)
  Stack * stack = genTrees(asciiChars);

  //Order the linked list of trees (aka, the stack)
  while(stack->size > 1) {
    StackNode * smallest;
    StackNode * secondsmallest;
    if(stack->head->tree->weight < stack->head->next->tree->weight) {
      smallest = stack->head;
      secondsmallest = stack->head->next;
    }
    else {
      smallest = stack->head->next;
      secondsmallest = stack->head;
    }
    findTwoSmallest(stack, &smallest, &secondsmallest); 
    combineAndRemoveOld(stack, smallest, secondsmallest);
  }
  
  //Get rid of extraneous freed data after the head
  stack->head->next = NULL;

  //NOTE: We now have the complete binary tree for the file built!
  Node * huffTree = stack->head->tree;

  //Figure out where we're going to write to
  char * compfilename = malloc(strlen(filename) + strlen(".huff") + 1);
  strcpy(compfilename, filename);
  strcat(compfilename, ".huff");
  FILE * compressedfile = fopen(compfilename, "wb");

  //Store the header information into a string
  char * headerstring = malloc(sizeof(char) * 512);
  treeToHeaderString(&headerstring, huffTree);
  
  //NOTE: We now have the header data stored into a string!

  //Write the header in binary to the file
  BitFile * bf = writeHeaderToFile(compressedfile, headerstring, filelen);
  free(headerstring);

  //Write encoded data to file!
  rewind(fp);
  writeDataToFile(bf, fp, huffTree);
  //fputc(0xEE, bf->fp);

  //Clean up memory and return success
  fclose(fp);
  fclose(compressedfile);
  return EXIT_SUCCESS;
}
