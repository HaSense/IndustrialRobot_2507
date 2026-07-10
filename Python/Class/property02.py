class Student:
    def __init__(self, name):
        self.__name = name

    #property부분을 주석처리한 후 비교해 보세요.
    # @property
    # def name(self):
    #     print('안녕하세요')
    #     return self.__name
    # @name.setter
    # def name(self, value):
    #     #암호화, 압축, 보안, 동기화, 로그
    #     self.__name = value

s = Student('학생')
s.name = '홍길동'
print(s.name)
print(s.__dict__)


#property를 이용하지 않으면 name멤버변수가 추가되는 형태 실수하기 쉽다.
#property를 추가해야 private멤버를 일반멤버처럼 사용할 수 있다.
#property를 사용하면 public멤버에 직접접근하는 형태로 보이지만 실제로는 메소드를 이용한 간접접근 형태로 만들어준다.
