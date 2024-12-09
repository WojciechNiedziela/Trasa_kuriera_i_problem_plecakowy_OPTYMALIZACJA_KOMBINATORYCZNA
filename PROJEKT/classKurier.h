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
    Kurier(int id, double ladownosc);

    // Gettery
    int getId() const;

   double getLadownosc() const;
    // Metoda do wyświetlania informacji o kurierze
    void displayKurier() const;
};

#endif // CLASSKURIER_H
