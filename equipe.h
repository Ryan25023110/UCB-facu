#ifndef EQUIPE_H
#define EQUIPE_H

typedef struct {

    int Posicao;
    char Estado[32];
    char Time[32];
    int Pontos;
    int J;
    int V;
    int E;
    int D;
    int GP;
    int GC;
    int SG;
    float Aproveitamento;

} Equipe;

Equipe * lerDados(char * Arquivo);

#endif
