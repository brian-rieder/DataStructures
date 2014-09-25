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
  fgets(strbuffer, MAX_LONG_LEN, fp);
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
  sprintf(strbuffer, "%d\n", Size);
  fputs(strbuffer, fp);

  //Convert each index of array to string and put to file.
  int i;
  for(i = 0; i < Size; ++i) {
    sprintf(strbuffer,"%ld\n", Array[i]);
    fputs(strbuffer, fp);
  }
  
  //Clean up memory and return success
  free(strbuffer);
  fclose(fp);
  return 1;
}

//-----------------------------------------------------------------------------

/*---------------------------------------------------------Shell Insertion Sort
Takes in an array of long integers (stored in Array) and sorts them in 
ascending order. The Shell Sort function should use the sequence Seq1.

Input:
  long * Array -- array of long integers, should be from Seq1 for shell sort.
  int Size -- the number of integers to be sorted.
  double * N_Comp -- pointer to store the number of comparisons involving
    items in Array.
  double * N_Move -- pointer to store the number of moves involving items in
    Array.
  

Output:
  NONE -- void function
-----------------------------------------------------------------------------*/

void Shell_Insertion_Sort(long * Array, int Size, 
			  double * N_Comp, double * N_Move)
{
}

//-----------------------------------------------------------------------------

/*---------------------------------------------------------Improved Bubble Sort
Takes in an array of long integers (stored in Array) and sorts them in 
ascending order. The Bubble Sort function should use the sequence Seq2.

Input:
  long * Array -- array of long integers, should be from Seq1 for shell sort.
  int Size -- the number of integers to be sorted.
  double * N_Comp -- pointer to store the number of comparisons involving
    items in Array.
  double * N_Move -- pointer to store the number of moves involving items in
    Array.
  

Output:
  NONE -- void function
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

  {2^p′3^q′, ..., 2^p3^q, ..., 16, 12, 9, 8, 6, 4, 3, 2, 1}, where 2^p′3^q′ is 
  the largest integer smaller than N (the size of the array to be sorted). 

Input:
  char * Filename -- the name of a file in the current directory as a
    string.
  int N -- the size of the array to be sorted.

Output:
  NONE -- void function
-----------------------------------------------------------------------------*/

// THOUGHTS PRIOR TO STOPPING WORK
// Generate all numbers in a function -- sort in array?
// qsort the numbers descending
// output array to file


//Puts one integer to a file opened for writing.
void intToFile(FILE * fp, int n)
{
  char strbuffer[6];
  sprintf(strbuffer, "%d\n", n);
  fputs(strbuffer, fp);
}

//Generates the next value in the sequence
int gen1val (int num)
{

}

void seq1_Helper(fp, N, -1)
{

}

void Save_Seq1 (char *Filename, int N)
{
  //Open to WRITE to file
  FILE * fp = fopen(Filename,"w");

  //Use helper function to generate and write to file
  seq1_Helper(fp, N, -1);

  //Clean up memory
  fclose(fp);
}

//-----------------------------------------------------------------------------

/*--------------------------------------------------------------------Save Seq2
  The function should generate Seq2 that is identical to the one used in
  your sorting code, and write Seq1 to an external file specified by 
  Filename. N is the size of the array to be sorted, NOT the size of Seq1.
  You implement this function to allow the grader to check whether you 
  can generate the right Seq2.
  
  {N/1.3, N/(1.3)2, N/(1.3)3, …, 1}, where N is the size of the array to be sorted. 
  In other words, the next value in the gap sequence is the previous gap divided
  by 1.3. However, if any value in this sequence becomes 9 or 10, it is replaced 
  by a value of 11. Note that since the gap value has to be an integer, the floor 
  of the gap value computed will need to be taken. 
  
Input:
  char * Filename -- the name of a file in the current directory as a
    string.
  int N -- the size of the array to be sorted.
  
Output:
  NONE -- void function
------------------------------------------------------------------------------*/

//Generates the next value in the sequence
int gen2val (int num)
{
  //Divide value by 1.3
  int ret = (int)((double)num/1.3);

  //If any value is becomes 9 or 10, it is replaced by 11.
  if(ret == 9 || ret == 10)
    ret = 11;

  //Return adjusted value
  return ret;
}

void seq2_Helper (FILE * fp, int N, int prevval)
{
  //Base Case: Terminate if previous value was 1
  if(prevval == 1)
    return;
  //Initial Case: First value in sequence is N/1.3
  if(prevval == -1) {
    prevval = gen2val(N);
    intToFile(fp, prevval);
    seq2_Helper(fp, N, prevval);
    return;
  }
  //Recursive Case: Next value is the previous divided by 1.3
  prevval = gen2val(prevval);
  intToFile(fp, prevval);
  seq2_Helper(fp, N, prevval);
}

void Save_Seq2 (char *Filename, int N)
{
  //Open to WRITE to file
  FILE * fp = fopen(Filename,"w");

  //Use helper function to generate and write to file
  seq2_Helper(fp, N, -1);

  //Clean up memory
  fclose(fp);
}

//-----------------------------------------------------------------------------

//-----------------------------------------------TEMPORARY MAIN, PLEASE REMOVE!
int main(int argc, char * * argv)
{
  /*
  int z = 0;
  long * output = Load_File("sample-input.txt", &z);
  //printf("Length of file: %d\n",z);
  int i;
  for(i = 0; i < z; ++i) {
    //printf("%ld\n",output[i]);
  }
  Save_File("empty_file.txt", output, z);
  */

  Save_Seq2 ("seq2_output.txt", 50);

  return 0;
}
