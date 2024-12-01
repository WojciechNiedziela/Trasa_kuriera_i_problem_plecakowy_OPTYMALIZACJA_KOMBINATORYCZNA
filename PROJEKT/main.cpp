// g++ -o program main.cpp "classDraw.cpp" "classHandlingEvents.cpp" "classDrawMap.cpp" "classPaczka.cpp" "classMagazyn.cpp" "classKurier.cpp" "classMapa.cpp" "classTrasa.cpp" -lsfml-graphics -lsfml-window -lsfml-system

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
    std::vector<std::string> magazynInputBuffers(2, "");
    int magazynStep = 0;
    double magazynX = 0.0, magazynY = 0.0;
    
    classDraw classDraw;

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

    classDraw.MainWindow(paczki, magazyn, kurier, mapa);

    return 0;
}
