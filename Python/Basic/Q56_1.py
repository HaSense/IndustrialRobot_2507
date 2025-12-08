op = input('연산자:')
a = int(input('a:'))    #정수입력
b = int(input('b:'))

if op == '+':       #파이썬은 문자열 비교 가능
    print('결과 : ', a + b) #연산
elif op == '-':
    print('결과 : ', a - b)
elif op == '*':
    print('결과 : ', a * b)
elif op == '/':
    print('결과 : ', a * b)
else:
    print("원하는 연산자가 없습니다. 다시 시작하세요.")
