#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
//1. return(x) , argument(x)
void myPrint() {
	printf("myPrint()\n");
}
//2. reutrn(0), argument(x)
char myChar() {
	return 'Z';
}
//3. return(x), argument(0)
void myInt(int a) {
	printf("%d\n", a);
}
//4. return(0), argument(0)
int myInt2(int b) {
	return b;
}
//5. return(0), arguemnt(0) - 다수
int sum(int a, int b, int c) {
	int result = a + b + c;
	return result;
}

int main()
{
	myPrint();
	char c = myChar();
	printf("%c\n", c);
	myInt(5550);
	int result = myInt2(7777);
	printf("%d\n", result);
	printf("%d\n", sum(100, 200, 300));

	return 0;
}
