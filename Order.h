//
// Created by kamakshya on 6/2/25.
//
#include <chrono>
#include<string>

enum class OrderType {BUY, SELL};

#ifndef ORDER_H
#define ORDER_H
struct Order {
    int id;
    OrderType type;
    double price;
    int quantity;
    std::chrono::steady_clock::time_point time;
    /*
     * Order constructor declaration. Lets us use the Order struct
     * as Order order1(1, OrderType::Buy, 100.0, 10);
     * rather than doing this:
     * Order o1;
     * o1.id = 1;
     * o1.type = OrderType::Buy;
     * o1.price = 100.0;
     * o1.quantity = 10;
     */
    Order(int id, orderType type, double price, int quantity);
};
#endif //ORDER_H
