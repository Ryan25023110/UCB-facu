#include <stdio.h>
#include <stdlib.h>

int main () {

    FILE * in =fopen("Carrinho.csv", "r");

    if (in == NULL) {
        printf ("ERRO: não foi possivel abrir o arquivo!\n");
        exit(1);

    }
  

  char Buffer[10000];
  fscanf(in, "%[^\n]\n", Buffer);

  int Contador; 

  char Produto[100];
  int Quantidade;
  float Preco;
  float Total;

    while(fscanf(in,"%[^,],%d,%f\n", Produto, &Quantidade, &Preco) == 3){
    
    Contador++;
    float Subtotal = Quantidade * Preco;
    Total = Total + Subtotal;
    printf("%s: %d * %.2f\n", Produto, Quantidade, Preco);

    }

    printf("O valor total da compra é R$ %.2f\n", Total);

    fclose(in);

return 0;

}