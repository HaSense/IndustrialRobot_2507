class Student:
    def __init__(self, name='학생', age=18):
        self.__name = name
        self.__age = age

    @property
    def name(self):
        return self.__name
    @property
    def age(self):
        return self.__age
    @name.setter
    def name(self, value):
        self.__name = value
    @age.setter
    def age(self, value):
        self.__age = value
        
#main
hong = Student()
hong.name = '홍길동'
hong.age = 30
print(hong.name) #property 사용
print(hong.age)

