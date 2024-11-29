#include "classMapa.h"
#include <iostream>
#include <cmath>

using namespace std;

// Konstruktor
Mapa::Mapa() {}

// Dodawanie lokalizacji do mapy
void Mapa::dodajLokalizacje(const string& nazwa, double szerokosc, double dlugosc) {
    this->lokalizacje.push_back(make_pair(nazwa, make_pair(szerokosc, dlugosc)));
}

// Dodawanie odległości między lokalizacjami
void Mapa::dodajOdleglosc(const string& lokalizacja1, const string& lokalizacja2, double odleglosc) {
    this->odleglosci[make_pair(lokalizacja1, lokalizacja2)] = odleglosc;
    this->odleglosci[make_pair(lokalizacja2, lokalizacja1)] = odleglosc; // Odległość w drugą stronę
}

double Mapa::odleglosc(double x1, double y1, double x2, double y2) const {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

// Wyświetlanie wszystkich lokalizacji
void Mapa::displayLokalizacje() const {
    cout << "Lokalizacje na mapie:" << endl;
    for (const auto& lokalizacja : this->lokalizacje) {
        cout << "Nazwa: " << lokalizacja.first 
             << ", Szerokosc: " << lokalizacja.second.first 
             << ", Długosc: " << lokalizacja.second.second << endl;
    }
}

// Wyświetlanie wszystkich odległości
void Mapa::displayOdleglosci() const {
    cout << "Odleglosci między lokalizacjami:" << endl;
    for (const auto& odleglosc : this->odleglosci) {
        cout << "Odległosc miedzy " << odleglosc.first.first 
             << " a " << odleglosc.first.second 
             << ": " << odleglosc.second << " km" << endl;
    }
}

// Znajdowanie lokalizacji
pair<double, double> Mapa::znajdzLokalizacje(const string& nazwa) const {
    for (const auto& lokalizacja : this->lokalizacje) {
        if (lokalizacja.first == nazwa) {
            return lokalizacja.second; // Zwraca współrzędne
        }
    }
    return make_pair(0.0, 0.0); // W przypadku nieznalezienia, zwraca 0,0
}
