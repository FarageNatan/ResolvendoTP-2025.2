#include <stdio.h>
#include <stdlib.h>

int stringParaInt(char palavra[]) {
    int resultado = 0;
    int i = 0;
    while (palavra[i] != '\0') {
        if (palavra[i] >= '0' && palavra[i] <= '9') {
            resultado = resultado * 10 + (palavra[i] - '0'); //quando uma operacao numerica eh realizada, ocorre a conversao de caracter para valor
        }
        i++;
    }
    return resultado;
}

int somaDigitosRec(int valor) {
    if (valor == 0) {
        return 0;
    } else {
        return (valor % 10) + somaDigitosRec(valor / 10); //somar o último dígito (valor % 10) e depois chamar a si mesma com o restante do número (valor / 10).
    }
}

int main() {
    char palavra[1000];
    
    scanf(" %[^\n]", palavra);
    
    while (!(palavra[0] == 'F' && palavra[1] == 'I' && palavra[2] == 'M' && palavra[3] == '\0')) {
        int numero = stringParaInt(palavra);
        int soma = somaDigitosRec(numero);
        
        printf("%i\n", soma);
        
        scanf(" %[^\n]", palavra);
    }
    
    return 0;
}
