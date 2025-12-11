class Person:
    def __init__(self, name):
        self.__name = name
        # _언더바 한개 protected, __언더바 두개는 private, 없으면 public

    #Getter
    def getName(self):
        return self.__name
    #Setter
    def setName(self, name):
        self.__name = name

#main
soonsin = Person('이순신')
print(soonsin.getName())
soonsin.setName('강감찬')
print(soonsin.getName())
