#include <iostream>
#include <sstream>
#include <string>
#include <complex>
#include <stdexcept>
#include <cctype>
#include <fcntl.h>

#include "objectlist.h"
#include "object.h"
#include "complexObject.h"
#include "doubleObject.h"
#include "intObject.h"
#include "stringObject.h"

/// @todo Uzupelnij swoje dane:
constexpr const char* const FIRSTNAME = "Dzmitry";
constexpr const char* const SURNAME = "Nikitsin";
constexpr const char* const MAIL = "dnikitin@agh.edu.pl";
constexpr const char* const BOOK_ID = "429337";

constexpr const char* const TEACHER_MAIL = "bazior[at]agh.edu.pl"; // change if not correct

void validateStudentsInfo();


int main() {
#ifdef _WIN32
    _setmode(_fileno(stdin), _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);
#endif

    validateStudentsInfo();

    ObjectList list;
    std::string line;
    while (std::getline(std::cin, line)) {
        // 1) usuń końcowy '\r' jeśli jest (Windowsowe CRLF)
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        // 2) pomiń puste
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;

        if (cmd == "add") {
            // znajdź pierwszy ':' w oryginalnej linii
            auto colonPos = line.find(':');
            if (colonPos == std::string::npos) {
                std::cerr << "Error: bad add format\n";
                continue;
            }
            // typ = wszystko między "add " a dwukropkiem
            std::string type = line.substr(4, colonPos - 4);
            // val = reszta po dwukropku, obetnij początek i koniec
            std::string val = line.substr(colonPos + 1);
            // usuń wiodące spacje
            val.erase(0, val.find_first_not_of(' '));
            // usuń końcowe spacje
            val.erase(val.find_last_not_of(' ') + 1);

            try {
                if (type == "IntObject") {
                    int v = std::stoi(val);
                    list.push_back(new IntObject(v));
                } else if (type == "DoubleObject") {
                    double d = std::stod(val);
                    list.push_back(new DoubleObject(d));
                } else if (type == "ComplexObject") {
                    int re = 0, im = 0;

                    const std::size_t iPos = val.find('i');
                    if (iPos == std::string::npos) {
                        // brak 'i'  → czysta część rzeczywista
                        re = std::stoi(val);
                        im = 0;
                    } else {
                        std::string beforeI = val.substr(0, iPos); // wszystko przed 'i'

                        // szukamy ostatniego '+' lub '-' (-> oddziela re i im),
                        // pomijamy znak na pozycji 0, bo to leading sign realnej części
                        std::size_t signPos = std::string::npos;
                        for (std::size_t k = 1; k < beforeI.size(); ++k)
                            if (beforeI[k] == '+' || beforeI[k] == '-')
                                signPos = k;

                        if (signPos == std::string::npos) {
                            // brak separatora → tylko część urojona
                            re = 0;
                            const std::string& imagStr = beforeI; // może "", "+", "-", "22", "-7"
                            if (imagStr.empty() || imagStr == "+") im = 1;
                            else if (imagStr == "-") im = -1;
                            else im = std::stoi(imagStr);
                        } else {
                            // mamy realną i urojoną
                            std::string realStr = beforeI.substr(0, signPos);
                            std::string imagStr = beforeI.substr(signPos); // łącznie ze znakiem
                            re = std::stoi(realStr);
                            if (imagStr == "+") im = 1;
                            else if (imagStr == "-") im = -1;
                            else im = std::stoi(imagStr);
                        }
                    }
                    list.push_back(new ComplexObject(re, im));
                } else if (type == "StringObject") {
                    list.push_back(new StringObject(val));
                } else {
                    std::cerr << "Error: wrong type\n";
                }
            } catch (...) {
                std::cerr << "Error: invalid value\n";
            }
        } else if (cmd == "sum") {
            std::string type;
            iss >> type;
            if (type.empty()) {
                std::cerr << "Error: missing argument\n";
                continue;
            }

            if (type == "IntObject") {
                long long acc = 0;
                for (auto& o: list)
                    if (auto p = dynamic_cast<IntObject *>(&o))
                        acc += p->num;
                std::cout << "IntObject(" << acc << ")\n";
            } else if (type == "DoubleObject") {
                double acc = 0;
                for (auto& o: list)
                    if (auto p = dynamic_cast<DoubleObject *>(&o))
                        acc += p->doubleNum;
                std::cout.setf(std::ios::fixed);
                std::cout.precision(6);
                std::cout << "DoubleObject(" << acc << ")\n";
            } else if (type == "ComplexObject") {
                std::complex<int> acc{0, 0};
                for (auto& o: list)
                    if (auto p = dynamic_cast<ComplexObject *>(&o))
                        acc += p->complex;
                std::cout << "ComplexObject(" << acc.real()
                        << (acc.imag() >= 0 ? "+" : "") << acc.imag()
                        << "i)\n";
            } else if (type == "StringObject") {
                std::string acc;
                bool first = true;
                for (auto& o: list)
                    if (auto p = dynamic_cast<StringObject *>(&o)) {
                        if (!first) acc += " ";
                        acc += p->text; // Twoje pole z napisem
                        first = false;
                    }
                std::cout << "StringObject(\"" << acc << "\")\n";
            } else {
                std::cerr << "Error: wrong type\n";
            }
        } else if (cmd == "get") {
            int idx;
            if (!(iss >> idx)) {
                std::cerr << "Error: missing argument\n";
                continue;
            }
            if (idx < 0 || size_t(idx) >= list.size()) {
                std::cerr << "Error: bad index\n";
                continue;
            }
            size_t steps = list.size() - 1 - idx;
            auto it = list.begin();
            while (steps--) ++it;
            std::cout << *it << "\n";
        } else if (cmd == "mul") {
            int m;
            if (!(iss >> m)) {
                std::cerr << "Error: missing argument\n";
                continue;
            }
            for (auto& o: list)
                o.multiply(m);
        } else {
            std::cerr << "Error: unknown command\n";
        }
    }
    return 0;
}

constexpr inline size_t compileTimeStrlen(const char* text) noexcept {
    return (*text ? 1 + compileTimeStrlen(text + 1) : 0);
}

constexpr inline size_t compileTimeCountFirstDigits(const char* text) noexcept {
    return (*text && ('0' <= *text && *text <= '9') ? 1 + compileTimeCountFirstDigits(text + 1) : 0);
}

constexpr inline bool compileTimeIsDigit(const char* text) noexcept {
    return compileTimeStrlen(text) == compileTimeCountFirstDigits(text);
}

constexpr inline bool compileTimeContains(const char* text, char letter) noexcept {
    if ('\n' == *text)
        return false;

    return *text == letter ? 1 : compileTimeContains(text + 1, letter);
}

void validateStudentsInfo() {
    static_assert(compileTimeStrlen(FIRSTNAME) > 2, "You must input your first name!");
    static_assert(compileTimeStrlen(SURNAME) > 2, "You must input your surname!");

    static_assert(compileTimeStrlen(BOOK_ID) > 1, "You must input your student book id!");
    static_assert(compileTimeIsDigit(BOOK_ID), "You student book id should consist only digits, shouldn't it?!");

    static_assert(compileTimeStrlen(MAIL) > 3, "You must input your mail (preferred AGH mail)!");
    static_assert(compileTimeContains(MAIL, '@'), "Invalid mail! It should contain '@'!");
    static_assert(compileTimeContains(MAIL, '.'), "Invalid mail! It should contain '.'!");
}
