class Student:
    def __init__(self, name, korean, math, english, science):
        self.__name = name
        self.__korean = korean
        self.__math = math
        self.__english = english
        self.__science = science

    #총점
    def get_sum(self):
        return self.__korean + self.__math + \
            self.__english + self.__science
    #평균
    def get_average(self):
        return self.get_sum() / 4  #함수이름앞에 self 빠뜨리기 쉬움
        
    #출력을 위한 문자열 문들기
    def to_string(self):
        return "{}\t{}\t{}".format(self.__name, \
                    self.get_sum(), self.get_average())

#main함수 부분입니다.
if __name__ == '__main__':

    students = [
        Student('홍길동', 100, 100, 100, 100),
        Student('이순신', 99, 99, 99, 99),
        Student('강감찬', 97, 97, 97, 97),
        Student('을지문', 95, 95, 95, 95),
        Student('연개소', 85, 85, 85, 85)
    ]

    print('이름', '총점', '평균', sep='\t')
    for person in students:
        print(person.to_string())
