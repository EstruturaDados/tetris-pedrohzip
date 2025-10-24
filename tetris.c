#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_FILA 5
#define MAX_PILHA 3

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

typedef struct {
    Peca dados[MAX_PILHA];
    int topo;
} PilhaReserva;

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

void initPilha(PilhaReserva *p) {
    p->topo = -1;
}

int pilhaVazia(PilhaReserva *p) {
    return p->topo == -1;
}

int pilhaCheia(PilhaReserva *p) {
    return p->topo == MAX_PILHA - 1;
}

int push(PilhaReserva *p, Peca peca) {
    if (pilhaCheia(p)) return 0;
    p->topo++;
    p->dados[p->topo] = peca;
    return 1;
}

int pop(PilhaReserva *p, Peca *out) {
    if (pilhaVazia(p)) return 0;
    if (out) *out = p->dados[p->topo];
    p->topo--;
    return 1;
}

void mostrarPilha(PilhaReserva *p) {
    printf("\nEstado da pilha de reserva [tamanho=%d]:\n", p->topo + 1);
    if (pilhaVazia(p)) {
        printf("  <vazia>\n");
        return;
    }
    for (int i = p->topo; i >= 0; i--) {
        Peca peca = p->dados[i];
        printf("  [%d] %c", peca.id, peca.nome);
        if (i == p->topo) printf("  <- topo");
        printf("\n");
    }
    printf("\n");
}

int main(void) {
    srand((unsigned) time(NULL));

    FilaCircular fila;
    PilhaReserva pilha;
    initFila(&fila);
    initPilha(&pilha);

    int opcao = -1;
    while (opcao != 0) {
        mostrarFila(&fila);
        mostrarPilha(&pilha);
        printf("\nMenu:\n");
        printf(" 1 - Jogar peça\n");
        printf(" 2 - Reservar peça\n");
        printf(" 3 - Usar peça reservada\n");
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
                    printf("Falha ao inserir nova peça (fila cheia).\n");
                }
            }
        } else if (opcao == 2) {
            if (pilhaCheia(&pilha)) {
                printf("Não é possível reservar: pilha de reserva está cheia.\n");
            } else {
                Peca peca;
                if (!dequeue(&fila, &peca)) {
                    printf("Fila vazia: nada para reservar.\n");
                } else {
                    if (push(&pilha, peca)) {
                        printf("Peça reservada com sucesso: id=%d nome=%c\n", peca.id, peca.nome);
                        // inserir automaticamente nova peça no final da fila
                        Peca nova = gerarPeca();
                        if (enqueue(&fila, nova)) {
                            printf("Nova peça inserida automaticamente: id=%d nome=%c\n", nova.id, nova.nome);
                        }
                    } else {
                        printf("Erro ao reservar peça.\n");
                        // Devolver a peça para a fila
                        enqueue(&fila, peca);
                    }
                }
            }
        } else if (opcao == 3) {
            Peca reservada;
            if (!pop(&pilha, &reservada)) {
                printf("Pilha de reserva vazia: nenhuma peça para usar.\n");
            } else {
                printf("Usando peça reservada: id=%d nome=%c\n", reservada.id, reservada.nome);
            }
        } else if (opcao == 0) {
            printf("Saindo...\n");
        } else {
            printf("Opção inválida.\n");
        }
    }

    return 0;
}