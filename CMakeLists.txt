cmake_minimum_required(VERSION 3.31)
project(stock_exchange)

set(CMAKE_CXX_STANDARD 20)

add_executable(stock_exchange main.cpp
        Order.h
        OrderBook.cpp
        OrderBook.h
        TradeLog.cpp
        TradeLog.h)
