#include "classKurier.h"
#include <iostream>

using namespace std;

// Konstruktor
Kurier::Kurier(int id_kuriera, double ladownosc) {
    this->id_kuriera = id_kuriera; // Przypisanie id kuriera
    this->ladownosc = ladownosc;     // Przypisanie ładowności
}

// Getter dla id kuriera
int Kurier::getId() const {
    return this->id_kuriera;
}


double Kurier::getLadownosc() const {
    return this->ladownosc;
}

// Metoda do wyświetlania informacji o kurierze
void Kurier::displayKurier() const {
    cout << "Kurier ID: " << this->id_kuriera << endl;;
    cout << "Ładowność pojazdu: " << this->ladownosc << " kg" << endl;
}
