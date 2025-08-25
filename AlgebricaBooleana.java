import java.util.Scanner;

public class AlgebricaBooleana {

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        int numEntradas;
        
        numEntradas = scanner.nextInt();
        while (numEntradas != 0) {
        
            // Após ler o número, a chamada para nextLine() consome o resto da linha,
            // que contém os valores e a expressão.
            String restoDaLinha = scanner.nextLine();
            
            // Constrói a string completa para ser processada, adicionando o número
            // de entradas no início.
            String entradaCompleta = numEntradas + restoDaLinha;

            // Chama o método para avaliar a expressão
            boolean resultado = avaliarExpressaoBooleana(entradaCompleta);
            
            if (resultado) {
                System.out.println("1");
            } else {
                System.out.println("0");
            }
            numEntradas = scanner.nextInt();
        }
        
        scanner.close();
    }

    public static boolean avaliarExpressaoBooleana(String entrada) {
        int numEntradas = entrada.charAt(0) - '0';
        boolean[] valores = new boolean[numEntradas];
        
        //pula a quantidade de entradas e o espaço
        int proximoValor = 2; 

        for (int i = 0; i < numEntradas; i++) {
            valores[i] = entrada.charAt(proximoValor) == '1';
            proximoValor += 2;
        }

        //considera apenas a expressao booleana da string
        int inicioExpressao = proximoValor;
        String expressao = "";
        for (int i = inicioExpressao; i < entrada.length(); i++) {
            expressao += entrada.charAt(i);
        }

        return resolver(expressao, valores);
    }

    private static boolean resolver(String expressao, boolean[] valores) {
        // Substitui as variáveis de entrada pelos seus valores binários
        String temp = "";
        for (int i = 0; i < expressao.length(); i++) {
            char c = expressao.charAt(i);
            if (c >= 'A' && c <= 'Z') {
                int indice = c - 'A';
                temp += (valores[indice] ? '1' : '0');
            } else {
                temp += c;
            }
        }
        expressao = temp;

        // Loop para resolver a expressão iterativamente de dentro para fora
        while (expressao.length() > 1) {
            int posFim = -1;
            for (int i = 0; i < expressao.length(); i++) {
                if (expressao.charAt(i) == ')') {
                    posFim = i;
                    break;
                }
            }
            if (posFim == -1) break; // Expressoes simples como "1" ou "0"

            int posInicio = -1;
            for (int i = posFim - 1; i >= 0; i--) {
                if (expressao.charAt(i) == '(') {
                    posInicio = i;
                    break;
                }
            }

            // A string da sub-expressão a ser avaliada
            String subExpressao = "";
            for (int i = posInicio + 1; i < posFim; i++) {
                subExpressao += expressao.charAt(i);
            }

            // Avalia a sub-expressão
            boolean resultadoSub = avaliarSubExpressao(subExpressao);

            // Reconstrói a string da expressão com o resultado
            String parteAntes = "";
            for (int i = 0; i < posInicio; i++) {
                parteAntes += expressao.charAt(i);
            }

            String parteDepois = "";
            for (int i = posFim + 1; i < expressao.length(); i++) {
                parteDepois += expressao.charAt(i);
            }

            expressao = parteAntes + (resultadoSub ? '1' : '0') + parteDepois;
        }

        // Retorna o resultado final
        return expressao.charAt(0) == '1';
    }

    private static boolean avaliarSubExpressao(String subExpressao) {
        if (subExpressao.charAt(0) == 'a' && subExpressao.charAt(1) == 'n' && subExpressao.charAt(2) == 'd') {
            // AND
            for (int i = 4; i < subExpressao.length(); i += 2) {
                if (subExpressao.charAt(i) == '0') {
                    return false;
                }
            }
            return true;
        } else if (subExpressao.charAt(0) == 'o' && subExpressao.charAt(1) == 'r') {
            // OR
            for (int i = 3; i < subExpressao.length(); i += 2) {
                if (subExpressao.charAt(i) == '1') {
                    return true;
                }
            }
            return false;
        } else if (subExpressao.charAt(0) == 'n' && subExpressao.charAt(1) == 'o' && subExpressao.charAt(2) == 't') {
            // NOT
            return subExpressao.charAt(4) == '0';
        }
        return false; // Caso inválido
    }
}