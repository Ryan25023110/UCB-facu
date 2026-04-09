#include "justica.h"

void menu() {
    printf("\n--- SISTEMA DE GESTAO JUSTICA ELEITORAL 2026 ---\n");
    printf("1. Carregar Arquivos CSV e Concatenar\n");
    printf("2. Gerar Resumo de Metas por Tribunal\n");
    printf("3. Buscar Dados por Municipio (Gerar TXT)\n");
    printf("4. Sair\n");
    printf("Escolha uma opcao: ");
}

int main() {
    Node *lista = criar_lista();
    int opcao;
    char busca[100];

    do {
        menu();
        scanf("%d", &opcao);
        fflush(stdin);

        switch(opcao) {
            case 1:
                // Exemplo: carregar arquivos. Em um cenario real, voce pode usar um loop.
                printf("Carregando dados...\n");
                // Substitua pelos nomes reais dos seus arquivos
                if(carregar_arquivo(&lista, "dados1.csv")) {
                    concatenar_arquivos(lista, "todos_arquivos.csv");
                    printf("Arquivos concatenados em 'todos_arquivos.csv'\n");
                } else {
                    printf("Erro ao abrir arquivos. Verifique se 'dados1.csv' existe.\n");
                }
                break;

            case 2:
                if (lista == NULL) printf("Carregue os dados primeiro (Opcao 1).\n");
                else {
                    gerar_resumo_metas(lista, "resumo_metas.csv");
                    printf("Resumo gerado em 'resumo_metas.csv'\n");
                }
                break;

            case 3:
                if (lista == NULL) printf("Carregue os dados primeiro.\n");
                else {
                    printf("Informe o nome do municipio (ex: MACAPA): ");
                    gets(busca);
                    buscar_por_municipio(lista, busca);
                }
                break;

            case 4:
                liberar_lista(&lista);
                printf("Saindo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    } while(opcao != 4);

    return 0;
}