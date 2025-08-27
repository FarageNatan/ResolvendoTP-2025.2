// package TP01;
import java.util.Random;
import java.util.Scanner;

public class TP01Q04_AlteracaoAleatoria {
    static Scanner sc = new Scanner(System.in);
    
    public static String alteracaoAleatoria(String palavra, Random gerador){
        char[] palavraModificada = new char[palavra.length()];
        char letraOriginal = (char) ('a' + Math.abs(gerador.nextInt()) % 26);
        char letraSubstituta = (char) ('a' + Math.abs(gerador.nextInt()) % 26);

        for(int i = 0; i < palavra.length(); i++){
            char atual = palavra.charAt(i);
            if(atual == letraOriginal){
                palavraModificada[i] = letraSubstituta;
            } else {
                palavraModificada[i] = atual;
            }
        }
        return new String(palavraModificada);
    }

    public static void main(String[] args) {
        String palavra = sc.nextLine();
        Random gerador = new Random(); // Gerador com seed fixa
        gerador.setSeed(4);
        while(palavra.length() != 3 || palavra.charAt(0) != 'F' || palavra.charAt(1) != 'I' || palavra.charAt(2) != 'M'){
            String palavraModificada = alteracaoAleatoria(palavra, gerador);
            System.out.println(palavraModificada);
            palavra = sc.nextLine();
        }
    }
}