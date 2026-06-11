#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


#define MAX 512
#define PI 3.14159265358979323846

typedef struct {
    float v[MAX];
    int   topo;
} Pilha;

static void iniciar(Pilha *p)       { p->topo = -1; }
static int  vazia  (Pilha *p)       { return p->topo == -1; }

static void push(Pilha *p, float n)
{
    if (p->topo < MAX - 1)
        p->v[++p->topo] = n;
}

static float pop(Pilha *p)
{
    return p->v[p->topo--];
}


typedef struct {
    char *texto[MAX]; 
    int   prio [MAX];
    int   topo;
} PilhaStr;

static void iniciarStr(PilhaStr *p)
{
    p->topo = -1;
    memset(p->texto, 0, sizeof(p->texto));
}


static void liberarStr(PilhaStr *p)
{
    for (int i = 0; i <= p->topo; i++) {
        free(p->texto[i]);
        p->texto[i] = NULL;
    }
    p->topo = -1;
}

static int vaziaStr(PilhaStr *p) { return p->topo == -1; }

static void pushStr(PilhaStr *p, const char *s, int prioridade)
{
    if (p->topo >= MAX - 1) return;
    free(p->texto[p->topo + 1]);          /* garante sem vazamento */
    p->texto[++p->topo] = strdup(s);
    p->prio [  p->topo] = prioridade;
}


static char *popStr(PilhaStr *p, int *prioridade)
{
    *prioridade    = p->prio[p->topo];
    char *ret      = p->texto[p->topo];
    p->texto[p->topo] = NULL;
    p->topo--;
    return ret;  
}

static int ehOperador(const char *s)
{
    return (strcmp(s,"+") == 0 || strcmp(s,"-") == 0 ||
            strcmp(s,"*") == 0 || strcmp(s,"/") == 0 ||
            strcmp(s,"%") == 0 || strcmp(s,"^") == 0);
}

static int ehFuncao(const char *s)
{
    return (strcmp(s,"log")  == 0 || strcmp(s,"sen")  == 0 ||
            strcmp(s,"cos")  == 0 || strcmp(s,"tg")   == 0 ||
            strcmp(s,"raiz") == 0);
}

static int ehNumero(const char *s)
{
    char *fim;
    strtod(s, &fim);
    return (*fim == '\0' && fim != s);   /* CORRECAO 4: rejeita string vazia */
}


static int prioOp(const char *op)
{
    if (strcmp(op,"+") == 0 || strcmp(op,"-") == 0) return 1;
    if (strcmp(op,"*") == 0 || strcmp(op,"/") == 0 ||
        strcmp(op,"%") == 0)                         return 2;
    if (strcmp(op,"^") == 0)                         return 3;
    return 0;
}

static float toRad(float g) { return g * (float)PI / 180.0f; }


float getValor(char *Str)
{
    if (!Str) return NAN;

    Pilha p;
    iniciar(&p);

    char aux[MAX];
    strncpy(aux, Str, MAX - 1);
    aux[MAX - 1] = '\0';

    char *token = strtok(aux, " ");

    while (token != NULL)
    {
     
        if (ehNumero(token))
        {
            push(&p, (float)atof(token));
        }

        else if (ehOperador(token))
        {
            if (p.topo < 1)         
                return NAN;

            float b = pop(&p);
            float a = pop(&p);
            float r;

            if      (strcmp(token,"+") == 0) r = a + b;
            else if (strcmp(token,"-") == 0) r = a - b;
            else if (strcmp(token,"*") == 0) r = a * b;
            else if (strcmp(token,"/") == 0) {
                if (fabsf(b) < 1e-9f) return NAN;   /* divisao por zero */
                r = a / b;
            }
            else if (strcmp(token,"%") == 0) {
                /* CORRECAO 5: modulo exige inteiros e divisor != 0 */
                if ((long)b == 0) return NAN;
                r = (float)((long)a % (long)b);
            }
            else /* ^ */
                r = (float)pow(a, b);

            push(&p, r);
        }

     
        else if (ehFuncao(token))
        {
            if (vazia(&p)) return NAN;

            float a = pop(&p);
            float r;

            if (strcmp(token,"log") == 0) {
                if (a <= 0.0f) return NAN;           
                r = (float)log10(a);
            }
            else if (strcmp(token,"raiz") == 0) {
                if (a < 0.0f) return NAN;          
                r = (float)sqrt(a);
            }
            else if (strcmp(token,"sen") == 0)
                r = (float)sin(toRad(a));
            else if (strcmp(token,"cos") == 0)
                r = (float)cos(toRad(a));
            else /* tg */ {
                float ang = toRad(a);
                if (fabsf(cosf(ang)) < 1e-9f) return NAN; /* tg indefinida */
                r = (float)tan(ang);
            }

            push(&p, r);
        }

        
        else
            return NAN;

        token = strtok(NULL, " ");
    }

    if (p.topo != 0) return NAN;
    return pop(&p);
}


char *getInFixa(char *Str)
{
    if (!Str) return NULL;

    PilhaStr p;
    iniciarStr(&p);

    char aux[MAX];
    strncpy(aux, Str, MAX - 1);
    aux[MAX - 1] = '\0';

    char *token = strtok(aux, " ");

    while (token != NULL)
    {
     
        if (ehNumero(token))
        {
            pushStr(&p, token, PRIO_ATOMO);
        }

      
        else if (ehOperador(token))
        {
            if (p.topo < 1) { liberarStr(&p); return NULL; }

            int pb, pa;
            char *sb = popStr(&p, &pb);
            char *sa = popStr(&p, &pa);

            int pAtual = prioOp(token);

          
            int parenEsq = (pa < pAtual);
            int parenDir = (pb < pAtual) ||
                           ((pb == pAtual) &&
                            (strcmp(token,"-") == 0 ||
                             strcmp(token,"/") == 0 ||
                             strcmp(token,"%") == 0 ||
                             strcmp(token,"^") == 0));

            
            int len = (int)(strlen(sa) + strlen(sb) + strlen(token) + 8);
            char *temp = (char *)malloc(len);
            if (!temp) { free(sa); free(sb); liberarStr(&p); return NULL; }

            char *cur = temp;
            if (parenEsq) { *cur++ = '('; strcpy(cur, sa); cur += strlen(sa); *cur++ = ')'; }
            else          {               strcpy(cur, sa); cur += strlen(sa);               }

            strcpy(cur, token); cur += strlen(token);

            if (parenDir) { *cur++ = '('; strcpy(cur, sb); cur += strlen(sb); *cur++ = ')'; }
            else          {               strcpy(cur, sb); cur += strlen(sb);               }

            *cur = '\0';

            free(sa);
            free(sb);

            pushStr(&p, temp, pAtual);
            free(temp);
        }

    
        else if (ehFuncao(token))
        {
            if (vaziaStr(&p)) { liberarStr(&p); return NULL; }

            int pa;
            char *sa = popStr(&p, &pa);

            int len = (int)(strlen(token) + strlen(sa) + 4);
            char *temp = (char *)malloc(len);
            if (!temp) { free(sa); liberarStr(&p); return NULL; }

           
            snprintf(temp, len, "%s(%s)", token, sa);
            free(sa);

            pushStr(&p, temp, PRIO_ATOMO);
            free(temp);
        }

      
        else
        {
            liberarStr(&p);
            return NULL;
        }

        token = strtok(NULL, " ");
    }

    if (p.topo != 0) { liberarStr(&p); return NULL; }

   
    static char resp[MAX];
    int pfinal;
    char *resultado = popStr(&p, &pfinal);
    strncpy(resp, resultado, MAX - 1);
    resp[MAX - 1] = '\0';
    free(resultado);

    liberarStr(&p);
    return resp;
}
