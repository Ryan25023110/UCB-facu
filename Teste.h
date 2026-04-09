#include "justica.h"

Node* criar_lista() {
    return NULL;
}

void inserir_final(Node **lista, Registro reg) {
    Node *novo = (Node*)malloc(sizeof(Node));
    novo->dado = reg;
    novo->proximo = NULL;

    if (*lista == NULL) {
        *lista = novo;
    } else {
        Node *temp = *lista;
        while (temp->proximo != NULL) temp = temp->proximo;
        temp->proximo = novo;
    }
}

void liberar_lista(Node **lista) {
    Node *atual = *lista;
    while (atual != NULL) {
        Node *prox = atual->proximo;
        free(atual);
        atual = prox;
    }
    *lista = NULL;
}

int carregar_arquivo(Node **lista, char *nome_arquivo) {
    FILE *f = fopen(nome_arquivo, "r");
    if (!f) return 0;

    char linha[2048];
    // Pular cabeçalho
    fgets(linha, sizeof(linha), f);

    while (fgets(linha, sizeof(linha), f)) {
        Registro r;
        char *token = strtok(linha, ";");
        if (!token) continue;

        strcpy(r.sigla_tribunal, token);
        strcpy(r.procedimento, strtok(NULL, ";"));
        strcpy(r.ramo_justica, strtok(NULL, ";"));
        strcpy(r.sigla_grau, strtok(NULL, ";"));
        strcpy(r.uf_oj, strtok(NULL, ";"));
        strcpy(r.municipio_oj, strtok(NULL, ";"));
        strcpy(r.id_ultimo_oj, strtok(NULL, ";"));
        strcpy(r.nome, strtok(NULL, ";"));
        strcpy(r.mesano_cnm1, strtok(NULL, ";"));
        strcpy(r.mesano_sent, strtok(NULL, ";"));
        
        r.casos_novos_2026 = atol(strtok(NULL, ";"));
        r.julgados_2026 = atol(strtok(NULL, ";"));
        r.prim_sent2026 = atol(strtok(NULL, ";"));
        r.suspensos_2026 = atol(strtok(NULL, ";"));
        r.dessobrestados_2026 = atol(strtok(NULL, ";"));
        r.cumprimento_meta1 = atof(strtok(NULL, ";"));
        r.distm2_a = atol(strtok(NULL, ";"));
        r.julgm2_a = atol(strtok(NULL, ";"));
        r.suspm2_a = atol(strtok(NULL, ";"));
        r.cumprimento_meta2a = atof(strtok(NULL, ";"));
        r.distm2_ant = atol(strtok(NULL, ";"));
        r.julgm2_ant = atol(strtok(NULL, ";"));
        r.suspm2_ant = atol(strtok(NULL, ";"));
        r.desom2_ant = atol(strtok(NULL, ";"));
        r.cumprimento_meta2ant = atof(strtok(NULL, ";"));
        r.distm4_a = atol(strtok(NULL, ";"));
        r.julgm4_a = atol(strtok(NULL, ";"));
        r.suspm4_a = atol(strtok(NULL, ";"));
        r.cumprimento_meta4a = atof(strtok(NULL, ";"));
        r.distm4_b = atol(strtok(NULL, ";"));
        r.julgm4_b = atol(strtok(NULL, ";"));
        r.suspm4_b = atol(strtok(NULL, ";"));
        r.cumprimento_meta4b = atof(strtok(NULL, ";"));

        inserir_final(lista, r);
    }
    fclose(f);
    return 1;
}

// 1) Concatenar arquivos
void concatenar_arquivos(Node *lista, char *nome_saida) {
    FILE *f = fopen(nome_saida, "w");
    if (!f) return;

    // Escreve cabeçalho
    fprintf(f, "sigla_tribunal;procedimento;ramo_justica;sigla_grau;uf_oj;municipio_oj;id_ultimo_oj;nome;mesano_cnm1;mesano_sent;casos_novos_2026;julgados_2026;prim_sent2026;suspensos_2026;dessobrestados_2026;cumprimento_meta1;distm2_a;julgm2_a;suspm2_a;cumprimento_meta2a;distm2_ant;julgm2_ant;suspm2_ant;desom2_ant;cumprimento_meta2ant;distm4_a;julgm4_a;suspm4_a;cumprimento_meta4a;distm4_b;julgm4_b;suspm4_b;cumprimento_meta4b\n");

    Node *atual = lista;
    while (atual != NULL) {
        Registro r = atual->dado;
        fprintf(f, "%s;%s;%s;%s;%s;%s;%s;%s;%s;%s;%ld;%ld;%ld;%ld;%ld;%.2f;%ld;%ld;%ld;%.2f;%ld;%ld;%ld;%ld;%.2f;%ld;%ld;%ld;%.2f;%ld;%ld;%ld;%.2f\n",
                r.sigla_tribunal, r.procedimento, r.ramo_justica, r.sigla_grau, r.uf_oj, r.municipio_oj, r.id_ultimo_oj, r.nome, r.mesano_cnm1, r.mesano_sent,
                r.casos_novos_2026, r.julgados_2026, r.prim_sent2026, r.suspensos_2026, r.dessobrestados_2026, r.cumprimento_meta1,
                r.distm2_a, r.julgm2_a, r.suspm2_a, r.cumprimento_meta2a, r.distm2_ant, r.julgm2_ant, r.suspm2_ant, r.desom2_ant, r.cumprimento_meta2ant,
                r.distm4_a, r.julgm4_a, r.suspm4_a, r.cumprimento_meta4a, r.distm4_b, r.julgm4_b, r.suspm4_b, r.cumprimento_meta4b);
        atual = atual->proximo;
    }
    fclose(f);
}

// 2) Gerar Resumo (Agrupado por Tribunal)
void gerar_resumo_metas(Node *lista, char *nome_saida) {
    FILE *f = fopen(nome_saida, "w");
    if (!f) return;

    fprintf(f, "sigla_tribunal;total_julgados_2026;Meta1;Meta2A;Meta2Ant;Meta4A;Meta4B\n");

    // Para simplificar no TAD Lista sem criar outra estrutura complexa:
    // Vamos identificar tribunais únicos e calcular
    char tribunais[500][20];
    int qtd_tribunais = 0;

    Node *aux = lista;
    while (aux != NULL) {
        int existe = 0;
        for (int i = 0; i < qtd_tribunais; i++) {
            if (strcmp(tribunais[i], aux->dado.sigla_tribunal) == 0) {
                existe = 1; break;
            }
        }
        if (!existe && qtd_tribunais < 500) {
            strcpy(tribunais[qtd_tribunais++], aux->dado.sigla_tribunal);
        }
        aux = aux->proximo;
    }

    for (int i = 0; i < qtd_tribunais; i++) {
        long sum_julg2026 = 0, sum_novos = 0, sum_dessob = 0, sum_susp = 0;
        long sum_j2a = 0, sum_d2a = 0, sum_s2a = 0;
        long sum_j2ant = 0, sum_d2ant = 0, sum_s2ant = 0, sum_deso2ant = 0;
        long sum_j4a = 0, sum_d4a = 0, sum_s4a = 0;
        long sum_j4b = 0, sum_d4b = 0, sum_s4b = 0;

        aux = lista;
        while (aux != NULL) {
            if (strcmp(tribunais[i], aux->dado.sigla_tribunal) == 0) {
                Registro r = aux->dado;
                sum_julg2026 += r.julgados_2026;
                sum_novos += r.casos_novos_2026;
                sum_dessob += r.dessobrestados_2026;
                sum_susp += r.suspensos_2026;
                sum_j2a += r.julgm2_a; sum_d2a += r.distm2_a; sum_s2a += r.suspm2_a;
                sum_j2ant += r.julgm2_ant; sum_d2ant += r.distm2_ant; sum_s2ant += r.suspm2_ant; sum_deso2ant += r.desom2_ant;
                sum_j4a += r.julgm4_a; sum_d4a += r.distm4_a; sum_s4a += r.suspm4_a;
                sum_j4b += r.julgm4_b; sum_d4b += r.distm4_b; sum_s4b += r.suspm4_b;
            }
            aux = aux->proximo;
        }

        // Fórmulas trocando subtração por ADIÇÃO conforme instrução
        double m1 = (sum_novos + sum_dessob + sum_susp) == 0 ? 0 : ((double)sum_julg2026 / (sum_novos + sum_dessob + sum_susp)) * 100;
        double m2a = (sum_d2a + sum_s2a) == 0 ? 0 : ((double)sum_j2a / (sum_d2a + sum_s2a)) * (1000.0/7.0);
        double m2ant = (sum_d2ant + sum_s2ant + sum_deso2ant) == 0 ? 0 : ((double)sum_j2ant / (sum_d2ant + sum_s2ant + sum_deso2ant)) * 100;
        double m4a = (sum_d4a + sum_s4a) == 0 ? 0 : ((double)sum_j4a / (sum_d4a + sum_s4a)) * 100;
        double m4b = (sum_d4b + sum_s4b) == 0 ? 0 : ((double)sum_j4b / (sum_d4b + sum_s4b)) * 100;

        fprintf(f, "%s;%ld;%.2f;%.2f;%.2f;%.2f;%.2f\n", tribunais[i], sum_julg2026, m1, m2a, m2ant, m4a, m4b);
    }
    fclose(f);
}

// 3) Buscar por município e gerar TXT
void buscar_por_municipio(Node *lista, char *municipio) {
    char nome_arq[150];
    sprintf(nome_arq, "%s.txt", municipio);
    FILE *f = fopen(nome_arq, "w");
    if (!f) return;

    Node *aux = lista;
    int encontrou = 0;
    while (aux != NULL) {
        if (stricmp(aux->dado.municipio_oj, municipio) == 0) {
            Registro r = aux->dado;
            fprintf(f, "Tribunal: %s | Orgao: %s | Municipio: %s | Julgados: %ld\n", 
                    r.sigla_tribunal, r.nome, r.municipio_oj, r.julgados_2026);
            encontrou = 1;
        }
        aux = aux->proximo;
    }
    fclose(f);
    if (encontrou) printf("Arquivo %s gerado com sucesso!\n", nome_arq);
    else printf("Municipio nao encontrado.\n");
}