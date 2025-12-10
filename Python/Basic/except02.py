
try:
    with open('abc.txt', 'r') as file :
        text = file.read()
except:
    print('파일이 경로에 없습니다.')
else:
    print('정상적이라면 여기에 코드가 실행됩니다')
finally:
    print('프로그램을 안전하게 종료합니다.')
