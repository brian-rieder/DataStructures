#include "sorting.h"

/******************************************************************************

ECE 368 Project 1

Name: Brian Rieder
PUID: brieder

Submission Deadline: October 7th, 2014 at 11:59 PM

 *****************************************************************************/

#define MAX_LONG_LEN 12

/*--------------------------------------------------------------------Load File
The file contains N+1 integers, one in each line. The first line of the 
file indicates the number of integers to be sorted, i.e., N. *Size should 
be assigned the value of N at the end of the routine. The subsequent lines 
contain (long) integers.

Input: 
  char * Filename -- the name of a file in the current directory as a
    string.
  int * Size -- a return pointer of the number of long integers in the file.
    It does not need to be initialized to a value.

Output:
  long * longArr -- array of the long integers within the file
-----------------------------------------------------------------------------*/

long * Load_File(char * Filename, int * Size)
{
  //Open to READ file and zero out any initial size.
  FILE * fp = fopen(Filename,"r");
  *Size = 0;

  //Buffer that will contain strings read from file
  char * strbuffer = malloc(sizeof(char) * MAX_LONG_LEN);

  //Get the length of the file (number of long integers)
  fgets(strbuffer, 12, fp);
  *Size = atoi(strbuffer);
  
  //Create and prep to write to buffer of long integers
  int i;
  long * longArr = malloc(sizeof(long *) * *Size);

  //Take file input line-by-line to get long integers to store to array
  for(i = 0; i < *Size; ++i) {
    fgets(strbuffer, MAX_LONG_LEN, fp);
    longArr[i] = strtol(strbuffer, NULL, 0);
  }

  //Clean up memory and return.
  free(strbuffer);
  fclose(fp);
  return longArr;
}

//-----------------------------------------------------------------------------

/*--------------------------------------------------------------------Save File
The function saves Array to an external file specified by Filename. The 
output file should have the same format as the input file.

Input:
  char * Filename -- the name of a file in the current directory as a
    string.
  long * Array -- array of long integers to go into the file.
  int Size -- the number of long integers within the file

Output:
  int success -- output of 0 for write failure and 1 for write success.
-----------------------------------------------------------------------------*/

int Save_File(char * Filename, long * Array, int Size)
{
  //Open to WRITE to file
  FILE * fp = fopen(Filename,"w");

  //Create buffer to contain the length of file and long integers
  char * strbuffer = malloc(sizeof(char) * MAX_LONG_LEN);

  //Convert size to string to put to file
  sprintf(strbuffer, "%d", Size);
  fputs(strbuffer, fp);

  return 0;
}

//-----------------------------------------------------------------------------

/*---------------------------------------------------------Shell Insertion Sort
Function description.

Input:
  char * Filename -- the name of a file in the current directory as a
    string.
  int Size -- the number of long integers within the file.
  double * N_Comp -- pointer to store the number of comparisons done within
    the sorting algorithm.
  double * N_Move -- pointer to store the number of moves done within the
    sorting algorithm

Output:

-----------------------------------------------------------------------------*/

void Shell_Insertion_Sort(long * Array, int Size, 
			  double * N_Comp, double * N_Move)
{
}

//-----------------------------------------------------------------------------

/*---------------------------------------------------------Improved Bubble Sort
Function description.

Input:
  char * Filename -- the name of a file in the current directory as a
    string.
  int Size -- the number of long integers within the file.
  double * N_Comp -- pointer to store the number of comparisons done within
    the sorting algorithm.
  double * N_Move -- pointer to store the number of moves done within the
    sorting algorithm

Output:

-----------------------------------------------------------------------------*/

void Improved_Bubble_Sort(long * Array, int Size, 
			  double * N_Comp, double * N_Move)
{
}

//-----------------------------------------------------------------------------

/*--------------------------------------------------------------------Save Seq1
The function should generate Seq1 that is identical to the one used in 
your sorting code, and write Seq1 to an external file specified by 
Filename. N is the size of the array to be sorted, NOT the size of Seq1.
You implement this function to allow the grader to check whether you 
can generate the right Seq1. 

Input:


Output:

-----------------------------------------------------------------------------*/

void Save_Seq1 (char *Filename, int N)
{
}

//-----------------------------------------------------------------------------

/*--------------------------------------------------------------------Save Seq2
The function should generate Seq2 that is identical to the one used in
your sorting code, and write Seq1 to an external file specified by 
Filename. N is the size of the array to be sorted, NOT the size of Seq1.
You implement this function to allow the grader to check whether you 
can generate the right Seq2.

Input:


Output:

-----------------------------------------------------------------------------*/

void Save_Seq2 (char *Filename, int N)
{
}

//-----------------------------------------------------------------------------

//-----------------------------------------------TEMPORARY MAIN, PLEASE REMOVE!
int main(int argc, char * * argv)
{
  int z = 0;
  long * output = Load_File("sample-input.txt", &z);
  printf("Length of file: %d\n",z);
  int i;
  for(i = 0; i < z; ++i) {
    printf("%ld\n",output[i]);
  }
  return 0;
}
