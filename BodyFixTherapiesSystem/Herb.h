#pragma once
#include <string>
#include "Money.h"

struct Herb {
    // TODO: change id to rowID
    Herb() : rowID(0), name(""), category(""), currentStockTotal(0), costPerGram(Money(0.0)), preferredSupplier("") {}

    Herb(int initRowID, std::string initName, std::string initCategory, int initCurrentStockTotal, Money initCostPerGram, std::string initPreferredSupplier)
        : rowID(initRowID),
        name(initName),
        category(initCategory),
        currentStockTotal(initCurrentStockTotal),
        costPerGram(initCostPerGram),
        preferredSupplier(initPreferredSupplier) {}

    int rowID;
    std::string name;
    std::string category;
    int currentStockTotal;
    Money costPerGram;
    std::string preferredSupplier;

    bool operator== (Herb&);
    friend std::ostream& operator<< (std::ostream& os, Herb& obj);
};