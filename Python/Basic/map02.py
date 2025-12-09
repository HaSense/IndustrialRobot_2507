numbers = [1,2,3,4,5]

def func(x):
    return x * 2

#result = map(func, numbers)
result = map(lambda x: x*2, numbers)

print(list(result))
