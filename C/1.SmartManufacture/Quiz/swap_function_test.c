/* 
  C 함수에서 실제로 매개변수 값을 변경하고 싶다면 포인터를 이용하여 Call by Reference 형태로 swap 함수를 만들면 된다.
  */

#include <stdio.h>

void swap(int *x, int *y)
{
    int temp;
    
    temp = *x;
    *x = *y;
    *y = temp;
    
    printf("(x, y) : %d, %d\n", *x, *y);
}

int main()
{
    int a = 10, b = 20;

    swap(&a, &b);
    printf("(a, b) : %d, %d\n", a, b);
    return 0;
}
