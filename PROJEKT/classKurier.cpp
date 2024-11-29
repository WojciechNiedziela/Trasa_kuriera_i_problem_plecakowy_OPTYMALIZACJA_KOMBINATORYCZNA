#include "classKurier.h"
#include <iostream>

using namespace std;

// Konstruktor
Kurier::Kurier(int id_kuriera, const string& imie, const string& nazwisko, const string& typ_pojazdu, Magazyn* magazyn) {
    this->id_kuriera = id_kuriera; // Przypisanie id kuriera
    this->imie = imie;               // Przypisanie imienia kuriera
    this->nazwisko = nazwisko;       // Przypisanie nazwiska kuriera
    this->typ_pojazdu = typ_pojazdu; // Przypisanie typu pojazdu
    this->magazyn = magazyn;         // Przypisanie wskaźnika do magazynu
}

// Getter dla id kuriera
int Kurier::getId() const {
    return this->id_kuriera;
}

// Getter dla imienia kuriera
string Kurier::getImie() const {
    return this->imie;
}

// Getter dla nazwiska kuriera
string Kurier::getNazwisko() const {
    return this->nazwisko;
}

// Getter dla pełnej nazwy kuriera
string Kurier::getNazwa() const {
    return this->imie + " " + this->nazwisko; // Łączenie imienia i nazwiska
}

// Getter dla typu pojazdu
string Kurier::getTypPojazdu() const {
    return this->typ_pojazdu;
}

// Getter dla magazynu
Magazyn* Kurier::getMagazyn() const {
    return this->magazyn;
}

// Metoda do wyświetlania informacji o kurierze
void Kurier::displayKurier() const {
    cout << "Kurier ID: " << this->id_kuriera << endl;
    cout << "Pełna nazwa: " << this->getNazwa() << endl; // Użycie getNazwa()
    cout << "Typ pojazdu: " << this->typ_pojazdu << endl;
    cout << "Magazyn: " << (this->magazyn ? this->magazyn->getAdresMagazynu() : "Brak magazynu") << endl;
}
