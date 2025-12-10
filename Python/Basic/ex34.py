import random

#0.0 ~ 1.0 사이 float값을 리턴합니다.
print(random.random())

#uniform(min, max) : 지정된 범위 사이의 float리턴
print(random.uniform(10, 20))

#randrange() : 지정한 범위의 int를 리턴합니다.
print(random.randrange(10))

#choice() : 리스트 내부에 있는 요소를 랜덤하게 출력
print(random.choice([1,2,3,4,5]))

#shuffle() : 리스트 요소들을 섞습니다.
alist = [1,2,3,4,5]
random.shuffle(alist)
print(alist)

#sample() : 리스트의 요소 중에 k개를 뽑습니다.
blist = [1,2,3,4,5,6,7,8,9]
print(random.sample(blist, k=2))
