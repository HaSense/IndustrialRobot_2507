numbers = [1,2,3,4,5]

# def is_odd(x):
#     return x % 2 != 0
#filter(함수, 리스트)  #함수부분을 람다식으로 표현가능

result = filter(lambda x : x%2 != 0, numbers)
# result = filter(is_odd, numbers)
print(list(result))
