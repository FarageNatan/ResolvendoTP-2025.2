#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#define TAM_MAX_NOME 100
#define TAM_MAX_TEXTO_ARRAY 100
#define NUM_MAX_ITENS 10
#define MAX_SAIDA 4096 
#define TAM_MAX_TEXTO_ARRAY 100
#define NUM_MAX_ITENS 10
#define NUM_CAMPOS_CSV 14
#define MAX_JOGOS_ARQUIVO 100 
#define TAM_LINHA_CSV 8192
#define MAX_JOGOS_ARQUIVO 100 
#define TAM_LINHA_CSV 8192
#define TAM_MAX_ENTRADA 100

typedef struct Game {
    
    int id;
    
    char nome[TAM_MAX_NOME]; 
    char dataLancamento[20];
    int donosEstimados;
    float preco;
    char idiomasSuportados[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY];
    int notaMetacritic;
    float notaUsuario;
    int conquistas; 
    char publicadoras[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY];
    char desenvolvedoras[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY];
    char categorias[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY];
    char generos[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY];
    char tags[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY];
    

} Game;

//-------- Funcoes Auxiliares -----------

//Split em C
int dividirTextoSimples(char entrada[], char saida[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY]) {
    int i = 0;
    int indiceCampo = 0;
    int indiceLetra = 0;

    int k = 0;
    while(k < NUM_MAX_ITENS) {
        saida[k][0] = '\0'; 
        k = k + 1;
    }

    int tamanho = strlen(entrada);

    while (i < tamanho) {
        char c = entrada[i];

        if (c == ',') {
            if (indiceCampo < NUM_MAX_ITENS) {
                saida[indiceCampo][indiceLetra] = '\0';
                indiceCampo++;
                indiceLetra = 0;
            }
        } else {
            if (indiceLetra < TAM_MAX_TEXTO_ARRAY - 1 && indiceCampo < NUM_MAX_ITENS) {
                saida[indiceCampo][indiceLetra] = c;
                indiceLetra++;
            }
        }
        i++;
    }
    if (indiceCampo < NUM_MAX_ITENS) {
        saida[indiceCampo][indiceLetra] = '\0';
    }

    return 1;
}

int copiarString(char destino[], char origem[]) {
    int i = 0; 
    int terminou = 0;
    int resultado = 1;

    while (terminou == 0) {
        
        destino[i] = origem[i];
        if (origem[i] == '\0') {
            terminou = 1;
        }
        i = i + 1;
    }
    return resultado;
}

int juntarString(char destino[], char adicionar[]) {
    int pos = strlen(destino);
    int i = 0;
    int terminou = 0;
    while (terminou == 0) {
        destino[pos + i] = adicionar[i];
        if (adicionar[i] == '\0') {
            terminou = 1;
        }
        i = i + 1;
    }
    
    return 1;
}

char* formatarArray(char array[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY]) {
    static char resultado[MAX_SAIDA];
    int i = 0;
    int arrayVazio = 1;
    resultado[0] = '\0';
    if (strlen(array[0]) > 0) {
        arrayVazio = 0;
    }

    if (arrayVazio == 1) {
        copiarString(resultado, "[]");
    } else {
        copiarString(resultado, "[");
        while (i < NUM_MAX_ITENS) {
            if (strlen(array[i]) > 0) {
                juntarString(resultado, array[i]);
                if (i < NUM_MAX_ITENS - 1) {
                    if (strlen(array[i + 1]) > 0) {
                        juntarString(resultado, ", ");
                    }
                }
            }
            i = i + 1;
        }
        juntarString(resultado, "]");
    }
    return resultado;
}

char* removerEspacosBrancos(char* s) {
    static char resultado[TAM_MAX_TEXTO_ARRAY]; 
    
    int length;
    int inicio = 0; 
    int fim = 0; 
    int encontrouInicio = 0;
    int encontrouFim = 0;

    if (s == NULL) {
        length = 0;
    } else {
        length = strlen(s);
    }
    if (length == 0) {
        resultado[0] = '\0';
    } else {
        fim = length - 1;
        while (inicio < length && encontrouInicio == 0) {
            char c = s[inicio];
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                encontrouInicio = 1;
            } else {
                inicio++;
            }
        }
        while (fim >= inicio && encontrouFim == 0) {
            char c = s[fim];
            if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
                encontrouFim = 1;
            } else {
                fim--;
            }
        }
        if (inicio <= fim) {
            int i = inicio;
            int j = 0;
            while (i <= fim) {
                resultado[j] = s[i];
                i++;
                j++;
            }
            resultado[j] = '\0'; 
        } else {
            resultado[0] = '\0';
        }
    }
    return resultado;
}

// Recebe o texto de entrada (linhaCsv) e retorna o array de strings (char*).
// Em C, para retornar o array de strings (matriz), usamos um char* para o primeiro elemento.
char* dividirPorAspas(char* linhaCsv) {
    static char campos[NUM_CAMPOS_CSV][TAM_MAX_TEXTO_ARRAY]; 
    char campoAtual[TAM_MAX_TEXTO_ARRAY]; 
    int indiceCampo = 0;
    int i = 0;
    int j = 0;
    int tamanhoLinha = strlen(linhaCsv);
    int emAspas = 0;
    int k = 0;
    while(k < NUM_CAMPOS_CSV) {
        campos[k][0] = '\0';
        k = k + 1;
    }
    campoAtual[0] = '\0'; 

    while (i < tamanhoLinha) {
        char c = linhaCsv[i];
        if (c == '"') {
            if (emAspas == 0) {
                emAspas = 1;
            } else {
                emAspas = 0;
            }
        }
        if (c == ',' && emAspas == 0) {
            if (indiceCampo < NUM_CAMPOS_CSV) {
                campoAtual[j] = '\0'; 
                copiarString(campos[indiceCampo], campoAtual);
            }
            campoAtual[0] = '\0';
            j = 0;
            indiceCampo++;
        } else {
            if (c != '"') {
                if (j < (TAM_MAX_TEXTO_ARRAY - 1)) {
                    campoAtual[j] = c;
                    j++;
                }
            }
        }
        i = i + 1;
    }
    if (indiceCampo < NUM_CAMPOS_CSV) {
        campoAtual[j] = '\0'; 
        copiarString(campos[indiceCampo], campoAtual);
    }
    return (char*)campos;
}

Game leitura(char* linhaCsv) {
    
    char* ponteiroCampos = dividirPorAspas(linhaCsv); 
    char (*partesJogo)[TAM_MAX_TEXTO_ARRAY] = (char (*)[TAM_MAX_TEXTO_ARRAY])ponteiroCampos;

    int idTratado;
    char* nomeTratado;
    char estimativaLimpa[TAM_MAX_TEXTO_ARRAY];
    int estimativaTratada;
    char dataTratada[TAM_MAX_TEXTO_ARRAY];
    char precoLimpo[TAM_MAX_TEXTO_ARRAY];
    float precoTratado;

    char linguagensTratadas[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY];
    char publicadorasTratadas[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY];
    char desenvolvedorasTratadas[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY];
    char categoriasTratadas[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY];
    char generosTratados[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY];
    char tagsTratadas[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY];

    char metacriticLimpo[TAM_MAX_TEXTO_ARRAY];
    int metacriticTratado;
    char userScoreLimpo[TAM_MAX_TEXTO_ARRAY];
    float userScoreTratado;
    char achievementsLimpo[TAM_MAX_TEXTO_ARRAY];
    int achievementsTratado;

    sscanf(partesJogo[0], "%d", &idTratado);

    nomeTratado = partesJogo[1];

    trataEstimativa(partesJogo[3], estimativaLimpa); // [0,3]

    if (strlen(estimativaLimpa) > 0) {
        sscanf(estimativaLimpa, "%d", &estimativaTratada);
    } else {
        estimativaTratada = 0;
    }

    trataData(partesJogo[2], dataTratada);

    trataPreco(partesJogo[4], precoLimpo);
    sscanf(precoLimpo, "%f", &precoTratado);

    trataLinguagens(partesJogo[5], linguagensTratadas);
    
    trataNota(partesJogo[6], "-1", metacriticLimpo);
    sscanf(metacriticLimpo, "%d", &metacriticTratado);

    trataNota(partesJogo[7], "-1.0", userScoreLimpo);
    sscanf(userScoreLimpo, "%f", &userScoreTratado);

    trataAchievements(partesJogo[8], achievementsLimpo);
    sscanf(achievementsLimpo, "%d", &achievementsTratado);

    trataEmpresas(partesJogo[9], publicadorasTratadas);

    trataEmpresas(partesJogo[10], desenvolvedorasTratadas);

    trataListaColchetes(partesJogo[11], categoriasTratadas);

    trataListaColchetes(partesJogo[12], generosTratados);

    trataListaColchetes(partesJogo[13], tagsTratadas);
    
    return novoJogo;
}

//-------- Tratamento de Dados ------------

char* trataListaColchetes(char* listaTexto, char arrayResultado[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY]) {
    static char semColchetes[TAM_MAX_TEXTO_ARRAY]; 
    
    int i = 0;
    int j = 0;
    int tamanho = strlen(listaTexto);

    int k = 0;
    while(k < NUM_MAX_ITENS) {
        arrayResultado[k][0] = '\0';
        k = k + 1;
    }
    
    if(tamanho > 2){
        i = 1;
        j = 0;
        while(i < tamanho - 1){
            if(j < TAM_MAX_TEXTO_ARRAY - 1){
                semColchetes[j] = listaTexto[i];
                j++;
            }
            i++;
        }
        semColchetes[j] = '\0';
    } else {
        semColchetes[0] = '\0';
    }
    
    if(strlen(semColchetes) > 0){
        dividirTextoSimples(semColchetes, arrayResultado); 
    }
    return (char*)arrayResultado;
}

char* trataEmpresas(char* empresasTexto, char arrayResultado[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY]) {
    dividirTextoSimples(empresasTexto, arrayResultado);
    return (char*)arrayResultado;
}


void trataAchievements(char* achievementsTexto, char resultado[TAM_MAX_TEXTO_ARRAY]) {
    
    copiarString(resultado, achievementsTexto);

    if(strlen(achievementsTexto) == 0) {
        copiarString(resultado, "0");
    }
    return;
}

int trataAchievements(char* achievementsTexto, char resultado[TAM_MAX_TEXTO_ARRAY]) {
    copiarString(resultado, achievementsTexto);
    if(strlen(achievementsTexto) == 0) {
        copiarString(resultado, "0");
    }
    return 1;
}

int trataPreco(char* precoTexto, char resultado[TAM_MAX_TEXTO_ARRAY]) {
    copiarString(resultado, precoTexto);
    if(strcmp(precoTexto, "Free to Play") == 0) {
        copiarString(resultado, "0.0");
    }
    return 1;
}

char* trataLinguagens(char* linguagensTexto, char arrayResultado[NUM_MAX_ITENS][TAM_MAX_TEXTO_ARRAY]) {
    static char semColchetesAspas[TAM_MAX_TEXTO_ARRAY]; 
    int i = 0;
    int j = 0; 
    int tamanho = strlen(linguagensTexto);
    
    int k = 0;
    while(k < NUM_MAX_ITENS) {
        arrayResultado[k][0] = '\0';
        k = k + 1;
    }

    if(tamanho > 2) {
        i = 0; // Começa depois do '['
        j = 0;
        while(i < tamanho){
            char c = linguagensTexto[i];
            if (c != '\'') {
                if (j < TAM_MAX_TEXTO_ARRAY - 1) {
                    semColchetesAspas[j] = c;
                    j++;
                }
            }
            i++;
        }
        semColchetesAspas[j] = '\0';
    } else {
        semColchetesAspas[0] = '\0';
    }

    if (strlen(semColchetesAspas) > 0) {
        dividirTextoSimples(semColchetesAspas, arrayResultado);
        
        i = 0;
        while (i < NUM_MAX_ITENS) {
            char temp[TAM_MAX_TEXTO_ARRAY];
            char* textoLimpo;
            
            copiarString(temp, arrayResultado[i]);
            textoLimpo = removerEspacosBrancos(temp);
            copiarString(arrayResultado[i], textoLimpo);
            if(strlen(arrayResultado[i]) == 0) {

            }
            i++;
        }
    }
    return (char*)arrayResultado;
}

int trataNota(char* notaTexto, char* valorVazio, char resultado[TAM_MAX_TEXTO_ARRAY]) {
    copiarString(resultado, notaTexto);
    if(strlen(notaTexto) == 0) {
        copiarString(resultado, valorVazio);
    } else {
        if (strcmp(notaTexto, "tbd") == 0) {
            copiarString(resultado, valorVazio);
        }
    }
    return 1;
}

int trataEstimativa(char* estimativaUsuarios, char resultado[TAM_MAX_TEXTO_ARRAY]) {
    resultado[0] = '\0';
    int i = 0; 
    int j = 0;
    int tamanho = strlen(estimativaUsuarios);

    while(i < tamanho){
        char caracter = estimativaUsuarios[i];
        if(caracter >= '0' && caracter <= '9'){
            if (j < TAM_MAX_TEXTO_ARRAY - 1) {
                resultado[j] = caracter;
                j++;
            }
        }
        i++;
    }
    resultado[j] = '\0';
    return 1;
}

void converterMes(char* mesNome, char mesNum[3]) {
    copiarString(mesNum, "01");

    if (strlen(mesNome) == 3) {
        if (strcmp(mesNome, "Jan") == 0) copiarString(mesNum, "01");
        else if (strcmp(mesNome, "Feb") == 0) copiarString(mesNum, "02");
        else if (strcmp(mesNome, "Mar") == 0) copiarString(mesNum, "03");
        else if (strcmp(mesNome, "Apr") == 0) copiarString(mesNum, "04");
        else if (strcmp(mesNome, "May") == 0) copiarString(mesNum, "05");
        else if (strcmp(mesNome, "Jun") == 0) copiarString(mesNum, "06");
        else if (strcmp(mesNome, "Jul") == 0) copiarString(mesNum, "07");
        else if (strcmp(mesNome, "Aug") == 0) copiarString(mesNum, "08");
        else if (strcmp(mesNome, "Sep") == 0) copiarString(mesNum, "09");
        else if (strcmp(mesNome, "Oct") == 0) copiarString(mesNum, "10");
        else if (strcmp(mesNome, "Nov") == 0) copiarString(mesNum, "11");
        else if (strcmp(mesNome, "Dec") == 0) copiarString(mesNum, "12");
    }
    return;
}

int trataData(char* data, char resultado[11]) {
    char partes[3][10];
    char mes[3], dia[3], ano[5];
    char tempDia[5];
    int i = 0; 
    int j = 0;
    int indiceParte = 0;

    if (data == NULL || strlen(data) == 0) {
        copiarString(resultado, "01/01/0000");
        return 1;
    }

    while (i < strlen(data)) {
        char c = data[i];

        if (c == ' ') {
            if (indiceParte < 3) {
                partes[indiceParte][j] = '\0';
                indiceParte++;
                j = 0;
            }
        } else{
            if (j < 9 && indiceParte < 3) {
                partes[indiceParte][j] = c;
                j++;
            }
        }
        i++;
    }
    if (indiceParte < 3) {
        partes[indiceParte][j] = '\0';
    }

    converterMes(partes[0], mes);
    copiarString(dia, "01");
    copiarString(ano, "0000");
    
    if (indiceParte == 2) {
        copiarString(ano, partes[1]);
    } else if (indiceParte >= 3) {
        copiarString(tempDia, partes[1]);
        j = 0;
        int k = 0;
        while(j < strlen(tempDia)){
            if(tempDia[j] != ','){
                if(k < 2){
                    dia[k] = tempDia[j];
                    k++;
                }
            }
            j++;
        }
        dia[k] = '\0';
        copiarString(ano, partes[2]);
    }

    if (strlen(dia) == 1) {
        dia[1] = dia[0];
        dia[0] = '0';
        dia[2] = '\0'; 
    }
    resultado[0] = '\0';
    juntarString(resultado, dia);
    juntarString(resultado, "/");
    juntarString(resultado, mes);
    juntarString(resultado, "/");
    juntarString(resultado, ano);
    
    return 1;
}

int imprimirGame(Game jogo) {
    static char textoIdiomas[MAX_SAIDA];
    static char textoPublishers[MAX_SAIDA];
    static char textoDevelopers[MAX_SAIDA];
    static char textoCategories[MAX_SAIDA];
    static char textoGenres[MAX_SAIDA];
    static char textoTags[MAX_SAIDA];

    // formatarArray(jogo.idiomasSuportados, textoIdiomas);
    // formatarArray(jogo.publicadoras, textoPublishers);
    // formatarArray(jogo.desenvolvedoras, textoDevelopers);
    // formatarArray(jogo.categorias, textoCategories);
    // formatarArray(jogo.generos, textoGenres);
    // formatarArray(jogo.tags, textoTags);

    formatarArray(textoIdiomas);
    formatarArray(textoPublishers);
    formatarArray(textoDevelopers);
    formatarArray(textoCategories);
    formatarArray(textoGenres);
    formatarArray(textoTags);

    printf("=> %d ## %s ## %s ## %d ## %.2f ## %s ## %d ## %.1f ## %d ## %s ## %s ## %s ## %s ## %s ##\n", 
        jogo.id, 
        jogo.nome, 
        jogo.dataLancamento, 
        jogo.donosEstimados, 
        jogo.preco, 
        textoIdiomas, 
        jogo.notaMetacritic, 
        jogo.notaUsuario, 
        jogo.conquistas, 
        textoPublishers, 
        textoDevelopers, 
        textoCategories, 
        textoGenres,     
        textoTags        
    );
    return 1;
}

int lerArquivoCSV(char* caminhoArquivo, Game jogos[MAX_JOGOS_ARQUIVO]) {
    FILE *arquivo = NULL;
    char linha[TAM_LINHA_CSV];
    int quantidade = 0;
    int primeiraLinha = 1;
    int terminouDeLer = 0; 
    arquivo = fopen(caminhoArquivo, "r");

    if (arquivo != NULL) {
        while (fgets(linha, TAM_LINHA_CSV, arquivo) != NULL && quantidade < MAX_JOGOS_ARQUIVO) {
            
            if (primeiraLinha == 0) {
                quantidade = quantidade + 1;
            } else {
                primeiraLinha = 0;
            }
        }
        fclose(arquivo); 
    }

    primeiraLinha = 1;
    int indice = 0;
    arquivo = fopen(caminhoArquivo, "r");

    if (arquivo != NULL) {
        while (fgets(linha, TAM_LINHA_CSV, arquivo) != NULL && indice < quantidade) {
            
            if (primeiraLinha == 0) {
                int tamanhoLinha = strlen(linha);
                if (tamanhoLinha > 0 && linha[tamanhoLinha - 1] == '\n') {
                    linha[tamanhoLinha - 1] = '\0';
                }
                jogos[indice] = leitura(linha); 
                indice = indice + 1;
            } else {
                primeiraLinha = 0;
            }
        }
        
        fclose(arquivo);
    } 
    return quantidade;
}

int buscarEImprimirJogo(Game jogos[], int tamanho, char* entrada) {
    int i = 0;
    int achou = 0;
    int retorno = 0;
    char idTexto[12];
    while (i < tamanho && achou == 0) {
        sprintf(idTexto, "%d", jogos[i].id); 
        if (strcmp(idTexto, entrada) == 0) {
            imprimirGame(jogos[i]); 
            achou = 1; 
        } 
        i = i + 1;
    }
    if (achou == 1) {
        retorno = 1;
    }
    return retorno;
}

int main() {
    Game jogos[MAX_JOGOS_ARQUIVO]; 
    char entrada[TAM_MAX_ENTRADA]; 
    int quantidadeJogos = 0; 
    quantidadeJogos = lerArquivoCSV("/tmp/games.csv", jogos); 
    if (fgets(entrada, TAM_MAX_ENTRADA, stdin) != NULL) {
        int len = strlen(entrada);
        if (len > 0 && entrada[len - 1] == '\n') {
            entrada[len - 1] = '\0';
        }
    }
    while (strcmp(entrada, "FIM") != 0) {
        buscarEImprimirJogo(jogos, quantidadeJogos, entrada);
        if (fgets(entrada, TAM_MAX_ENTRADA, stdin) != NULL) {
            int len = strlen(entrada);
            if (len > 0 && entrada[len - 1] == '\n') {
                entrada[len - 1] = '\0';
            }
        }
    }
    return 0;
}






