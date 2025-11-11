#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define MAX 5  // Tamanho máximo da fila

// Estrutura que representa uma peça do Tetris
typedef struct {
    char tipo;  // Tipo da peça ('I', 'O', 'T', 'L')
    int id;     // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca itens[MAX];
    int inicio;
    int fim;
    int total;
} Fila;

// ---------- Funções de controle da fila ----------

// Inicializa a fila
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->total = 0;
}

// Verifica se a fila está cheia
int filaCheia(Fila *f) {
    return f->total == MAX;
}

// Verifica se a fila está vazia
int filaVazia(Fila *f) {
    return f->total == 0;
}

// ---------- Função auxiliar ----------

// Gera uma nova peça automaticamente com tipo aleatório e ID único
Peca gerarPeca() {
    static int contador = 0;  // Garante IDs únicos mesmo após várias chamadas
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.tipo = tipos[rand() % 4];
    nova.id = contador++;
    return nova;
}

// ---------- Operações da fila ----------

// Insere uma nova peça na fila (enqueue)
// A peça é gerada automaticamente apenas se houver espaço
void enfileirar(Fila *f) {
    if (filaCheia(f)) {
        printf("\nFila cheia! Não é possível adicionar mais peças.\n");
        return;
    }

    // Só gera a peça se houver espaço
    Peca nova = gerarPeca();
    f->itens[f->fim] = nova;
    f->fim = (f->fim + 1) % MAX;
    f->total++;

    printf("\nNova peça gerada: [%c %d]\n", nova.tipo, nova.id);
}

// Remove a peça da frente da fila (dequeue)
Peca desenfileirar(Fila *f) {
    if (filaVazia(f)) {
        printf("\nFila vazia! Nenhuma peça para jogar.\n");
        Peca vazia = {'-', -1};
        return vazia;
    }

    Peca removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    f->total--;
    return removida;
}

// Mostra o conteúdo atual da fila
void mostrarFila(Fila *f) {
    printf("\nFila de peças futuras:\n");

    if (filaVazia(f)) {
        printf("[Fila vazia]\n");
        return;
    }

    for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX) {
        printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
    }

    printf("\n");
}

// ---------- Função principal ----------

int main() {
    setlocale(LC_ALL, "Portuguese"); // Suporte a português

    srand(time(NULL));
    Fila fila;
    inicializarFila(&fila);

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < MAX; i++) {
        enfileirar(&fila);
    }

    int opcao;
    do {
        mostrarFila(&fila);

        printf("\nOpções:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Peca jogada = desenfileirar(&fila);
                if (jogada.id != -1) {
                    printf("\nPeça jogada: [%c %d]\n", jogada.tipo, jogada.id);
                }
                break;
            }
            case 2:
                enfileirar(&fila);
                break;
            case 0:
                printf("\nSaindo do jogo...\n");
                break;
            default:
                printf("\nOpção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
