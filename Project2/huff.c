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
  return(s);
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
	tracker = tracker->next;
      }
    }
  }
  return stack;
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

  //Count characters and assign weights in an integer array
  int asciiChars[128] = {};
  weighCharacters(fp, asciiChars);

  //Generate trees based on the array (not in order yet)
  Stack * stack = genTrees(asciiChars);

  //Order the linked list of trees (aka, the stack)

  return EXIT_FAILURE;
}
