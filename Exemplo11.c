#include <stdio.h>
#include <stdlib.h>

int ehPrimo(int N); // Prototipo de função ehPrimo

int main() {

system("cls");

int Numero;

INICIO:

printf("Digite um Numero: ");
scanf("%d", &Numero);

if (ehPrimo(Numero)== 1) printf("%d eh PRIMO.\n", Numero);
else printf("%d NAO e PRIMO.\n");

goto INICIO;

return 0;

} 

// Implementação da função ehPrimo 

int ehPrimo(int N){

 if (N <= 1) return 0;

 for (int i = 2; i < N; i++) {
    if(N % i == 0){
        return 0;


    }


 }
 return 1; // Se todos testes if;

}