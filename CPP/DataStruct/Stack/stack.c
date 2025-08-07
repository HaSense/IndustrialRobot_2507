// stack_c.c
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 5

typedef struct {
    int data[MAX_SIZE];
    int top;
} Stack;

void init(Stack* s) {
    s->top = -1;
}

int isEmpty(Stack* s) {
    return s->top == -1;
}

int isFull(Stack* s) {
    return s->top == MAX_SIZE - 1;
}

void push(Stack* s, int value) {
    if (isFull(s)) {
        printf("스택이 가득 찼습니다.\n");
        return;
    }
    s->data[++(s->top)] = value;
}

int pop(Stack* s) {
    if (isEmpty(s)) {
        printf("스택이 비었습니다.\n");
        return -1;
    }
    return s->data[(s->top)--];
}

int top(Stack* s) {
    if (isEmpty(s)) {
        printf("스택이 비었습니다.\n");
        return -1;
    }
    return s->data[s->top];
}

int main() {
    Stack s;
    init(&s);
    push(&s, 10);
    push(&s, 20);
    printf("Top: %d\n", top(&s));
    printf("Pop: %d\n", pop(&s));
    printf("Pop: %d\n", pop(&s));
    printf("IsEmpty: %d\n", isEmpty(&s));
    return 0;
}
