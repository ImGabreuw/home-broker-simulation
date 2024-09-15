#include "system.h"

#include <stdio.h>
#include <string.h>

#include "stock.h"

stock stocks[TOTAL_STOCKS];

void ipo_stocks() {
    create_stock(&stocks[0], "PETR4", "Petrobras", 32.50, 1000);
    create_stock(&stocks[1], "VALE3", "Vale", 67.90, 800);
    create_stock(&stocks[2], "ITUB4", "Itaú Unibanco", 26.30, 1200);
    create_stock(&stocks[3], "BBDC4", "Bradesco", 22.15, 1500);
    create_stock(&stocks[4], "ABEV3", "Ambev", 14.10, 1800);
    create_stock(&stocks[5], "BBAS3", "Banco do Brasil", 46.80, 700);
    create_stock(&stocks[6], "GGBR4", "Gerdau", 28.50, 900);
    create_stock(&stocks[7], "MGLU3", "Magazine Luiza", 4.80, 5000);
    create_stock(&stocks[8], "JBSS3", "JBS", 23.75, 1100);
    create_stock(&stocks[9], "SUZB3", "Suzano", 45.00, 850);
}
