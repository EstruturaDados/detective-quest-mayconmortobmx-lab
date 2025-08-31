#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// -----------------------------
// 🌱 Estruturas de Salas (Árvore Binária)
// -----------------------------
typedef struct Sala {
    char nome[50];
    struct Sala* esquerda;
    struct Sala* direita;
} Sala;

Sala* criarSala(const char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esquerda = NULL;
    nova->direita = NULL;
    return nova;
}

void explorarSalas(Sala* sala) {
    if (!sala) return;
    printf("Você está na sala: %s\n", sala->nome);
    char opcao;
    printf("Ir para esquerda (e), direita (d) ou sair (s)? ");
    scanf(" %c", &opcao);
    if (opcao == 'e') explorarSalas(sala->esquerda);
    else if (opcao == 'd') explorarSalas(sala->direita);
    else printf("Saindo da exploração...\n");
}

// -----------------------------
// 🔍 Estruturas de Pistas (BST)
// -----------------------------
typedef struct Pista {
    char texto[100];
    struct Pista* esquerda;
    struct Pista* direita;
} Pista;

Pista* criarPista(const char* texto) {
    Pista* p = (Pista*)malloc(sizeof(Pista));
    strcpy(p->texto, texto);
    p->esquerda = p->direita = NULL;
    return p;
}

Pista* inserirPista(Pista* raiz, const char* texto) {
    if (!raiz) return criarPista(texto);
    if (strcmp(texto, raiz->texto) < 0) raiz->esquerda = inserirPista(raiz->esquerda, texto);
    else raiz->direita = inserirPista(raiz->direita, texto);
    return raiz;
}

void listarPistas(Pista* raiz) {
    if (!raiz) return;
    listarPistas(raiz->esquerda);
    printf("- %s\n", raiz->texto);
    listarPistas(raiz->direita);
}

// -----------------------------
// 🧠 Estruturas de Suspeitos (Hash Simples)
// -----------------------------
typedef struct NodoPista {
    char texto[100];
    struct NodoPista* prox;
} NodoPista;

typedef struct Suspeito {
    char nome[50];
    NodoPista* pistas;
    struct Suspeito* prox;
} Suspeito;

#define HASH_SIZE 26
Suspeito* tabelaHash[HASH_SIZE];

int hash(char* nome) {
    return (nome[0] - 'A') % HASH_SIZE;
}

Suspeito* buscarSuspeito(char* nome) {
    int h = hash(nome);
    Suspeito* atual = tabelaHash[h];
    while (atual) {
        if (strcmp(atual->nome, nome) == 0) return atual;
        atual = atual->prox;
    }
    return NULL;
}

void inserirHash(char* nomeSuspeito, char* textoPista) {
    Suspeito* s = buscarSuspeito(nomeSuspeito);
    if (!s) {
        s = (Suspeito*)malloc(sizeof(Suspeito));
        strcpy(s->nome, nomeSuspeito);
        s->pistas = NULL;
        int h = hash(nomeSuspeito);
        s->prox = tabelaHash[h];
        tabelaHash[h] = s;
    }
    NodoPista* np = (NodoPista*)malloc(sizeof(NodoPista));
    strcpy(np->texto, textoPista);
    np->prox = s->pistas;
    s->pistas = np;
}

void listarAssociacoes() {
    for (int i = 0; i < HASH_SIZE; i++) {
        Suspeito* s = tabelaHash[i];
        while (s) {
            printf("Suspeito: %s\n", s->nome);
            NodoPista* np = s->pistas;
            while (np) {
                printf("  - Pista: %s\n", np->texto);
                np = np->prox;
            }
            s = s->prox;
        }
    }
}

// -----------------------------
// Main
// -----------------------------
int main() {
    // Inicializa hash
    for (int i = 0; i < HASH_SIZE; i++) tabelaHash[i] = NULL;

    // Criando mapa da mansão
    Sala* hall = criarSala("Hall de Entrada");
    Sala* biblioteca = criarSala("Biblioteca");
    Sala* cozinha = criarSala("Cozinha");
    Sala* sotao = criarSala("Sótão");

    hall->esquerda = biblioteca;
    hall->direita = cozinha;
    biblioteca->esquerda = sotao;

    explorarSalas(hall);

    // BST de pistas
    Pista* arvorePistas = NULL;
    arvorePistas = inserirPista(arvorePistas, "Pegada de sapato");
    arvorePistas = inserirPista(arvorePistas, "Chave no chão");
    arvorePistas = inserirPista(arvorePistas, "Rastro de lama");

    printf("\nPistas coletadas em ordem:\n");
    listarPistas(arvorePistas);

    // Inserindo suspeitos e associando pistas
    inserirHash("Alice", "Pegada de sapato");
    inserirHash("Bob", "Chave no chão");
    inserirHash("Alice", "Rastro de lama");

    printf("\nSuspeitos e pistas:\n");
    listarAssociacoes();

    return 0;
}

