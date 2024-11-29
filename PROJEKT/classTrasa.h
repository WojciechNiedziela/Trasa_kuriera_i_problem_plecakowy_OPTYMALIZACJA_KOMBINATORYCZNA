#ifndef CLASSTRASA_H
#define CLASSTRASA_H

#include <vector>
#include <string>
#include "classPaczka.h"  // Zakładamy, że trasa może mieć paczki
#include "classKurier.h"  // Zakładamy, że trasa jest związana z kurierem
#include "classMapa.h" // Upewnij się, że nagłówek Mapa jest dołączony
#include "classMagazyn.h" // Upewnij się, że nagłówek Magazyn jest dołączony
#include <SFML/Graphics.hpp>

using namespace std;

class Trasa {
private:
    Kurier* kurier;                // Wskaźnik do kuriera
    Mapa* mapa;                    // Wskaźnik do mapy
    Magazyn* magazyn;              // Wskaźnik do magazynu
    vector<Paczka> paczki;         // Lista paczek do dostarczenia
    string start;                  // Miejsce startowe
    string koniec;                 // Miejsce końcowe


    double obliczDlugoscTrasy(const vector<Paczka>& trasa) const;
    vector<Paczka> krzyzowanie(const vector<Paczka>& rodzic1, const vector<Paczka>& rodzic2);
    void mutacja(vector<Paczka>& trasa);

public:
    // Konstruktor
    //Trasa(Kurier* kurier, const string& start, const string& koniec);
    Trasa(Kurier* kurier, Magazyn* magazyn, const vector<Paczka>& paczki, Mapa* mapa);

    // Dodawanie paczki do trasy
    void dodajPaczke(const Paczka& paczka);

    // Wyświetlanie informacji o trasie
    void displayTrasa() const;

    // Metoda do obliczania optymalnej trasy (szkic)
    void obliczOptymalnaTrase() const;

    void draw(sf::RenderWindow& window);

    vector<Paczka> znajdzTraseAlgorytmZachlanny();  // Deklaracja metody zachłannej

    vector<Paczka> znajdzTraseAlgorytmGenetyczny(int rozmiarPopulacji = 50, int liczbaPokolen = 1000); 

    vector<Paczka> znajdzTraseWyzarzanie();  // Deklaracja metody wyżarzania
    // Deklaracja metody genetycznej
};

#endif // CLASSTRASA_H
