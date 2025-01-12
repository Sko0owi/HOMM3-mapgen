#pragma once


#include <string>
#include "../types/int3.h"


class Creature {
public:

    Creature(std::string name, int3 position, int quantity, std::string disposition, bool neverFlees, bool doesNotGrow);

    std::string getName() const;
    int3 getPosition() const;
    int getQuantity() const;
    std::string getDisposition() const;
    bool getNeverFlees() const;
    bool getDoesNotGrow() const;

private:
    std::string name;
    int3 position;
    int quantity;
    std::string disposition;
    bool neverFlees;
    bool doesNotGrow;
};