#include <stdio.h>

struct list
{
    int num;
    struct list *next;
};

int main()
{
    struct list a = {10, NULL};
    struct list b = {20, NULL};
    struct list c = {30, NULL};
    struct list *head = NULL;
    struct list *current = NULL;

    head = &a;
    a.next = &b;
    b.next = &c;
    current = head;

    //링크드리스트(LinkedList를 순환해 보자.)
    while(current != NULL){
        printf("%d\n", current->num);
        current = current->next;
    }

    return 0;
}
