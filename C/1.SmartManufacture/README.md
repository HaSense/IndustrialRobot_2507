# C언어 이야기

1\. 기본 자료형 (Basic Types) 

| 종류  | 자료형 | 크기  | 값의 범위 |
| --- | --- | --- | --- |
| **문자형** | `char` | 1 byte | `-128` ~ `127` |
| **정수형** | `short` | 2 bytes | `-32,768` ~ `32,767` |
|     | `int` | 4 bytes | `-2,147,483,648` ~ `2,147,483,647` |
|     | `long` | 4 bytes | `-2,147,483,648` ~ `2,147,483,647` |
|     | `long long` | 8 bytes | 약 -9 × 10¹⁸ ~ 9 × 10¹⁸ |
| **실수형** | `float` | 4 bytes | 유효숫자 6자리 |
|     | `double` | 8 bytes | 유효숫자 15자리 |


```c
//hello.c
#include <stdio.h> //헤더, 라이브러리

int main() 
{
    printf("Hello World~!\n");

    return 0;
}
```
