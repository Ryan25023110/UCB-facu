#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#ifndef EXPRESSAO_H
#define EXPRESSAO_H

typedef struct {

    char posFixa[512];
    char inFixa[512];
    float Valor;

} Expressao;

char *getInFixa(char *Str);
float getValor(char *Str);

#endif

#define MAX 512
#define PI 3.14159265358979323846

typedef struct {

    float v[MAX];
    int topo;

} Pilha;

void iniciar(Pilha *p) {

    p->topo = -1;
}

int vazia(Pilha *p) {

    return p->topo == -1;
}

void push(Pilha *p, float n) {

    p->v[++p->topo] = n;
}

float pop(Pilha *p) {

    return p->v[p->topo--];
}

typedef struct {

    char texto[MAX][MAX];
    int prio[MAX];
    int topo;

} PilhaStr;

void iniciarStr(PilhaStr *p) {

    p->topo = -1;
}

int vaziaStr(PilhaStr *p) {

    return p->topo == -1;
}

void pushStr(PilhaStr *p, char s[], int prioridade) {

    strcpy(p->texto[++p->topo], s);
    p->prio[p->topo] = prioridade;
}

char *popStr(PilhaStr *p, int *prioridade) {

    *prioridade = p->prio[p->topo];

    return p->texto[p->topo--];
}


int operador(char s[]) {

    if(strcmp(s, "+") == 0)
        return 1;

    if(strcmp(s, "-") == 0)
        return 1;

    if(strcmp(s, "*") == 0)
        return 1;

    if(strcmp(s, "/") == 0)
        return 1;

    if(strcmp(s, "%") == 0)
        return 1;

    if(strcmp(s, "^") == 0)
        return 1;

    return 0;
}

int funcao(char s[]) {

    if(strcmp(s, "log") == 0)
        return 1;

    if(strcmp(s, "sen") == 0)
        return 1;

    if(strcmp(s, "cos") == 0)
        return 1;

    if(strcmp(s, "tg") == 0)
        return 1;

    if(strcmp(s, "raiz") == 0)
        return 1;

    return 0;
}


int numero(char s[]) {

    char *fim;

    strtod(s, &fim);

    return *fim == '\0';
}

int prioridade(char op[]) {

    if(strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
        return 1;

    if(strcmp(op, "*") == 0 ||
       strcmp(op, "/") == 0 ||
       strcmp(op, "%") == 0)
        return 2;

    if(strcmp(op, "^") == 0)
        return 3;

    return 4;
}

    float rad(float g) {

    return g * PI / 180.0;

}

float getValor(char *Str)
{
    Pilha p;

    iniciar(&p);

    char aux[MAX];

    strcpy(aux, Str);

    char *token = strtok(aux, " ");

    while(token != NULL) {


        if(numero(token)) {

            push(&p, atof(token));
        }


        else if(operador(token)) {

            if(p.topo < 1)
                return NAN;

            float b = pop(&p);
            float a = pop(&p);

            float r;

            if(strcmp(token, "+") == 0)
                r = a + b;

            else if(strcmp(token, "-") == 0)
                r = a - b;

            else if(strcmp(token, "*") == 0)
                r = a * b;

            else if(strcmp(token, "/") == 0) {

                if(b == 0)
                    return NAN;

                r = a / b;
            }

           else if(strcmp(token, "%") == 0) {

             if((int)b == 0)
             return NAN;

            if(a != (int)a || b != (int)b)
            return NAN;

             r = (int)a % (int)b;

            }

            else {

                r = pow(a, b);
            }

            push(&p, r);
        }


        else if(funcao(token)) {

            if(vazia(&p))
                return NAN;

            float a = pop(&p);

            float r;

            if(strcmp(token, "log") == 0) {

                if(a <= 0)
                    return NAN;

                r = log10(a);
            }

            else if(strcmp(token, "raiz") == 0) {

                if(a < 0)
                    return NAN;

                r = sqrt(a);
            }

            else if(strcmp(token, "sen") == 0) {

                r = sin(rad(a));
            }

            else if(strcmp(token, "cos") == 0) {

                r = cos(rad(a));
            }

            else {

                r = tan(rad(a));
            }

            push(&p, r);
        }


        else {

            return NAN;
        }

        token = strtok(NULL, " ");
    }

    if(p.topo != 0)
        return NAN;

  return pop(&p);
}

char *getInFixa(char *Str)
{
    PilhaStr p;

    iniciarStr(&p);

    static char resp[MAX];
    char aux[MAX];

    strcpy(aux, Str);

    char *token = strtok(aux, " ");

    while(token != NULL)
    {
        if(numero(token))
        {
            pushStr(&p, token, 4);
        }

        else if(operador(token))
        {
            if(p.topo < 1)
                return NULL;

            int pa, pb;

            char a[MAX];
            char b[MAX];
            char temp[MAX];

            strcpy(b, popStr(&p, &pb));
            strcpy(a, popStr(&p, &pa));

            int pAtual = prioridade(token);

            char esq[MAX];
            char dir[MAX];

            if(pa < pAtual)
                sprintf(esq, "(%s)", a);
            else
                strcpy(esq, a);

            if(pb < pAtual ||
              ((strcmp(token,"-")==0 ||
                strcmp(token,"/")==0 ||
                strcmp(token,"^")==0) && pb == pAtual))
                sprintf(dir, "(%s)", b);
            else
                strcpy(dir, b);

            sprintf(temp, "%s%s%s",
                    esq,
                    token,
                    dir);

            pushStr(&p, temp, pAtual);
        }

        else if(funcao(token))
        {
            if(vaziaStr(&p))
                return NULL;

            int prio;

            char a[MAX];
            char temp[MAX];

            strcpy(a, popStr(&p, &prio));

            sprintf(temp, "%s(%s)", token, a);

            pushStr(&p, temp, 4);
        }

        else
        {
            return NULL;
        }

        token = strtok(NULL, " ");
    }

    if(p.topo != 0)
        return NULL;

    int pfinal;

    strcpy(resp, popStr(&p, &pfinal));

    return resp;
}

char *getPreFixaInterna(char *Str)
{
    PilhaStr p;

    iniciarStr(&p);

    static char resp[MAX];
    char aux[MAX];

    strcpy(aux, Str);

    char *token = strtok(aux, " ");

    while(token != NULL)
    {
        if(numero(token))
        {
            pushStr(&p, token, 0);
        }

        else if(operador(token))
        {
            if(p.topo < 1)
                return NULL;

            int p1, p2;

            char a[MAX];
            char b[MAX];
            char temp[MAX];

            strcpy(b, popStr(&p, &p2));
            strcpy(a, popStr(&p, &p1));

            sprintf(temp, "%s %s %s", token, a, b);

            pushStr(&p, temp, 0);
        }

        else if(funcao(token))
        {
            if(vaziaStr(&p))
                return NULL;

            int prio;

            char a[MAX];
            char temp[MAX];

            strcpy(a, popStr(&p, &prio));

            sprintf(temp, "%s %s", token, a);

            pushStr(&p, temp, 0);
        }

        else
        {
            return NULL;
        }

        token = strtok(NULL, " ");
    }

    if(p.topo != 0)
        return NULL;

    int pFinal;

    strcpy(resp, popStr(&p, &pFinal));

    return resp;
}

       
