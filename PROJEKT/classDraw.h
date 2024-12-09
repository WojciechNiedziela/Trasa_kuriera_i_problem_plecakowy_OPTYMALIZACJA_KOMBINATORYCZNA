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
    //void MagazynInput(double &magazynX, double &magazynY);

    // Funkcja do ustawienia liczby kurierów
    void SetCouriers(std::vector<Kurier> &kurierzy);

    // Funkcja do wyświetlenia szczegółów kurierów
    void DisplayCouriers(const std::vector<Kurier> &kurierzy);

    void LoadPackagesFromFile(std::vector<Paczka>& paczki, const std::string& fileName);


    // Główne okno aplikacji
    void MainWindow(std::vector<Paczka> &paczki, Magazyn &magazyn, std::vector<Kurier> &kurierzy, Mapa &mapa);

    // Dodawanie paczki
    void AddPackage(std::vector<Paczka> &paczki);

    // Wyświetlanie listy paczek
    void DisplayPackages(const std::vector<Paczka> &paczki);

    // Wyświetlanie tras dla różnych algorytmów
    void DisplayRoutes(std::vector<Paczka> &paczki, Magazyn &magazyn, std::vector<Kurier> &kurierzy, Mapa &mapa, const std::string &algorithm);

private:
    // Metoda do obliczania dystansu między punktami
    double calculateDistance(double x1, double y1, double x2, double y2);
    std::vector<Kurier> kurierzy; // Lista kurierów
};

#endif // CLASS_DRAW_H
