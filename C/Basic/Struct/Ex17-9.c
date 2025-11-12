#include <stdio.h>

//자기 참조 구조체
struct list
{
    int num;
    struct list* next;
};

int main()
{
    //LinkedList
    struct list a = {10, NULL};
    struct list b = {20, NULL};
    struct list c = {30, NULL};
    struct list* head;
    struct list* current;

    head = &a;
    a.next = &b;
    b.next = &c;

    printf("head->num : %d\n", head->num);
    printf("head->next->num : %d\n", head->next->num);

    printf("list all : ");
    current = head;
    while(current != NULL){
        printf("%d  ", current->num);
        current = current->next;
    }
    printf("\n");


    return 0;
}






