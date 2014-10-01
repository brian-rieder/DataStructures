#include "sorting.h"

/******************************************************************************

ECE 368 Project 1

Name: Brian Rieder
PUID: brieder

Submission Deadline: October 7th, 2014 at 11:59 PM

 *****************************************************************************/

#define INPUT_FILE "myrand.txt"

#define MAX_LONG_LEN 50

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

/*
long * Load_File(char * Filename, int * Size)
{
  //Open to READ file and zero out any initial size.
  FILE * fp = fopen(Filename,"r");
  *Size = 0;

  //Buffer that will contain strings read from file
  char * strbuffer = malloc(sizeof(char
}
*/

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

//I need to somehow find a way to generate one value...

//Power operation without specific library
int my_pow(int num, int exp)
{
  int prod = 1;
  for(; exp > 0; --exp) {
    prod *= num;
  }
  return prod;
}

//Generates a value in the sequence
int gen1val (int p, int q)
{
  return my_pow(2,p) * my_pow(3,q);
}

void Shell_Insertion_Sort(long * Array, int Size, 
			  double * N_Comp, double * N_Move)
{
  //Initialize iterative variables
  int i, j, k;

  //Allow gap value to be generated
  int gap = gen1val(0,0);

  //Outer loop will update gap
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

void Improved_Bubble_Sort(long * Array, int Size, 
			  double * N_Comp, double * N_Move)
{
  //Initialize variables for traditional bubble sort
  int i;
  long tmp;
  int swapflag = 1;

  //Find our starting gap
  int gap = gen2val(Size);

  //Outer loop controls size of gap, ends when it is only 1.
  while(gap != 0) {
    //Reset swapflag
    swapflag = 1;
    //Inner loop is traditional bubble sort with gap compares.
    while(swapflag) {                    
      swapflag = 0;                      
      for(i = gap; i < Size ; ++i) {      
	if((Array[i - gap] > Array[i]) && (i - gap >= 0)) {  
	  ++(*N_Comp);
	  *N_Move += 2;
	  tmp = Array[i];              
	  Array[i] = Array[i - gap];          
	  Array[i - gap] = tmp;             
	  swapflag = 1;                  
	}
      }
    }
    gap = gen2val(gap);
  }
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

void seq1_Helper(FILE * fp, int N, int x)
{
  int generated = 0;
  while(generated < N) {
    break; 
  }
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

int compar(const void * a, const void * b)
{
  long aval = *(long*)a;
  long bval = *(long*)b;
  if(aval > bval)
    return 1;
  if(bval > aval)
    return -1;
  return 0;
}

int main(int argc, char * * argv)
{
  //Initialization of size and execution trackers
  int z = 0;
  double compnum = 0;
  double movenum = 0;

  //Loading comparison and execution arrays
  long * array = Load_File(INPUT_FILE, &z);
  long * orig_array = Load_File(INPUT_FILE, &z);

  //Calling specified sorting function
  //Improved_Bubble_Sort(array, z, &compnum, &movenum);
  Shell_Insertion_Sort(array, z, &compnum, &movenum);

  //Display of original input and final output in a pretty format
  printf("     Original Input:            Final Output:\n");
  int i;
  for(i = 0; i < z; ++i) {
    printf("%20ld%25ld\n",orig_array[i],array[i]);
  }

  //Displaying execution trackers
  printf("\nIn the process of sorting, we used %f comparisons and %f moves.\n\n",
  	 compnum,movenum);

  //Printing checker using qsort to sort the orignal array
  printf("Checking if printing was valid...\n");
  qsort(orig_array, z, sizeof(long), compar);

  for(i = 0; i < z; ++i) {
    if(array[i] != orig_array[i]) {
      printf("The sorting did not work!\n\n");
      return EXIT_FAILURE;
    }
  }
  printf("Sorting was completed successfully.\n\n");
  return EXIT_SUCCESS;
}
