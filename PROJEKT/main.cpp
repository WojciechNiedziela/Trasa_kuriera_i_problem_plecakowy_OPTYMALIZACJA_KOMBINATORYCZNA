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

    // Inicjalizacja magazynu z wprowadzonymi współrzędnymi
    Magazyn magazyn("Warszawa, ul. Przykladowa 1", magazynX, magazynY);

    // Inicjalizacja kuriera
    //Kurier kurier(1, 100.0);

    // Kontener na paczki
    std::vector<Paczka> paczki;
    std::vector<Kurier> kurierzy;

    // Inicjalizacja mapy
    Mapa mapa;

    classDraw.MainWindow(paczki, magazyn, kurierzy, mapa);

    return 0;
}
