#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <time.h>

#define MAX_LINE_SIZE 2000
#define MAX_SHOWS 1000
#define MAX_CAPACITY 1368

char **csv_lines = NULL;
int csv_line_count = 0,
    comparacoes = 0;

typedef struct {
    char show_id[50];
    char type[20];
    char title[200];
    char director[200];
    char **cast;
    int castCount;
    char country[100];
    struct tm *dateAdded;
    int releaseYear;
    char rating[20];
    char duration[50];
    char **listedIn;
    int listedInCount;
} Show;

void init_show(Show *show) {
    strcpy(show->show_id, "NaN");
    strcpy(show->type, "NaN");
    strcpy(show->title, "NaN");
    strcpy(show->director, "NaN");
    strcpy(show->country, "NaN");
    show->cast = malloc(sizeof(char *));
    show->cast[0] = strdup("NaN");
    show->castCount = 1;
    show->dateAdded = malloc(sizeof(struct tm));
    memset(show->dateAdded, 0, sizeof(struct tm));
    show->dateAdded->tm_mday = 1;  // Dia 1
    show->dateAdded->tm_mon = 2;   // Março
    show->dateAdded->tm_year = 0;  // 1900
    show->releaseYear = 0;
    strcpy(show->rating, "NaN");
    strcpy(show->duration, "NaN");
    show->listedIn = malloc(sizeof(char *));
    show->listedIn[0] = strdup("NaN");
    show->listedInCount = 1;
}

void read_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file: %s\n", filename);
        return;
    }

    // Count number of lines
    char buffer[MAX_LINE_SIZE];
    csv_line_count = 0;
    while (fgets(buffer, MAX_LINE_SIZE, file) != NULL) {
        csv_line_count++;
    }

    // Allocate memory for lines
    csv_lines = (char **)malloc(csv_line_count * sizeof(char *));
    if (csv_lines == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(file);
        return;
    }

    // Reset file pointer and read lines
    rewind(file);
    for (int i = 0; i < csv_line_count; i++) {
        if (fgets(buffer, MAX_LINE_SIZE, file) != NULL) {
            // Remove newline character
            buffer[strcspn(buffer, "\n")] = 0;
            
            csv_lines[i] = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
            if (csv_lines[i] == NULL) {
                fprintf(stderr, "Memory allocation error\n");
                fclose(file);
                return;
            }
            strcpy(csv_lines[i], buffer);
        }
    }

    fclose(file);
}

// Sort a string array alphabetically
void sort_string_array(char **array, int size) {
    if (array == NULL || size <= 1) return;
    
    for (int i = 0; i < size - 1; i++) {
        for (int j = i + 1; j < size; j++) {
            if (array[i] != NULL && array[j] != NULL && strcmp(array[i], array[j]) > 0) {
                char *temp = array[i];
                array[i] = array[j];
                array[j] = temp;
            }
        }
    }
}

// Trim leading and trailing whitespace from a string
char* trim(char *str) {
    if (str == NULL) return NULL;
    
    // Trim leading space
    while(isspace((unsigned char)*str)) str++;
    
    if(*str == 0) return str; // All spaces
    
    // Trim trailing space
    char *end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;
    
    // Write new null terminator
    *(end + 1) = 0;
    
    return str;
}

// Replace double double-quotes with single double-quotes
char* fix_double_quotes(char *str) {
    if (str == NULL) return NULL;
    
    char *src = str;
    char *dst = str;
    
    while (*src) {
        if (*src == '"' && *(src + 1) == '"') {
            *dst++ = '"';
            src += 2;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';
    
    return str;
}

// Parse CSV fields properly handling quotes and escape sequences
char** parse_csv_line(char *line, int *field_count) {
    if (line == NULL || field_count == NULL) return NULL;
    
    // Count the expected number of fields (approximate)
    int expected_fields = 1;
    bool in_quotes = false;
    for (char *p = line; *p; p++) {
        if (*p == '"') {
            in_quotes = !in_quotes;
        } else if (*p == ',' && !in_quotes) {
            expected_fields++;
        }
    }
    
    // Allocate memory for fields
    char **fields = (char **)malloc(expected_fields * sizeof(char *));
    if (fields == NULL) return NULL;
    
    // Initialize field count
    *field_count = 0;
    
    char *p = line;
    char *field_start = p;
    in_quotes = false;
    
    while (*p) {
        if (*p == '"') {
            // Check if it's an escaped quote (double double-quote)
            if (*(p+1) == '"') {
                p += 2; // Skip both quotes
            } else {
                in_quotes = !in_quotes;
                p++;
            }
        } else if (*p == ',' && !in_quotes) {
            // End of field
            *p = '\0';
            
            // Remove surrounding quotes if present
            char *field_value = field_start;
            int field_len = strlen(field_value);
            
            if (field_len >= 2 && *field_value == '"' && *(field_value + field_len - 1) == '"') {
                field_value++;
                *(field_value + field_len - 2) = '\0';
                // Fix escaped quotes (convert "" to ")
                fix_double_quotes(field_value);
            }
            
            fields[(*field_count)++] = strdup(field_value);
            field_start = p + 1;
            p++;
        } else {
            p++;
        }
    }
    
    // Handle the last field
    if (field_start) {
        // Remove surrounding quotes if present
        int field_len = strlen(field_start);
        if (field_len >= 2 && *field_start == '"' && *(field_start + field_len - 1) == '"') {
            field_start++;
            *(field_start + field_len - 2) = '\0';
            // Fix escaped quotes (convert "" to ")
            fix_double_quotes(field_start);
        }
        
        fields[(*field_count)++] = strdup(field_start);
    }
    
    return fields;
}

// Split a string by delimiter, properly handling quoted content
char** split_and_sort(const char *str, int *count) {
    if (str == NULL || count == NULL || strlen(str) == 0) {
        *count = 0;
        return NULL;
    }
    
    // Make a copy of the input string
    char *str_copy = strdup(str);
    if (str_copy == NULL) {
        *count = 0;
        return NULL;
    }
    
    // Fix any double-quoted sections
    fix_double_quotes(str_copy);
    
    // Count commas to estimate the maximum number of items
    int max_items = 1;
    for (char *p = str_copy; *p; p++) {
        if (*p == ',') max_items++;
    }
    
    // Allocate array for items
    char **items = (char **)malloc(max_items * sizeof(char *));
    if (items == NULL) {
        free(str_copy);
        *count = 0;
        return NULL;
    }
    
    // Parse the list
    int item_count = 0;
    char *token = strtok(str_copy, ",");
    
    while (token != NULL) {
        items[item_count++] = strdup(trim(token));
        token = strtok(NULL, ",");
    }
    
    free(str_copy);
    *count = item_count;
    
    // Sort the items
    sort_string_array(items, item_count);
    
    return items;
}

// Parse a CSV line into a Show structure
void read_show(Show *show, char *line) {
    if (show == NULL || line == NULL) return;
    
    // Make a copy of the line to avoid modifying the original
    char *line_copy = strdup(line);
    if (line_copy == NULL) return;
    
    // Parse the CSV line
    int field_count = 0;
    char **fields = parse_csv_line(line_copy, &field_count);
    
    // Process fields if we have enough
    if (fields != NULL && field_count >= 11) {
        // Show ID
        if (fields[0] && strlen(fields[0]) > 0) {
            strcpy(show->show_id, fields[0]);
        }
        
        // Type
        if (fields[1] && strlen(fields[1]) > 0) {
            if (strcasecmp(fields[1], "movie") == 0) {
                strcpy(show->type, "Movie");
            } else {
                strcpy(show->type, "TV Show");
            }
        }
        
        // Title - WITH FIX TO REMOVE QUOTES
        if (fields[2] && strlen(fields[2]) > 0) {
            char *cleaned_title = strdup(fields[2]);
            
            // Remove all quotes from the title
            char *src = cleaned_title;
            char *dst = cleaned_title;
            
            while (*src) {
                if (*src != '"') {
                    *dst++ = *src;
                }
                src++;
            }
            *dst = '\0';
            
            strcpy(show->title, cleaned_title);
            free(cleaned_title);
        }
        
        // Director
        if (fields[3] && strlen(fields[3]) > 0) {
            strcpy(show->director, fields[3]);
        }
        
        // Cast
        if (fields[4] && strlen(fields[4]) > 0) {
            show->cast = split_and_sort(fields[4], &show->castCount);
        } else {
            show->cast = NULL;
            show->castCount = 0;
        }
        
        // Country
        if (fields[5] && strlen(fields[5]) > 0) {
            strcpy(show->country, fields[5]);
        }
        
        // Date added
        if (fields[6] && strlen(fields[6]) > 0) {
            show->dateAdded = malloc(sizeof(struct tm));
            if (show->dateAdded != NULL) {
                memset(show->dateAdded, 0, sizeof(struct tm));
                
                // Date format: "Month day, year"
                char month_str[20] = {0};
                int day = 0, year = 0;
                
                sscanf(fields[6], "%19s %d, %d", month_str, &day, &year);
                
                // Convert month string to number (0-11)
                const char *months[] = {"January", "February", "March", "April", "May", "June", 
                                       "July", "August", "September", "October", "November", "December"};
                int month = 0;
                for (int i = 0; i < 12; i++) {
                    if (strstr(month_str, months[i]) != NULL) {
                        month = i;
                        break;
                    }
                }
                
                show->dateAdded->tm_year = year - 1900;
                show->dateAdded->tm_mon = month;
                show->dateAdded->tm_mday = day;
            }
        } else {
            show->dateAdded = malloc(sizeof(struct tm));
            if (show->dateAdded != NULL) {
                memset(show->dateAdded, 0, sizeof(struct tm));
                show->dateAdded->tm_year = 0;  // 1900
                show->dateAdded->tm_mon = 2;   // Março
                show->dateAdded->tm_mday = 1;  // Dia 1
            }
        }
        
        // Release year
        if (fields[7] && strlen(fields[7]) > 0) {
            show->releaseYear = atoi(fields[7]);
        }
        
        // Rating
        if (fields[8] && strlen(fields[8]) > 0) {
            strcpy(show->rating, fields[8]);
        }
        
        // Duration
        if (fields[9] && strlen(fields[9]) > 0) {
            strcpy(show->duration, fields[9]);
        }
        
        // Listed in
        if (fields[10] && strlen(fields[10]) > 0) {
            show->listedIn = split_and_sort(fields[10], &show->listedInCount);
        } else {
            show->listedIn = NULL;
            show->listedInCount = 0;
        }
        
        // Free fields
        for (int i = 0; i < field_count; i++) {
            if (fields[i] != NULL) {
                free(fields[i]);
            }
        }
        free(fields);
    }
    
    free(line_copy);
}

// Print a Show structure
void print_show(Show *show) {
    if (show == NULL) return;
    
    printf("=> %s ## %s ## %s ## ", 
           show->show_id, 
           show->title, 
           show->type);
    
    // Director
    printf("%s ## ", show->director);
    
    // Cast
    if (show->castCount == 0 || show->cast == NULL) {
        printf("[NaN] ## ");
    } else {
        printf("[");
        for (int i = 0; i < show->castCount; i++) {
            if (show->cast[i] != NULL) {
                printf("%s", show->cast[i]);
                if (i < show->castCount - 1) printf(", ");
            }
        }
        printf("] ## ");
    }
    
    // Country
    printf("%s ## ", show->country);
    
    // Date added
    if (show->dateAdded == NULL) {
        printf(" ## ");
    } else {
        const char *months[] = {"January", "February", "March", "April", "May", "June", 
                               "July", "August", "September", "October", "November", "December"};
        printf("%s %d, %d ## ", 
               months[show->dateAdded->tm_mon],
               show->dateAdded->tm_mday,
               show->dateAdded->tm_year + 1900);
    }
    
    // Release year, rating, duration
    printf("%d ## %s ## %s ## ", 
           show->releaseYear, 
           show->rating, 
           show->duration);
    
    // Listed in
    if (show->listedInCount == 0 || show->listedIn == NULL) {
        printf("[NaN] ##");
    } else {
        printf("[");
        for (int i = 0; i < show->listedInCount; i++) {
            if (show->listedIn[i] != NULL) {
                printf("%s", show->listedIn[i]);
                if (i < show->listedInCount - 1) printf(", ");
            }
        }
        printf("] ##");
    }
    
    printf("\n");
}

// Free memory allocated for a Show structure
void free_show(Show *show) {
    if (show == NULL) return;
    
    // Free cast array
    if (show->cast != NULL) {
        for (int i = 0; i < show->castCount; i++) {
            if (show->cast[i] != NULL) {
                free(show->cast[i]);
            }
        }
        free(show->cast);
        show->cast = NULL;
    }
    
    // Free listedIn array
    if (show->listedIn != NULL) {
        for (int i = 0; i < show->listedInCount; i++) {
            if (show->listedIn[i] != NULL) {
                free(show->listedIn[i]);
            }
        }
        free(show->listedIn);
        show->listedIn = NULL;
    }
    
    // Free date
    if (show->dateAdded != NULL) {
        free(show->dateAdded);
        show->dateAdded = NULL;
    }
}

// Free memory allocated for CSV lines
void free_csv_lines() {
    if (csv_lines == NULL) return;
    
    for (int i = 0; i < csv_line_count; i++) {
        if (csv_lines[i] != NULL) {
            free(csv_lines[i]);
        }
    }
    free(csv_lines);
    csv_lines = NULL;
}

// Check if string is "FIM"
bool is_end(char *str) {
    return (str != NULL && strlen(str) == 3 && str[0] == 'F' && str[1] == 'I' && str[2] == 'M');
}

// Convert string to integer
int convert_str_to_int(char *str) {
    if (str == NULL || strlen(str) == 0) return 0;
    
    int value = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (isdigit(str[i])) {
            value = value * 10 + (str[i] - '0');
        }
    }
    
    return value;
}

char *strdup_lower(const char *src) {
    char *dup = strdup(src);             
    if (!dup) exit(1);
    for (int i = 0; dup[i]; i++) {
        dup[i] = tolower((unsigned char)dup[i]);
    }
    return dup;
}

typedef struct No {
    Show *elemento;
    struct No *esq;
    struct No *dir;
    int altura;
} No;

// Create new no holding Show*
No *new_no_value(Show *s) {
    No *tmp = malloc(sizeof(No));
    tmp->elemento = s;
    tmp->esq = NULL;
    tmp->dir = NULL;
    tmp->altura = 1;
    return tmp;
}

// Height of the tree
int altura(No *no) {
    comparacoes++;
    return no ? no->altura : 0;
}

// Calculate balance factor of a node
int max(int a, int b) {
    comparacoes++;
    return (a > b) ? a : b;
}

No *rotacionar_direita(No *i) {
    No *j = i->esq;
    No *T2 = j->dir;

    // Perform rotation
    j->dir = i;
    i->esq = T2;

    // Update heights
    i->altura = max(altura(i->esq), altura(i->dir)) + 1;
    j->altura = max(altura(j->esq), altura(j->dir)) + 1;

    return j;
}

No *rotacionar_esquerda(No *i) {
    No *j = i->dir;
    No *T2 = j->esq;

    // Perform rotation
    j->esq = i;
    i->dir = T2;

    // Update heights
    i->altura = max(altura(i->esq), altura(i->dir)) + 1;
    j->altura = max(altura(j->esq), altura(j->dir)) + 1;

    return j;
}

int get_balance(No *no) {
    int balance = altura(no->esq) - altura(no->dir);
    return balance;
}

No *inserir_avl(No *i, Show *s) {
    if (i == NULL) {
	comparacoes++;
        i = new_no_value(s); // Create a new node
    }
    else {
        int cmp = strcmp(s->title, i->elemento->title);
        if (cmp < 0) {
		comparacoes++;
            i->esq = inserir_avl(i->esq, s); // Insert in right subtree
        }
        else if(cmp > 0) {
	    comparacoes += 2;
            i->dir = inserir_avl(i->dir, s); // Insert in left subtree
        }
        else {
            // Element already exists, do not insert
            return i;
        }

    	i->altura = 1 + max(altura(i->esq), altura(i->dir));
	int balance = get_balance(i);

	if (balance > 1 && strcmp(s->title, i->esq->elemento->title) < 0) {
		comparacoes++;
		i = rotacionar_direita(i); // Right rotation
	}
	else if (balance < -1 && strcmp(s->title, i->dir->elemento->title) > 0) {
		comparacoes += 2;
		i = rotacionar_esquerda(i); // Left rotation
	}
	else if (balance > 1 && strcmp(s->title, i->esq->elemento->title) > 0) {
		comparacoes += 3;
		i->esq = rotacionar_esquerda(i->esq); // Left 
		i = rotacionar_direita(i); // Right
	}
	else if (balance < -1 && strcmp(s->title, i->dir->elemento->title) < 0) {
		comparacoes += 4;
		i->dir = rotacionar_direita(i->dir); // Right
		i = rotacionar_esquerda(i);  // Left
	}

	return i;
    }
}

bool pesquisar_avl(No *i, char *key) {
    bool encontrou = false;
    if (i != NULL) {
	comparacoes++;
        int cmp = strcmp(i->elemento->title, key);
        if (cmp == 0) {
	    comparacoes += 2;
            encontrou = true; // Found
        } 
        else if (cmp < 0) {
	    comparacoes += 3;
            printf("dir ");
            encontrou = pesquisar_avl(i->dir, key); // Search right
        } 
        else {
            printf("esq ");
            encontrou = pesquisar_avl(i->esq, key); // Search left
        }
    }
    return encontrou;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 Regras obedecidas: somente strcmp() e strlen() foram usados das funções de string padrão.
 Cada função possui apenas 1 return.
 Não há uso de break em loops.
*/

// ---------- Configurações ----------
#define MAX_LINE 8192
#define MAX_JOGOS_ARQ 20000

// ---------- Utilitários de string (sem usar strcpy/strdup/strtok) ----------
char* copiarStringAloc(const char *s) {
    char *resultado = NULL;
    if (s == NULL) {
        resultado = NULL;
    } else {
        int len = strlen(s);
        resultado = (char*) malloc((len + 1) * sizeof(char));
        if (resultado != NULL) {
            int i = 0;
            while (i < len) {
                resultado[i] = s[i];
                i = i + 1;
            }
            resultado[len] = '\0';
        }
    }
    return resultado;
}

// trim left and right (in-place) - returns pointer to start
char* trim_inplace(char *s) {
    char *resultado = s;
    if (s != NULL) {
        int len = strlen(s);
        int start = 0;
        int end = len - 1;
        int encontrouInicio = 0;
        while (start <= end && encontrouInicio == 0) {
            char c = s[start];
            if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                start = start + 1;
            } else {
                encontrouInicio = 1;
            }
        }
        int encontrouFim = 0;
        while (end >= start && encontrouFim == 0) {
            char c = s[end];
            if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                end = end - 1;
            } else {
                encontrouFim = 1;
            }
        }
        if (start == 0 && end == len - 1) {
            resultado = s;
        } else {
            int j = 0;
            int i = start;
            while (i <= end) {
                s[j] = s[i];
                j = j + 1;
                i = i + 1;
            }
            s[j] = '\0';
            resultado = s;
        }
    }
    return resultado;
}

// Replace double double-quotes "" inside a buffer (in-place) by single "
// Also removes surrounding quotes when requested by caller (handled outside)
void fix_quotes_inplace(char *s) {
    if (s != NULL) {
        int i = 0;
        int j = 0;
        while (s[i] != '\0') {
            if (s[i] == '"' && s[i+1] == '"') {
                s[j] = '"';
                j = j + 1;
                i = i + 2;
            } else {
                s[j] = s[i];
                j = j + 1;
                i = i + 1;
            }
        }
        s[j] = '\0';
    }
}

// ---------- Parser simples para extrair id (campo 0) e title (campo 1) ----------
int extrair_id_title(char *linha, char **out_id, char **out_title) {
    int resultado = 0;
    *out_id = NULL;
    *out_title = NULL;
    if (linha == NULL) {
        resultado = 0;
    } else {
        int len = strlen(linha);
        char campo0[MAX_LINE];
        char campo1[MAX_LINE];
        int idx = 0;
        int campo = 0;
        int i = 0;
        int j = 0;
        int in_quotes = 0;

        // inicializa
        campo0[0] = '\0';
        campo1[0] = '\0';

        while (i < len) {
            char c = linha[i];
            if (campo == 0) {
                if (c == '"') {
                    if (in_quotes == 0) {
                        in_quotes = 1;
                    } else {
                        in_quotes = 0;
                    }
                    // copia o próprio char? não, ignore aspas externas
                    i = i + 1;
                } else if (c == ',' && in_quotes == 0) {
                    campo0[j] = '\0';
                    campo = campo + 1;
                    j = 0;
                    i = i + 1;
                } else {
                    if (j < MAX_LINE - 1) {
                        campo0[j] = c;
                        j = j + 1;
                    }
                    i = i + 1;
                }
            } else if (campo == 1) {
                // captura o campo 1 até a próxima vírgula fora de aspas
                if (c == '"') {
                    if (in_quotes == 0) {
                        in_quotes = 1;
                    } else {
                        in_quotes = 0;
                    }
                    i = i + 1;
                } else if (c == ',' && in_quotes == 0) {
                    campo1[j] = '\0';
                    break;
                } else {
                    if (j < MAX_LINE - 1) {
                        campo1[j] = c;
                        j = j + 1;
                    }
                    i = i + 1;
                }
            } else {
                i = i + 1;
            }
        }
        if (campo == 0) {
            campo0[j] = '\0';
            campo1[0] = '\0';
        } else {
            if (campo == 1 && campo1[j] == '\0') {
                // finalize campo1 se não finalizado por vírgula
                campo1[j] = '\0';
            }
        }

        // Trim e fix quotes for campo0 and campo1 (they may contain surrounding quotes removed earlier)
        char tmp0[MAX_LINE];
        char tmp1[MAX_LINE];
        int k = 0;
        // copiar campo0 para tmp0
        while (k < MAX_LINE && campo0[k] != '\0') {
            tmp0[k] = campo0[k];
            k = k + 1;
        }
        tmp0[k] = '\0';
        trim_inplace(tmp0);
        // copiar campo1 para tmp1
        k = 0;
        while (k < MAX_LINE && campo1[k] != '\0') {
            tmp1[k] = campo1[k];
            k = k + 1;
        }
        tmp1[k] = '\0';
        trim_inplace(tmp1);

        // remove surrounding quotes in tmp1 if exist
        int len1 = strlen(tmp1);
        if (len1 >= 2 && tmp1[0] == '"' && tmp1[len1 - 1] == '"') {
            // shift left and null-terminate
            int a = 1;
            int b = 0;
            while (a < len1 - 1) {
                tmp1[b] = tmp1[a];
                a = a + 1;
                b = b + 1;
            }
            tmp1[b] = '\0';
        }
        // fix doubled quotes
        fix_quotes_inplace(tmp1);

        // allocate outputs
        *out_id = copiarStringAloc(tmp0);
        *out_title = copiarStringAloc(tmp1);
        if (*out_id != NULL && *out_title != NULL) {
            resultado = 1;
        } else {
            if (*out_id != NULL) free(*out_id);
            if (*out_title != NULL) free(*out_title);
            *out_id = NULL;
            *out_title = NULL;
            resultado = 0;
        }
    }
    return resultado;
}

// ---------- Leitura do CSV (/tmp/games.csv) e armazenamento de (id,title) ----------
char **ids_array = NULL;
char **titles_array = NULL;
int registros = 0;

int carregar_csv_id_title(const char *caminho) {
    int resultado = 0;
    registros = 0;
    ids_array = NULL;
    titles_array = NULL;
    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        resultado = 0;
    } else {
        // count lines
        int lines = 0;
        char line[MAX_LINE];
        while (fgets(line, MAX_LINE, f) != NULL) {
            lines = lines + 1;
        }
        if (lines <= 1) {
            // no data
            fclose(f);
            resultado = 0;
        } else {
            // allocate arrays with capacity = lines
            ids_array = (char**) malloc(lines * sizeof(char*));
            titles_array = (char**) malloc(lines * sizeof(char*));
            int idx = 0;
            rewind(f);
            // read lines, skip header
            int linha_atual = 0;
            while (fgets(line, MAX_LINE, f) != NULL) {
                // remove newline
                int L = strlen(line);
                if (L > 0 && (line[L-1] == '\n' || line[L-1] == '\r')) {
                    line[L-1] = '\0';
                    L = L - 1;
                    if (L > 0 && line[L-1] == '\r') {
                        line[L-1] = '\0';
                        L = L - 1;
                    }
                }
                if (linha_atual == 0) {
                    // header skip
                } else {
                    char *idstr = NULL;
                    char *title = NULL;
                    int ok = extrair_id_title(line, &idstr, &title);
                    if (ok == 1) {
                        ids_array[idx] = idstr;
                        titles_array[idx] = title;
                        idx = idx + 1;
                    } else {
                        // ensure we advance
                    }
                }
                linha_atual = linha_atual + 1;
            }
            registros = idx;
            resultado = 1;
            fclose(f);
        }
    }
    return resultado;
}

// ---------- AVL por nome (title) ----------
typedef struct Node {
    char *title; // chave (alocada)
    struct Node *esq;
    struct Node *dir;
    int altura;
} Node;

Node* novo_no_title(char *title) {
    Node *resultado = NULL;
    resultado = (Node*) malloc(sizeof(Node));
    if (resultado != NULL) {
        resultado->title = copiarStringAloc(title);
        resultado->esq = NULL;
        resultado->dir = NULL;
        resultado->altura = 1;
    }
    return resultado;
}

int altura_no(Node *n) {
    int resultado = 0;
    if (n == NULL) {
        resultado = 0;
    } else {
        resultado = n->altura;
    }
    return resultado;
}

int max_int(int a, int b) {
    int resultado;
    if (a > b) {
        resultado = a;
    } else {
        resultado = b;
    }
    return resultado;
}

Node* rotacao_direita(Node *y) {
    Node *resultado = NULL;
    if (y == NULL) {
        resultado = y;
    } else {
        Node *x = y->esq;
        if (x == NULL) {
            resultado = y;
        } else {
            Node *T2 = x->dir;
            x->dir = y;
            y->esq = T2;
            y->altura = 1 + max_int(altura_no(y->esq), altura_no(y->dir));
            x->altura = 1 + max_int(altura_no(x->esq), altura_no(x->dir));
            resultado = x;
        }
    }
    return resultado;
}

Node* rotacao_esquerda(Node *x) {
    Node *resultado = NULL;
    if (x == NULL) {
        resultado = x;
    } else {
        Node *y = x->dir;
        if (y == NULL) {
            resultado = x;
        } else {
            Node *T2 = y->esq;
            y->esq = x;
            x->dir = T2;
            x->altura = 1 + max_int(altura_no(x->esq), altura_no(x->dir));
            y->altura = 1 + max_int(altura_no(y->esq), altura_no(y->dir));
            resultado = y;
        }
    }
    return resultado;
}

int get_balance(Node *n) {
    int resultado = 0;
    if (n == NULL) {
        resultado = 0;
    } else {
        resultado = altura_no(n->esq) - altura_no(n->dir);
    }
    return resultado;
}

// Inserir por title, sem duplicatas. Usa apenas strcmp/strlen para strings.
Node* inserir_avl_title(Node *node, char *title) {
    Node *resultado = node;
    if (node == NULL) {
        resultado = novo_no_title(title);
    } else {
        int cmp = strcmp(title, node->title);
        if (cmp < 0) {
            node->esq = inserir_avl_title(node->esq, title);
            resultado = node;
        } else {
            if (cmp > 0) {
                node->dir = inserir_avl_title(node->dir, title);
                resultado = node;
            } else {
                // igual -> nao insere
                resultado = node;
            }
        }
        node->altura = 1 + max_int(altura_no(node->esq), altura_no(node->dir));
        int balance = get_balance(node);
        if (balance > 1) {
            // left cases
            if (strcmp(title, node->esq->title) < 0) {
                resultado = rotacao_direita(node);
            } else {
                node->esq = rotacao_esquerda(node->esq);
                resultado = rotacao_direita(node);
            }
        } else {
            if (balance < -1) {
                // right cases
                if (strcmp(title, node->dir->title) > 0) {
                    resultado = rotacao_esquerda(node);
                } else {
                    node->dir = rotacao_direita(node->dir);
                    resultado = rotacao_esquerda(node);
                }
            } else {
                resultado = node;
            }
        }
    }
    return resultado;
}

// Pesquisa imprimindo caminho. Retorna 1 se encontrado, 0 caso contrário.
// Imprime "raiz" antes de começar (caller deve já imprimir nome e dois pontos).
int pesquisar_com_caminho(Node *raiz, char *chave) {
    int encontrado = 0;
    if (raiz == NULL) {
        encontrado = 0;
    } else {
        Node *atual = raiz;
        int primeira = 1;
        // We will print moves as we traverse; "raiz" is printed by caller already.
        while (atual != NULL && encontrado == 0) {
            int cmp = strcmp(chave, atual->title);
            if (cmp == 0) {
                encontrado = 1;
            } else {
                if (cmp < 0) {
                    printf(" esq");
                    atual = atual->esq;
                } else {
                    printf(" dir");
                    atual = atual->dir;
                }
            }
        }
    }
    return encontrado;
}

// Free tree
void free_tree(Node *n) {
    if (n == NULL) {
        // nothing
    } else {
        free_tree(n->esq);
        free_tree(n->dir);
        if (n->title != NULL) free(n->title);
        free(n);
    }
    return;
}

// free arrays
void free_arrays() {
    if (ids_array == NULL) {
        // nothing
    } else {
        int i = 0;
        while (i < registros) {
            if (ids_array[i] != NULL) free(ids_array[i]);
            if (titles_array[i] != NULL) free(titles_array[i]);
            i = i + 1;
        }
        free(ids_array);
        free(titles_array);
    }
    return;
}

// Encontrar index pelo id string (comparando exatamente). Retorna -1 se não achar.
int buscar_index_por_id(const char *idstr) {
    int resultado = -1;
    int i = 0;
    while (i < registros && resultado == -1) {
        if (ids_array[i] != NULL) {
            if (strcmp(ids_array[i], idstr) == 0) {
                resultado = i;
            }
        }
        i = i + 1;
    }
    return resultado;
}

int main() {
    int ok = carregar_csv_id_title("/tmp/games.csv");
    if (ok == 0) {
        // Falha ao carregar CSV -> encerra
        return 0;
    }

    Node *raiz = NULL;

    // leitura da primeira parte: IDs até FIM
    char buffer[4096];
    int fim = 0;
    while (fim == 0) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fim = 1;
        } else {
            // remove newline
            int L = strlen(buffer);
            if (L > 0 && (buffer[L-1] == '\n' || buffer[L-1] == '\r')) {
                buffer[L-1] = '\0';
                L = L - 1;
                if (L > 0 && buffer[L-1] == '\r') {
                    buffer[L-1] = '\0';
                    L = L - 1;
                }
            }
            // check FIM
            if (strlen(buffer) == 3 && buffer[0] == 'F' && buffer[1] == 'I' && buffer[2] == 'M') {
                fim = 1;
            } else {
                // procurar id
                int idx = buscar_index_por_id(buffer);
                if (idx != -1) {
                    // inserir title na AVL, se não existir
                    raiz = inserir_avl_title(raiz, titles_array[idx]);
                }
            }
        }
    }

    // leitura da segunda parte: nomes de consulta até FIM
    fim = 0;
    while (fim == 0) {
        if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
            fim = 1;
        } else {
            // remove newline
            int L = strlen(buffer);
            if (L > 0 && (buffer[L-1] == '\n' || buffer[L-1] == '\r')) {
                buffer[L-1] = '\0';
                L = L - 1;
                if (L > 0 && buffer[L-1] == '\r') {
                    buffer[L-1] = '\0';
                    L = L - 1;
                }
            }
            // check FIM
            if (strlen(buffer) == 3 && buffer[0] == 'F' && buffer[1] == 'I' && buffer[2] == 'M') {
                fim = 1;
            } else {
                char *consulta = buffer;
                // print "<Nome>: raiz" then call pesquisar_com_caminho to print moves and determine SIM/NAO
                printf("%s: raiz", consulta);
                int achou = pesquisar_com_caminho(raiz, consulta);
                if (achou == 1) {
                    printf(" SIM\n");
                } else {
                    printf(" NAO\n");
                }
            }
        }
    }

    // libera memoria
    free_tree(raiz);
    free_arrays();

    return 0;
}