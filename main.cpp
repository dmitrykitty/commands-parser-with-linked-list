#include <iostream>
#include <sstream>
#include <string>
#include <complex>
#include <stdexcept>
#include <cctype>
#include "objectlist.h"
#include "object.h"
#include "complexObject.h"
#include "doubleObject.h"
#include "intObject.h"
#include "stringObject.h"

/// @todo Uzupelnij swoje dane:
constexpr const char* const FIRSTNAME = "Dzmitry";
constexpr const char* const SURNAME   = "Nikitsin";
constexpr const char* const MAIL    = "dnikitin@agh.edu.pl";
constexpr const char* const BOOK_ID = "429337";

constexpr const char* const TEACHER_MAIL = "bazior[at]agh.edu.pl"; // change if not correct

void validateStudentsInfo();


int main()
{
    validateStudentsInfo();

     ObjectList list;
    std::string line;
    while(std::getline(std::cin,line)){
        if(line.empty()) continue;
        std::istringstream iss(line);
        std::string cmd; iss>>cmd;
        if(cmd=="add"){
            std::string arg;
            if(!(iss>>arg)){ std::cerr<<"Error: missing argument\n"; continue; }
            auto pos = arg.find(':');
            if(pos==std::string::npos){ std::cerr<<"Error: bad add format\n"; continue; }
            std::string type = arg.substr(0,pos);
            std::string val  = arg.substr(pos+1);
            try {
                if(type=="IntObject"){
                    int v=std::stoi(val);
                    list.push_back(new IntObject(v));
                }
                else if(type=="DoubleObject"){
                    double d=std::stod(val);
                    list.push_back(new DoubleObject(d));
                }
                else if(type=="ComplexObject"){
                    // format re+imi
                    auto plus = val.find('+');
                    auto i    = val.find('i',plus);
                    if(plus==std::string::npos||i==std::string::npos){
                        throw std::runtime_error("bad complex");
                    }
                    int re=std::stoi(val.substr(0,plus));
                    int im=std::stoi(val.substr(plus+1,i-plus-1));
                    list.push_back(new ComplexObject(re,im));
                }
                else if(type=="StringObject"){
                    list.push_back(new StringObject(val));
                }
                else {
                    std::cerr<<"Error: wrong type\n";
                }
            }
            catch(...){
                std::cerr<<"Error: invalid value\n";
            }
        }
        else if(cmd=="sum"){
            std::string type; iss>>type;
            if(type.empty()){ std::cerr<<"Error: missing argument\n"; continue; }
            if(type=="IntObject"){
                long long acc=0;
                for(auto& o: list)
                    if(auto p = dynamic_cast<IntObject*>(&o))
                        acc += p->num;
                std::cout<<"IntObject("<<acc<<")\n";
            }
            else if(type=="DoubleObject"){
                double acc=0;
                for(auto& o: list)
                    if(auto p = dynamic_cast<DoubleObject*>(&o))
                        acc += p->doubleNum;
                std::cout.setf(std::ios::fixed); std::cout.precision(6);
                std::cout<<"DoubleObject("<<acc<<")\n";
            }
            else if(type=="ComplexObject"){
                std::complex<int> acc{0,0};
                for(auto& o: list)
                    if(auto p = dynamic_cast<ComplexObject*>(&o))
                        acc += p->complex;
                std::cout<<"ComplexObject("<<acc.real()
                         <<"+"<<acc.imag()<<"i)\n";
            }
            else {
                std::cerr<<"Error: wrong type\n";
            }
        }
        else if(cmd=="get"){
            int idx; if(!(iss>>idx)){ std::cerr<<"Error: missing argument\n"; continue;}
            if(idx<0||size_t(idx)>=list.size()){ std::cerr<<"Error: bad index\n"; continue; }
            size_t steps = list.size() - 1 - idx;
            auto it = list.begin();
            while(steps--) ++it;
            std::cout<<*it<<"\n";
        }
        else if(cmd=="mul"){
            int m; if(!(iss>>m)){ std::cerr<<"Error: missing argument\n"; continue;}
            for(auto& o: list) o.multiply(m);
        }
        else {
            std::cerr<<"Error: unknown command\n";
        }
    }


    /// następnie można to usunąć:
    std::cout << "Hello " << FIRSTNAME << " " << SURNAME << "!" << '\n'
              << "make Your teacher: " << TEACHER_MAIL << " proud of you!" << std::endl;
}

constexpr inline size_t compileTimeStrlen(const char* text) noexcept
{
    return (*text ? 1+compileTimeStrlen(text+1) : 0);
}

constexpr inline size_t compileTimeCountFirstDigits(const char* text) noexcept
{
    return (*text && ('0'<=*text && *text<='9') ? 1+compileTimeCountFirstDigits(text+1) : 0);
}

constexpr inline bool compileTimeIsDigit(const char* text) noexcept
{
    return compileTimeStrlen(text) == compileTimeCountFirstDigits(text);
}

constexpr inline bool compileTimeContains(const char* text, char letter) noexcept
{
    if ('\n' == *text)
        return false;

    return *text == letter ? 1 : compileTimeContains(text+1, letter);
}

void validateStudentsInfo()
{
    static_assert(compileTimeStrlen(FIRSTNAME) > 2, "You must input your first name!");
    static_assert(compileTimeStrlen(SURNAME) > 2, "You must input your surname!");

    static_assert(compileTimeStrlen(BOOK_ID) > 1, "You must input your student book id!");
    static_assert(compileTimeIsDigit(BOOK_ID), "You student book id should consist only digits, shouldn't it?!");

    static_assert(compileTimeStrlen(MAIL) > 3, "You must input your mail (preferred AGH mail)!");
    static_assert(compileTimeContains(MAIL, '@'), "Invalid mail! It should contain '@'!");
    static_assert(compileTimeContains(MAIL, '.'), "Invalid mail! It should contain '.'!");
}
