class Car:
    def __init__(self, brand):
        self.__brand = brand
    
    # def get_brand(self):
    #     return self.brand
    @property
    def brand(self):
        return self.__brand
    # def set_brand(self, brand):
    #     self.brand = brand
    @brand.setter
    def brand(self, brand):
        self.__brand = brand    
    

if __name__ == '__main__':
    car = Car('차')
    print(car.brand)
    car.brand = '현대'
    print(car.brand)

#1.getter, setter를 먼저 만들어보고,
#2.property 방식으로 변경해 보아라.
