#include "stdio.h"
#include <pthread.h>
#include <stdbool.h>

#define MAX_STOCKS (10)

// Classe das acoes
typedef struct Stocks {
  int id;
  float value;
  int availableAmount;
  bool isAvailable;
} stock;

/*TODO: Metodos de acoes*/

//Construtor
void create_stock(stock *s, int id, float value, int avaliableAmount) {
  s->id = id;
  s->value = value;
  s->availableAmount = avaliableAmount;
  s->isAvailable = true;
}

// Classe dos investidores
typedef struct Investidor {
  pthread_t id;
  int currentBalance;
  stock ownedStocks[MAX_STOCKS];

} investidor;

/*TODO: Metodos de investidores*/

//Construtor
void create_investidor(investidor *inv, pthread_t id, int currentBalance) {
  inv->id = id;
  inv->currentBalance = currentBalance;
}

int main(void) {
  printf("Hello World\n");
  return 0;
}
