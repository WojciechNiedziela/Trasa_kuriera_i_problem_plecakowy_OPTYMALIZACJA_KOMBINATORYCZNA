#ifndef DRAW_H
#define DRAW_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <map>
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classPaczka.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classMagazyn.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classKurier.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classMapa.h"

class Paczka;
class Magazyn;
class Kurier;
class Mapa;

struct UIElements
{
    sf::Text inputText;
    std::vector<sf::RectangleShape> inputFields;       // Pola wejściowe
    std::vector<sf::Text> inputTexts;                  // Teksty wprowadzane
    std::vector<sf::Text> inputLabels;                 // Etykiety dla pól wejściowych
    std::map<std::string, sf::RectangleShape> buttons; // Przycisk mapowany na nazwy
    std::map<std::string, sf::Text> buttonTexts;       // Teksty na przyciskach
};

class classDraw
{
public:
    // Konstruktor domyślny
    classDraw();

    // Funkcja do wprowadzania współrzędnych magazynu
    void MagazynInput(sf::Font &font, std::vector<std::string> &magazynInputBuffers, int &magazynStep, double &magazynX, double &magazynY);

    void MainWindow(sf::RenderWindow &window, sf::Font &font, std::vector<Paczka> &paczki, Magazyn &magazyn, Kurier &kurier, Mapa &mapa);

    UIElements initializeUIElements(sf::Font &font);

    std::pair<sf::RectangleShape, sf::Text> createButton(const sf::Vector2f &position, const std::string &text, const sf::Color &color, sf::Font &font);

    void drawMainWindow(sf::RenderWindow &window, sf::Font &font, std::vector<Paczka> &paczki, std::vector<std::string> &routes, UIElements &uiElements);

    void drawUIElements(sf::RenderWindow &window, UIElements &uiElements);

    void drawPackages(sf::RenderWindow &window, sf::Font &font, std::vector<Paczka> &paczki);
    
    void drawRoutes(sf::RenderWindow &window, sf::Font &font, std::vector<std::string> &routes);

    void addPackageDetails(sf::Font &font, std::vector<Paczka> &paczki, float x, float y);

private:
    double calculateDistance(double x1, double y1, double x2, double y2);
    std::vector<std::string> inputBuffers{5, ""}; // 5 fields: ID, address, weight, x, y
    std::vector<Paczka> currentRoute;
    int step = 0;
    bool selectingFromMap = false; // Flaga wskazująca, czy wybieramy punkt z mapy
};

#endif // DRAW_H
