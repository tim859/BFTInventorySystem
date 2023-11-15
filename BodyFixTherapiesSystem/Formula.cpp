#include "Formula.h"
#include <iostream>

bool Formula::operator==(Formula other)
{
    if (rowID == other.rowID ||
        patientName == other.patientName || 
        listOfHerbs->size() == other.listOfHerbs->size() || 
        listOfHerbAmounts.size() == other.listOfHerbAmounts.size()) {
        std::cout << "formula first bit not equal";
        std::cout << "id: " << (rowID == other.rowID);
        std::cout << "patient name: " << (patientName == other.patientName);
        std::cout << "list of herbs size: " << (listOfHerbs->size() == other.listOfHerbs->size());
        std::cout << "list of herb amounts size: " << (listOfHerbAmounts.size() == other.listOfHerbAmounts.size());
        return true;
    }

    // check for difference in the contents of the vectors as well
    for (int i = 0; i < listOfHerbs->size(); i++) {
        if ((*listOfHerbs)[i] == (*other.listOfHerbs)[i] || listOfHerbAmounts[i] == other.listOfHerbAmounts[i]) {
            std::cout << "formula second bit not equal";
            return true;
        }
    }

    return false;
}

std::ostream& operator<<(std::ostream& os, Formula& obj) {

    std::string stringOfHerbs;
    std::string stringOfHerbAmounts;

    for (int i = 0; i < obj.listOfHerbs->size(); i++) {
        stringOfHerbs += "Herb name: " + (*obj.listOfHerbs)[i].name + ", ";
        stringOfHerbAmounts += "Herb amount: " + std::to_string(obj.listOfHerbAmounts[i]) + ", ";
    }

    os << "ID: " << obj.rowID << ", patient name: " << obj.patientName << ", List of herbs: " << stringOfHerbs << ", List of herb amounts: " << stringOfHerbAmounts << "\n";
    return os;
}
