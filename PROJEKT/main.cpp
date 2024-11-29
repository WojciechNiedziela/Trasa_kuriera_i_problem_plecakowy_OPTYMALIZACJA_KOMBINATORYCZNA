// g++ -o program main.cpp "GRAPHIC INTERFACE/classDraw.cpp" "GRAPHIC INTERFACE/classHandlingEvents.cpp" "GRAPHIC INTERFACE/classDrawMap.cpp" "MECHANICS/classPaczka.cpp" "MECHANICS/classMagazyn.cpp" "MECHANICS/classKurier.cpp" "MECHANICS/classMapa.cpp" "MECHANICS/classTrasa.cpp" -lsfml-graphics -lsfml-window -lsfml-system

// ./program

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <cmath>

#include "classPaczka.h"
#include "classMagazyn.h"
#include "classKurier.h"
#include "classMapa.h"
#include "classTrasa.h"
#include "classDraw.h"


int main()
{
    // sf::Font font;
    // if (!font.loadFromFile("C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/FONTS/arial.ttf"))
    // {
    //     std::cerr << "Failed to load font!" << std::endl;
    //     return -1;
    // }

    std::vector<std::string> magazynInputBuffers(2, "");
    int magazynStep = 0;
    double magazynX = 0.0, magazynY = 0.0;
    
    classDraw classDraw;

    // Wywołanie metody MagazynInput
    //classDraw.MagazynInput(font, magazynInputBuffers, magazynStep, magazynX, magazynY);

    classDraw.MagazynInput(magazynX, magazynY);

    // Jeśli okno zostało zamknięte, kontynuuj do głównego programu
    sf::RenderWindow window(sf::VideoMode(800, 800), "Trasa kuriera");

    // Inicjalizacja magazynu z wprowadzonymi współrzędnymi
    Magazyn magazyn("Warszawa, ul. Przykladowa 1", magazynX, magazynY);

    // Inicjalizacja kuriera
    Kurier kurier(1, "Jan", "Kowalski", "samochód dostawczy", &magazyn);

    // Kontener na paczki
    std::vector<Paczka> paczki;

    // Inicjalizacja mapy
    Mapa mapa;

    //classDraw.MainWindow(window, font, paczki, magazyn, kurier, mapa);
    classDraw.MainWindow(paczki, magazyn, kurier, mapa);

    return 0;
}
