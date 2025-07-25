#include <stdio.h>

int main()
{
	//쓰기먼저!!
	FILE* fwp, * frp;
	char str[80] = "hello world~!";
	fwp = fopen("C:\\Temp\\hello.txt", "w");
	if (fwp == NULL) {
		printf("파일이 없습니다.\n");
		exit(1);
	}
	int i = 0;
	while (str[i] != '\0') {
		fputc(str[i], fwp);
		i++;
	}
	fclose(fwp);

	//읽어서 콘솔에 출력
	frp = fopen("c:\\Temp\\hello.txt", "r");
	if (frp == NULL) {
		printf("파일 읽기에 문제가 발생했습니다.\n");
		exit(1);
	}
	char ch;
	while (1) {
		ch = fgetc(frp);
		if (ch == EOF)
			break;
		putchar(ch);
	}
	fclose(frp);

	return 0;
}
