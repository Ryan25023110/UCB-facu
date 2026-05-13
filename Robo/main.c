#include <stdio.h>
#include "caminho.h"

void mostrarHistorico(Posicao *p) {

    while (p != NULL) {

        printf("(%d,%d)", p->X, p->Y);

        if (p->Proximo != NULL) {
            printf(" -> ");
        }

        p = p->Proximo;
    }

    printf("\n");
}

int main() {

    Caminho *c;

    Posicao *fim;

    Posicao *historico;

    c = InicializarCaminho("LLLLLLLNNNNN", 2, 2);

    if (c == NULL) {

        printf("Erro ao criar caminho\n");

        return 1;
    }

    fim = DeterminarFim(c);

    if (fim != NULL) {

        printf("Posicao final: (%d,%d)\n",
               fim->X,
               fim->Y);
    }

    historico = HistoricoPosicoes(c);

    printf("\nHistorico de posicoes:\n");

    mostrarHistorico(historico);

    printf("\n");

    printf("Distancia total: %d\n",
           CalcularDistanciaTotal(c));

    printf("Distancia geometrica: %.2lf\n",
           CalcularDistanciaGeometrica(c));

    printf("Distancia Manhattan: %d\n",
           CalcularDistanciaManhattan(c));

    printf("Quantidade de instrucoes: %d\n",
           ContarInstrucoes(c));

    DestruirCaminho(c);

    return 0;
}
