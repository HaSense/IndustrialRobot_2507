# 4개 사칙연산 함수
def plus(a, b):
    return a+b

def minus(a, b):
    return a-b

def multiple(a, b):
    return a*b

def divide(a, b):
    if b==0 :
        print("0으로는 나눌 수 없습니다.")
        return #에러처리
    return a/b

a = int(input('a : '))
b = int(input('b : '))

print("+ : ", plus(a,b))
print("- : ", minus(a,b))
print("* : ", multiple(a,b))
print("/ : ", divide(a,b))
