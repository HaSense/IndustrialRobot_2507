import random

seven = random.sample(range(1, 46), 7)
lotto = seven[:6]
print('로또번호 : ', *sorted(lotto)) # *unpack 기능 [] 떼어 출력
print('보너스번호 : ', seven[6])
