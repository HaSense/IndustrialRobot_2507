import math

class Circle:
    def __init__(self, radius=5):
        self.__radius = radius

    @property
    def radius(self):
        return self.__radius
    @radius.setter
    def radius(self, value):
        if value <= 0:
            raise TypeError('길이는 양의 숫자여야 합니다.')
        self.__radius = value

if __name__ == '__main__':
    circle = Circle()
    #circle.__radius = 7 #에러나 나면 ok
    circle.radius = 8
    print(circle.radius)
    
