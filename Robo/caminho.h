#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "caminho.h"

Posicao *criarPosicao(int x, int y) {

    Posicao *novo = (Posicao *) malloc(sizeof(Posicao));

    if (novo == NULL)
        return NULL;

    novo->X = x;
    novo->Y = y;
    novo->Proximo = NULL;

    return novo;
}

Comando *criarComando(char letra) {

    Comando *novo = (Comando *) malloc(sizeof(Comando));

    if (novo == NULL)
        return NULL;

    novo->Direcao = letra;
    novo->Proximo = NULL;

    return novo;
}

int ehValido(char c) {

    if (c == 'N')
        return 1;

    if (c == 'S')
        return 1;

    if (c == 'L')
        return 1;

    if (c == 'O')
        return 1;

    return 0;
}

Caminho *InicializarCaminho(const char *Sequencia, int xInicial, int yInicial) {

    if (Sequencia == NULL)
        return NULL;

    Caminho *c = (Caminho *) malloc(sizeof(Caminho));

    if (c == NULL)
        return NULL;

    c->Inicio = NULL;
    c->Fim = NULL;
    c->Historico = NULL;
    c->Instrucoes = NULL;
    c->N = 0;

    Posicao *inicio = criarPosicao(xInicial, yInicial);

    if (inicio == NULL) {
        free(c);
        return NULL;
    }

    c->Inicio = inicio;
    c->Historico = inicio;

    Comando *ultimo = NULL;

    int i = 0;

    while (Sequencia[i] != '\0') {

        if (ehValido(Sequencia[i]) == 0) {
            DestruirCaminho(c);
            return NULL;
        }

        Comando *novo = criarComando(Sequencia[i]);

        if (novo == NULL) {
            DestruirCaminho(c);
            return NULL;
        }

        if (c->Instrucoes == NULL) {
            c->Instrucoes = novo;
        }
        else {
            ultimo->Proximo = novo;
        }

        ultimo = novo;

        c->N++;

        i++;
    }

    HistoricoPosicoes(c);

    DeterminarFim(c);

    return c;
}

Posicao *HistoricoPosicoes(Caminho *C) {

    if (C == NULL)
        return NULL;

    if (C->Historico == NULL)
        return NULL;

    if (C->Historico->Proximo != NULL)
        return C->Historico;

    int x = C->Inicio->X;
    int y = C->Inicio->Y;

    Posicao *ultimo = C->Inicio;

    Comando *aux = C->Instrucoes;

    while (aux != NULL) {

        if (aux->Direcao == 'N') {
            y++;
        }

        else if (aux->Direcao == 'S') {
            y--;
        }

        else if (aux->Direcao == 'L') {
            x++;
        }

        else if (aux->Direcao == 'O') {
            x--;
        }

        Posicao *nova = criarPosicao(x, y);

        if (nova == NULL)
            return NULL;

        ultimo->Proximo = nova;

        ultimo = nova;

        aux = aux->Proximo;
    }

    return C->Historico;
}

Posicao *DeterminarFim(Caminho *C) {

    if (C == NULL)
        return NULL;

    Posicao *aux = C->Historico;

    while (aux->Proximo != NULL) {
        aux = aux->Proximo;
    }

    C->Fim = aux;

    return aux;
}

int CalcularDistanciaTotal(Caminho *C) {

    if (C == NULL)
        return -1;

    return C->N;
}

double CalcularDistanciaGeometrica(Caminho *C) {

    if (C == NULL)
        return -1.0;

    Posicao *fim = DeterminarFim(C);

    if (fim == NULL)
        return -1.0;

    int x1 = C->Inicio->X;
    int y1 = C->Inicio->Y;

    int x2 = fim->X;
    int y2 = fim->Y;

    double resultado;

    resultado = sqrt((x2 - x1) * (x2 - x1) +
                     (y2 - y1) * (y2 - y1));

    return resultado;
}

int CalcularDistanciaManhattan(Caminho *C) {

    if (C == NULL)
        return -1;

    Posicao *fim = DeterminarFim(C);

    if (fim == NULL)
        return -1;

    int dx = abs(fim->X - C->Inicio->X);
    int dy = abs(fim->Y - C->Inicio->Y);

    return dx + dy;
}

int ContarInstrucoes(Caminho *C) {

    if (C == NULL)
        return -1;

    return C->N;
}

void DestruirCaminho(Caminho *C) {

    if (C == NULL)
        return;

    Posicao *p = C->Historico;

    while (p != NULL) {

        Posicao *temp = p;

        p = p->Proximo;

        free(temp);
    }

    Comando *cmd = C->Instrucoes;

    while (cmd != NULL) {

        Comando *temp = cmd;

        cmd = cmd->Proximo;

        free(temp);
    }

    free(C);
}