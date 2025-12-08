sum_value = 0
limit = 10000
i = 1

for i in range(10000):
    if sum_value > 10000 :
        #print('{}번째'.format(i))
        print('{}를 더할 때 {}을 넘으며 그때의 값은 {} 입니다'.format(i-1, limit, sum_value))
        break
    sum_value += i

 

