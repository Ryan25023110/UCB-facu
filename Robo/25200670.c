#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "caminho.h"

Posicao *novaPos(int x, int y) {

    Posicao *p;

    p = (Posicao *) malloc(sizeof(Posicao));

    if (p == NULL)
        return NULL;

    p->X = x;
    p->Y = y;
    p->Proximo = NULL;

    return p;
}

Comando *novoCmd(char c) {

    Comando *cmd;

    cmd = (Comando *) malloc(sizeof(Comando));

    if (cmd == NULL)
        return NULL;

    cmd->Direcao = c;
    cmd->Proximo = NULL;

    return cmd;
}

int valido(char c) {

    return (c == 'N' || c == 'S' || c == 'L' || c == 'O');
}

Caminho *InicializarCaminho(const char *Sequencia, int xInicial, int yInicial) {

    Caminho *c;

    if (Sequencia == NULL)
        return NULL;

    c = (Caminho *) malloc(sizeof(Caminho));

    if (c == NULL)
        return NULL;

    c->Inicio = novaPos(xInicial, yInicial);

    if (c->Inicio == NULL) {
        free(c);
        return NULL;
    }

    c->Historico = c->Inicio;
    c->Fim = NULL;
    c->Instrucoes = NULL;
    c->N = 0;

    Comando *ultimo = NULL;

    int i = 0;

    while (Sequencia[i] != '\0') {

        if (!valido(Sequencia[i])) {
            DestruirCaminho(c);
            return NULL;
        }

        Comando *novo = novoCmd(Sequencia[i]);

        if (novo == NULL) {
            DestruirCaminho(c);
            return NULL;
        }

        if (c->Instrucoes == NULL)
            c->Instrucoes = novo;
        else
            ultimo->Proximo = novo;

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

    if (C->Historico->Proximo != NULL)
        return C->Historico;

    int x = C->Inicio->X;
    int y = C->Inicio->Y;

    Posicao *fimLista = C->Inicio;

    Comando *cmd = C->Instrucoes;

    while (cmd != NULL) {

        if (cmd->Direcao == 'N')
            y++;

        if (cmd->Direcao == 'S')
            y--;

        if (cmd->Direcao == 'L')
            x++;

        if (cmd->Direcao == 'O')
            x--;

        Posicao *nova = novaPos(x, y);

        if (nova == NULL)
            return NULL;

        fimLista->Proximo = nova;

        fimLista = nova;

        cmd = cmd->Proximo;
    }

    return C->Historico;
}

Posicao *DeterminarFim(Caminho *C) {

    if (C == NULL)
        return NULL;

    Posicao *p = C->Historico;

    while (p->Proximo != NULL) {
        p = p->Proximo;
    }

    C->Fim = p;

    return p;
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

    int dx = fim->X - C->Inicio->X;
    int dy = fim->Y - C->Inicio->Y;

    return sqrt(dx * dx + dy * dy);
}

int CalcularDistanciaManhattan(Caminho *C) {

    if (C == NULL)
        return -1;

    Posicao *fim = DeterminarFim(C);

    int x;
    int y;

    x = abs(fim->X - C->Inicio->X);
    y = abs(fim->Y - C->Inicio->Y);

    return x + y;
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

        Posicao *aux = p;

        p = p->Proximo;

        free(aux);
    }

    Comando *c = C->Instrucoes;

    while (c != NULL) {

        Comando *aux = c;

        c = c->Proximo;

        free(aux);
    }

    free(C);
}