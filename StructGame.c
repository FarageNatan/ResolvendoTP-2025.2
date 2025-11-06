#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_GAMES 5000
#define MAX_LINE 5000

typedef struct {
    int id;
    char name[100];
    char releaseDate[11];
    int estimatedOwners;
    float price;
    char supportedLanguages[20][30]; 
    int totalLanguages;
    int metacriticScore;
    float userScore;
    int achievements;
    char publishers[20][50];
    int totalPublishers;
    char developers[20][50];
    int totalDevelopers;
    char categories[20][50];
    int totalCategories;
    char genres[20][50];
    int totalGenres;
    char tags[50][50];
    int totalTags;

} Game;

void tratarData(char data[], char dataNormalizada[]) {
    int i = 0;
    int j = 0;
    int tamanho = strlen(data);
    int barra1 = -1;
    int barra2 = -1;
    
    // Zera o buffer
    for(j=0; j<11; j++) { dataNormalizada[j] = '\0'; }

    // Procura por '/'
    for (i = 0; i < tamanho; i++) {
        if (data[i] == '/') {
            if (barra1 == -1) {
                barra1 = i;
            } else if (barra2 == -1) {
                barra2 = i;
            }
        }
    }

    // A. Data Completa (DD/MM/YYYY)
    if (barra1 != -1 && barra2 != -1) {
        // Copia a string inteira (limitando a 10 caracteres + \0)
        for (i = 0; i < 10 && i < tamanho; i++) {
            dataNormalizada[i] = data[i];
        }
        dataNormalizada[i] = '\0';
    } 
    // B. Data Incompleta (Apenas Ano YYYY ou Mês/Ano MM/YYYY)
    else {
        // Assume '01/01/' e anexa o ano (últimos 4 caracteres)
        dataNormalizada[0] = '0';
        dataNormalizada[1] = '1';
        dataNormalizada[2] = '/';
        dataNormalizada[3] = '0';
        dataNormalizada[4] = '1';
        dataNormalizada[5] = '/';
        
        // Copia o ano. Tenta pegar os últimos 4 caracteres
        j = 6;
        i = tamanho - 4; // Começa 4 caracteres antes do fim
        
        while (i < tamanho && j < 10) {
            dataNormalizada[j++] = data[i++];
        }
        dataNormalizada[10] = '\0';
    }

    // Único return (função void)
    return; 
}

int tratarEstimatedOwners(char texto[]) {
    int i = 0, j = 0, tamanho = strlen(texto);
    char numeros[20];
    int resultado = 0;

    for (i = 0; i < tamanho; i++) {
        if (texto[i] >= '0' && texto[i] <= '9') {
            numeros[j++] = texto[i];
        }
    }
    numeros[j] = '\0';

    int valor = 0;
    for (i = 0; i < j; i++) {
        valor = valor * 10 + (numeros[i] - '0');
    }

    resultado = valor;
    return resultado;
}

float tratarPreco(char texto[]) {
    int i = 0, tamanho = strlen(texto);
    float resultado = 0.0f;

    if (strcmp(texto, "Free to Play") == 0) {
        resultado = 0.0f;
    } else {
        int ponto = -1;
        for (i = 0; i < tamanho; i++) {
            if (texto[i] == '.') {
                ponto = i;
            }
        }

        int parteInteira = 0;
        int parteDecimal = 0;
        int decimais = 0;

        for (i = 0; i < tamanho; i++) {
            if (texto[i] >= '0' && texto[i] <= '9') {
                if (ponto == -1 || i < ponto) {
                    parteInteira = parteInteira * 10 + (texto[i] - '0');
                } else {
                    parteDecimal = parteDecimal * 10 + (texto[i] - '0');
                    decimais++;
                }
            }
        }

        float divisor = 1.0f;
        for (i = 0; i < decimais; i++) {
            divisor = divisor * 10.0f;
        }

        resultado = parteInteira + (parteDecimal / divisor);
    }

    return resultado;
}

int tratarIdiomas(char texto[], char linguas[20][30]) {
    int i = 0;
    int k = 0; // Índice do caractere dentro da lingua atual
    int tamanho = strlen(texto);
    int total = 0; // Número total de linguas encontradas
    int resultado = 0;

    // A lógica de dentro só pode ser acionada se o total for menor que o limite (20)
    for (i = 0; i < tamanho; i++) {
        char c = texto[i];

        // Se for [ ou ], ignora e continua
        if (c == '[' || c == ']' || c == ' ' || c == '\'') {
            // Ignora o caractere, apenas avança i
        } 
        // Se for o separador ',' e não for a primeira lingua, finaliza a palavra atual
        else if (c == ',') {
            // Termina a lingua atual e avança para a próxima
            if (k > 0) { // Se houver caracteres capturados
                linguas[total][k] = '\0';
                total++;
                k = 0;
            }
        } 
        // É um caractere de lingua
        else {
            if (total < 20 && k < 29) { // Verifica limites
                linguas[total][k++] = c;
            }
        }
    }
    
    // Processa a última lingua (se houver)
    if (k > 0 && total < 20) {
        linguas[total][k] = '\0';
        total++;
    }

    resultado = total;
    return resultado;
}

int tratarMetacriticScore(char texto[]) {
    int resultado = -1;
    int tamanho = strlen(texto);
    int i = 0;
    int valor = 0;

    if (tamanho > 0) {
        for (i = 0; i < tamanho; i++) {
            if (texto[i] >= '0' && texto[i] <= '9') {
                valor = valor * 10 + (texto[i] - '0');
            }
        }
        resultado = valor;
    }

    return resultado;
}

float tratarUserScore(char texto[]) {
    float resultado = -1.0f;
    int tamanho = strlen(texto);
    int i = 0;

    if (tamanho > 0 && strcmp(texto, "tbd") != 0) {
        int ponto = -1;
        for (i = 0; i < tamanho; i++) {
            if (texto[i] == '.') {
                ponto = i;
            }
        }

        int parteInteira = 0;
        int parteDecimal = 0;
        int decimais = 0;

        for (i = 0; i < tamanho; i++) {
            if (texto[i] >= '0' && texto[i] <= '9') {
                if (ponto == -1 || i < ponto) {
                    parteInteira = parteInteira * 10 + (texto[i] - '0');
                } else {
                    parteDecimal = parteDecimal * 10 + (texto[i] - '0');
                    decimais++;
                }
            }
        }

        float divisor = 1.0f;
        for (i = 0; i < decimais; i++) {
            divisor = divisor * 10.0f;
        }

        resultado = parteInteira + (parteDecimal / divisor);
    }

    return resultado;
}

int tratarAchievements(char texto[]) {
    int resultado = 0;
    int tamanho = strlen(texto);
    int i = 0;

    if (tamanho > 0) {
        int valor = 0;
        for (i = 0; i < tamanho; i++) {
            if (texto[i] >= '0' && texto[i] <= '9') {
                valor = valor * 10 + (texto[i] - '0');
            }
        }
        resultado = valor;
    }

    return resultado;
}

int tratarPublishers(char texto[], char publishers[20][50]) {
    int i = 0; 
    int k = 0; // Índice do caractere dentro do publisher atual
    int tamanho = strlen(texto);
    int total = 0; // Número total de publishers encontrados
    int resultado = 0;
    
    // A lógica deve ser robusta para limpar aspas e espaços
    for (i = 0; i < tamanho; i++) {
        char c = texto[i];

        // Se for aspas simples, aspas duplas, [ ou ], ignora
        if (c == '"' || c == '[' || c == ']' || c == '\'') {
            // Ignora o caractere
        } 
        // Se for o separador ','
        else if (c == ',') {
            // Termina a palavra atual (se houver caracteres capturados)
            if (k > 0) { 
                publishers[total][k] = '\0';
                total++;
                k = 0;
            }
        } 
        // Se for um caractere normal, copia, mas ignora espaços em branco se for o primeiro
        else {
            // Se for um espaço e for o primeiro caractere, ignora. Senão, copia.
            if (!(c == ' ' && k == 0)) {
                if (total < 20 && k < 49) { // Verifica limites
                    publishers[total][k++] = c;
                }
            }
        }
    }
    
    // Processa o último item (se houver)
    if (k > 0 && total < 20) {
        publishers[total][k] = '\0';
        total++;
    }

    resultado = total;
    return resultado;
}

// **tratarDevelopers** deve ser idêntico a tratarPublishers.
// **tratarCategories/Genres/Tags** devem ser idênticos a tratarIdiomas (da correção anterior).

int tratarDevelopers(char texto[], char developers[20][50]) {
    int i = 0, j = 0, k = 0;
    int tamanho = strlen(texto);
    int total = 0;

    for (i = 0; i < tamanho; i++) {
        if (texto[i] == ',') {
            developers[total][k] = '\0';
            total++;
            k = 0;
        } else if (texto[i] != ' ') {
            developers[total][k++] = texto[i];
        }
    }

    if (k > 0) {
        developers[total][k] = '\0';
        total++;
    }

    int resultado = total;
    return resultado;
}

int tratarCategories(char texto[], char categorias[20][50]) {
    int i = 0, j = 0, k = 0;
    int dentro = 0;
    int tamanho = strlen(texto);
    int total = 0;

    for (i = 0; i < tamanho; i++) {
        if (texto[i] == '[') {
            dentro = 1;
        } else if (texto[i] == ']') {
            categorias[total][k] = '\0';
            total++;
            dentro = 0;
        } else if (dentro) {
            if (texto[i] == ',') {
                categorias[total][k] = '\0';
                total++;
                k = 0;
            } else if (texto[i] != ' ') {
                categorias[total][k++] = texto[i];
            }
        }
    }

    int resultado = total;
    return resultado;
}

int tratarGenres(char texto[], char generos[20][50]) {
    int i = 0, j = 0, k = 0;
    int dentro = 0;
    int tamanho = strlen(texto);
    int total = 0;

    for (i = 0; i < tamanho; i++) {
        if (texto[i] == '[') {
            dentro = 1;
        } else if (texto[i] == ']') {
            generos[total][k] = '\0';
            total++;
            dentro = 0;
        } else if (dentro) {
            if (texto[i] == ',') {
                generos[total][k] = '\0';
                total++;
                k = 0;
            } else if (texto[i] != ' ') {
                generos[total][k++] = texto[i];
            }
        }
    }

    int resultado = total;
    return resultado;
}

int tratarTags(char texto[], char tags[50][50]) {
    int i = 0, j = 0, k = 0;
    int dentro = 0;
    int tamanho = strlen(texto);
    int total = 0;

    for (i = 0; i < tamanho; i++) {
        if (texto[i] == '[') {
            dentro = 1;
        } else if (texto[i] == ']') {
            tags[total][k] = '\0';
            total++;
            dentro = 0;
        } else if (dentro) {
            if (texto[i] == ',') {
                tags[total][k] = '\0';
                total++;
                k = 0;
            } else if (texto[i] != ' ') {
                tags[total][k++] = texto[i];
            }
        }
    }

    int resultado = total;
    return resultado;
}

typedef struct Celula {
    Game *elemento;
    struct Celula *prox;
} Celula;

typedef struct {
    Celula *topo;
} Pilha;

void start(Pilha *p) {
    p->topo = NULL;
}

void empilhar(Pilha *p, Game *g) {
    Celula *tmp = (Celula*) malloc(sizeof(Celula));
    tmp->elemento = g;
    tmp->prox = p->topo;
    p->topo = tmp;
}

Game* desempilhar(Pilha *p) {
    Game *removido = NULL;
    if (p->topo != NULL) {
        Celula *tmp = p->topo;
        removido = tmp->elemento;
        p->topo = tmp->prox;
        free(tmp);
    }
    return removido;
}

void imprimirGame(Game *g, int indice) {
    int i = 0;
    printf("[%d] => %d ## %s ## %s ## %d ## %.2f ## ", indice, g->id, g->name, g->releaseDate, g->estimatedOwners, g->price);

    /* supportedLanguages */
    printf("[");
    for (i = 0; i < g->totalLanguages; i++) {
        printf("%s", g->supportedLanguages[i]);
        if (i < g->totalLanguages - 1) { 
            printf(", ");
        }
    }
    printf("] ## ");

    /* metacriticScore */
    printf("%d ## ", g->metacriticScore);

    /* userScore */
    printf("%.1f ## ", g->userScore);

    /* achievements */
    printf("%d ## ", g->achievements);

    /* publishers */
    printf("[");
    for (i = 0; i < g->totalPublishers; i++) {
        printf("%s", g->publishers[i]);
        if (i < g->totalPublishers - 1) {
            printf(", ");
        }
    }
    printf("] ## ");

    /* developers */
    printf("[");
    for (i = 0; i < g->totalDevelopers; i++) {
        printf("%s", g->developers[i]);
        if (i < g->totalDevelopers - 1) {
            printf(", ");
        }
    }
    printf("] ## ");

    /* categories */
    printf("[");
    for (i = 0; i < g->totalCategories; i++) {
        printf("%s", g->categories[i]);
        if (i < g->totalCategories - 1) {
            printf(", ");
        }
    }
    printf("] ## ");

    /* genres */
    printf("[");
    for (i = 0; i < g->totalGenres; i++) {
        printf("%s", g->genres[i]);
        if (i < g->totalGenres - 1) {
            printf(", ");
        }
    }
    printf("] ## ");

    /* tags */
    printf("[");
    for (i = 0; i < g->totalTags; i++) {
        printf("%s", g->tags[i]);
        if (i < g->totalTags - 1) {
            printf(", ");
        }
    }
    printf("] ##\n");
}

void mostrarRecursivoIndex(Celula *i, int *idx) {
    if (i != NULL) {
        mostrarRecursivoIndex(i->prox, idx);
        imprimirGame(i->elemento, *idx);
        *idx = *idx + 1;
    }
}

void mostrar(Pilha *p) {
    int inicio = 0;
    mostrarRecursivoIndex(p->topo, &inicio);
}

Game* lerLinhaCSV(char linha[]) {
    Game *g = (Game*) malloc(sizeof(Game));
    // Inicialização da struct
    int i = 0;
    int k = 0;

    // Zera o struct antes de preencher
    g->id = 0;
    g->name[0] = '\0';
    g->releaseDate[0] = '\0';
    g->estimatedOwners = 0;
    g->price = 0.0f;
    g->totalLanguages = 0;
    g->metacriticScore = 0;
    g->userScore = 0.0f;
    g->achievements = 0;
    g->totalPublishers = 0;
    g->totalDevelopers = 0;
    g->totalCategories = 0;
    g->totalGenres = 0;
    g->totalTags = 0;

    // Reseta campos compostos
    for (i = 0; i < 20; i++) {
        g->supportedLanguages[i][0] = '\0';
        g->publishers[i][0] = '\0';
        g->developers[i][0] = '\0';
        g->categories[i][0] = '\0';
        g->genres[i][0] = '\0';
    }
    for (i = 0; i < 50; i++) {
        g->tags[i][0] = '\0';
    }


    i = 0;
    int j = 0; // Ponteiro para o campo
    int coluna = 0;
    int tamanho = strlen(linha);
    char campo[5000];
    
    // ** VARIÁVEL CHAVE **: Rastreia se estamos dentro de aspas duplas
    // O campo 'name' e possivelmente outros campos de lista usam aspas
    int entreAspas = 0; 
    
    // Laço de leitura manual
    while (i <= tamanho) {
        
        // 1. Lógica para detectar e alternar o estado de 'entreAspas'
        // Detecta aspas no início ou final de um campo
        if (linha[i] == '"') {
            // Se encontrou aspas, alterna o estado
            // Move i para o próximo caractere antes de processar
            entreAspas = !entreAspas;
            i++; 
        } 
        
        // 2. Lógica para identificar o fim da coluna
        // Fim da coluna é: (',' E não estamos entre aspas) OU (fim da linha)
        if ((linha[i] == ',' && !entreAspas) || linha[i] == '\0' || linha[i] == '\n') {
            campo[j] = '\0';

            // 3. Preenchimento do Struct
            if (coluna == 0) {
                // ID
                int valor = 0;
                k = 0;
                // Converte string para int manualmente
                for (k = 0; k < strlen(campo); k++) {
                    if (campo[k] >= '0' && campo[k] <= '9') {
                        valor = valor * 10 + (campo[k] - '0');
                    }
                }
                g->id = valor;
            } else if (coluna == 1) {
                // NAME
                k = 0;
                // Copia o nome
                for (k = 0; k < strlen(campo); k++) {
                    g->name[k] = campo[k];
                }
                g->name[k] = '\0';
            } else if (coluna == 2) {
                tratarData(campo, g->releaseDate);
            } else if (coluna == 3) {
                g->estimatedOwners = tratarEstimatedOwners(campo);
            } else if (coluna == 4) {
                g->price = tratarPreco(campo);
            } else if (coluna == 5) {
                g->totalLanguages = tratarIdiomas(campo, g->supportedLanguages);
            } else if (coluna == 6) {
                g->metacriticScore = tratarMetacriticScore(campo);
            } else if (coluna == 7) {
                g->userScore = tratarUserScore(campo);
            } else if (coluna == 8) {
                g->achievements = tratarAchievements(campo);
            } else if (coluna == 9) {
                g->totalPublishers = tratarPublishers(campo, g->publishers);
            } else if (coluna == 10) {
                g->totalDevelopers = tratarPublishers(campo, g->developers);
            } else if (coluna == 11) {
                g->totalCategories = tratarCategories(campo, g->categories);
            } else if (coluna == 12) {
                g->totalGenres = tratarGenres(campo, g->genres);
            } else if (coluna == 13) {
                g->totalTags = tratarTags(campo, g->tags);
            }

            coluna++;
            j = 0;
            // Move para o próximo caractere, pulando o delimitador (,)
            i++; 

        } else {
            // Caractere normal, copia para o campo
            campo[j++] = linha[i++];
        }
    }
    
    return g;
}

int lerArquivoCSV(char nomeArquivo[], Game *jogos[], int maxJogos) {
    FILE *arq = fopen(nomeArquivo, "r");
    int qtd = 0;
    int resultado = 0;
    if (arq != NULL) {
        char linha[MAX_LINE];
        fgets(linha, MAX_LINE, arq); // pula o cabeçalho

        while (fgets(linha, MAX_LINE, arq) != NULL && qtd < maxJogos) {
            jogos[qtd] = lerLinhaCSV(linha);
            qtd++;
        }

        fclose(arq);
        resultado = qtd;
    }
    return resultado;
}

int main() {
    Game* jogos[MAX_GAMES];
    int n = lerArquivoCSV("/tmp/games.csv", jogos, MAX_GAMES);
    //int n = lerArquivoCSV("C:/Users/natan/OneDrive/Documentos/ResolvendoTP/games.csv", jogos, MAX_GAMES);

    Pilha pilha;
    start(&pilha);

    char entrada[100];
    int i = 0;
    int id = 0;
    bool fim = false;

    // Parte 1: leitura dos IDs até "FIM"
    while (!fim && scanf("%s", entrada) != EOF) {
        if (strcmp(entrada, "FIM") == 0) {
            fim = true;
        } else {
            id = 0;
            int j = 0;
            int tamanho = strlen(entrada);
            for (j = 0; j < tamanho; j++) {
                if (entrada[j] >= '0' && entrada[j] <= '9') {
                    id = id * 10 + (entrada[j] - '0');
                }
            }

            int k = 0;
            bool encontrado = false;
            while (k < n && !encontrado) {
                if (jogos[k]->id == id) {
                    empilhar(&pilha, jogos[k]);
                    encontrado = true;
                } else {
                    k++;
                }
            }
        }
    }

    int numComandos = 0;
    scanf("%d", &numComandos);

    for (i = 0; i < numComandos; i++) {
        char comando[3];
        scanf("%s", comando);

        if (strcmp(comando, "I") == 0) {
            scanf("%d", &id);
            int k = 0;
            bool encontrado = false;
            while (k < n && !encontrado) {
                if (jogos[k]->id == id) {
                    empilhar(&pilha, jogos[k]);
                    encontrado = true;
                } else {
                    k++;
                }
            }
        } else if (strcmp(comando, "R") == 0) {
            Game* removido = desempilhar(&pilha);
            if (removido != NULL) {
                printf("(R) %s\n", removido->name);
            }
        }
    }

    mostrar(&pilha);

    int resultado = 0;
    return resultado;
}