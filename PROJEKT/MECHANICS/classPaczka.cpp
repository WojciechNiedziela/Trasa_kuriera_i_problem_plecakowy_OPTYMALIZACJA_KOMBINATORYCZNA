#include "classPaczka.h"
#include <iostream>

using namespace std;

// Konstruktor
Paczka::Paczka(int id, const string& adres, double waga, double x, double y) {
    this->id_paczki = id;
    this->adres_dostawy = adres;
    this->waga = waga;
    this->x = x;
    this->y = y;
}

// Getter dla ID paczki
int Paczka::getId() const {
    return this->id_paczki;
}

// Getter dla adresu dostawy paczki
string Paczka::getAdres() const {
    return this->adres_dostawy;
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

// Wyświetlanie informacji o paczce
void Paczka::display() const {
    cout << "Paczka ID: " << id_paczki << ", Adres: " << adres_dostawy
         << ", Waga: " << waga << " kg, Wspolrzedne: (" << x << ", " << y << ")" << endl;
}
