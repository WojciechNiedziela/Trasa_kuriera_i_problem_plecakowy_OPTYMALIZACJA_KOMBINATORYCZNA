#ifndef CLASS_DRAW_H
#define CLASS_DRAW_H

#include <vector>
#include <string>
#include "classPaczka.h"
#include "classMagazyn.h"
#include "classKurier.h"
#include "classMapa.h"
#include "classTrasa.h"
#include <iostream>
#include <cmath>

class classDraw {
public:
    // Konstruktor
    classDraw();

    // Metoda do wprowadzania współrzędnych magazynu
    void MagazynInput(double &magazynX, double &magazynY);

    // Główne okno aplikacji
    void MainWindow(std::vector<Paczka> &paczki, Magazyn &magazyn, Kurier &kurier, Mapa &mapa);

    // Dodawanie paczki
    void AddPackage(std::vector<Paczka> &paczki);

    // Wyświetlanie listy paczek
    void DisplayPackages(const std::vector<Paczka> &paczki);

    // Wyświetlanie tras dla różnych algorytmów
    void DisplayRoutes(std::vector<Paczka> &paczki, Magazyn &magazyn, Kurier &kurier, Mapa &mapa, const std::string &algorithm);

private:
    // Metoda do obliczania dystansu między punktami
    double calculateDistance(double x1, double y1, double x2, double y2);
};

#endif // CLASS_DRAW_H
