# Documentação

```mermaid
classDiagram
    class Investor {
        - id: pthread_t
        - name: char[120]
        - balance: double
        - portfolio: Portfolio
        + create_investor() int
        + view_balance() void
        + view_portfolio() void
    }

    class Portfolio {
        - owner: Investor
        - positions: Position[]
        + buy_asset(code: char[], quantity: int, price: double) int
        + sell_asset(code: char[], quantity: int) int
        + reconcile_positions() void "updates positions with average price"
    }

    class Position {
        - asset_code: char[6]
        - quantity: int
        - average_price: double
        + update_position(quantity: int, price: double) void
        + calculate_average_price(new_quantity: int, new_price: double) double
    }

    class Order {
        - id: int
        - type: OrderType
        - asset_code: char[6]
        - quantity: int
        - price: double
        - status: OrderStatus
        + process() int
        + cancel() int
    }

    class OrderType {
        <<Enumeration>>
        BUY
        SELL
    }

    class OrderStatus {
        <<Enumeration>>
        PENDING
        CANCELED
        DONE
    }

    class Asset {
        - code: char[6]
        - company_name: char[100]
        - quotation: double
        - ipo_quantity: int "initial number of shares"
        + create_asset() int
    }

    class System {
	    - inventory: Asset[]
        - order_queue: Queue<Order>
        + process_orders() void
        + register_order(order: Order) void
        + asset_ipo(asset: Asset, initialQuantity: int) void
    }

    class Transaction {
        - id: int
        - order: Order
        - value: double
        - createdAt: struct tm
        + create_transaction() int
    }

    Investor "1" -- "0..*" Order : emits >
    Investor "1" -- "1" Portfolio : owns >
    System "1" -- "0..*" Order : processes >
    System "1" -- "0..*" Asset : registers >
    Order "1" -- "0..*" Transaction : results in >
    Order "1" -- "1" OrderType : has
    Order "1" -- "1" OrderStatus : has
    Transaction "1" -- "1" Asset : refers to >
    Portfolio "1" -- "0..*" Position : contains >
```

## Investidores

Os investidores realizam ações de compra e venda de ações de forma aleatória, com intervalos variáveis e revezamento em grupos, a aplicação pode seguir o conceito de escalonamento circular por quantum, semelhante ao Round Robin. Além disso, a lógica de multithreading será usada para simular a execução de cada investidor, e uma fila de execução será usada para gerenciar os grupos de investidores ativos.

Estrutura:

- Cada investidor será uma thread, que executa operações de compra e venda em intervalos aleatórios entre 2 a 5 segundos.
- O tempo de vida de cada investidor será aleatório, por exemplo, entre 20 e 30 segundos.
- Apenas grupos de 5 investidores estarão ativos simultaneamente.
- Um pool de 20 investidores será gerenciado para revezar grupos de threads após o tempo de vida de cada investidor expirar.

```

```
