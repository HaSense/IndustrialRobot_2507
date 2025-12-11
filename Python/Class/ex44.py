from abc import ABC, abstractmethod


class Shape(ABC):
    @abstractmethod
    def draw(self):
        """도형을 그리는 추상 메소드"""
        pass


class Triangle(Shape):
    def draw(self):
        print("삼각형을 그립니다.")


class Rectangle(Shape):
    def draw(self):
        print("사각형을 그립니다.")


class Circle(Shape):
    def draw(self):
        print("원을 그립니다.")


# main 함수
if __name__ == "__main__":
    # 부모 클래스 타입으로 자식 인스턴스를 관리 (C++의 Shape* 개념)
    shapes: list[Shape] = [
        Triangle(),
        Rectangle(),
        Circle()
    ]

    # 다형성: Shape 타입으로 호출해도 실제 객체의 draw() 실행
    for shape in shapes:
        shape.draw()
