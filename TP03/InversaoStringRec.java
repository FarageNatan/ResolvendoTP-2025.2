//package TP03;


public class InversaoStringRec {
     public static String inverter(String s, int indice) {
        String resultado;
        if (indice < 0) {
            resultado = "";
        } else {
            resultado = s.charAt(indice) + inverter(s, indice - 1);
        }
        return resultado;
    }

    public static void main(String[] args) {
        String palavra = MyIO.readLine();
        while(palavra.length() != 3 || palavra.charAt(0) != 'F' || palavra.charAt(1) != 'I' || palavra.charAt(2) != 'M'){
            String palavraModificada = inverter(palavra, palavra.length() - 1);
            MyIO.println(palavraModificada);
            palavra = MyIO.readLine();
        }
    }
}
