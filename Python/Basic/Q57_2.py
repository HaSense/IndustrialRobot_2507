array = [ 
    [1, 2, 3],
    [4, 5, 6],
    [7, 8, 9]
]

sum = 0     #변수 초기화 부분이 있어야 정상동작!!!

for line in array:
    for i in line:
        sum += i

print('요소의 총 합 : ', sum)
