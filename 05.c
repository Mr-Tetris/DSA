#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int id;
    char firstName[30];
    char lastName[30];
    char birthDate[11];
    int height;
    struct Node *left;
    struct Node *right;
} Node;

Node* newNode(int id, char* firstName, char* lastName, char* birthDate);
int height(Node *N);
int max(int a, int b);
Node *rightRotate(Node *y);
Node *leftRotate(Node *x);
int getBalance(Node *N);
Node* insert(Node* node, int id, char* firstName, char* lastName, char* birthDate);
Node* deleteNode(Node* root, int id);
Node * findMinValueNode(Node* node);
void printInOrder(Node* root, int low, int high, int* firstPrint);
Node* search(Node* root, int id);


void myStrcpy(char *dest, const char *src) {
    while (*src) {
        *dest++ = *src++;
    }
    *dest = '\0';
}



Node* newNode(int id, char* firstName, char* lastName, char* birthDate) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->id = id;
    myStrcpy(node->firstName, firstName);
    myStrcpy(node->lastName, lastName);
    myStrcpy(node->birthDate, birthDate);
    node->height = 1;
    node->left = NULL;
    node->right = NULL;
    return(node);
}


int height(Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b) {
    return (a > b)? a : b;
}

Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;
    return x;
}

Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;
    return y;
}

int getBalance(Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node* insert(Node* node, int id, char* firstName, char* lastName, char* birthDate) {
    if (node == NULL)
        return(newNode(id, firstName, lastName, birthDate));
    if (id < node->id)
        node->left = insert(node->left, id, firstName, lastName, birthDate);
    else if (id > node->id)
        node->right = insert(node->right, id, firstName, lastName, birthDate);
    else
        return node;
    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);
    if (balance > 1 && id < node->left->id)
        return rightRotate(node);
    if (balance < -1 && id > node->right->id)
        return leftRotate(node);
    if (balance > 1 && id > node->left->id) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && id < node->right->id) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

Node* deleteNode(Node* root, int id) {
    if (root == NULL)
        return root;
    if (id < root->id)
        root->left = deleteNode(root->left, id);
    else if (id > root->id)
        root->right = deleteNode(root->right, id);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            Node *temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            Node* temp = findMinValueNode(root->right);
            root->id = temp->id;
            myStrcpy(root->firstName, temp->firstName);
            myStrcpy(root->lastName, temp->lastName);
            myStrcpy(root->birthDate, temp->birthDate);
            root->right = deleteNode(root->right, temp->id);
        }
    }
    if (root == NULL)
        return root;
    root->height = max(height(root->left), height(root->right)) + 1;
    int balance = getBalance(root);
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

Node * findMinValueNode(Node* node) {
    Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

Node* search(Node* root, int id) {
    if (root == NULL || root->id == id)
        return root;
    if (root->id < id)
        return search(root->right, id);
    return search(root->left, id);
}

void printInOrder(Node* root, int low, int high, int* firstPrint) {
    if (root != NULL) {
        printInOrder(root->left, low, high, firstPrint);
        if (root->id >= low && root->id <= high) {
            if (!(*firstPrint)) {
                printf("\n");
            }
            printf("%d %s %s %s", root->id, root->firstName, root->lastName, root->birthDate);
            *firstPrint = 0;
        }
        printInOrder(root->right, low, high, firstPrint);
    }
}

int main() {
    Node* root = NULL;
    char operation;
    int firstPrint = 1;
    while (scanf(" %c", &operation) != EOF) {
        if (operation == 'i') {
            int id;
            char firstName[24], lastName[24], birthDate[11];
            if (scanf("%d %s %s %s", &id, firstName, lastName, birthDate) == 4) {
                root = insert(root, id, firstName, lastName, birthDate);
            }
        } else if (operation == 's') {
            int id1, id2;
            if (scanf("%d %d", &id1, &id2) == 2) {
                if (!firstPrint) printf("\n");
                firstPrint = 1;
                printInOrder(root, id1, id2, &firstPrint);
                if (!firstPrint) {
                    firstPrint = 0;
                }
            } else {
                Node *result = search(root, id1);
                if (result != NULL) {
                    if (!firstPrint) printf("\n");
                    printf("%d %s %s %s", result->id, result->firstName, result->lastName, result->birthDate);
                    firstPrint = 0;
                }
            }
        } else if (operation == 'd') {
            int id;
            if (scanf("%d", &id) == 1) {
                root = deleteNode(root, id);
            }
        }
    }
    return 0;
}