# 1. 문제 1: 가상 학생 성적 CSV 파일 생성하기
# 다음 조건을 만족하는 코드를 작성하라.
# 조건
# students.csv 파일을 생성한다.
# 학생 수는 1000명으로 한다.
# 각 학생은 아래 필드를 가진다.
# id (1~1000 번호)
# name (랜덤 한글 3글자 조합)
# math 점수 (0~100 랜덤)
# english 점수 (40~100 랜덤)
# CSV 형식으로 저장한다. (콤마 구분)

# id,name,math,english
# 1,박민수,87,74
# 2,김하준,45,93

import random

familyName = list('김이박하최천방지마피')
word = list('지현숙영성호동석원구자')

with open('student.csv', 'w') as file:
    for i in range(1000):
        name = random.choice(familyName) + random.choice(word) + random.choice(word)
        math = random.randrange(00, 100)
        english = random.randrange(40, 100)
        file.write(f'{i+1}, {name}, {math}, {english}\n')
