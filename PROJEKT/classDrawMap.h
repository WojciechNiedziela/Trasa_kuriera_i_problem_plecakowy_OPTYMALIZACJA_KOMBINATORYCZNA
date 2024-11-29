#ifndef CLASSDRAWMAP_H
#define CLASSDRAWMAP_H

#include "classPaczka.h"
#include "classMagazyn.h"
#include <vector>
#include <string>

// Klasa obsługująca operacje na mapie i wyświetlanie w terminalu
class classDrawMap {
public:
    // Konstruktor domyślny
    classDrawMap();

    // Metoda główna do obsługi operacji związanych z mapą
    void showMapWindow(std::vector<Paczka> &paczki, Magazyn &magazyn, std::vector<std::string> &routes, std::vector<Paczka> &currentRoute);

    // Wyświetlanie paczek na mapie w terminalu
    void displayPackagesOnMap(const std::vector<Paczka> &paczki, const Magazyn &magazyn);

    // Dodawanie paczki z terminala
    void addPackageTerminal(std::vector<Paczka> &paczki);

    // Wyświetlanie tras w terminalu
    void displayRoutes(const std::vector<Paczka> &route, const Magazyn &magazyn);

private:
    // Obliczanie odległości między dwoma punktami
    double calculateDistance(double x1, double y1, double x2, double y2);
};

#endif // CLASSDRAWMAP_H
