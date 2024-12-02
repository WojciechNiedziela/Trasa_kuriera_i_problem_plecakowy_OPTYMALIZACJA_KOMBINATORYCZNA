#ifndef CLASSTRASA_H
#define CLASSTRASA_H

#include <vector>
#include "classPaczka.h"
#include "classKurier.h"
#include "classMagazyn.h"
#include "classMapa.h"

class Trasa {
private:
    Kurier* kurier;
    Magazyn* magazyn;
    std::vector<Paczka> paczki;
    Mapa* mapa;

    // Funkcja pomocnicza: rozwiązywanie problemu plecakowego
    std::vector<Paczka> rozwiazProblemPlecakowy(const std::vector<Paczka>& dostepnePaczki, double ladownosc);

    // Funkcja pomocnicza: obliczanie długości trasy
    double obliczDlugoscTrasy(const std::vector<Paczka>& trasa) const;

    // Funkcje pomocnicze dla algorytmu genetycznego
    std::vector<Paczka> krzyzowanie(const std::vector<Paczka>& rodzic1, const std::vector<Paczka>& rodzic2);
    void mutacja(std::vector<Paczka>& trasa);

public:
    // Konstruktor
    Trasa(Kurier* kurier, Magazyn* magazyn, const std::vector<Paczka>& paczki, Mapa* mapa);

    // Funkcja dodawania paczki do trasy
    void dodajPaczke(const Paczka& paczka);

    // Funkcja wyświetlająca szczegóły trasy
    void displayTrasa() const;

    // Algorytmy optymalizacyjne
    std::vector<Paczka> znajdzTraseAlgorytmZachlanny(); // Algorytm zachłanny z podziałem tras
    std::vector<Paczka> znajdzTraseAlgorytmGenetyczny(int rozmiarPopulacji = 50, int liczbaPokolen = 100); // Algorytm genetyczny
    std::vector<Paczka> znajdzTraseAlgorytmWyzarzania(); // Algorytm wyżarzania

    // Funkcja do wyświetlania tras
    void displayTrasy(const std::vector<std::vector<Paczka>>& trasy);
};

#endif // CLASSTRASA_H
