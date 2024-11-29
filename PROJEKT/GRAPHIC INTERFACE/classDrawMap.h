#ifndef DRAW_MAP_H
#define DRAW_MAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classPaczka.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classMagazyn.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classKurier.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classMapa.h"  
#include "classHandlingEvents.h"  


class Paczka;
class Magazyn;
class Kurier;
class Mapa;


class classDrawMap
{
public:
    // Konstruktor domyślny
    classDrawMap();

    std::vector<Paczka> currentRoute;


    bool selectingFromMap = true; // Flaga wskazująca, czy wybieramy punkt z mapy

    void showMapWindow(sf::Font &font, std::vector<Paczka> &paczki, Magazyn &magazyn, std::vector<std::string> &routes, std::vector<Paczka> &currentRoute);

    void drawMapWindow(sf::RenderWindow &mapWindow, sf::Font &font, std::vector<Paczka> &paczki, Magazyn &magazyn, std::vector<std::string> &routes);

    void drawGrid(sf::RenderWindow &mapWindow);
    void drawAxis(sf::RenderWindow &mapWindow);
    void drawPackagesOnMap(sf::RenderWindow &mapWindow, sf::Font &font, std::vector<Paczka> &paczki);
    void drawWarehouse(sf::RenderWindow &mapWindow, sf::Font &font, Magazyn &magazyn);
    void drawRoutesOnMap(sf::RenderWindow &mapWindow, Magazyn &magazyn, const std::vector<Paczka> &optimalRoute);



    void addPackageDetails(sf::Font &font, std::vector<Paczka> &paczki, float x, float y);

private:
//    double calculateDistance(double x1, double y1, double x2, double y2);
    std::vector<std::string> inputBuffers{5, ""}; // 5 fields: ID, address, weight, x, y
//    std::vector<Paczka> currentRoute;
    int step = 0;
};

#endif // DRAW_MAP_H
