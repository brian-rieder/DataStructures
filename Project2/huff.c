#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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
  if(stack->size == 1) {
    printf("Calling findTwoSmallest with insufficient size!\n");
    assert(1);
  }
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
    if(ptr->tree->weight < (*smallest)->tree->weight)
      *smallest = ptr;
    else if(ptr->tree->weight < (*secondsmallest)->tree->weight)
      *secondsmallest = ptr;
    ptr = ptr->next;
  }
}

StackNode * findPrev(Stack * stack, StackNode * curr)
{
  if(stack->size == 1) {
    printf("Calling findPrev with insufficient stack size!\n");
    assert(1);
  }
  StackNode * ptr = stack->head;
  while(ptr->next != NULL) {
    if(ptr->next == curr) 
      return(ptr);
    ptr = ptr->next;
  }
  printf("Node does not exist within the stack, exiting...\n");
  assert(1);
  return NULL;
}

StackNode * combineAndRemoveOld(Stack * stack, StackNode * node1, StackNode * node2)
{
  return NULL;
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

  return EXIT_FAILURE;
}
