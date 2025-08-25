#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool isPalindromoRec(char palavra[], int inicio, int final) {
    if (inicio >= final) {
        return true;
    }
    if (palavra[inicio] != palavra[final]) {
        return false;
    }
    return isPalindromoRec(palavra, inicio + 1, final - 1);
}

int contarCaracteres(char palavra[]) {
    int i = 0;
    while (palavra[i] != '\0') {
        i++;
    }
    return i;
}


int main() {
    char palavra[1000];
    scanf(" %[^\n]", palavra);
    int tamanho = contarCaracteres(palavra);
    while (!(palavra[0] == 'F' && palavra[1] == 'I' && palavra[2] == 'M' && palavra[3] == '\0')) {
        bool palindromo = isPalindromoRec(palavra, 0, tamanho - 1);
        if (palindromo) {
            printf("SIM\n");
        } else {
            printf("NAO\n");
        }
        scanf(" %[^\n]", palavra);
    }
    return 0;
}
