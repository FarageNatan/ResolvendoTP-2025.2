package TP01;

public class TP01Q03_CiframentoDeCesar {

    public static String ciframentoCesar(String palavra){
        char[] palavraModificada = new char[palavra.length()];
        for(int i = 0; i < palavra.length(); i++){
            palavraModificada[i] = (char)(palavra.charAt(i)+3);
        }
        return new String(palavraModificada);
    }

    
    public static void main(String[] args) {
        String palavra = MyIO.readLine();
        while(palavra.length() != 3 || palavra.charAt(0) != 'F' || palavra.charAt(1) != 'I' || palavra.charAt(2) != 'M'){
            String palavraModificada = ciframentoCesar(palavra);
            MyIO.println(palavraModificada);
            palavra = MyIO.readLine();
        }
    }
}
