#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[30];       // 이름
    char phone[20];      // 전화번호
    char company[40];    // 회사명
    char email[40];      // 이메일
    char info[100];      // 추가정보
} biznessCard;

int main(void)
{
    int menu = 0; 

    while (1)
    {
        printf("\n=============================\n");
        printf("   미니 명함 관리 프로그램\n");
        printf("=============================\n");
        printf("1. 명함 목록 보기\n");
        printf("2. 명함 추가\n");
        printf("3. 명함 수정\n");
        printf("4. 명함 삭제\n");
        printf("5. 저장 후 종료\n");
        printf("-----------------------------\n");
        printf("메뉴를 선택하세요: ");

        if (scanf("%d", &menu) != 1) {
            
            // 잘못된 문자 입력 처리
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF);
            printf("잘못된 입력입니다. 다시 입력해 주세요.\n");
            continue;
        }

        switch (menu)
        {
        case 1:
            // 명함 목록 보기 
            // 각 case는 직접 case 내부에 코드를 작성하거나 함수로 빼내면 됩니다.
            break;

        case 2:
            // 명함 추가
            break;

        case 3:
            // 명함 수정
            break;

        case 4:
            // 명함 삭제
            break;

        case 5:
            // 저장 후 종료
            printf("프로그램을 종료합니다.\n");
            exit(1);

        default:
            printf("잘못된 메뉴입니다. 다시 입력하세요.\n");
            break;
        }
    }

    return 0;
}
