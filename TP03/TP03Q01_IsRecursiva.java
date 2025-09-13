package TP03;

import java.util.Scanner;

public class TP03Q01_IsRecursiva {
    static Scanner sc = new Scanner(System.in);

    public static boolean ehIgual(String s1, String s2) { //Metodo para conferir se a entrada eh igual FIM
        boolean resp = true;
        if (s1.length() != s2.length()) {
            resp = false;
        }
        for (int i = 0; i < s1.length(); i++) {
            if (s1.charAt(i) != s2.charAt(i)) {
                resp = false;
            }
        }
        return resp;
    }

    public static boolean ehVogal(char c) { 
        boolean resp = false;
        if(c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' || c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U'){
            resp = true;
        }
        return resp;
    }

    public static boolean ehConsoante(char c){
        boolean resp = true;
        if(c != 'a' || c != 'e' || c != 'i' || c != 'o' || c != 'u' || c != 'A' || c != 'E' || c != 'I' || c != 'O' || c != 'U'){
            resp = false;
        }
        return resp;
    }

    public static boolean ehLetra(char c) {
        boolean resp = false;
        if(c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z'){
            resp = true;
        }
        return resp;
    }

    public static boolean isSomenteConsoante(String teste) {
        return isSomenteConsoante(teste, 0);
    }

    private static boolean isSomenteConsoante(String teste, int i) {
        boolean resp = true;
        if (i < teste.length()) {
            char confere = teste.charAt(i);
            
            if (!ehLetra(confere) || ehVogal(confere)) {
                resp = false;
            } else {
                resp = isSomenteConsoante(teste, i + 1);
            }
        }
        return resp;
    }

    private static boolean isSomenteVogal(String teste, int i) {
        boolean resp = true;
        if (i < teste.length()) {
            char confere = teste.charAt(i);
            
            if (!ehLetra(confere) || ehVogal(confere)) {
                resp = false;
            } else {
                resp = isSomenteConsoante(teste, i + 1);
            }
        }
        return resp;
    }

    public static void main(String[] args) {
        String entrada;
        do {
            entrada = sc.nextLine();
            if (!ehIgual(entrada, "FIM")) {
                if (isSomenteConsoante(entrada)) {
                    System.out.println("SIM");
                } else {
                    System.out.println("NAO");
                }
            }
        } while (!ehIgual(entrada, "FIM"));
    }
}