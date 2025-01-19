//
// Created by sphdx on 17.01.2025.
//

#ifndef LICENSEPLATE_H
#define LICENSEPLATE_H

struct licensePlate {
    char letter1{};
    int number{};
    char letter2{};
    char letter3{};

    bool operator<(const licensePlate& other) const {
        if (letter1 != other.letter1) return letter1 < other.letter1;
        if (number != other.number) return number < other.number;
        if (letter2 != other.letter2) return letter2 < other.letter2;
        return letter3 < other.letter3;
    }

    bool operator>(const licensePlate& other) const {
        if (letter1 != other.letter1) return letter1 > other.letter1;
        if (number != other.number) return number > other.number;
        if (letter2 != other.letter2) return letter2 > other.letter2;
        return letter3 > other.letter3;
    }

    bool operator==(const licensePlate& other) const {
        return letter1 == other.letter1 && number == other.number && letter2 == other.letter2 && letter3 == other.letter3;
    }

    bool operator!() const {
        return letter1 == 0 && number == 0 && letter2 == 0 && letter3 == 0;
    }

    friend std::ostream& operator<<(std::ostream& os, const licensePlate& lp) {
        os << lp.letter1 << lp.number << lp.letter2 << lp.letter3;
        return os;
    }

    [[nodiscard]] std::string toString() const {
        return std::string(1, letter1) + std::to_string(number) + letter2 + letter3;
    }

    // isValidLicensePlate takes string like 'A123AA' and returns false if it is invalid license plate or true if valid
    static bool isValidLicensePlate(const std::string& plate) {
        return plate.length() == 6 &&
               std::isalpha(plate[0]) &&
               std::isdigit(plate[1]) &&
               std::isdigit(plate[2]) &&
               std::isdigit(plate[3]) &&
               std::isalpha(plate[4]) &&
               std::isalpha(plate[5]);
    }

    // parseLicensePlate takes string like 'A123AA' and returns an object of licensePlate structure
    static licensePlate parseLicensePlate(const std::string& plate) {
        return { plate[0], std::stoi(plate.substr(1, 3)), plate[4], plate[5]};
    }
};




#endif //LICENSEPLATE_H
