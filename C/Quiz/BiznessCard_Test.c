#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>   // malloc, free, exit

#define MAX_CARDS   100
#define NAME_LEN    30
#define PHONE_LEN   20
#define COMPANY_LEN 40
#define EMAIL_LEN   40
#define FILENAME    "biznesscards.txt"

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char company[COMPANY_LEN];
    char email[EMAIL_LEN];
    char* info;   // 동적 문자열
} BizCard;

BizCard cards[MAX_CARDS];
int card_count = 0;

// ----------------- 유틸리티 함수 -----------------

// 입력 버퍼에 남은 '\n' 제거용
void clear_input_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ; // 버퍼 비우기
    }
}

// 문자열 입력용 (엔터까지 읽고 '\n' 제거)
void input_string(const char* prompt, char* buffer, int size)
{
    printf("%s", prompt);
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';   // '\n' 제거
    }
}

// 공백 입력(엔터만)인지 확인
int is_empty(const char* s)
{
    return (s[0] == '\0');
}

// info 메모리 전체 해제
void free_all_infos()
{
    for (int i = 0; i < card_count; i++) {
        if (cards[i].info != NULL) {
            free(cards[i].info);
            cards[i].info = NULL;
        }
    }
}

// ----------------- 파일 입출력 -----------------

void load_from_file()
{
    FILE* fp = fopen(FILENAME, "r");
    if (fp == NULL) {
        // 처음 실행할 때 파일이 없어도 에러 아님
        return;
    }

    char line[512];

    while (fgets(line, sizeof(line), fp) != NULL) {
        if (card_count >= MAX_CARDS) {
            printf("파일에 더 많은 데이터가 있으나, 메모리 한계로 일부만 로드되었습니다.\n");
            break;
        }

        // 줄 끝의 개행 제거
        line[strcspn(line, "\n")] = '\0';

        // 토큰 분리
        char* token;

        token = strtok(line, "|");
        if (token == NULL) continue;
        strncpy(cards[card_count].name, token, NAME_LEN);
        cards[card_count].name[NAME_LEN - 1] = '\0';

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy(cards[card_count].phone, token, PHONE_LEN);
        cards[card_count].phone[PHONE_LEN - 1] = '\0';

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy(cards[card_count].company, token, COMPANY_LEN);
        cards[card_count].company[COMPANY_LEN - 1] = '\0';

        token = strtok(NULL, "|");
        if (token == NULL) continue;
        strncpy(cards[card_count].email, token, EMAIL_LEN);
        cards[card_count].email[EMAIL_LEN - 1] = '\0';

        // info (5번째 필드, 없을 수도 있음)
        token = strtok(NULL, "|");
        if (token != NULL) {
            cards[card_count].info = (char*)malloc(strlen(token) + 1);
            if (cards[card_count].info == NULL) {
                printf("메모리 할당 실패\n");
                fclose(fp);
                exit(1);
            }
            strcpy(cards[card_count].info, token);
        }
        else {
            // 이전 포맷(4개 필드) 대비: 빈 문자열로 할당
            cards[card_count].info = (char*)malloc(1);
            if (cards[card_count].info == NULL) {
                printf("메모리 할당 실패\n");
                fclose(fp);
                exit(1);
            }
            cards[card_count].info[0] = '\0'; //비워둘때
        }

        card_count++;
    }

    fclose(fp);
}

void save_to_file()
{
    FILE* fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("파일을 열 수 없습니다: %s\n", FILENAME);
        return;
    }

    for (int i = 0; i < card_count; i++) {
        const char* info_str = (cards[i].info != NULL) ? cards[i].info : "";
        fprintf(fp, "%s|%s|%s|%s|%s\n",
            cards[i].name,
            cards[i].phone,
            cards[i].company,
            cards[i].email,
            info_str);
    }

    fclose(fp);
}

// ----------------- 기능 함수 -----------------

void print_all_cards()
{
    printf("\n[명함 목록]\n\n");

    if (card_count == 0) {
        printf("등록된 명함이 없습니다.\n");
        printf("계속하려면 엔터를 누르세요...");
        getchar();
        return;
    }

    printf("번호  이름                       전화번호           회사                         이메일                        추가정보\n");
    printf("--------------------------------------------------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < card_count; i++) {

        const char* info_str;

        if (cards[i].info != NULL) {
            info_str = cards[i].info;
        }
        else {
            info_str = "";
        }

        printf("%-4d %-25s %-15s %-28s %-30s %-s\n",
            i,
            cards[i].name,
            cards[i].phone,
            cards[i].company,
            cards[i].email,
            info_str
        );
    }

    printf("\n(총 %d명)\n", card_count);
    printf("계속하려면 엔터를 누르세요...");
    getchar();
}

void add_card()
{
    if (card_count >= MAX_CARDS) {
        printf("더 이상 명함을 추가할 수 없습니다 (최대 %d명).\n", MAX_CARDS);
        printf("계속하려면 엔터를 누르세요...");
        getchar();
        return;
    }

    BizCard new_card;
    char temp[256];

    printf("\n[명함 추가]\n\n");

    input_string("이름 입력 : ", new_card.name, NAME_LEN);
    input_string("전화번호 입력 : ", new_card.phone, PHONE_LEN);
    input_string("회사 입력 : ", new_card.company, COMPANY_LEN);
    input_string("이메일 입력 : ", new_card.email, EMAIL_LEN);
    input_string("추가 정보 입력 : ", temp, sizeof(temp));

    new_card.info = (char*)malloc(strlen(temp) + 1);
    if (new_card.info == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    strcpy(new_card.info, temp);

    cards[card_count] = new_card;
    card_count++;

    printf("\n=> 명함이 추가되었습니다.\n");
    printf("계속하려면 엔터를 누르세요...");
    getchar();
}

void edit_card()
{
    if (card_count == 0) {
        printf("\n수정할 명함이 없습니다.\n");
        printf("계속하려면 엔터를 누르세요...");
        getchar();
        return;
    }

    int index;
    char temp[256];

    printf("\n[명함 수정]\n\n");
    printf("수정할 명함 번호 입력 (0 ~ %d): ", card_count - 1);
    if (scanf("%d", &index) != 1) {
        printf("잘못된 입력입니다.\n");
        clear_input_buffer();
        printf("계속하려면 엔터를 누르세요...");
        getchar();
        return;
    }
    clear_input_buffer();

    if (index < 0 || index >= card_count) {
        printf("존재하지 않는 번호입니다.\n");
        printf("계속하려면 엔터를 누르세요...");
        getchar();
        return;
    }

    BizCard* c = &cards[index];

    printf("\n현재 정보\n");
    printf("이름       : %s\n", c->name);
    printf("전화번호   : %s\n", c->phone);
    printf("회사       : %s\n", c->company);
    printf("이메일     : %s\n", c->email);
    printf("추가 정보  : %s\n\n", (c->info != NULL) ? c->info : "");

    printf("새 값을 입력하지 않고 엔터만 치면 기존 값 유지\n\n");

    // 이름
    input_string("새 이름 입력 : ", temp, sizeof(temp));
    if (!is_empty(temp)) {
        strncpy(c->name, temp, NAME_LEN);
        c->name[NAME_LEN - 1] = '\0';
    }

    // 전화번호
    input_string("새 전화번호 입력 : ", temp, sizeof(temp));
    if (!is_empty(temp)) {
        strncpy(c->phone, temp, PHONE_LEN);
        c->phone[PHONE_LEN - 1] = '\0';
    }

    // 회사
    input_string("새 회사 입력 : ", temp, sizeof(temp));
    if (!is_empty(temp)) {
        strncpy(c->company, temp, COMPANY_LEN);
        c->company[COMPANY_LEN - 1] = '\0';
    }

    // 이메일
    input_string("새 이메일 입력 : ", temp, sizeof(temp));
    if (!is_empty(temp)) {
        strncpy(c->email, temp, EMAIL_LEN);
        c->email[EMAIL_LEN - 1] = '\0';
    }

    // info
    input_string("새 추가 정보 입력 : ", temp, sizeof(temp));
    if (!is_empty(temp)) {
        if (c->info != NULL) {
            free(c->info);
        }
        c->info = (char*)malloc(strlen(temp) + 1);
        if (c->info == NULL) {
            printf("메모리 할당 실패\n");
            exit(1);
        }
        strcpy(c->info, temp);
    }

    printf("\n=> 수정이 완료되었습니다.\n");
    printf("계속하려면 엔터를 누르세요...");
    getchar();
}

void delete_card(void)
{
    if (card_count == 0) {
        printf("\n삭제할 명함이 없습니다.\n");
        printf("계속하려면 엔터를 누르세요...");
        getchar();
        return;
    }

    int index;
    char answer;

    printf("\n[명함 삭제]\n\n");
    printf("삭제할 명함 번호 입력 (0 ~ %d): ", card_count - 1);
    if (scanf("%d", &index) != 1) {
        printf("잘못된 입력입니다.\n");
        clear_input_buffer();
        printf("계속하려면 엔터를 누르세요...");
        getchar();
        return;
    }
    clear_input_buffer();

    if (index < 0 || index >= card_count) {
        printf("존재하지 않는 번호입니다.\n");
        printf("계속하려면 엔터를 누르세요...");
        getchar();
        return;
    }

    printf("정말 삭제하시겠습니까? (y/n) : ");
    scanf("%c", &answer);
    clear_input_buffer();

    if (answer == 'y' || answer == 'Y') {

        // 지울 카드의 info 먼저 해제
        if (cards[index].info != NULL) {
            free(cards[index].info);
            cards[index].info = NULL;
        }

        // 뒤의 요소들을 한 칸씩 앞으로 당김
        for (int i = index; i < card_count - 1; i++) {
            cards[i] = cards[i + 1];
        }
        card_count--;

        printf("\n=> 삭제되었습니다.\n");
    }
    else {
        printf("\n=> 삭제를 취소했습니다.\n");
    }

    printf("계속하려면 엔터를 누르세요...");
    getchar();
}

int print_menu(void)
{
    int choice;

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

    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        return 0;  // 잘못된 입력
    }
    clear_input_buffer();
    return choice;
}

// ----------------- main -----------------

int main(void)
{
    load_from_file();

    while (1) {
        int menu = print_menu();

        switch (menu) {
        case 1:
            print_all_cards();
            break;
        case 2:
            add_card();
            break;
        case 3:
            edit_card();
            break;
        case 4:
            delete_card();
            break;
        case 5:
            save_to_file();
            free_all_infos();
            printf("\n데이터를 저장하고 프로그램을 종료합니다.\n");
            return 0;
        default:
            printf("\n잘못된 메뉴입니다. 다시 선택하세요.\n");
            break;
        }
    }

    // 논리상 도달하지 않지만 형식상 추가
    free_all_infos();
    return 0;
}
