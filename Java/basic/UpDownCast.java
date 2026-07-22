class Car{
    public void run(){
        System.out.println("차가 달린다.");
    }
}
class Truk extends Car{
    @Override
    public void run() {
        System.out.println("트럭이 달린다.");
    }
}
class Taxi extends Car{
    @Override
    public void run() {
        System.out.println("택시가 달린다.");
    }
}
class Bus extends Car{
    @Override
    public void run() {
        System.out.println("버스가 달린다.");
    }
}

public class UpDownCast {
    public static void main(String[] args) {
        Car[] car = new Car[3]; //java 객체 배열 생성
        car[0] = new Truk(); //업캐스팅
        car[1] = new Taxi(); //업캐스팅
        car[2] = new Bus(); //업캐스팅       

         Truk truk = (Truk)car[0];  //다운캐스팅
         truk.run();
        System.out.println();

        for(int i=0; i<car.length; i++){    //c++은 가상함수(virtual)
            car[i].run();           //java는 오버라이딩되면 사용가능
        }

    }
}
