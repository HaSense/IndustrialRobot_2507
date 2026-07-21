class Calculator 
{
    //1.멤버변수
    private int number;
    //2.생성자
    Calculator(){
        //default 생성자
    }
    //3.멤버메소드
    public int sum(int a, int b){
        return a + b;
    }
}
public class Hello 
{
    public static void main(String[] args) {
        int i = 20;
        int result;

        Calculator cal = new Calculator();

        result = cal.sum(i, 10);
        System.out.println("함수의 결과는 : " + result);
        
    }
}
