#include <stdio.h>

int main()
{
    FILE *fp;
    char line[256];

    fp = fopen("products.csv", "r");

    if (fp == NULL)
    {
        printf("파일을 열 수 없습니다.\n");
        return 1;
    }

    printf("=== CSV 내용 ===\n");

    while (fgets(line, sizeof(line), fp) != NULL)
    {
        printf("%s", line);
    }

    fclose(fp);

    return 0;
}
