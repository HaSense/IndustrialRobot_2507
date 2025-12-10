try:
    number =  int(input('정수입력 : ')) #1.정수가 아닌값 입력시 예외발생
    if number > 0:
        raise NotImplementedError #2.number가 양수면 무조건 예외발생
    else:
        raise NotImplementedError #3.number 0이거나 음수면 무조건 예외발생
except:
    print('예외가 발생하였습니다.')
