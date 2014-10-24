#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    printf("Calling findPrev with insufficient stack size!\n");
    assert(1);
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
  free(node1);
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
  //We've checked the leaves, now put a zero
  
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
  char headerstring[512] = {};
  treeToHeaderString(&headerstring, huffTree); 

  //Clean up memory and return success
  fclose(fp);
  fclose(compressedfile);
  return EXIT_SUCCESS;
}
