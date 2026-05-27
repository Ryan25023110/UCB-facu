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

/* pilha numeros */

typedef struct {

    float v[MAX];
    int topo;

} Pilha;

void iniciar(Pilha *p) {

    p->topo = -1;
}

void push(Pilha *p, float n) {

    p->v[++p->topo] = n;
}

float pop(Pilha *p) {

    return p->v[p->topo--];
}

int vazia(Pilha *p) {

    if(p->topo == -1)
        return 1;

    return 0;
}

/* pilha texto */

typedef struct {

    char texto[MAX][MAX];
    int topo;

} PilhaStr;

void iniciarStr(PilhaStr *p) {

    p->topo = -1;
}

void pushStr(PilhaStr *p, char s[]) {

    strcpy(p->texto[++p->topo], s);
}

char *popStr(PilhaStr *p) {

    return p->texto[p->topo--];
}

int vaziaStr(PilhaStr *p) {

    if(p->topo == -1)
        return 1;

    return 0;
}

/* verifica operador */

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

/* verifica funcao */

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

/* graus para radianos */

float rad(float g) {

    return g * M_PI / 180;
}

/* calcula valor */

float getValor(char *Str) {

    Pilha p;

    iniciar(&p);

    char aux[MAX];

    strcpy(aux, Str);

    char *token = strtok(aux, " ");

    while(token != NULL) {

        /* numero */

        if(!operador(token) && !funcao(token)) {

            push(&p, atof(token));
        }

        /* operador */

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

                r = (int)a % (int)b;
            }

            else
                r = pow(a, b);

            push(&p, r);
        }

        /* funcoes */

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

            else if(strcmp(token, "sen") == 0)
                r = sin(rad(a));

            else if(strcmp(token, "cos") == 0)
                r = cos(rad(a));

            else
                r = tan(rad(a));

            push(&p, r);
        }

        token = strtok(NULL, " ");
    }

    if(p.topo != 0)
        return NAN;

    return pop(&p);
}

/* converter infixa */

char *getInFixa(char *Str) {

    PilhaStr p;

    iniciarStr(&p);

    static char resposta[MAX];

    char aux[MAX];

    strcpy(aux, Str);

    char *token = strtok(aux, " ");

    while(token != NULL) {

        /* numero */

        if(!operador(token) && !funcao(token)) {

            pushStr(&p, token);
        }

        /* operador */

        else if(operador(token)) {

            if(p.topo < 1)
                return NULL;

            char b[MAX];
            char a[MAX];
            char temp[MAX];

            strcpy(b, popStr(&p));
            strcpy(a, popStr(&p));

            sprintf(temp, "(%s%s%s)", a, token, b);

            pushStr(&p, temp);
        }

        /* funcao */

        else if(funcao(token)) {

            if(vaziaStr(&p))
                return NULL;

            char a[MAX];
            char temp[MAX];

            strcpy(a, popStr(&p));

            if(strcmp(token, "raiz") == 0)
                sprintf(temp, "raiz(%s)", a);

            else
                sprintf(temp, "%s(%s)", token, a);

            pushStr(&p, temp);
        }

        token = strtok(NULL, " ");
    }

    if(p.topo != 0)
        return NULL;

    strcpy(resposta, popStr(&p));

    return resposta;
} 