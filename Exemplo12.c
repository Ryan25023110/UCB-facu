#include <stdio.h>
#include <stdio.h>

typedef struct{
int numerador;
int denominador;

} fracao;


fracao criarfracao(int n, int d);
    void exibirfracao(fracao f);
    fracao inverefracaofracao(fscanf);

// Definição das operações (funções) do TAO fração

int main() {
    
    fracao F1= criarfracao = (3,7);
     fracao F2= criarfracao = (12,20);
      fracao F3= criarfracao = (6,0);

      exibirfracao(F1);
      exibirfracao(F2);
      exibirfracao(F3);

}
fracao criarfracao(int n, int d) {

    if(d == 0) {
        printf("ERRO: não é possivel criar função com denominador 0!\n");
        exit(1);

    }

fracao f;
  f.numerador = n;
  f.denominador = d;
  return f;
}

void exibirfracao(fracao f) {

    printf("%d / %d \n", f.numerador,f.denominador);
}
