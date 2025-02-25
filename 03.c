#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char* elements;
    int top;
    int maxSize;
} Stack;

void initStack(Stack* stack, int size) {
    stack->elements = (char*)malloc(size * sizeof(char));
    if (stack->elements == NULL) { // Kontrola, či bola pamäť úspešne alokovaná
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    stack->top = -1;
    stack->maxSize = size;
}

int isFull(Stack* stack) {
    return stack->top == stack->maxSize - 1;
}

int isEmpty(Stack* stack) {
    return stack->top == -1;
}

void push(Stack* stack, char element) {
    if (!isFull(stack)) {
        stack->elements[++stack->top] = element;
    } else {
        fprintf(stderr, "Stack overflow\n");
        exit(EXIT_FAILURE);
    }
}

char pop(Stack* stack) {
    if (!isEmpty(stack)) {
        return stack->elements[stack->top--];
    }
    return '\0'; // Vrátí nulový znak, ak je zásobník prázdny
}

int isMatchingPair(char character1, char character2) {
    if (character1 == '(' && character2 == ')') return 1;
    if (character1 == '{' && character2 == '}') return 1;
    if (character1 == '[' && character2 == ']') return 1;
    if (character1 == '<' && character2 == '>') return 1;
    return 0;
}

int countBracketErrors(char* expression) {
    Stack stack;
    initStack(&stack, 100000); // Zvýšená kapacita pre dlhšie reťazce
    int errorCount = 0;

    for (int i = 0; expression[i]; i++) {
        if (expression[i] == '(' || expression[i] == '{' || expression[i] == '[' || expression[i] == '<') {
            push(&stack, expression[i]);
        } else if (expression[i] == ')' || expression[i] == '}' || expression[i] == ']' || expression[i] == '>') {
            if (isEmpty(&stack) || !isMatchingPair(pop(&stack), expression[i])) {
                errorCount++;
            }
        }
    }

    while (!isEmpty(&stack)) {
        pop(&stack);
        errorCount++;
    }

    free(stack.elements);
    return errorCount;
}

int main() {
    int N;
    if (scanf("%d", &N) != 1) {
        fprintf(stderr, "Invalid input\n");
        return 1;
    }

    for (int i = 0; i < N; i++) {
        char* expression = (char*)malloc(100001 * sizeof(char));
        if (expression == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
        if (scanf("%s", expression) != 1) {
            fprintf(stderr, "Failed to read the string\n");
            free(expression);
            continue;
        }
        printf("Chyby v postupnosti %d: %d\n", i + 1, countBracketErrors(expression));
        free(expression);
    }

    return 0;
}
