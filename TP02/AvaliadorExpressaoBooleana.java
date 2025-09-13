import java.util.Scanner;

public class AvaliadorExpressaoBooleana {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        String entrada = scanner.nextLine();
        while (entrada.charAt(0) == '0' && entrada.length() == 1) {
            boolean resultado = avaliarExpressao(entrada);
            System.out.println(resultado ? "1" : "0");
        }
        scanner.close();
    }

    public static boolean avaliarExpressao(String entrada) {
        // Obter número de variáveis
        int n = entrada.charAt(0) - '0';
        boolean[] valores = new boolean[n];
        
        // Extrair valores das variáveis
        for (int i = 0; i < n; i++) {
            valores[i] = entrada.charAt(i * 2 + 2) == '1';
        }
        
        // Extrair expressão
        int inicioExpr = n * 2 + 2;
        int tamanhoExpr = entrada.length() - inicioExpr;
        char[] expressao = new char[tamanhoExpr];
        for (int i = 0; i < tamanhoExpr; i++) {
            expressao[i] = entrada.charAt(i + inicioExpr);
        }
        
        // Pilhas para valores e operadores
        int maxTamanho = tamanhoExpr;
        boolean[] pilhaValores = new boolean[maxTamanho];
        int topoValores = -1;
        char[] pilhaOperadores = new char[maxTamanho];
        int topoOperadores = -1;
        
        int i = 0;
        while (i < tamanhoExpr) {
            char c = expressao[i];
            
            // Ignorar espaços e vírgulas
            if (c == ' ' || c == ',') {
                i++;
                continue;
            }
            
            // Lidar com variáveis (A, B, C, etc.)
            if (c >= 'A' && c <= 'Z') {
                topoValores++;
                if (topoValores >= maxTamanho) return false;
                pilhaValores[topoValores] = valores[c - 'A'];
                i++;
                continue;
            }
            
            // Lidar com parênteses e operadores
            if (c == '(') {
                topoOperadores++;
                if (topoOperadores >= maxTamanho) return false; // Proteção contra estouro
                pilhaOperadores[topoOperadores] = c;
            } else if (c == ')') {
                while (topoOperadores >= 0 && pilhaOperadores[topoOperadores] != '(') {
                    if (topoValores < 1 && pilhaOperadores[topoOperadores] != '!') return false; // Insuficiência de valores
                    realizarOperacao(pilhaValores, pilhaOperadores, topoValores, topoOperadores);
                    topoValores--;
                    topoOperadores--;
                }
                if (topoOperadores >= 0) topoOperadores--; // Remove '('
            } else if (c == 'a' || c == 'o' || c == 'n') {
                char opChar = identificarOperador(expressao, i, tamanhoExpr);
                int opLength = (opChar == '&') ? 3 : (opChar == '|') ? 2 : 3;
                i += opLength - 1; // Ajustar para o próximo caractere após o operador
                
                while (topoOperadores >= 0 && pilhaOperadores[topoOperadores] != '(' && 
                       temMaiorPrecedencia(pilhaOperadores[topoOperadores], opChar)) {
                    if (topoValores < 1 && pilhaOperadores[topoOperadores] != '!') return false; // Insuficiência de valores
                    realizarOperacao(pilhaValores, pilhaOperadores, topoValores, topoOperadores);
                    topoValores--;
                    topoOperadores--;
                }
                topoOperadores++;
                if (topoOperadores >= maxTamanho) return false; // Proteção contra estouro
                pilhaOperadores[topoOperadores] = opChar;
            }
            i++;
        }
        
        // Avaliar operações restantes
        while (topoOperadores >= 0) {
            if (topoValores < 1 && pilhaOperadores[topoOperadores] != '!') return false; // Insuficiência de valores
            realizarOperacao(pilhaValores, pilhaOperadores, topoValores, topoOperadores);
            topoValores--;
            topoOperadores--;
        }
        
        return topoValores == 0 ? pilhaValores[0] : false;
    }

    private static void realizarOperacao(boolean[] pilhaValores, char[] pilhaOperadores, int topoValores, int topoOperadores) {
        char operador = pilhaOperadores[topoOperadores];
        
        if (operador == '!') {
            pilhaValores[topoValores] = !pilhaValores[topoValores];
        } else {
            boolean valor2 = pilhaValores[topoValores];
            boolean valor1 = pilhaValores[topoValores - 1];
            if (operador == '&') {
                pilhaValores[topoValores - 1] = valor1 && valor2;
            } else if (operador == '|') {
                pilhaValores[topoValores - 1] = valor1 || valor2;
            }
        }
    }

    private static boolean temMaiorPrecedencia(char op1, char op2) {
        if (op1 == '!') return true;
        if (op2 == '!') return false;
        if (op1 == '&' && op2 == '|') return true;
        return false;
    }

    private static char identificarOperador(char[] expressao, int pos, int tamanho) {
        if (pos + 2 < tamanho && expressao[pos] == 'a' && expressao[pos + 1] == 'n' && expressao[pos + 2] == 'd') {
            return '&';
        } else if (pos + 1 < tamanho && expressao[pos] == 'o' && expressao[pos + 1] == 'r') {
            return '|';
        } else {
            return '!';
        }
    }
}