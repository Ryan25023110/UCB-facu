#include <stdio.h>
#include <stdio.h>

#include "equipe.h"

int main() {

    System ("cls")

Equipe * Tabela = lerDados("tabela2024.csv");

for(int i = 0; i < 20; i++) {
 printf("%d\t %s \t %.2f \n", 
 Tabela[i].Posicao,
 Tabela[i].Time,
 Tabela[i].Aproveitamento);

}

return 0;


}