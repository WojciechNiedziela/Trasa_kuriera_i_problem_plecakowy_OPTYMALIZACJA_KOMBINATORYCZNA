#include "classMagazyn.h"
#include <iostream>

// Użycie przestrzeni nazw
using namespace std;

// Konstruktor
Magazyn::Magazyn(const string& adres, double x, double y) {
    this->adres_magazynu = adres;
    this->x = x;
    this->y = y;
}

// Dodawanie paczki do magazynu
void Magazyn::dodajPaczke(const Paczka& paczka) {
    this->paczki.push_back(paczka);
}

// Usuwanie paczki z magazynu
void Magazyn::usunPaczke(int id_paczki) {
    for (auto it = this->paczki.begin(); it != this->paczki.end(); ++it) {
        if (it->getId() == id_paczki) {
            this->paczki.erase(it);
            break;
        }
    }
}

// Pobranie wszystkich paczek z magazynu
vector<Paczka> Magazyn::getPaczki() const {
    return this->paczki;
}

// Metoda do pobierania adresu magazynu
string Magazyn::getAdresMagazynu() const {
    return this->adres_magazynu; // Zwraca adres magazynu
}

// Metoda do pobierania współrzędnej x magazynu
double Magazyn::getX() const {
    return this->x; // Zwraca współrzędną x magazynu
}

// Metoda do pobierania współrzędnej y magazynu
double Magazyn::getY() const {
    return this->y; // Zwraca współrzędną y magazynu
}

// Wyświetlanie szczegółów magazynu i przechowywanych paczek
void Magazyn::displayMagazyn() const {
    cout << "Magazyn pod adresem: " << adres_magazynu 
         << ", Współrzędne: (" << x << ", " << y << ")" << endl;
    cout << "Przechowywane paczki:" << endl;
    for (const auto& paczka : paczki) {
        paczka.display();
    }
}
