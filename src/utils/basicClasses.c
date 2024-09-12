#include "stdio.h"
#include <pthread.h>
#include <stdbool.h>

#define MAX_STOCKS (10)

// Classe das acoes
typedef struct Stocks {
  char *code;
  char *companyName;
  double currentPrice;
  int availableAmount;
  bool isAvailable;
} stock;

/*TODO: Metodos de acoes*/

//Construtor
void create_stock(stock *s, char *id, char *cName, double value, int avaliableAmount) {
  s->code = id;
  s->companyName = cName;
  s->currentPrice = value;
  s->availableAmount = avaliableAmount;
  s->isAvailable = true;
}

// Classe dos Investidores
typedef struct Investor {
  pthread_t id;
  char* name;
  double currentBalance;
  stock ownedStocks[MAX_STOCKS];

} investor;

/*TODO: Metodos de Investores*/

//Construtor
void create_Investor(investor *inv, char* name, pthread_t id, double currentBalance) {
  inv->id = id;
  inv->name = name;
  inv->currentBalance = currentBalance;
}

int main(void) {
  printf("Hello World\n");
  return 0;
}
