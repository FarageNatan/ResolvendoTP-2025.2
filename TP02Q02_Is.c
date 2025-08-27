#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int contarCaracteres(char palavra[]) {
    int i = 0;
    while (palavra[i] != '\0') {
        i++;
    }
    return i;
}

bool somenteVogal(char palavra[], int tam){
    bool resposta = true;
    for(int i = 0; i < tam; i++){
        if(palavra[i] != 'a' && palavra[i] != 'e' && palavra[i] != 'i' && palavra[i] != 'o' && palavra[i] != 'u' && palavra[i] != 'A' && palavra[i] != 'E' && palavra[i] != 'I' && palavra[i] != 'O' && palavra[i] != 'U'){
            resposta = false;
            i = tam;
        }
    }
    return resposta;
}

bool somenteConsoante(char palavra[], int tam){
    bool resposta = true;
    for(int i = 0; i < tam; i++){
        if(palavra[i] == 'a' || palavra[i] == 'e' || palavra[i] == 'i' || palavra[i] == 'o' || palavra[i] == 'u' && palavra[i] == 'A' && palavra[i] == 'E' && palavra[i] == 'I' && palavra[i] == 'O' && palavra[i] == 'U'){
            resposta = false;
            i = tam;
        }
    }
    return resposta;
}

bool ehInteiro(char palavra[], int tam){
    bool resposta = true;
    for(int i = 0; i < tam; i++){
        if(!(palavra[i] >= '0' && palavra[i] <= '9')){
            resposta = false;
            i = tam;
        }
    }
    return resposta;
}

bool ehReal(char palavra[], int tam) {
    int countPonto = 0;
    bool resposta = false;
    for (int i = 0; i < tam; i++) {
        if (palavra[i] == '.' || palavra[i] == ',') {
            countPonto++;
        }
    }
    if (countPonto == 1) {
        resposta = true;
    }
    return resposta;
}

int main(){
    char palavra[1000];
    scanf(" %[^\n]", palavra);
    int tamanho = contarCaracteres(palavra);
    while (!(palavra[0] == 'F' && palavra[1] == 'I' && palavra[2] == 'M' && palavra[3] == '\0')) {
        bool soVogal = somenteVogal(palavra, tamanho);
        bool soConsoante = somenteConsoante(palavra, tamanho);
        bool numInteiro = ehInteiro(palavra, tamanho);
        bool numReal = ehReal(palavra, tamanho);
        if (soVogal == true) {
            printf("SIM ");
        } else {
            printf("NAO ");
        }
        if (soConsoante == true) {
            printf("SIM ");
        } else {
            printf("NAO ");
        }
        if (numInteiro == true) {
            printf("SIM ");
        } else {
            printf("NAO ");
        }
        if (numReal == true) {
            printf("SIM ");
        } else {
            printf("NAO ");
        }
        printf("\n");
        scanf(" %[^\n]", palavra);
        tamanho = contarCaracteres(palavra);
    }

    return 0;
}