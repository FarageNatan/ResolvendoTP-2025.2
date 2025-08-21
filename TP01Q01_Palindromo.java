import java.util.Scanner;

public class TP01Q01_Palindromo{
    static Scanner sc = new Scanner(System.in);
    
    public static boolean isPalindromo(String palavra){
        boolean ehPalindromo = true;
        for (int i = 0; i < palavra.length() / 2; i++) {
            if (palavra.charAt(i) != palavra.charAt(palavra.length() - 1 - i)) { //indicie no inicio e no final, vai comparando ate os indices serem iguais
                ehPalindromo = false;
                i = palavra.length(); //para o loop sem precisar de break
            }
        }
        return ehPalindromo;
    }

    /*public static String trocaDeLetra(String palavra){ //metodo para evitar problemas com letras maiusculas e minusculas, ja que sao consideradas diferentes
        String minuscula = "";
        for (int i = 0; i < palavra.length(); i++) {
            char min = palavra.charAt(i);
            if (min >= 'A' && min <= 'Z') {
                // Converte letra maiúscula para minúscula
                min = (char)(min + 32);
            }
            minuscula += min;
        }
        return minuscula;
    }*/

    public static void main(String[] args) {
        String palavra = sc.nextLine();
        while (palavra.length() != 3 || palavra.charAt(0) != 'F' || palavra.charAt(1) != 'I' || palavra.charAt(2) != 'M'){ //repete o conjunto de instrucoes ate a palavra ser diferente de "FIM".
            boolean palindromo = isPalindromo(palavra);
            if(palindromo == false){
                System.out.println("NAO");
            } else{
                System.out.println("SIM");
            }
            palavra = sc.nextLine();
        }
    }
}