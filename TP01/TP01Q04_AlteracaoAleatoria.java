package TP01;
import java.util.Random;
import java.util.Scanner;

public class TP01Q04_AlteracaoAleatoria {
    static Scanner sc = new Scanner(System.in);
    static Random gerador = new Random(4); // Gerador com seed fixa
    static int chamadas = 0;

    public static String alteracaoAleatoria(String palavra){
        for (int i = 0; i < chamadas * 2; i++) { //chamadas diferentes, avançando o gerador;
            gerador.nextInt();
        }

        char letraOriginal = (char) ('a' + Math.abs(gerador.nextInt()) % 26);
        char letraSubstituta = (char) ('a' + Math.abs(gerador.nextInt()) % 26);

        chamadas++;

        String resultado = "";
        for(int i = 0; i < palavra.length(); i++){
            char atual = palavra.charAt(i);
            if(atual == letraOriginal){
                resultado += letraSubstituta;
            } else {
                resultado += atual;
            }
        }
        return resultado;
    }

    public static void main(String[] args) {
        String palavra = sc.nextLine();

        while(palavra.length() != 3 || palavra.charAt(0) != 'F' || palavra.charAt(1) != 'I' || palavra.charAt(2) != 'M'){
            String palavraModificada = alteracaoAleatoria(palavra);
            System.out.println(palavraModificada);
            palavra = sc.nextLine();
        }
    }
}