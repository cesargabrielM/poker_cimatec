#include <stdio.h>

#define NUMERO_DE_CARTAS 13
#define NUMERO_DE_NAIPES 4
#define TOTAL_CARTAS 52

// Estrutura para representar uma carta
typedef struct {
    char *valor;
    char *naipe;
} Carta;

int main() {
    // Valores das cartas
    char *valores[NUMERO_DE_CARTAS] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    
    // Naipes
    char *naipes[NUMERO_DE_NAIPES] = {"Copas", "Ouros", "Espadas", "Paus"};
    
    // Criando o baralho
    Carta baralho[TOTAL_CARTAS];
    int index = 0;
    
    for (int i = 0; i < NUMERO_DE_NAIPES; i++) {
        for (int j = 0; j < NUMERO_DE_CARTAS; j++) {
            baralho[index].valor = valores[j];
            baralho[index].naipe = naipes[i];
            index++;
        }
    }
    
    // Imprimindo o baralho
    for (int i = 0; i < TOTAL_CARTAS; i++) {
        printf("%s de %s\n", baralho[i].valor, baralho[i].naipe);
    }
    
    return 0;
}


