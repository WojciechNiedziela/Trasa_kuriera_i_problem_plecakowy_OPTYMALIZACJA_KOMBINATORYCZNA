#ifndef HANDLING_EVENTS_H
#define HANDLING_EVENTS_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classPaczka.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classMagazyn.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classKurier.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classMapa.h"
#include "classDraw.h"


class Paczka;
class Magazyn;
class Kurier;
class Mapa;

class classHandlingEvents
{
public:
    // Konstruktor domyślny
    classHandlingEvents();

    std::vector<Paczka> currentRoute;


    void handleTextInput(sf::Event &event, std::vector<std::string> &inputBuffers, int &step, UIElements &uiElements);

    void handleMouseClick(sf::Event &event, std::vector<Paczka> &paczki, Magazyn &magazyn, Kurier &kurier, Mapa &mapa, std::vector<std::string> &routes, bool &showMap, 
    UIElements &uiElements, std::vector<Paczka> &currentRoute);

    void handleMainWindowEvents(sf::Event &event, sf::RenderWindow &window, std::vector<Paczka> &paczki, Magazyn &magazyn, Kurier &kurier, Mapa &mapa,
                                std::vector<std::string> &routes, bool &showMap, UIElements &uiElements, std::vector<std::string> &inputBuffers, int &step, 
                                std::vector<Paczka> &currentRoute);

private:
    double calculateDistance(double x1, double y1, double x2, double y2);
    std::vector<std::string> inputBuffers{5, ""}; // 5 fields: ID, address, weight, x, y
    int step = 0;
    bool selectingFromMap = false; // Flaga wskazująca, czy wybieramy punkt z mapy
};

#endif // HANDLING_EVENTS_H
