#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main()
{
	char str[80];
	scanf("%s", str);
	int cntBig = 0, cntSmall = 0, cntNumber = 0, cntSpecial = 0;

	for (int i = 0; i < strlen(str); i++) {
		if ((str[i] >= 'A') && (str[i] <= 'Z')) cntBig++;
		else if ((str[i] >= 'a') && (str[i] <= 'z')) cntSmall++;
		else if ((str[i] >= '0') && (str[i] <= '9')) cntNumber++;
		else cntSpecial++;
	}
	printf("대문자 : %d\n", cntBig);
	printf("소문자 : %d\n", cntSmall);
	printf("숫자 : %d\n", cntNumber);
	printf("특수문자 : %d\n", cntSpecial);

	return 0;
}
