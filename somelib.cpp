#include "somelib.hpp"

#include <iostream>


void SomeClass::setToOne() {
    SomeClass::m_var = 1;
    std::cout << "set var to 1" << std::endl;
}

int SomeClass::var() {
    return SomeClass::m_var;
}

void SomeClass::setVar(int value) {
    SomeClass::m_var = value;
}
