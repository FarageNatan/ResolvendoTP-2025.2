#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE 12000
#define MAX_GAMES 1850
#define MAX_FIELD 5000

#define MAX_NAME 512
#define DATE_LEN 12

#define MAX_LANGS 64
#define LANG_LEN 64

#define MAX_PUBS 64
#define PUB_LEN 128

#define MAX_CATS 64
#define CAT_LEN 128

#define MAX_GENRES 64
#define GENRE_LEN 128

#define MAX_TAGS 256
#define TAG_LEN 128

typedef struct {
    int id;
    char name[MAX_NAME];
    char releaseDate[DATE_LEN];
    int estimatedOwners;
    float price;
    char supportedLanguages[MAX_LANGS][LANG_LEN];
    int supportedLanguagesCount;
    int metacriticScore;
    float userScore;
    int achievements;
    char publishers[MAX_PUBS][PUB_LEN];
    int publishersCount;
    char developers[MAX_PUBS][PUB_LEN];
    int developersCount;
    char categories[MAX_CATS][CAT_LEN];
    int categoriesCount;
    char genres[MAX_GENRES][GENRE_LEN];
    int genresCount;
    char tags[MAX_TAGS][TAG_LEN];
    int tagsCount;
} Game;

static int is_space(char c) {
    int retorno;
    if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v') {
        retorno = 1;
    } else {
        retorno = 0;
    }
    return retorno;
}

static void my_strcpy(char *dst, char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

static void copy_field(char *dst, char *src, int maxlen) {
    int i = 0;
    while (src[i] != '\0' && i < maxlen - 1) {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
}

static void trim_whitespace(char *in, char *out) {
    int L = (int) strlen(in);
    int start = 0;
    int end = L - 1;
    int j = 0;
    while (start < L && is_space(in[start]) == 1) {
        start = start + 1;
    }
    while (end >= start && is_space(in[end]) == 1) {
        end = end - 1;
    }
    for (int i = start; i <= end; i++) {
        out[j++] = in[i];
    }
    out[j] = '\0';
}

/* extrai apenas dígitos */
static void somente_digitos(char *in, char *out) {
    int i = 0, j = 0;
    int L = (int) strlen(in);
    while (i < L) {
        if (in[i] >= '0' && in[i] <= '9') {
            out[j++] = in[i];
        }
        i = i + 1;
    }
    out[j] = '\0';
}

/*separa em 14 campos*/
static void splitPorAspas(char linha[], char campos[14][MAX_FIELD]) {
    int idx = 0;
    char campoAtual[MAX_FIELD];
    int posCampo = 0;
    int emAspas = 0;
    int i = 0;
    int L = (int) strlen(linha);

    for (int k = 0; k < 14; k++) campos[k][0] = '\0';
    campoAtual[0] = '\0';

    while (i < L && idx < 14) {
        char c = linha[i];
        if (c == '"') {
            if (emAspas == 0) emAspas = 1;
            else emAspas = 0;
        } else {
            if (c == ',' && emAspas == 0) {
                campoAtual[posCampo] = '\0';
                copy_field(campos[idx], campoAtual, MAX_FIELD);
                posCampo = 0;
                campoAtual[0] = '\0';
                idx = idx + 1;
            } else {
                if (posCampo < MAX_FIELD - 1) {
                    campoAtual[posCampo++] = c;
                }
            }
        }
        i = i + 1;
    }
    /* último campo */
    if (idx < 14) {
        campoAtual[posCampo] = '\0';
        copy_field(campos[idx], campoAtual, MAX_FIELD);
    }
}

static void trataPreco(char in[], char out[]) {
    if (strcmp(in, "Free to Play") == 0) {
        copy_field(out, "0.0", 16);
    } else {
        copy_field(out, in, 64);
    }
}

static void trataAchievements(char in[], char out[]) {
    if (in[0] == '\0') copy_field(out, "0", 8);
    else copy_field(out, in, 32);
}

static void trataNota(char in[], char valorVazio[], char out[]) {
    if (in[0] == '\0') {
        copy_field(out, valorVazio, 16);
    } else if (in[0] == 't' && in[1] == 'b' && in[2] == 'd') {
        copy_field(out, valorVazio, 16);
    } else {
        copy_field(out, in, 32);
    }
}

static void trataData(char in[], char out[]) {
    char part1[64]; part1[0] = '\0';
    char part2[64]; part2[0] = '\0';
    char part3[64]; part3[0] = '\0';

    if (in == NULL || in[0] == '\0') {
        copy_field(out, "00/01/0000", DATE_LEN);
        return;
    }

    int state = 0;
    int p = 0, q = 0, r = 0;
    int i = 0;
    int L = (int) strlen(in);
    while (i < L) {
        char c = in[i];
        if (c == ' ') {
            state = state + 1;
            i = i + 1;
            continue;
        }
        if (state == 0) {
            if (p < 63) part1[p++] = c;
        } else if (state == 1) {
            if (q < 63) part2[q++] = c;
        } else {
            if (r < 63) part3[r++] = c;
        }
        i = i + 1;
    }
    part1[p] = '\0'; part2[q] = '\0'; part3[r] = '\0';

    char mes[3]; mes[0] = '0'; mes[1] = '1'; mes[2] = '\0';
    if (strcmp(part1, "Jan") == 0) my_strcpy(mes, "01");
    else if (strcmp(part1, "Feb") == 0) my_strcpy(mes, "02");
    else if (strcmp(part1, "Mar") == 0) my_strcpy(mes, "03");
    else if (strcmp(part1, "Apr") == 0) my_strcpy(mes, "04");
    else if (strcmp(part1, "May") == 0) my_strcpy(mes, "05");
    else if (strcmp(part1, "Jun") == 0) my_strcpy(mes, "06");
    else if (strcmp(part1, "Jul") == 0) my_strcpy(mes, "07");
    else if (strcmp(part1, "Aug") == 0) my_strcpy(mes, "08");
    else if (strcmp(part1, "Sep") == 0) my_strcpy(mes, "09");
    else if (strcmp(part1, "Oct") == 0) my_strcpy(mes, "10");
    else if (strcmp(part1, "Nov") == 0) my_strcpy(mes, "11");
    else if (strcmp(part1, "Dec") == 0) my_strcpy(mes, "12");

    char dia[4]; dia[0] = '0'; dia[1] = '1'; dia[2] = '\0';
    char ano[8]; ano[0] = '0'; ano[1] = '0'; ano[2] = '0'; ano[3] = '0'; ano[4] = '\0';

    if (part2[0] != '\0' && part3[0] != '\0') {
        char temp[8]; int tpos = 0; int j = 0;
        while (part2[j] != '\0') {
            if (part2[j] != ',') {
                if (tpos < 7) temp[tpos++] = part2[j];
            }
            j = j + 1;
        }
        temp[tpos] = '\0';
        if (strlen(temp) == 1) {
            dia[0] = '0'; dia[1] = temp[0]; dia[2] = '\0';
        } else {
            my_strcpy(dia, temp);
        }
        my_strcpy(ano, part3);
    } else if (part2[0] != '\0') {
        my_strcpy(ano, part2);
        my_strcpy(dia, "01");
    }

    char buffer[DATE_LEN];
    int lenDia = (int) strlen(dia);
    if (lenDia == 1) {
        buffer[0] = '0'; buffer[1] = dia[0];
        buffer[2] = '/';
        buffer[3] = mes[0]; buffer[4] = mes[1];
        buffer[5] = '/';
        int k = 0;
        while (ano[k] != '\0') { buffer[6 + k] = ano[k]; k = k + 1; }
        buffer[6 + (int) strlen(ano)] = '\0';
    } else {
        int k = 0;
        while (dia[k] != '\0') { buffer[k] = dia[k]; k = k + 1; }
        buffer[k++] = '/';
        buffer[k++] = mes[0]; buffer[k++] = mes[1];
        buffer[k++] = '/';
        int t = 0;
        while (ano[t] != '\0') { buffer[k + t] = ano[t]; t = t + 1; }
        buffer[k + t] = '\0';
    }
    copy_field(out, buffer, DATE_LEN);
}

static int splitListaSimples(char in[], char dest[][128], int maxItems, int maxLen) {
    int ret_count = 0;
    int L = (int) strlen(in);
    if (L <= 2) {
        ret_count = 0;
        return ret_count;
    }
    char buffer[MAX_FIELD];
    int bpos = 0;
    int count = 0;
    int i = 0; /* começa em 1 para evitar '[' */
    while (i < L && count < maxItems) {
        char c = in[i];
        if (c == '\'') {
            /* ignora */
        } else if (c == ',') {
            buffer[bpos] = '\0';
            char trimmed[128];
            trim_whitespace(buffer, trimmed);
            if (trimmed[0] != '\0') {
                copy_field(dest[count], trimmed, maxLen);
                count = count + 1;
            }
            bpos = 0;
            buffer[0] = '\0';
        } else {
            if (bpos < MAX_FIELD - 1) buffer[bpos++] = c;
        }
        i = i + 1;
    }
    /* último item */
    buffer[bpos] = '\0';
    {
        char trimmed[128];
        trim_whitespace(buffer, trimmed);
        if (trimmed[0] != '\0' && count < maxItems) {
            copy_field(dest[count], trimmed, maxLen);
            count = count + 1;
        }
    }
    ret_count = count;
    return ret_count;
}

/* split por vírgula simples (para publishers/developers) */
static int splitPorVirgulaSimples(char in[], char dest[][PUB_LEN], int maxItems) {
    int ret_count = 0;
    int L = (int) strlen(in);
    if (L == 0) {
        ret_count = 0;
        return ret_count;
    }
    char buffer[MAX_FIELD];
    int bpos = 0;
    int count = 0;
    int i = 0;
    while (i < L && count < maxItems) {
        char c = in[i];
        if (c == ',') {
            buffer[bpos] = '\0';
            char trimmed[128];
            trim_whitespace(buffer, trimmed);
            if (trimmed[0] != '\0') {
                copy_field(dest[count], trimmed, PUB_LEN);
                count = count + 1;
            }
            bpos = 0;
            buffer[0] = '\0';
        } else {
            if (bpos < MAX_FIELD - 1) buffer[bpos++] = c;
        }
        i = i + 1;
    }
    buffer[bpos] = '\0';
    {
        char trimmed[128];
        trim_whitespace(buffer, trimmed);
        if (trimmed[0] != '\0' && count < maxItems) {
            copy_field(dest[count], trimmed, PUB_LEN);
            count = count + 1;
        }
    }
    ret_count = count;
    return ret_count;
}

/* trataLinguagensParaDest */
static void trataLinguagensParaDest(char in[], char dest[][LANG_LEN], int *outCount) {
    int cnt = splitListaSimples(in, dest, MAX_LANGS, LANG_LEN);
    int i = 0;
    while (i < cnt) {
        char tmp[LANG_LEN];
        tmp[0] = '\0';
        trim_whitespace(dest[i], tmp);
        copy_field(dest[i], tmp, LANG_LEN);
        i = i + 1;
    }
    *outCount = cnt;
}

/* parseGameFromLine: converte linha em Game alocado dinamicamente */
static Game* parseGameFromLine(char linha[]) {
    Game *g;
    g = (Game*) malloc(sizeof(Game));
    
    g->supportedLanguagesCount = 0;
    g->publishersCount = 0;
    g->developersCount = 0;
    g->categoriesCount = 0;
    g->genresCount = 0;
    g->tagsCount = 0;

    char campos[14][MAX_FIELD];
    for (int i = 0; i < 14; i++) campos[i][0] = '\0';

    splitPorAspas(linha, campos);

    /* id */
    if (campos[0][0] != '\0') g->id = atoi(campos[0]); else g->id = 0;

    /* name */
    copy_field(g->name, campos[1], MAX_NAME);

    /* release date */
    trataData(campos[2], g->releaseDate);

    /* estimated owners */
    {
        char onlyDigits[64];
        onlyDigits[0] = '\0';
        somente_digitos(campos[3], onlyDigits);
        if (onlyDigits[0] != '\0') g->estimatedOwners = atoi(onlyDigits);
        else g->estimatedOwners = 0;
    }

    /* price */
    {
        char precoTratado[64];
        precoTratado[0] = '\0';
        trataPreco(campos[4], precoTratado);
        if (precoTratado[0] != '\0') g->price = (float) atof(precoTratado);
        else g->price = 0.0f;
    }

    /* supported languages */
    trataLinguagensParaDest(campos[5], g->supportedLanguages, &(g->supportedLanguagesCount));

    /* metacritic */
    {
        char metacriticTratado[16];
        metacriticTratado[0] = '\0';
        trataNota(campos[6], "-1", metacriticTratado);
        g->metacriticScore = atoi(metacriticTratado);
    }

    /* user score */
    {
        char userScoreTratado[16];
        userScoreTratado[0] = '\0';
        trataNota(campos[7], "-1.0", userScoreTratado);
        g->userScore = (float) atof(userScoreTratado);
    }

    /* achievements */
    {
        char achTratado[32];
        achTratado[0] = '\0';
        trataAchievements(campos[8], achTratado);
        g->achievements = atoi(achTratado);
    }

    /* publishers */
    g->publishersCount = splitPorVirgulaSimples(campos[9], g->publishers, MAX_PUBS);

    /* developers */
    g->developersCount = splitPorVirgulaSimples(campos[10], g->developers, MAX_PUBS);

    /* categories */
    g->categoriesCount = splitListaSimples(campos[11], g->categories, MAX_CATS, CAT_LEN);

    /* genres */
    g->genresCount = splitListaSimples(campos[12], g->genres, MAX_GENRES, GENRE_LEN);

    /* tags */
    g->tagsCount = splitListaSimples(campos[13], g->tags, MAX_TAGS, TAG_LEN);

    return g;
}

/* print arrays */
static void printArrayFornecedor(char arr[][128], int count) {
    int i = 0;
    printf("[");
    while (i < count) {
        printf("%s", arr[i]);
        if (i < count - 1) printf(", ");
        i = i + 1;
    }
    printf("]");
}

static void printArrayLangs(char arr[][LANG_LEN], int count) {
    int i = 0;
    printf("[");
    while (i < count) {
        printf("%s", arr[i]);
        if (i < count - 1) printf(", ");
        i = i + 1;
    }
    printf("]");
}

/* imprimeGameCompatível com Java */
static void imprimeGameC(Game *g) {
    printf("=> %d ## %s ## %s ## %d", g->id, g->name, g->releaseDate, g->estimatedOwners);
    printf(" ## %.2f ## ", g->price);
    printArrayLangs((char (*)[LANG_LEN])g->supportedLanguages, g->supportedLanguagesCount);
    printf(" ## %d ## %.1f ## %d ## ", g->metacriticScore, g->userScore, g->achievements);
    printArrayFornecedor((char (*)[128])g->publishers, g->publishersCount);
    printf(" ## ");
    printArrayFornecedor((char (*)[128])g->developers, g->developersCount);
    printf(" ## ");
    printArrayFornecedor((char (*)[128])g->categories, g->categoriesCount);
    printf(" ## ");
    printArrayFornecedor((char (*)[128])g->genres, g->genresCount);
    printf(" ## ");
    printArrayFornecedor((char (*)[128])g->tags, g->tagsCount);
    printf(" ##\n");
}

static int lerArquivoCSV(char caminho[], Game* jogos[], int maxGames) {
    int ret_count = 0;
    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        fprintf(stderr, "Erro ao abrir arquivo: %s\n", caminho);
        ret_count = 0;
        return ret_count;
    }

    char linha[MAX_LINE];
    int quantidade = 0;
    int primeiraLinha = 1;

    while (fgets(linha, sizeof(linha), f) != NULL && quantidade < maxGames) {
        int L = (int) strlen(linha);
        while (L > 0 && (linha[L-1] == '\n' || linha[L-1] == '\r')) { linha[L-1] = '\0'; L = L - 1; }
        if (primeiraLinha == 1) {
            primeiraLinha = 0;
        } else {
            Game *j = parseGameFromLine(linha);
            if (j != NULL) {
                jogos[quantidade] = j;
                quantidade = quantidade + 1;
            } else {
                /* malloc falhou; ignoramos a linha */
            }
        }
    }

    fclose(f);
    ret_count = quantidade;
    return ret_count;
}

static void buscarEImprimir(Game* jogos[], int n, char entrada[]) {
    int i = 0;
    while (i < n) {
        char idTexto[32];
        sprintf(idTexto, "%d", jogos[i]->id);
        if (strcmp(idTexto, entrada) == 0) {
            imprimeGameC(jogos[i]);
            i = n;
        } else {
            i = i + 1;
        }
    }
}

int main() {
    Game* jogos[MAX_GAMES];
    int n = lerArquivoCSV("/tmp/games.csv", jogos, MAX_GAMES);

    char entrada[128];
    if (fgets(entrada, sizeof(entrada), stdin) != NULL) {
        int L = (int) strlen(entrada);
        while (L > 0 && (entrada[L-1] == '\n' || entrada[L-1] == '\r')) {
            entrada[L-1] = '\0';
            L = L - 1;
        }
        int terminou = 0;
        while (terminou == 0) {
            if (strcmp(entrada, "FIM") == 0) {
                terminou = 1;
            } else {
                buscarEImprimir(jogos, n, entrada);
                if (fgets(entrada, sizeof(entrada), stdin) != NULL) {
                    L = (int) strlen(entrada);
                    while (L > 0 && (entrada[L-1] == '\n' || entrada[L-1] == '\r')) {
                        entrada[L-1] = '\0';
                        L = L - 1;
                    }
                } else {
                    terminou = 1;
                }
            }
        }
    }


    {
        int i = 0;
        while (i < n) {
            free(jogos[i]);
            i = i + 1;
        }
    }

    return 0;
}
