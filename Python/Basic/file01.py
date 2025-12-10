file = open('basic.txt', 'w')
print(type(file))
file.write('오늘 점심 뭐먹지??')
file.close()

#with를 사용하여 close()를 호출
with open('basic2.txt', 'w') as file2:
    file2.write('오늘 저녁은 치킨이다~!')
