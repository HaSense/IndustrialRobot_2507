public class ArrayTest {
    public static void main(String[] args) {
        
        int[] arr1 = new int[5]; //java 배열 선언법
        int[] arr2 = {1,2,3,4,5}; //선언과 동시 초기화

        arr1[0] = 200;
        System.out.println(arr1[0]);

        for(int i=0; i<5; i++){
            System.out.print(arr2[i] + " ");
        }
        System.out.println();
    }
}
