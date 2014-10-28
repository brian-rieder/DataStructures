#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 1
#define FALSE 0

#define FATAL(msg) {							\
    fprintf(stderr, "FATAL %s:%d %s\n", __FILE__, (int) __LINE__, msg); \
    exit(1);								\
  }

/**************************************************************************

ECE 368 Project 2

Name: Brian Rieder
PUID: brieder

UNHUFF.C:
Accepts one argument, which is the name of an input file. De-compresses the 
input file and writes out the de-compressed output to a file that has the 
same name as the input file with an “.unhuff” appended to it.

**************************************************************************/

typedef struct leaf {
  int value;
  struct leaf * left;  // left sub-tree
  struct leaf * right; // right sub-tree
} HuffNode;

typedef struct StackNode_st
{
  HuffNode * tree;
  struct StackNode_st * next;
} StackNode;

typedef struct Stack_st
{
  StackNode * head; // head node of a linked-list
} Stack;

HuffNode * HuffNode_create(int value)
{
  HuffNode * node = malloc(sizeof(HuffNode));
  node->value = value;
  node->left = NULL;
  node->right = NULL;
  return node;
}

void HuffNode_destroy(HuffNode * node)
{
  if(node == NULL)
    return;
  HuffNode_destroy(node->left);
  HuffNode_destroy(node->right);
  free(node);
}


Stack * Stack_create()
{
  return calloc(1, sizeof(Stack));
}

void Stack_destroy(Stack * stack)
{
  if(stack == NULL)
    return;
  StackNode * itr = stack->head;
  while(itr != NULL) {
    StackNode * tmp = itr;
    itr = itr->next;
    HuffNode_destroy(tmp->tree);
    free(tmp);
  }
  free(stack);
}

int Stack_isEmpty(Stack * stack)
{
  return stack->head == NULL;
}

HuffNode * Stack_popFront(Stack * stack)
{
  if(Stack_isEmpty(stack))
    FATAL("stack underflow");
  HuffNode * ret = stack->head->tree;
  StackNode * tmp = stack->head;
  stack->head = stack->head->next;
  free(tmp);
  return ret;
}

void Stack_pushFront(Stack * stack, HuffNode * tree)
{
  StackNode * head = malloc(sizeof(StackNode));
  head->tree = tree;
  head->next = stack->head;
  stack->head = head;
}

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
  bp->offset = 8; // The next read will trigger init of byte
  return bp;
}

void BitFile_destroy(BitFile * bp)
{
  if(bp != NULL)
    free(bp);
}

/**
 * Return 0 or 1, as per the next bit in the file
 * Return -1 if there are no more bits in the file
 * 'byte' is the current byte being processed
 * 'bitcounter' is the offset within byte for the current bit
 */
int BitFile_nextBit(BitFile * bf)
{
  if(bf->offset == 8) {
    bf->offset = 0;
    if(fread(&(bf->byte), sizeof(unsigned char), 1, bf->fp) != 1)
      return -1; // we're out of bits
  }
  return ((bf->byte) >> (7 - (bf->offset++))) & 0x01;
}

int BitFile_nextByte(BitFile * bf)
{
  int ret = 0;
  int offset;
  for(offset = 0; offset < 8; ++offset) {
    int bit = BitFile_nextBit(bf);
    if(bit < 0)
      return -1; // we're out of bits
    ret = ret | (bit << (7-offset));
  }
  return ret;
}

/**************************************************************************

Program Body Functions:

**************************************************************************/

void Stack_popPopCombinePush(Stack * stack)
{
  HuffNode * leaf1 = Stack_popFront(stack);
  HuffNode * leaf2 = Stack_popFront(stack);
  HuffNode * newtree = HuffNode_create(leaf1->value + leaf2->value);
  newtree->left = leaf2;
  newtree->right = leaf1;
  Stack_pushFront(stack, newtree);
}

HuffNode * HuffTree_readBinaryHeader(FILE * fp)
{
  Stack * stack = Stack_create();
  BitFile * bp = BitFile_create(fp);
  int val = BitFile_nextBit(bp);
  while(val >= 0) {
    if(val == 1) {
      val = BitFile_nextByte(bp);
      if(val < 0) 
	FATAL("corrupt (incomplete) header");
      Stack_pushFront(stack, HuffNode_create(val));
    } else if(val == 0) {
      if(stack->head != NULL && stack->head->next == NULL)
	break; // stack size is one: we're done
      Stack_popPopCombinePush(stack);
    }
    val = BitFile_nextBit(bp);
  }

  if(Stack_isEmpty(stack))
    FATAL("corrupt (empty?) header");    
  HuffNode * tree = Stack_popFront(stack);
  if(!Stack_isEmpty(stack))
    FATAL("tree not empty when it was expected to be");
  Stack_destroy(stack);
  BitFile_destroy(bp);
  return tree;
}

unsigned char * Huffman_applyTree(FILE * fp, HuffNode * tree, size_t * len_ptr)
{    
  // The length of the file is stored next:
  size_t len = 0;
  int val = fgetc(fp);
  while(!feof(fp) && val >= '0' && val <= '9') {
    len = (len * 10) + (val - '0');
    val = fgetc(fp);
  }
  if(val != '\n' || feof(fp))
    FATAL("Corrupt input file: new line not in expected place");

  // Allocate result buffer
  unsigned char * buffer = malloc(len * (sizeof(unsigned char) + 1));
  if(buffer == NULL)
    FATAL("Could not allocate buffer for decoding tree");
  buffer[len] = '\0'; // So buffer can be treated as a string

  // Now read the rest of the file into 'encoded'
  size_t write_pos = 0;
  int bit_offset = -1;
  unsigned char byte = 0;
  HuffNode * itr = tree;
  while(write_pos < len) {
    if(bit_offset < 0) {
      byte = fgetc(fp);
      if(feof(fp)) 
	FATAL("Unexpected end of file");
      bit_offset = 7;
    }
    int bit = (byte >> bit_offset) & 0x01;
    bit_offset -= 1;
    if(bit == 1)
      itr = itr->right;
    else 
      itr = itr->left;
    if(itr->left == NULL && itr->right == NULL) {
      buffer[write_pos++] = itr->value;
      itr = tree;
    }
  }

  *len_ptr = len;
  return buffer;
}

int main(int argc, char * * argv)
{
  //Program accepts one argument, the name of the input file
  if(argc < 2) {
    printf("Error: Must provide at least one argument for compression!\n");
    return EXIT_FAILURE;
  }
  char * filename = argv[1];

  //Check to see if the filename actually contains ".huff"
  if(strcmp(&filename[strlen(filename) - 5], ".huff")) {
    printf("Error: File extension must be '.huff' for decompression!\n");
    return EXIT_FAILURE;
  }

  //Load the info from the file
  FILE * fp = fopen(filename, "rb");

  //Derive our binary tree from the header
  HuffNode * huffTree = NULL;
  huffTree = HuffTree_readBinaryHeader(fp);
  
  //Apply it to the file
  size_t len;
  unsigned char * output = Huffman_applyTree(fp, huffTree, &len);

  //Figure out where we're going to write to
  char * decompfilename = malloc(strlen(filename) + strlen(".unhuff") + 1);
  strcpy(decompfilename, filename);
  strcat(decompfilename, ".unhuff");
  FILE * decompressedfile = fopen(decompfilename, "w");

  //Remove extra character from extraneous byte
  output[strlen((char*)output) - 1] = '\0';

  //Print output to appended filename to get back the original file!
  fprintf(decompressedfile, "%s", output);
  
  //Clean up memory
  fclose(fp);
  fclose(decompressedfile);
  HuffNode_destroy(huffTree);
  free(decompfilename);
  free(output);

  return EXIT_SUCCESS;
}
