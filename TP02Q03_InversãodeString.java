import java.util.Scanner;

public class TP02Q03_InversãodeString {
    static Scanner sc = new Scanner(System.in);
    
    public static String stringInvertida(String palavra){
        char[] palavraInvertida = new char[palavra.length()];
        for(int i = 0; i < palavra.length(); i++){
            palavraInvertida[palavra.length() - 1 - i] = palavra.charAt(i); 
        }
        return new String(palavraInvertida);
    }
    
    public static void main(String[] args) {
         String palavra = MyIO.readLine();
        while(palavra.length() != 3 || palavra.charAt(0) != 'F' || palavra.charAt(1) != 'I' || palavra.charAt(2) != 'M'){
            String palavraModificada = stringInvertida(palavra);
            MyIO.println(palavraModificada);
            palavra = MyIO.readLine();
        }
    }
}
