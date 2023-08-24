#pragma once
#include <string>
#include "Money.h"

struct Herb {
    Herb() : id(0), name(""), category(""), currentStockTotal(0), costPerGram(Money(0.0)), preferredSupplier("") {}

    Herb(int initID, std::string initName, std::string initCategory, int initCurrentStockTotal, Money initCostPerGram, std::string initPreferredSupplier)
        : id(initID),
        name(initName),
        category(initCategory),
        currentStockTotal(initCurrentStockTotal),
        costPerGram(initCostPerGram),
        preferredSupplier(initPreferredSupplier) {}

    int id;
    std::string name;
    std::string category;
    int currentStockTotal;
    Money costPerGram;
    std::string preferredSupplier;
};
