#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#include <windows.h>
#endif

// 스택에 할당할 정수 개수 (실습시 값 조정 가능)
// int 크기 4바이트 기준: 250000 * 4 = 1,000,000 바이트 ≈ 1MB
// 기본 MSVC 스택(메인) 1MB를 넘기면 스택 오버플로우 발생 가능
#define STACK_ELEMS 300000  

// 힙에 할당할 정수 개수 (동일 크기 비교)
#define HEAP_ELEMS  STACK_ELEMS

// 스택 할당 시도 — 매우 큰 로컬 배열을 선언하여 스택 오버플로우를 유발하려는 목적
void try_stack_alloc(void)
{
    printf("스택에 큰 배열을 선언합니다: %d elements (약 %zu bytes)\n",
        STACK_ELEMS, (size_t)STACK_ELEMS * sizeof(int));

    // 의도적으로 큰 로컬 배열 선언 — 종종 프로그램 즉시 크래시
    // 주의: 이 함수 호출은 프로세스를 종료시킬 수 있음
    int big_array[STACK_ELEMS]; // <- stack 공간에 할당

    // 배열 초기화 (디버그 목적)
    for (int i = 0; i < STACK_ELEMS; ++i) {
        big_array[i] = i;
    }

    printf("스택 할당 성공 (예상치 못한 경우). 첫/마지막 값: %d / %d\n",
        big_array[0], big_array[STACK_ELEMS - 1]);
}

// 힙 할당 예제 — malloc 사용, 실패 검사 포함
void try_heap_alloc(void)
{
    printf("힙에서 메모리를 할당합니다: %d elements (약 %zu bytes)\n",
        HEAP_ELEMS, (size_t)HEAP_ELEMS * sizeof(int));

    int* p = (int*)malloc((size_t)HEAP_ELEMS * sizeof(int));
    if (!p) {
        fprintf(stderr, "malloc 실패: 메모리 할당 불가\n");
        return;
    }

    // 초기화 및 검증
    for (int i = 0; i < HEAP_ELEMS; ++i) p[i] = i;
    printf("힙 할당 성공. 첫/마지막 값: %d / %d\n", p[0], p[HEAP_ELEMS - 1]);

    free(p);
}

int main(int argc, char** argv)
{
    printf("테스트 프로그램 시작\n");
    printf("사용법: 실행파일 stack  --> 스택 할당 시도(크래시 가능)\n");
    printf("       실행파일 heap   --> 힙(malloc)으로 할당\n\n");

    if (argc < 2) {
        printf("옵션을 입력하세요 (stack 또는 heap)\n");
        return 0;
    }

    if (strcmp(argv[1], "stack") == 0) {
        printf("[주의] stack 옵션은 스택 오버플로우로 인해 프로그램이 비정상 종료될 수 있습니다.\n");
        try_stack_alloc();
    }
    else if (strcmp(argv[1], "heap") == 0) {
        try_heap_alloc();
    }
    else {
        printf("알 수 없는 옵션: %s\n", argv[1]);
    }

    printf("프로그램 종료\n");
    return 0;
}
