#include <stdio.h>
#include <stdlib.h>

void bubblesort(int * arr, int numarg)
{
  int i;
  int tmp;
  int swapped = 1;
  while(swapped) {
    swapped = 0;
    for(i=1;i<numarg;++i) {
      if(arr[i - 1] > arr[i]) {
	tmp = arr[i];
	arr[i] = arr[i - 1];
	arr[i - 1] = tmp;
	swapped = 1;
      }
    }
  }
}

void insertionsort(int * arr, int numarg)
{
  int i,j,tmp;
  for(i=1;i<numarg;++i) {
    j = i;
    while(j > 0 && arr[j-1] > arr[j]) {
      tmp = arr[j];
      arr[j] = arr[j-1];
      arr[j-1] = tmp;
      --j;
    }
  }
}

void selectionsort(int * arr, int numarg)
{
  int subsize = 0;
  int i, tmp;
  for(subsize=0;subsize<numarg;++subsize) {
    int smallestind = subsize;
    for(i=subsize;i<numarg;++i) {
      if(arr[i] < arr[smallestind])
	smallestind = i;
    }
    tmp = arr[subsize];
    arr[subsize] = arr[smallestind];
    arr[smallestind] = tmp;
  }
}

void printNumArr(int * arr, int numarg)
{
  int i;
  for(i=0;i<numarg - 1;++i)
    printf("%d, ",arr[i]);
  printf("%d\n",arr[i]);
}

void checkValid(int * arr, int numarg)
{
  int i;
  for(i=1;i<numarg;++i)
    if(arr[i - 1] > arr[i]) {
      printf("ARRAY SORTING FAILED!!!\n\n\n");
      return;
    }
  printf("Array Sorting Successful!!!\n\n\n");
}

int main(int argc, char * * argv)
{
  int numarg = 6;
  int arr[] = {3,2,4,6,1,2};
  printf("\nBUBBLE SORT\n");
  printf("Before Sorting:\n");
  printNumArr(arr, numarg);
  printf("After Sorting:\n");
  bubblesort(arr, numarg);
  printNumArr(arr, numarg);
  checkValid(arr, numarg);

  int arr2[] = {3,2,4,6,1,2};
  printf("INSERTION SORT\n");
  printf("Before Sorting:\n");
  printNumArr(arr2, numarg);
  printf("After Sorting:\n");
  insertionsort(arr2, numarg);
  printNumArr(arr2, numarg);
  checkValid(arr2, numarg);

  int arr3[] = {3,2,4,6,1,2};
  printf("SELECTION SORT\n");
  printf("Before Sorting:\n");
  printNumArr(arr3, numarg);
  printf("After Sorting:\n");
  selectionsort(arr3, numarg);
  printNumArr(arr3, numarg);
  checkValid(arr3, numarg);

  return 0;
}
