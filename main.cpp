#include <iostream>
#include <chrono>
#include <list>
#include <map>
#include <vector>

enum OrderSide {BUY,SELL};
enum OrderType {LIMIT, MARKET};

struct Order {
    int order_id;
    std::string symbol;
    double price;
    int quantity;
    OrderSide side;
    OrderType type;
    std::chrono::time_point<std::chrono::system_clock> timestamp;
    int client_id;
    Order(int id, const std::string& sym, double p, int q, OrderSide s, OrderType t, int c_id)
        : order_id(id), symbol(sym), price(p), quantity(q), side(s), type(t), client_id(c_id) {
        timestamp = std::chrono::system_clock::now(); // Set timestamp on creation
    }
    Order() : order_id(0), symbol(""), price(0.0), quantity(0), side(BUY), type(LIMIT), client_id(0) {}


};

struct Trade {
    int trade_id;
    std::string symbol;
    double price;
    int quantity;
    int buy_order_id;
    int sell_order_id;
    std::chrono::time_point<std::chrono::system_clock> timestamp;


};

class OrderBook {
public:
    void addOrder(const Order& order) {
        if (order.type == OrderType::LIMIT) {
            if (order.side == OrderSide::BUY) {
                bids_[order.price].push_back(order);
                std::cout << "Added BUY LIMIT order " << order.order_id << " @ " << order.price << " qty " << order.quantity << std::endl;
            }
            else if (order.side == OrderSide::SELL) {
                asks_[order.price].push_back(order);
                std::cout << "Added SELL LIMIT order " << order.order_id << " @ " << order.price << " qty " << order.quantity << std::endl;
            }
        }
        else if (order.type == OrderType::MARKET) {
            std::cout << "Market order " << order.order_id << " received. Will be handled by Matching Engine, not added to OrderBook directly." << std::endl;
        }
    }
    void removeOrder(int order_id, OrderSide side, double price) {
        if (side == OrderSide::BUY) {
            auto price_it = bids_.find(price);
            if (price_it != bids_.end()) { // Check if price level exists
                auto& orders_at_price = price_it->second; // Get the list of orders at this price
                for (auto it = orders_at_price.begin(); it != orders_at_price.end(); ++it) {
                    if (it->order_id == order_id) {
                        orders_at_price.erase(it);
                        if (orders_at_price.empty()) {
                            bids_.erase(price_it); // Erase the price level from the map
                        }
                        std::cout << "Removed BUY order " << order_id << std::endl;
                        return;
                    }
                }
            }
        }
        else if (side == OrderSide::SELL) {
            auto price_it = asks_.find(price);
            if (price_it != asks_.end()) { // Check if price level exists
                auto& orders_at_price = price_it->second; // Get the list of orders at this price
                for (auto it = orders_at_price.begin(); it != orders_at_price.end(); ++it) {
                    if (it->order_id == order_id) {
                        orders_at_price.erase(it);
                        if (orders_at_price.empty()) {
                            asks_.erase(price_it); // Erase the price level from the map
                        }
                        std::cout << "Removed BUY order " << order_id << std::endl;
                        return;
                    }
                }
            }
        }
        std::cout << "Error: Order " << order_id << " not found for removal." << std::endl;
    };
    double getBestBidPrice() {
        if (bids_.empty()) {
            return 0.0;
        }
        return bids_.begin()->first;
    }
    double getBestAskPrice() {
        if (asks_.empty()) {
            return 0.0;
        }
        return asks_.begin()->first;
    }
private:
    std::map<double, std::list<Order>, std::greater<double>> bids_;
    std::map<double, std::list<Order>> asks_;
};
class MatchingEngine {
public:

};
int main() {
    std::cout << "Starting Order Book Simulation.\n";

    OrderBook myOrderBook;

    // Add some limit orders
    Order buyOrder1(1, "AAPL", 150.00, 100, OrderSide::BUY, OrderType::LIMIT, 101);
    Order sellOrder1(2, "AAPL", 151.00, 50, OrderSide::SELL, OrderType::LIMIT, 102);
    Order buyOrder2(3, "AAPL", 149.50, 200, OrderSide::BUY, OrderType::LIMIT, 103);
    Order sellOrder2(4, "AAPL", 150.50, 150, OrderSide::SELL, OrderType::LIMIT, 104);
    Order buyOrder3(5, "AAPL", 150.00, 75, OrderSide::BUY, OrderType::LIMIT, 105);

    myOrderBook.addOrder(buyOrder1);
    myOrderBook.addOrder(sellOrder1);
    myOrderBook.addOrder(buyOrder2);
    myOrderBook.addOrder(sellOrder2);
    myOrderBook.addOrder(buyOrder3);

    std::cout << "\n--- Current Order Book Status ---\n";
    std::cout << "Best Bid Price: " << myOrderBook.getBestBidPrice() << std::endl;
    std::cout << "Best Ask Price: " << myOrderBook.getBestAskPrice() << std::endl;

    // Test removal
    std::cout << "\n--- Testing Order Removal ---\n";
    myOrderBook.removeOrder(1, OrderSide::BUY, 150.00); // Remove buyOrder1
    std::cout << "Best Bid Price after removal: " << myOrderBook.getBestBidPrice() << std::endl; // Should now be 149.50

    myOrderBook.removeOrder(2, OrderSide::SELL, 151.00); // Remove sellOrder1
    std::cout << "Best Ask Price after removal: " << myOrderBook.getBestAskPrice() << std::endl; // Should now be 150.50

    // Test removing a non-existent order
    myOrderBook.removeOrder(99, OrderSide::BUY, 100.00);

    // Test emptying one side
    myOrderBook.removeOrder(3, OrderSide::BUY, 149.50); // Remove buyOrder2
    myOrderBook.removeOrder(5, OrderSide::BUY, 150.00); // Remove buyOrder3 (last one at 150)
    std::cout << "Best Bid Price after emptying bids: " << myOrderBook.getBestBidPrice() << std::endl; // Should be 0.0

    return 0;
}