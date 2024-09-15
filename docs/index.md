# Documentação

```mermaid
classDiagram
    class Book {
        - orders: Order[200]
        - transactions: Transaction[100]
        - input_queue: OrderQueue*
        - output_queue: OrderQueue*
        - mutex: pthread_mutex_t*
        - cond: pthread_cond_t*

        + init_book(input_queue: Queue*, output_queue: Queue*, mutex: pthread_mutex_t*, cond: pthread_cond_t*) void
        + trade() void
        + add_transaction(transaction: Transaction*, mutex: pthread_mutex_t*, cond: pthread_cond_t*) void
    }

    class Order {
        - id: int
        - investor: Investor*
        - asset: Asset*
        - shares: int
        - pending_shares: int
        - price: double
        - order_type: OrderType
        - status: OrderStatus
        - transactions: Transaction*

        + emit_order(id: int, investor: Investor*, asset: Asset*, shares: int, price: double, order_type: char*) void
    }

    class OrderAction {
        <<Enumeration>>
        BUY
        SELL
    }

    class OrderStatus {
        <<Enumeration>>
        OPEN
        CLOSE
    }

    class Investor {
        - id: pthread_t
        - name: char[120]
        - positions: Position[10]

        + create_investor(id: pthread_t) void
        + add_asset_position(asset_position: Position*) void
        + update_asset_position(asset_code: char[6], shares: int) void
        + get_asset_position(asset_code: char[6]) Position*
    }

    class Position {
        - asset_code: char[6]
        - shares: int

        + create_position(asset_code: char[6], shares: int) void
    }

    class Asset {
        - code: char[6]
        - company_name: char[100]
        - market_volume: int

        + create_asset() int
    }

    class Transaction {
        - id: int
        - selling_order: Order*
        - buying_order: Order*
        - shares: int
        - price: double
        - total: double
        - created_at: time_t

        + create_transaction(selling_order: Order*, buying_order: Order*, shares: int, price: double) void
        + calculate_total(shares: int, price: double) void
        + close_buy_order() void
        + close_sell_order() void
        + add_buy_order_pending_shares(shares: int) void
        + add_sell_order_pending_shares(shares: int) void
    }

    class OrderQueue {
        - orders: Order[100]

        + create_order_queue(order_queue: OrderQueue*) int
        + push(order: Order*) void
        + pop() Order*
        + len() int
    }

    Book --> Order
    Book --> Transaction
    Book --> OrderQueue
    Book --> pthread_mutex_t
    Book --> pthread_cond_t

    Order --> Investor
    Order --> Asset
    Order --> Transaction
    Order --> OrderStatus
    Order --> OrderAction
    Investor --> Position
    Transaction --> Order
```


## Investidores

Os investidores realizam ações de compra e venda de ações de forma aleatória, com intervalos variáveis e revezamento em grupos, a aplicação pode seguir o conceito de escalonamento circular por quantum, semelhante ao Round Robin. Além disso, a lógica de multithreading será usada para simular a execução de cada investidor, e uma fila de execução será usada para gerenciar os grupos de investidores ativos.

Estrutura:

- Cada investidor será uma thread, que executa operações de compra e venda em intervalos aleatórios entre 2 a 5 segundos.
- O tempo de vida de cada investidor será aleatório, por exemplo, entre 20 e 30 segundos.
- Apenas grupos de 5 investidores estarão ativos simultaneamente.
- Um pool de 20 investidores será gerenciado para revezar grupos de threads após o tempo de vida de cada investidor expirar.
