#include <stdio.h>
#include <stdbool.h>

bool ehIgual(char* s1, char* s2);
bool ehIgualRecursivo(char* s1, char* s2, int i);
bool ehVogal(char c);
bool ehLetra(char c);
bool isSomenteVogal(char* s);
bool isSomenteVogalRecursivo(char* teste, int i);
bool isSomenteConsoante(char* s);
bool isSomenteConsoanteRecursivo(char* teste, int i);
bool isNumeroInteiro(char* s);
bool isNumeroInteiroRecursivo(char* teste, int i);
bool isNumeroReal(char* s);
bool isNumeroRealRecursivo(char* teste, int i, int contadorPonto);

int meuStrlen(char* s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

bool ehIgual(char* s1, char* s2) {
    bool resp;
    int len1 = meuStrlen(s1);
    int len2 = meuStrlen(s2);

    if (len1 != len2) {
        resp = false;
    } else {
        resp = ehIgualRecursivo(s1, s2, 0);
    }
    return resp;
}

bool ehIgualRecursivo(char* s1, char* s2, int i) {
    bool resp;
    if (s1[i] == '\0') {
        resp = true;
    } else {
        if (s1[i] != s2[i]) {
            resp = false;
        } else {
            resp = ehIgualRecursivo(s1, s2, i + 1);
        }
    }
    return resp;
}

bool ehVogal(char c) {
    bool resp;
    if (c >= 'A' && c <= 'Z') {
        c = c + 32;
    }

    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
        resp = true;
    } else {
        resp = false;
    }
    return resp;
}

bool ehLetra(char c) {
    bool resp;
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        resp = true;
    } else {
        resp = false;
    }
    return resp;
}

bool isSomenteVogal(char* s) {
    return isSomenteVogalRecursivo(s, 0);
}

bool isSomenteVogalRecursivo(char* teste, int i) {
    bool resp;
    if (teste[i] == '\0') {
        resp = true;
    } else {
        if (ehVogal(teste[i])) {
            resp = isSomenteVogalRecursivo(teste, i + 1);
        } else {
            resp = false;
        }
    }
    return resp;
}

bool isSomenteConsoante(char* s) {
    return isSomenteConsoanteRecursivo(s, 0);
}

bool isSomenteConsoanteRecursivo(char* teste, int i) {
    bool resp;
    if (teste[i] == '\0') {
        resp = true;
    } else {
        if (ehLetra(teste[i]) && !ehVogal(teste[i])) {
            resp = isSomenteConsoanteRecursivo(teste, i + 1);
        } else {
            resp = false;
        }
    }
    return resp;
}

bool isNumeroInteiro(char* s) {
    return isNumeroInteiroRecursivo(s, 0);
}

bool isNumeroInteiroRecursivo(char* teste, int i) {
    bool resp;
    if (teste[i] == '\0') {
        resp = true;
    } else {
        if (teste[i] >= '0' && teste[i] <= '9') {
            resp = isNumeroInteiroRecursivo(teste, i + 1);
        } else {
            resp = false;
        }
    }
    return resp;
}

bool isNumeroReal(char* s) {
    return isNumeroRealRecursivo(s, 0, 0);
}

bool isNumeroRealRecursivo(char* teste, int i, int contadorPonto) {
    bool resp;
    if (teste[i] == '\0') {
        if (contadorPonto == 1) {
            resp = true;
        } else {
            resp = false;
        }
    } else {
        char c = teste[i];
        if (c == '.') {
            contadorPonto++;
        }
        
        if ((c < '0' || c > '9') && c != '.') {
            resp = false;
        } else if (contadorPonto > 1) {
            resp = false;
        } else {
            resp = isNumeroRealRecursivo(teste, i + 1, contadorPonto);
        }
    }
    return resp;
}

int main() {
    char entrada[1000];
    
    fgets(entrada, sizeof(entrada), stdin);
    int len = meuStrlen(entrada);
    if (len > 0 && entrada[len-1] == '\n') {
        entrada[len-1] = '\0';
    }

    while (!ehIgual(entrada, "FIM")) {
        bool isVogal = isSomenteVogal(entrada);
        bool isConsoante = isSomenteConsoante(entrada);
        bool isInteiro = isNumeroInteiro(entrada);
        bool isReal = isNumeroReal(entrada);

        if (isVogal) {
            printf("SIM ");
        } else {
            printf("NAO ");
        }
        
        if (isConsoante) {
            printf("SIM ");
        } else {
            printf("NAO ");
        }

        if (isInteiro) {
            printf("SIM ");
        } else {
            printf("NAO ");
        }

        if (isReal) {
            printf("SIM\n");
        } else {
            printf("NAO\n");
        }
        
        fgets(entrada, sizeof(entrada), stdin);
        len = meuStrlen(entrada);
        if (len > 0 && entrada[len-1] == '\n') {
            entrada[len-1] = '\0';
        }
    }
    
    return 0;
}