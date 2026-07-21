//2차원 배열
public class ArrayTest2 {
    public static void main(String[] args) {
        int arr[][] = new int[5][5];
        int cnt = 1;

        for(int i=0; i<5; i++){
            for(int j=0; j<5; j++){
                arr[i][j] = cnt++;
                System.out.print(arr[i][j] + "\t");
            }
            System.out.println();
        }
    }
}
