#ifndef CLASSKURIER_H
#define CLASSKURIER_H

#include <string>
#include "classMagazyn.h"  // Zakładamy, że kurier współpracuje z magazynem

using namespace std;

class Kurier {
private:
    int id_kuriera;
    string imie;
    string nazwisko;  // Imię i nazwisko kuriera
    string typ_pojazdu;
    Magazyn* magazyn;  // Wskaźnik do obiektu magazynu
    double ladownosc;  // Ładowność pojazdu

public:
    // Konstruktor
    Kurier(int id, const string& imie, const string& nazwisko, const string& typ_pojazdu, 
    Magazyn* magazyn, double ladownosc);

    // Gettery
    int getId() const;
    string getImie() const;
    string getNazwisko() const;
    string getNazwa() const; // Gettera dla pełnej nazwy
    string getTypPojazdu() const;
    Magazyn* getMagazyn() const;
    double getLadownosc() const;
    // Metoda do wyświetlania informacji o kurierze
    void displayKurier() const;
};

#endif // CLASSKURIER_H
