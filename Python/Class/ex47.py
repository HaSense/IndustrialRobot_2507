# 1. 생성자 
#    멤버 private id:numeric, name:str, pnumber:str
#    메소드 공부하다() : '공부를 합니다.'
#          getter / setter
#

class Student:
    def __init__(self, id=0, name='학생', pnumber='010'):
        self.__id = id      #private  _한개 protected 없으면 public
        self.__name = name
        self.__pnumber = pnumber
    
    def study(self):
        print('공부하다')
    #getter
    def getId(self):
        return self.__id
    def getName(self):
        return self.__name
    def getPnumber(self):
        return self.__pnumber
    #setter
    def setId(self, id):
        self.__id = id
    def setName(self, name):
        self.__name = name
    def setPnumber(self, pnumber):
        self.__pnumber = pnumber
          

chulsoo = Student(1, '철수', '010-1111-1111')
chulsoo.study()
print(chulsoo.getName())
chulsoo.setName('철얼~~쑤')
print(chulsoo.getName())

younghee = Student()
print(younghee.getName())
