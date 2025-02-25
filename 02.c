#include <stdio.h>
#include <stdlib.h>

#define MAX_DECIMAL_LENGTH 11 // Desetinná část + null terminator

typedef struct {
    int celaCast;
    char desatinnaCast[MAX_DECIMAL_LENGTH];
    int jeZaporne;
} Cislo;

// Vlastná implementácia zjištění délky řetězce
int vlastneStrlen(const char *str) {
    int dlzka = 0;
    while (str[dlzka] != '\0') dlzka++;
    return dlzka;
}

// Funkce pro normalizaci desetinných částí
void normalizujDesatinne(char *desatinna) {
    int len = vlastneStrlen(desatinna);
    // Doplnění nul, aby byla délka desetinné části rovna 10
    while (len < 10) {
        desatinna[len++] = '0';
    }
    desatinna[len] = '\0'; // Ujistěte se, že řetězec je správně ukončený
    // Odstranění koncových nul
    while (len > 1 && desatinna[len - 1] == '0') {
        desatinna[--len] = '\0';
    }
}

// Funkce na odstranění vedoucích nul
void odstranVeducoNuly(char *s) {
    int i, j;
    for (i = 0; s[i] == '0'; i++); // Přeskočí všechny vedoucí nuly
    if (i > 0) {
        for (j = 0; s[i]; i++, j++) { // Posunout zbývající znaky
            s[j] = s[i];
        }
        s[j] = '\0'; // Nastavit nový konec řetězce
    }
    // Pokud je řetězec po odstranění vedoucích nul prázdný, vrátíme "0"
    if (s[0] == '\0') {
        s[0] = '0';
        s[1] = '\0';
    }
}

// Upravená funkce strcmp pro desetinné části
// Upravená funkce strcmp pro desetinné části s pomocnými výpisy
int vlastneStrcmp(const char *a, const char *b) {
    char tempA[MAX_DECIMAL_LENGTH], tempB[MAX_DECIMAL_LENGTH];
    for (int i = 0; i < MAX_DECIMAL_LENGTH; i++) {
        tempA[i] = a[i];
        tempB[i] = b[i];
    }
    normalizujDesatinne(tempA);
    normalizujDesatinne(tempB);

    for (int i = 0; tempA[i] != '\0' || tempB[i] != '\0'; i++) {
        if (tempA[i] != tempB[i]) {
            return (tempA[i] < tempB[i]) ? -1 : 1;
        }
    }
    return 0; // Řetězce jsou stejné
}


void vlastneStrcpy(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0'; // Uistite sa, že reťazec je správne ukončený
}

void vlastneStrtok(char *str, char **cela, char **desat, char delim) {
    int i = 0;
    *cela = str; // Začiatok celé časti
    while (str[i] != '\0') {
        if (str[i] == delim) {
            str[i] = '\0'; // Nastavíme delimitér na koniec reťazca
            *desat = &str[i + 1]; // Nastavíme začiatok desatinnej časti
            return;
        }
        i++;
    }
    *desat = NULL; // Ak delimitér nebol nájdený, desatinnej časti nie je
}

void nacitajCislo(Cislo *cislo) {
    char vstup[50];
    scanf("%s", vstup);
    cislo->jeZaporne = (vstup[0] == '-'); // Kontrola, či je číslo záporné
    if (cislo->jeZaporne) {
        // Odstránenie znamienka mínus
        for (int i = 0; i < vlastneStrlen(vstup); i++) {
            vstup[i] = vstup[i + 1];
        }
    }
    char *celaCast, *desatinnaCast;
    vlastneStrtok(vstup, &celaCast, &desatinnaCast, ',');
    cislo->celaCast = atoi(celaCast);
    if (desatinnaCast != NULL) {
        vlastneStrcpy(cislo->desatinnaCast, desatinnaCast);
    } else {
        cislo->desatinnaCast[0] = '\0'; // Nastavíme prázdny reťazec, ak neexistuje desatinná časť
    }
}

int porovnajCisla(const Cislo *a, const Cislo *b) {
    if (a->jeZaporne != b->jeZaporne) {
        return a->jeZaporne ? -1 : 1;
    }
    if (a->celaCast == b->celaCast) {
        int desatCompare = vlastneStrcmp(a->desatinnaCast, b->desatinnaCast);
        return a->jeZaporne ? -desatCompare : desatCompare;
    }
    return (a->jeZaporne ? (a->celaCast > b->celaCast) : (a->celaCast < b->celaCast)) ? -1 : 1;
}

int vyhladaj(Cislo *cisla, int N, Cislo hladane) {
    int low = 0, high = N - 1, mid;
    while (low <= high) {
        mid = low + (high - low) / 2;
        int result = porovnajCisla(&cisla[mid], &hladane);
        if (result == 0) {
            return mid + 1;
        } else if (result > 0) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return 0;
}

int main() {
    int N;
    scanf("%d", &N);
    if (N <= 1 || N >= 50000) {
        return 0;
    }
    Cislo *cisla = (Cislo *)malloc(N * sizeof(Cislo));

    for (int i = 0; i < N; i++) {
        nacitajCislo(&cisla[i]);
    }

    int M;
    scanf("%d", &M);
    if (M < 1 || M > 100000) {
        printf("Nespravna hodnota M\\n");
        free(cisla);
        return 1;
    }
    for (int i = 0; i < M; i++) {
        Cislo hladane;
        nacitajCislo(&hladane);
        int vysledok = vyhladaj(cisla, N, hladane);
        if (i == M - 1) {
            printf("%d", vysledok);
        } else {
            printf("%d\n", vysledok);
        }
    }

    free(cisla);

    return 0;
}
