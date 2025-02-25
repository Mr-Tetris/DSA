#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 10000

typedef struct person {
    char key[50];
    char firstName[30];
    char lastName[30];
    char dob[11];
    double balance; // Balance ako double
    struct person *next;
} Person;

Person *hashTable[TABLE_SIZE];

unsigned int my_strlen(const char *str) {
    unsigned int length = 0;
    while (*str++) {
        length++;
    }
    return length;
}

int my_strcmp(char* s1, char* s2)
{
    int i = 0;
    while (s1[i] != '\0')
    {
        if (s1[i] != s2[i])
        {
            return 1;
        }
        i++;
    }
    return 0;
}

char *my_strncpy(char *dest, const char *src, unsigned int n) {
    char *save = dest;
    while (n > 0 && (*dest++ = *src++)) {
        n--;
    }
    if (n > 0) {
        *dest = '\0';
    } else {
        *(dest - 1) = '\0';
    }

    return save;
}

char *my_strrchr(const char *s, int c) {
    const char *last_occurrence = NULL;
    do {
        if (*s == c) {
            last_occurrence = s;
        }
    } while (*s++);
    return (char *)last_occurrence;
}

void *my_memset(void *s, int c, unsigned int n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

unsigned int hash(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash % TABLE_SIZE;
}

Person *searchPerson(char *key) {
    int index = hash(key);
    Person *person = hashTable[index];
    while (person != NULL) {
        if (my_strcmp(person->key, key) == 0) {
            return person;
        }
        person = person->next;
    }
    return NULL;
}

void insertPerson(char *firstName, char *lastName, char *dob, double balance, int *first) {
    char key[50];
    sprintf(key, "%s %s %s", firstName, lastName, dob);

    if (searchPerson(key) != NULL) {
        printf(*first ? "insert failed" : "\ninsert failed");
        *first = 0;
        return;
    }

    Person *newPerson = (Person *)malloc(sizeof(Person));
    if (!newPerson) {
        printf(*first ? "insert failed" : "\ninsert failed");
        *first = 0;
        return;
    }

    my_strncpy(newPerson->firstName, firstName, sizeof(newPerson->firstName) - 1);
    my_strncpy(newPerson->lastName, lastName, sizeof(newPerson->lastName) - 1);
    my_strncpy(newPerson->dob, dob, sizeof(newPerson->dob) - 1);
    newPerson->balance = balance; // Priamo ukladá hodnotu bez zaokrúhľovania
    my_strncpy(newPerson->key, key, sizeof(newPerson->key) - 1);

    int index = hash(key);
    newPerson->next = hashTable[index];
    hashTable[index] = newPerson;
}

void updatePerson(char *key, double amount, int *first) {
    Person *person = searchPerson(key);
    if (person != NULL && (person->balance + amount > 0)) {

        person->balance += amount;
    } else {
        if (*first == 1){
            printf("update failed");
            *first = 0;
        } else {
            printf("\nupdate failed");
        }
    }
}

void deletePerson(char *key, int *first) {
    int index = hash(key);
    Person *person = hashTable[index], *prev = NULL;
    while (person != NULL) {
        if (my_strcmp(person->key, key) == 0) {
            if (prev == NULL) {
                hashTable[index] = person->next;
            } else {
                prev->next = person->next;
            }
            free(person);
            return;
        }
        prev = person;
        person = person->next;
    }
    printf(*first ? "delete failed" : "\ndelete failed");
    *first = 0;
}
void replaceCommaWithDot(char *str) {
    for (; *str; str++) {
        if (*str == ',') {
            *str = '.';
        }
    }
}

void replaceDotWithComma(char *str) {
    for (; *str; str++) {
        if (*str == '.') {
            *str = ',';
            break;
        }
    }
}

int main() {
    my_memset(hashTable, 0, sizeof(Person *) * TABLE_SIZE);
    char operation;
    int first = 1;
    char firstName[30], lastName[30], dob[11], key[50], balanceStr[30];
    double balance;

    while (scanf(" %c", &operation) != EOF) {

        switch (operation) {
            case 'i':
                if (scanf("%29s %29s %10s %29s", firstName, lastName, dob, balanceStr) == 4) {
                    replaceCommaWithDot(balanceStr);
                    sscanf(balanceStr, "%lf", &balance);
                    insertPerson(firstName, lastName, dob, balance, &first);
                } else {
                    printf(first ? "insert failed" : "\ninsert failed");
                    first = 0;
                }
                break;
            case 's':
                if (scanf("%29s %29s %10s", firstName, lastName, dob) == 3) {
                    sprintf(key, "%s %s %s", firstName, lastName, dob);
                    Person *person = searchPerson(key);
                    if (person) {
                        sprintf(balanceStr, "%.2lf", person->balance);
                        replaceDotWithComma(balanceStr); // Zmena . na ,
                        printf(first ? "%s" : "\n%s", balanceStr);
                        first = 0;
                    } else {
                        printf(first ? "search failed" : "\nsearch failed");
                        first = 0;
                    }
                }
                break;
            case 'u':
                if (scanf("%29s %29s %10s %29s", firstName, lastName, dob, balanceStr) == 4) {

                    replaceCommaWithDot(balanceStr); // Zmena , na .
                    sprintf(key, "%s %s %s", firstName, lastName, dob);
                    sscanf(balanceStr, "%lf", &balance);
                    updatePerson(key, balance, &first);
                } else {
                    printf(first ? "update failed" : "\nupdate failed");
                    first = 0;
                }
                break;
            case 'd':
                if (scanf("%29s %29s %10s", firstName, lastName, dob) == 3) {
                    sprintf(key, "%s %s %s", firstName, lastName, dob);
                    deletePerson(key, &first);
                } else {
                    printf(first ? "delete failed" : "\ndelete failed");
                    first = 0;
                }
                break;
            default:
                break;
        }
    }
    return 0;
}