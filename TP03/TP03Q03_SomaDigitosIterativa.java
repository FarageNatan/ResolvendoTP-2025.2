//package TP03;

import java.util.Scanner;

public class TP03Q03_SomaDigitosIterativa {
    static Scanner sc = new Scanner(System.in);

    public static boolean ehIgual(String s1, String s2) {
        boolean resp = true;

        if (s1.length() != s2.length()) {
            resp = false;
        } else {
            for (int i = 0; i < s1.length(); i++) {
                if (s1.charAt(i) != s2.charAt(i)) {
                    resp = false;
                }
            }
        }
        return resp;
    }

    public static int somaDigitos(String numero) {
        int soma = 0;
        for (int i = 0; i < numero.length(); i++) {
            char digitoChar = numero.charAt(i);
            soma = soma + (digitoChar - '0');
        }
        return soma;
    }

    public static void main(String[] args) {
        String entrada;
        do {
            entrada = sc.nextLine();
            if (!ehIgual(entrada, "FIM")) {
                int resultado = somaDigitos(entrada);
                System.out.println(resultado);
            }
        } while (!ehIgual(entrada, "FIM"));
    }
}
