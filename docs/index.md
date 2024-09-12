# Documentação

```mermaid
classDiagram
    %% Define the Investor class
    class Investor {
        - id: int
        - name: char[]
        - balance: double
        + createOrder()
        + checkOrderStatus()
        + viewPortfolio()
    }

    %% Define the Order class
    class Order {
        - id: int
        - type: char[]     % buy or sell
        - stock: Ticker
        - quantity: int
        - status: char[]
        + execute()
        + cancel()
    }

    %% Define the Ticket class
    class Ticket {
        - code: char[]      % stock code
        - companyName: char[]
        - currentPrice: double
        + generateTicket()
    }

    %% Define the System class
    class System {
	    - tickers: Ticket[]
        - orderQueue: Queue<Order>
        + processOrders()
        + addOrder(order: Order)
        + addTicket(order: Order)
    }

    %% Define the Transaction class
    class Transaction {
        - id: int
        - order: Order
        - value: double
        - date: struct tm
        - ticketCode: char[]
        + record()
    }

    %% Define the Portfolio class
    class Portfolio {
        - id: int
        - investor: Investor
        - stocks: Map<char[], int>
        + addStock(stock: char[], quantity: int)
        + removeStock(stock: char[], quantity: int)
        + checkQuantities()
        + updateCurrentValue()
    }

    %% Define relationships between classes
    Investor "1" -- "0..*" Order : creates >
    Investor "1" -- "1" Portfolio : owns >
    System "1" -- "0..*" Order : processes >
    System "1" -- "0..*" Ticket : register >
    Order "1" -- "0..*" Transaction : results in >
    Transaction "1" -- "1" Ticket : refers to >
    Portfolio "1" -- "0..*" Order : includes >
```