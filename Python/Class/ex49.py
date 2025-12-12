from abc import ABC, abstractmethod

class Vehicle(ABC):
    @abstractmethod
    def run(self):
        pass
    def __str__(self):
        return 'Vehicle __str__'
    

class Taxi(Vehicle):
    def run(self):
        print('Taxi 달리다')
    def __str__(self):
        return '자식 클래스 Taxi'
    
class Bus(Vehicle):
    def run(self):
        print('Bus 달리다')
    def __str__(self):
        return 'Bus __str__'
    
class Truck(Vehicle):
    def run(self):
        print('Truck 달리다')
    def __str__(self):
        return 'Truck __str__'

# vehicle = Vehicle()
taxi = Taxi()
print(taxi)
bus = Bus()
bus.run()
print(bus)
truck = Truck()
truck.run()
print(truck)
