#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <string.h>

#include "listade.h" // Inclui a estrutura de lista para poder manipular a lista de cartas.

// --- Códigos de Cor ANSI para o Terminal ---
// Estas constantes permitem colorir o texto no terminal. Nem todos os terminais suportam.
#define RESET           "\033[0m"       // Reseta a cor para o padrão
#define BOLD            "\033[1m"       // Texto em negrito
#define RED             "\033[31m"       // Texto vermelho
#define GREEN           "\033[32m"       // Texto verde
#define YELLOW          "\033[33m"       // Texto amarelo
#define BLUE            "\033[34m"       // Texto azul
#define MAGENTA         "\033[35m"       // Texto magenta
#define CYAN            "\033[36m"       // Texto ciano
#define WHITE           "\033[37m"       // Texto branco
#define BLACK           "\033[30m"       // Texto preto

// Converte o nome de um naipe (ex: "Copas") para seu símbolo gráfico (ex: ♥).
// Usa códigos UTF-8, que funcionam na maioria dos terminais modernos.
static const char* obter_simbolo_naipe(const char* naipe) {
    if (strcmp(naipe, "Copas") == 0) return "\xe2\x99\xa5";   // ♥
    if (strcmp(naipe, "Ouros") == 0) return "\xe2\x99\xa6";   // ♦
    if (strcmp(naipe, "Espadas") == 0) return "\xe2\x99\xa0"; // ♠
    if (strcmp(naipe, "Paus") == 0) return "\xe2\x99\xa3";    // ♣
    return "?"; // Retorna "?" se o naipe for desconhecido.
}

// Desenha uma representação visual (arte ASCII) de uma lista de cartas.
static void desenhar_cartas(tp_listad *lista, const char* titulo) {
    // Se a lista não existe ou está vazia, imprime o título e uma mensagem de aviso.
    if (!lista || listad_vazia(lista)) {
        if (titulo) printf("\n--- %s ---\n(Vazio)\n", titulo);
        return;
    }

    // Imprime o título acima das cartas.
    if (titulo) printf("\n--- %s ---\n", titulo);

    // Armazena os nós da lista de cartas num array para acesso rápido.
    int num_cartas = 0;
    tp_no_lista* nos_cartas[10]; // Suporta desenhar um máximo de 10 cartas.
    tp_no_lista* atu = lista->ini;
    while(atu != NULL && num_cartas < 10) {
        nos_cartas[num_cartas++] = atu;
        atu = atu->prox;
    }

    // O desenho é feito linha por linha, para que as cartas apareçam lado a lado.
    // Linha 1: Desenha o topo de todas as cartas.
    for (int i = 0; i < num_cartas; i++) printf(" .-------. ");
    printf("\n");

    // Linha 2: Desenha o valor da carta no canto superior esquerdo.
    for (int i = 0; i < num_cartas; i++) {
        const char* valor = nos_cartas[i]->info.valor;
        // %-2s alinha o texto à esquerda com 2 caracteres de espaço.
        if (strcmp(valor, "10") == 0) {
            printf(" |%s     | ", valor);
        } else {
            printf(" |%-2s     | ", valor);
        }
    }
    printf("\n");

    // Linha 3: Espaço em branco dentro da carta.
    for (int i = 0; i < num_cartas; i++) printf(" |       | ");
    printf("\n");

    // Linha 4: Desenha o símbolo do naipe no centro da carta.
    for (int i = 0; i < num_cartas; i++) {
        const char* simbolo = obter_simbolo_naipe(nos_cartas[i]->info.naipe);
        printf(" |   %s   | ", simbolo);
    }
    printf("\n");

    // Linha 5: Espaço em branco dentro da carta.
    for (int i = 0; i < num_cartas; i++) printf(" |       | ");
    printf("\n");
    
    // Linha 6: Desenha o valor da carta no canto inferior direito.
    for (int i = 0; i < num_cartas; i++) {
        const char* valor = nos_cartas[i]->info.valor;
        if (strcmp(valor, "10") == 0) {
            printf(" |     %s| ", valor);
        } else {
            printf(" |     %-2s| ", valor);
        }
    }
    printf("\n");

    // Linha 7: Desenha a base de todas as cartas.
    for (int i = 0; i < num_cartas; i++) printf(" '-------' ");
    printf("\n");
}

#endif // INTERFACE_H