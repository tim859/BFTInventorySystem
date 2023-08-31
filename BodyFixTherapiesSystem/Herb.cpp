#include "Herb.h"
#include <iostream>

std::ostream& operator<<(std::ostream& os, Herb& obj) {
    os << "ID: " << obj.id << ", name: " << obj.name << ", category: " << obj.category << ", current stock total: " << obj.currentStockTotal << ", cost per gram: " << obj.costPerGram << ", preferred supplier: " << obj.preferredSupplier << "\n";
    return os;
}
