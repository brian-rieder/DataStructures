#include <stdio.h>

#define TRUE 1
#define FALSE 0

//declare stack structure
#define STACK_INT_SIZE 100

struct Stack_Int {
  int Top;
  int Items[STACK_INT_SIZE];
};

typedef struct Stack_Int Stack_Int_t;

//function prototypes
void Stack_Int_Init(Stack_Int_t *S_Ptr);
int Stack_Int_Empty(Stack_Int_t *S_Ptr);
int Stack_Int_Stacktop(Stack_Int_t *S_Ptr);
int Stack_Int_Pop(Stack_Int_t *S_Ptr);
int Stack_Int_Push(Stack_Int_t *S_Ptr, int Item);

void Stack_Int_Init(Stack_Int_t *S_Ptr)
{
  //initialize the stack of integers
  S_Ptr->Top = -1;
}

int Stack_Int_Empty(Stack_Int_t *S_Ptr)
{
  //is the stack empty?? (TRUE/FALSE)
  return(S_Ptr->Top > 0);
}

int Stack_Int_Push(Stack_Int_t *S_Ptr, int Item)
{
  //place Item on the top of the stack
  if(S_Ptr->Top >= STACK_INT_SIZE - 1)
    return FALSE;
  S_Ptr->Items[(++S_Ptr)->Top] = Item;
  return TRUE;
}

int Stack_Int_Stacktop(Stack_Int_t *S_Ptr)
{
  return(S_Ptr->Items[S_Ptr->Top]);
}

int Stack_Int_Pop(Stack_Int_t *S_Ptr)
{
  int Results = 0;
  Results = S_Ptr->Items[S_Ptr->Top];
  S_Ptr->Top--;
  return Results;
}

void Insert_At_Bottom(Stack_Int_t *S_Ptr, int Item)
{
  Stack_Int_t Aux_Stack;
  int Temp;

  Stack_Int_Init(&Aux_Stack);
  while(!Stack_Int_Empty(S_Ptr)) {
    Temp = Stack_Int_Pop(S_Ptr);
    Stack_Int_Push(&Aux_Stack, Temp);
  }
  Stack_Int_Push(S_Ptr, Item);
  while(!Stack_Int_Empty(&Aux_Stack)) {
    Temp = Stack_Int_Pop(&Aux_Stack);
    Stack_Int_Push(S_Ptr, Temp);
  }
}
