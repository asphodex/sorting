//
// Created by sphdx on 19.01.2025.
//

#ifndef SORTINGTYPE_H
#define SORTINGTYPE_H

#include "licensePlate.h"
#include "personName.h"

struct sortingType {
    licensePlate licensePlate;
    personName personName;
    int line{};

    bool operator<(const sortingType& other) const {
        if (licensePlate != other.licensePlate) {
            return licensePlate < other.licensePlate;
        }
        return personName > other.personName;
    }

    bool operator>(const sortingType& other) const {
        if (licensePlate != other.licensePlate) {
            return licensePlate > other.licensePlate;
        }
        return personName < other.personName;
    }

    bool operator==(const sortingType& other) const {
        return licensePlate == other.licensePlate && personName == other.personName;
    }

    bool operator!() const {
        return !licensePlate && !personName;
    }

    friend std::ostream& operator<<(std::ostream& os, const sortingType& st) {
        os << st.licensePlate << " " << st.personName << " " << st.line;
        return os;
    }
};

#endif //SORTINGTYPE_H
