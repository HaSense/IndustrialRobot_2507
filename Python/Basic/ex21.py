example_dict = {
    "키A":"값A",
    "키B":"값B",
    "키C":"값C"
}

for key in example_dict: #value, value
    print('{} : {}'.format(example_dict[key], example_dict.get(key)))
    #존재하지 않는 키가 있을 시 현재 코드는 에러가 발생할 수 있다.

print()
for key, value in example_dict.items(): #조회는 items()를 사용하세요.
    print('{} : {}'.format(key, value))
