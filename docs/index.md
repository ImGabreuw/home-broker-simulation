# Documentação

```mermaid
classDiagram
    class Investor {
        - id: pthread_t
        - name: char[]
        - balance: double
        + create_investor() int
        + check_order_status()
        + viewPortfolio()
    }

    class OrderType {
        <<Enumeration>>
        BUY
        SELL
    }

    class Order {
        - id: int
        - type: OrderType
        - stock: Ticker
        - quantity: int
        - status: char[]
        + execute()
        + cancel()
    }

    class Ticket {
        - code: char[]
        - company_name: char[]
        - current_price: double
        + create_ticket() int
    }

    class System {
	    - tickers: Ticket[]
        - order_queue: Queue<Order>
        + process_orders()
        + emit_order(order: Order)
        + ticket_ipo(ticket: Ticker) "register a ticket in home broker"
    }

    class Transaction {
        - id: int
        - order: Order
        - value: double
        - date: struct tm
        - ticket_code: char[]
        + record()
    }

    class Portfolio {
        - id: int
        - investor: Investor
        - stocks: Map~char[], int~
        + buy_stock(stock: char[], quantity: int) int
        + sell_stock(stock: char[], quantity: int) int
        + check_positions() void "view amount per ticket"
        + refresh_value() void
    }

    Investor "1" -- "0..*" Order : creates >
    Investor "1" -- "1" Portfolio : owns >
    System "1" -- "0..*" Order : processes >
    System "1" -- "0..*" Ticket : register >
    Order "1" -- "0..*" Transaction : results in >
    Order "1" -- "1" OrderType : has
    Transaction "1" -- "1" Ticket : refers to >
    Portfolio "1" -- "0..*" Order : includes >
```