import random

def func1():
    while True:
        yield random.randint(1, 100)

int_generator = func1()

for _ in range(5):
    print(next(int_generator)) #랜덤 정수 생성기 처럼 동작
