//quiz_file1.c
/*
   파일을 먼저 읽고 그 다음 파일을 만들어서 써봅시다.
   read.txt  "I CAN DO IT !!"
   write.txt "I CAN DO IT !!"
*/

#include <stdio.h>
int main()
{
    //1. 파일 읽기
    FILE* fin;
    int ch;
    char str[80]; //읽어온 내용을 저장할 곳
    fin = fopen("read.txt", "r");
    int i=0;
    while( (ch=fgetc(fin)) != EOF ){
        //printf("%c", ch);
        str[i++] = (char)ch;
    }
    str[i] = '\0';
    fclose(fin);

    //2. 파일 쓰기
     FILE* fout;
     fout = fopen("write.txt", "w");
     fprintf(fout, "%s\n", str);   
     fclose(fout);

    return 0;
}
