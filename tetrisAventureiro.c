#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define MAX_FILA 5    // Capacidade máxima da fila
#define MAX_PILHA 3   // Capacidade máxima da pilha

// Estrutura que representa uma peça do Tetris
typedef struct {
    char tipo;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

// Estrutura da fila circular de peças futuras
typedef struct {
    Peca itens[MAX_FILA];
    int inicio;
    int fim;
    int total;
} Fila;

// Estrutura da pilha de peças reservadas
typedef struct {
    Peca itens[MAX_PILHA];
    int topo;
} Pilha;

// ---------- Funções da Fila ----------

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

int filaCheia(Fila *f) {
    return f->total == MAX_FILA;
}

int filaVazia(Fila *f) {
    return f->total == 0;
}

// ---------- Funções da Pilha ----------

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

// ---------- Função auxiliar ----------

// Gera uma nova peça automaticamente com tipo aleatório e ID único
Peca gerarPeca() {
    static int contador = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.tipo = tipos[rand() % 4];
    nova.id = contador++;
    return nova;
}

// ---------- Operações da Fila ----------

void enfileirar(Fila *f) {
    if (filaCheia(f)) {
        printf("\nFila cheia! Não é possível adicionar mais peças.\n");
        return;
    }

    Peca nova = gerarPeca();
    f->itens[f->fim] = nova;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;

    printf("Nova peça gerada: [%c %d]\n", nova.tipo, nova.id);
}

Peca desenfileirar(Fila *f) {
    if (filaVazia(f)) {
        printf("\nFila vazia! Nenhuma peça para jogar.\n");
        Peca vazia = {'-', -1};
        return vazia;
    }

    Peca removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return removida;
}

// ---------- Operações da Pilha ----------

void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("\nPilha cheia! Não é possível reservar mais peças.\n");
        return;
    }

    p->topo++;
    p->itens[p->topo] = nova;
    printf("Peça reservada: [%c %d]\n", nova.tipo, nova.id);
}

Peca desempilhar(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("\nPilha vazia! Nenhuma peça reservada para usar.\n");
        Peca vazia = {'-', -1};
        return vazia;
    }

    Peca removida = p->itens[p->topo];
    p->topo--;
    return removida;
}

// ---------- Exibição das estruturas ----------

void mostrarEstado(Fila *f, Pilha *p) {
    printf("\n================== ESTADO ATUAL ==================\n");

    // Exibe a fila
    printf("Fila de peças futuras:\n");
    if (filaVazia(f))
        printf("[Fila vazia]\n");
    else {
        for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_FILA) {
            printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
        }
        printf("\n");
    }

    // Exibe a pilha
    printf("\nPilha de reserva (Topo -> Base):\n");
    if (pilhaVazia(p))
        printf("[Pilha vazia]\n");
    else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
        }
        printf("\n");
    }

    printf("==================================================\n");
}

// ---------- Função principal ----------

int main() {
    setlocale(LC_ALL, "Portuguese");

    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Inicializa a fila com 5 peças
    for (int i = 0; i < MAX_FILA; i++) {
        enfileirar(&fila);
    }

    int opcao;
    do {
        mostrarEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                // Jogar peça
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1) {
                    printf("Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                    enfileirar(&fila); // Reposição automática
                }
                break;
            }
			case 2: {
			    // Reservar peça
			    if (pilhaCheia(&pilha)) {
			        printf("\nPilha cheia! Não é possível reservar mais peças.\n");
			        break;
			    }
			
			    Peca frente = desenfileirar(&fila);
			    if (frente.id != -1) {
			        empilhar(&pilha, frente);
			        enfileirar(&fila); // Mantém a fila cheia
			    }
			    break;
			}

            case 3: {
                // Usar peça reservada
                Peca usada = desempilhar(&pilha);
                if (usada.id != -1)
                    printf("Peça usada da reserva: [%c %d]\n", usada.tipo, usada.id);
                break;
            }
            case 0:
                printf("\nEncerrando o programa...\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
