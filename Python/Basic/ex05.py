number = input('정수 입력> ')
last_character = number[-1];

print(last_character)
print()

#짝수 조건
if last_character in "02468":
    print("짝수")

#홀수 조건
if last_character in "13579":
    print("홀수")
