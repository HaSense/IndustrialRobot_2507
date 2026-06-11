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

**변수처리**
```c 

#include <stdio.h>

int main()
{	
	//두 정수 더하여 결과를 출력하라.
	//1.변수 
	//해법 -> 알고리즘
	int a = 10, b = 20; //변수선언
	int result; //변수선언

	//a = 10; //대입연산자
	//b = 20;

	result = a + b; //수식
	printf("result : %d\n", result);

	return 0;
}

```

**두개의 변수를 받아 넓이를 구해보자.**
```c
#include <stdio.h>

int main()
{
	int width, height;
	
	printf("width 입력 : ");
	scanf_s("%d", &width);
	printf("height 입력 : ");
	scanf_s("%d", &height);

	int area = width * height; //수식

	printf("넓이 : %d\n", area);

	return 0;
}
```

### 학습목표 : 콘솔용 수식 계산기 만들기

#### - 나이 계산기 (만나이, 한국나이)
[콘솔용][나이계산기 만들기]  
태어난 해를(2000) 입력하면  

한국나이  
만나이(연도기준) 를 출력하자  

age1 = ???   // 수식  
age2 = 2026 - 2000 //만나이  
-----------------------------------

한국나이 : 27세  
만나이 : 26세  






