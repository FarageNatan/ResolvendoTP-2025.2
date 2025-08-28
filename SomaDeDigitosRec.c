#include <stdio.h>
#include <stdlib.h>

// int somaDigitosRec(int numero){
//     int soma = 0;
//     if(numero / 10 == 0){
//         soma += numero;
//     } else{
//         soma += numero % 10 + somaDigitosRec((numero / 10));
//     }
//     return soma;
// }


// int main(){
//     int numero = 0, soma = 0;
//     scanf("%i", &numero);
//     soma = somaDigitosRec(numero);
//     printf("%i", soma);


//     return 0;
// }

int contarCaracteres(char palavra[]) {
    int i = 0;
    while (palavra[i] != '\0') {
        i++;
    }
    return i;
}


int somaDigitosRec(char numero[], int tam, int i){
    int soma;
    if(numero[i] == '\0'){
        soma = 0;
    }else{
        
    }

}


int main(){
    char numero[1000];
    scanf(" %[^\n]", numero);
    int tamanho = contarCaracteres(numero);
    int soma = 0;
    
    while (!(numero[0] == 'F' && numero[1] == 'I' && numero[2] == 'M' && numero[3] == '\0')) {
        soma = somaDigitosRec(numero, tamanho);
        printf("%i\n", soma);
        scanf(" %[^\n]", numero);
    }
    return 0;
}