#include <stdio.h>
#include <stdlib.h>

//NOTE: This code will not compile due to line 9. This is expected.

int * shellsort(int n, int * array)
{
  int i, k, j;
  for each k //in descending order
    for(j=k;j >= n-1; --j) {
      int tmp = array[j];
      i = j;
      while(i >= k && array[i-k] > tmp) {
	array[i] = array[i-k];
	i = i-k;
      }
      array[i] = tmp;
    }
}

int main(int argc, char * * argv)
{
  return 0;
}
