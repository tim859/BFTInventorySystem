#pragma once
#include <string>
#include "Money.h"

struct Herb {
    Herb(const std::string& initName, const std::string& initCategory, int initCurrentStockTotal, Money initCostPerGram, bool initShowSupplierOrders, int initStartingBalance, int initTotalGramsPurchased, int initTotalGramsSold, const std::string& initPreferredSupplier, Money initStartingCost)
        : name(initName),
        category(initCategory),
        currentStockTotal(initCurrentStockTotal),
        costPerGram(initCostPerGram),
        showSupplierOrders(initShowSupplierOrders),
        startingBalance(initStartingBalance),
        totalGramsPurchased(initTotalGramsPurchased),
        totalGramsSold(initTotalGramsSold),
        preferredSupplier(initPreferredSupplier),
        startingCost(initStartingCost) {
    }

    // Overloaded constructor for only first three values
    Herb(const std::string& initName, const std::string& initCategory, int initCurrentStockTotal)
        : name(initName),
        category(initCategory),
        currentStockTotal(initCurrentStockTotal),
        costPerGram(Money(0.0f)),
        showSupplierOrders(false),
        startingBalance(0),
        totalGramsPurchased(0),
        totalGramsSold(0),
        preferredSupplier("none"),
        startingCost(Money(0.0f)) {
    }

    std::string name;
    std::string category;
    int currentStockTotal;
    Money costPerGram;
    bool showSupplierOrders;
    int startingBalance;
    int totalGramsPurchased;
    int totalGramsSold;
    std::string preferredSupplier;
    Money startingCost;
};
