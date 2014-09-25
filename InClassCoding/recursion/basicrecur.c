#include <stdio.h>

void recur(int arg)
{
  int local arg = arg * 2;
  if(arg == 3)
    printf("enough!\n");
  else
    recur(++arg);
}

int main(int argc, char * * argv)
{
  recur(0);
  return 1;
}
