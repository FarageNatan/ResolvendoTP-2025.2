#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

// Função recursiva para verificar se é palíndromo
bool isPalindromoRec(char palavra[], int inicio, int final) {
    if (inicio >= final) {
        return true;
    }
    if (palavra[inicio] != palavra[final]) {
        return false;
    }
    return isPalindromoRec(palavra, inicio + 1, final - 1);
}

int main() {
    char palavra[1000];
    scanf(" %[^\n]", palavra);
    while (!(strlen(palavra) == 3 && palavra[0] == 'F' && palavra[1] == 'I' && palavra[2] == 'M')) {
        bool palindromo = isPalindromoRec(palavra, 0, strlen(palavra) - 1); 
        if (palindromo) {
            printf("SIM\n");
        } else {
            printf("NAO\n");
        }
        scanf(" %[^\n]", palavra);
    }
    return 0;
}
