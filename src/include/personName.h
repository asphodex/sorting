//
// Created by sphdx on 17.01.2025.
//

#ifndef PERSONNAME_H
#define PERSONNAME_H

#include <iostream>
#include <string>
#include <algorithm>

struct personName {
    std::string firstName;  // Имя
    std::string lastName;   // Фамилия
    std::string middleName; // Отчество

    bool operator<(const personName& other) const {
        const std::string thisFirstThree = getFirstThree(lastName) + getFirstThree(firstName) + getFirstThree(middleName);
        const std::string otherFirstThree = getFirstThree(other.lastName) + getFirstThree(other.firstName) + getFirstThree(other.middleName);

        if (thisFirstThree != otherFirstThree) {
            return thisFirstThree < otherFirstThree;
        }

        // recursive calls
        if (firstName != other.firstName) return firstName < other.firstName;
        if (lastName != other.lastName) return lastName < other.lastName;
        return middleName < other.middleName;
    }

    bool operator>(const personName& other) const {
        return other < *this;
    }

    bool operator==(const personName& other) const {
        return firstName == other.firstName && lastName == other.lastName && middleName == other.middleName;
    }

    bool operator!() const {
        return firstName.empty() && lastName.empty() && middleName.empty();
    }

    friend std::ostream& operator<<(std::ostream& os, const personName& pn) {
        os << pn.lastName << " " << pn.firstName << " " << pn.middleName;
        return os;
    }

private:
    static std::string getFirstThree(const std::string& str) {
        return str.substr(0, std::min(3, static_cast<int>(str.length())));
    }
};

#endif //PERSONNAME_H
