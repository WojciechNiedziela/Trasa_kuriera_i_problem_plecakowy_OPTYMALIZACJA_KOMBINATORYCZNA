#ifndef CLASS_HANDLING_EVENTS_H
#define CLASS_HANDLING_EVENTS_H

#include <vector>
#include <string>
#include "classPaczka.h"
#include "classMagazyn.h"
#include "classKurier.h"
#include "classMapa.h"

class classHandlingEvents {
public:
    // Konstruktor domyślny
    classHandlingEvents();

    // Funkcja do obsługi głównego okna
    void handleMainWindow(std::vector<Paczka> &paczki, Magazyn &magazyn, Kurier &kurier, Mapa &mapa);

private:
    // Funkcja do dodawania paczki
    void addPackage(std::vector<Paczka> &paczki);

    // Funkcja do wyświetlania listy paczek
    void displayPackages(const std::vector<Paczka> &paczki);

    // Funkcja do wyznaczania i wyświetlania tras
    void displayRoutes(std::vector<Paczka> &paczki, Magazyn &magazyn, Kurier &kurier, Mapa &mapa,
                       std::vector<std::string> &routes, const std::string &algorithm);

    // Funkcja do obliczania odległości między dwoma punktami
    double calculateDistance(double x1, double y1, double x2, double y2);
};

#endif // CLASS_HANDLING_EVENTS_H
