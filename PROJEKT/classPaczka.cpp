#include "classPaczka.h"
#include <iostream>

using namespace std;


// Getter dla ID paczki
int Paczka::getId() const {
    return this->id_paczki;
}

// Getter dla wagi paczki
double Paczka::getWaga() const {
    return this->waga;
}

double Paczka::getX() const {
    return this->x;
}

double Paczka::getY() const {
    return this->y;
}

std::string Paczka::getDataWaznosci() const {
    return this->data_waznosci;
}

// Settery
void Paczka::setDataWaznosci(const std::string& data_waznosci="2099-12-31") {
    this->data_waznosci = data_waznosci;
}


// Wyświetlanie informacji o paczce
void Paczka::display() const {
    cout << "Paczka ID: " << id_paczki //<< ", Adres: " << adres_dostawy
         << ", Waga: " << waga << " kg, Wspolrzedne: (" << x << ", " << y << ")" << endl;
}
