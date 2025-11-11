#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define MAX_FILA 5
#define MAX_PILHA 3

// Estrutura que representa uma peça
typedef struct {
    char tipo;
    int id;
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

// ---------- Funções auxiliares ----------

// Gera uma nova peça com tipo aleatório e ID sequencial
Peca gerarPeca() {
    static int contador = 0;
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.tipo = tipos[rand() % 4];
    nova.id = contador++;
    return nova;
}

// ---------- Controle da fila ----------

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

void enfileirar(Fila *f) {
    if (filaCheia(f)) return;
    Peca nova = gerarPeca();
    f->itens[f->fim] = nova;
    f->fim = (f->fim + 1) % MAX_FILA;
    f->total++;
}

Peca desenfileirar(Fila *f) {
    Peca removida = {'-', -1};
    if (filaVazia(f)) return removida;
    removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % MAX_FILA;
    f->total--;
    return removida;
}

// ---------- Controle da pilha ----------

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

int pilhaCheia(Pilha *p) {
    return p->topo == MAX_PILHA - 1;
}

int pilhaVazia(Pilha *p) {
    return p->topo == -1;
}

void empilhar(Pilha *p, Peca nova) {
    if (pilhaCheia(p)) {
        printf("\nPilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    p->topo++;
    p->itens[p->topo] = nova;
}

Peca desempilhar(Pilha *p) {
    Peca removida = {'-', -1};
    if (pilhaVazia(p)) return removida;
    removida = p->itens[p->topo];
    p->topo--;
    return removida;
}

// ---------- Exibição ----------

void mostrarEstado(Fila *f, Pilha *p) {
    printf("\n================== ESTADO ATUAL ==================\n");

    printf("Fila de peças futuras:\n");
    if (filaVazia(f)) printf("[Fila vazia]\n");
    else {
        for (int i = 0, idx = f->inicio; i < f->total; i++, idx = (idx + 1) % MAX_FILA)
            printf("[%c %d] ", f->itens[idx].tipo, f->itens[idx].id);
        printf("\n");
    }

    printf("\nPilha de reserva (Topo -> Base):\n");
    if (pilhaVazia(p)) printf("[Pilha vazia]\n");
    else {
        for (int i = p->topo; i >= 0; i--)
            printf("[%c %d] ", p->itens[i].tipo, p->itens[i].id);
        printf("\n");
    }

    printf("==================================================\n");
}

// ---------- Operações principais ----------

// 1. Jogar peça da frente da fila
void jogarPeca(Fila *f) {
    if (filaVazia(f)) {
        printf("\nFila vazia! Nenhuma peça para jogar.\n");
        return;
    }
    Peca jogada = desenfileirar(f);
    printf("Peça jogada: [%c %d]\n", jogada.tipo, jogada.id);
    enfileirar(f); // Mantém a fila cheia
}

// 2. Reservar peça (mover da fila para a pilha)
void reservarPeca(Fila *f, Pilha *p) {
    if (pilhaCheia(p)) {
        printf("\nPilha cheia! Não é possível reservar mais peças.\n");
        return;
    }
    if (filaVazia(f)) {
        printf("\nFila vazia! Nenhuma peça para reservar.\n");
        return;
    }
    Peca frente = desenfileirar(f);
    empilhar(p, frente);
    printf("Peça reservada: [%c %d]\n", frente.tipo, frente.id);
    enfileirar(f); // repõe a fila
}

// 3. Usar peça reservada (remover do topo da pilha)
void usarPecaReservada(Pilha *p) {
    if (pilhaVazia(p)) {
        printf("\nPilha vazia! Nenhuma peça reservada.\n");
        return;
    }
    Peca usada = desempilhar(p);
    printf("Peça usada da reserva: [%c %d]\n", usada.tipo, usada.id);
}

// 4. Trocar peça atual da fila com o topo da pilha
void trocarPeca(Fila *f, Pilha *p) {
    if (filaVazia(f)) {
        printf("\nFila vazia! Nenhuma peça para trocar.\n");
        return;
    }
    if (pilhaVazia(p)) {
        printf("\nPilha vazia! Nenhuma peça reservada para trocar.\n");
        return;
    }

    int idxFila = f->inicio;
    int idxPilha = p->topo;

    Peca temp = f->itens[idxFila];
    f->itens[idxFila] = p->itens[idxPilha];
    p->itens[idxPilha] = temp;

    printf("Peça da frente da fila trocada com o topo da pilha.\n");
}

// 5. Troca múltipla flexível (até 3 peças)
void trocaMultipla(Fila *f, Pilha *p) {
    if (pilhaVazia(p)) {
        printf("\nPilha vazia! Nenhuma peça para troca múltipla.\n");
        return;
    }

    int qtdTrocar = p->topo + 1; // quantidade de peças na pilha
    if (qtdTrocar > 3) qtdTrocar = 3; // no máximo 3

    for (int i = 0; i < qtdTrocar; i++) {
        int idxFila = (f->inicio + i) % MAX_FILA;
        int idxPilha = p->topo - i;

        Peca temp = f->itens[idxFila];
        f->itens[idxFila] = p->itens[idxPilha];
        p->itens[idxPilha] = temp;
    }

    printf("Troca múltipla realizada entre as %d primeiras da fila e as %d da pilha.\n",
           qtdTrocar, qtdTrocar);
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
    for (int i = 0; i < MAX_FILA; i++) enfileirar(&fila);

    int opcao;
    do {
        mostrarEstado(&fila, &pilha);

        printf("\nOpções:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar as 3 primeiras da fila com as peças da pilha (até onde houver)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: jogarPeca(&fila); break;
            case 2: reservarPeca(&fila, &pilha); break;
            case 3: usarPecaReservada(&pilha); break;
            case 4: trocarPeca(&fila, &pilha); break;
            case 5: trocaMultipla(&fila, &pilha); break;
            case 0: printf("\nEncerrando o programa...\n"); break;
            default: printf("\nOpção inválida.\n");
        }

    } while (opcao != 0);

    return 0;
}
