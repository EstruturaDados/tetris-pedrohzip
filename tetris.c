#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5

typedef struct {
    char nome; // 'I', 'O', 'T', 'L'
    int id;    // identificador único
} Peca;

typedef struct {
    Peca dados[MAX_FILA];
    int head;
    int tail; // posição para inserir próxima peça
    int count;
} FilaCircular;

static int nextId = 1;

char nomesPossiveis[] = { 'I', 'O', 'T', 'L' };

Peca gerarPeca() {
    Peca p;
    p.nome = nomesPossiveis[rand() % (sizeof(nomesPossiveis)/sizeof(nomesPossiveis[0]))];
    p.id = nextId++;
    return p;
}

void initFila(FilaCircular *f) {
    f->head = 0;
    f->tail = 0;
    f->count = 0;
    for (int i = 0; i < MAX_FILA; ++i) {
        // enfileira 5 peças iniciais
        Peca p = gerarPeca();
        f->dados[f->tail] = p;
        f->tail = (f->tail + 1) % MAX_FILA;
        f->count++;
    }
}

int filaVazia(FilaCircular *f) {
    return f->count == 0;
}

int filaCheia(FilaCircular *f) {
    return f->count == MAX_FILA;
}

int enqueue(FilaCircular *f, Peca p) {
    if (filaCheia(f)) return 0; // falha
    f->dados[f->tail] = p;
    f->tail = (f->tail + 1) % MAX_FILA;
    f->count++;
    return 1; // sucesso
}

int dequeue(FilaCircular *f, Peca *out) {
    if (filaVazia(f)) return 0;
    if (out) *out = f->dados[f->head];
    f->head = (f->head + 1) % MAX_FILA;
    f->count--;
    return 1;
}

void mostrarFila(FilaCircular *f) {
    printf("\nEstado da fila (frente -> final) [count=%d]:\n", f->count);
    if (filaVazia(f)) {
        printf("  <vazia>\n");
        return;
    }
    int idx = f->head;
    for (int i = 0; i < f->count; ++i) {
        Peca p = f->dados[idx];
        printf("  [%d] %c", p.id, p.nome);
        if (i == 0) printf("  <- frente");
        printf("\n");
        idx = (idx + 1) % MAX_FILA;
    }
    printf("\n");
}

int main(void) {
    srand((unsigned) time(NULL));

    FilaCircular fila;
    initFila(&fila);

    int opcao = -1;
    while (opcao != 0) {
        mostrarFila(&fila);
        printf("Menu:\n");
        printf(" 1 - Jogar peça (dequeue) => remove frente e INSERE automaticamente nova peça no final\n");
        printf(" 2 - Inserir nova peça manualmente (enqueue)\n");
        printf(" 3 - Visualizar fila\n");
        printf(" 0 - Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            // entrada inválida: descartar restante da linha
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            opcao = -1;
            continue;
        }

        if (opcao == 1) {
            Peca jogada;
            if (!dequeue(&fila, &jogada)) {
                printf("Fila vazia: nada para jogar.\n");
            } else {
                printf("Peça jogada: id=%d nome=%c\n", jogada.id, jogada.nome);
                // inserir automaticamente nova peça no final
                Peca nova = gerarPeca();
                if (enqueue(&fila, nova)) {
                    printf("Nova peça inserida automaticamente: id=%d nome=%c\n", nova.id, nova.nome);
                } else {
                    // Normalmente não ocorre porque removemos antes
                    printf("Falha ao inserir nova peça (fila cheia).\n");
                }
            }
        } else if (opcao == 2) {
            if (filaCheia(&fila)) {
                printf("Não é possível inserir: fila está cheia.\n");
            } else {
                Peca nova = gerarPeca();
                enqueue(&fila, nova);
                printf("Peça inserida manualmente: id=%d nome=%c\n", nova.id, nova.nome);
            }
        } else if (opcao == 3) {
            // apenas mostra no início do loop; nada a fazer aqui
        } else if (opcao == 0) {
            printf("Saindo...\n");
        } else {
            printf("Opção inválida.\n");
        }
    }

    return 0;
}