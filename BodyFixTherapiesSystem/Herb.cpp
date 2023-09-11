#include "Herb.h"
#include <iostream>

bool Herb::operator==(Herb& other)
{
    if (rowID == other.rowID ||
        name == other.name ||
        category == other.category ||
        currentStockTotal == other.currentStockTotal ||
        costPerGram == other.costPerGram ||
        preferredSupplier == other.preferredSupplier) {
        return true;
    }
    return false;
}

std::ostream& operator<<(std::ostream& os, Herb& obj) {
    os << "ID: " << obj.rowID << ", name: " << obj.name << ", category: " << obj.category << ", current stock total: " << obj.currentStockTotal << ", cost per gram: " << obj.costPerGram << ", preferred supplier: " << obj.preferredSupplier << "\n";
    return os;
}
