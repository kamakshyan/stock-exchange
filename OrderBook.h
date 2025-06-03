//
// Created by kamakshya on 6/2/25.
//

#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <map>
#include <deque>
#include "Order.h";


class OrderBook {
public:
    void addLimitOrder(const Order& order);
    void printOrderBook() const;

private:
    std::map<double, std::deque<Order>, std::greater<double>> bids;
    std::map<double, std::deque<Order>> asks;

    void match(Order order);

    void printSide(const std::map<double, std::deque<Order>>& side, const std::string& label) const;
};

#endif //ORDERBOOK_H
