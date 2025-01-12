#include "./Creature.h"

Creature::Creature(std::string name, int3 position, int quantity, std::string disposition, bool neverFlees, bool doesNotGrow)
    : name(name), position(position), quantity(quantity), disposition(disposition), neverFlees(neverFlees), doesNotGrow(doesNotGrow) {}

std::string Creature::getName() const {
    return name;
}

int3 Creature::getPosition() const {
    return position;
}

int Creature::getQuantity() const {
    return quantity;
}

std::string Creature::getDisposition() const {
    return disposition;
}

bool Creature::getNeverFlees() const {
    return neverFlees;
}

bool Creature::getDoesNotGrow() const {
    return doesNotGrow;
}

