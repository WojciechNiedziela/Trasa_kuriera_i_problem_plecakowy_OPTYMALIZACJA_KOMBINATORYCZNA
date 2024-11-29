#include "classHandlingEvents.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classPaczka.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classMagazyn.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classKurier.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classMapa.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classTrasa.h" // Include this for the Trasa class
#include <cmath>
#include <iostream>

// Konstruktor klasy
classHandlingEvents::classHandlingEvents() {}

double classHandlingEvents::calculateDistance(double x1, double y1, double x2, double y2)
{
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void classHandlingEvents::handleMainWindowEvents(sf::Event &event, sf::RenderWindow &window, std::vector<Paczka> &paczki, Magazyn &magazyn, Kurier &kurier, Mapa &mapa,
                                                 std::vector<std::string> &routes, bool &showMap, UIElements &uiElements, std::vector<std::string> &inputBuffers, int &step,
                                                 std::vector<Paczka> &currentRoute)
{
    int paczkaId=-1;
    double waga=-1, x=-1, y=-1;

    if (event.type == sf::Event::Closed)
    {
        window.close();
    }
    else if (event.type == sf::Event::TextEntered)
    {
        // Obsługa wprowadzania tekstu
        handleTextInput(event, inputBuffers, step, uiElements);
    }
    else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
    {
        // Przechodzenie między polami i dodawanie paczki
        if (step == 4)
        {

            try
            {
                paczkaId = std::stoi(inputBuffers[0]);
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Blad. Ustawiono domyslna wartosc = -1" << std::endl;
            }

            std::string adres = inputBuffers[1];

            try
            {
                 waga = std::stod(inputBuffers[2]);
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Blad. Ustawiono domyslna wartosc = -1" << std::endl;
            }
            
            try
            {
                x = std::stod(inputBuffers[3]);
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Blad. Ustawiono domyslna wartosc = -1" << std::endl;
            }

            try
            {
                y = std::stod(inputBuffers[4]);
            }
            catch (const std::invalid_argument &e)
            {
                std::cout << "Blad. Ustawiono domyslna wartosc = -1" << std::endl;
            }
           
            
            

            paczki.emplace_back(paczkaId, adres, waga, x, y); // Dodanie paczki
            inputBuffers = std::vector<std::string>(5, "");   // Resetowanie buforów

            for (auto &inputText : uiElements.inputTexts)
            {
                inputText.setString("");
            }

            step = 0; // Powrót do pierwszego pola
        }
        else
        {
            step++;
        }
    }
    else if (event.type == sf::Event::MouseButtonPressed)
    {
        // Obsługa kliknięć
        handleMouseClick(event, paczki, magazyn, kurier, mapa, routes, showMap, uiElements, currentRoute);
    }
}

void classHandlingEvents::handleTextInput(sf::Event &event, std::vector<std::string> &inputBuffers, int &step, UIElements &uiElements)
{
    if (event.text.unicode < 128 && step < inputBuffers.size())
    {
        char enteredChar = static_cast<char>(event.text.unicode);
        if (enteredChar == '\b')
        { // Backspace
            if (!inputBuffers[step].empty())
            {
                inputBuffers[step].pop_back();
                uiElements.inputTexts[step].setString(inputBuffers[step]); // Aktualizacja tekstu
            }
        }
        else if (enteredChar >= 32 && enteredChar <= 126)
        { // Znak drukowalny
            inputBuffers[step] += enteredChar;
            uiElements.inputTexts[step].setString(inputBuffers[step]); // Aktualizacja tekstu
        }
    }
}

void classHandlingEvents::handleMouseClick(sf::Event &event, std::vector<Paczka> &paczki, Magazyn &magazyn, Kurier &kurier, Mapa &mapa,
                                           std::vector<std::string> &routes, bool &showMap, UIElements &uiElements, std::vector<Paczka> &currentRoute)
{
    // Obsługa kliknięcia przycisku algorytmu genetycznego
    if (uiElements.buttons["calculateGenetic"].getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)) 
    && paczki.size() > 0)
    {
        Trasa trasa(&kurier, &magazyn, paczki, &mapa);

        currentRoute = trasa.znajdzTraseAlgorytmGenetyczny(); // Zapisz trasę do `currentRoute`

        routes.clear();
        double totalDistance = 0.0;
        double prevX = magazyn.getX(), prevY = magazyn.getY();

        for (const auto &paczka : currentRoute)
        {
            routes.push_back("Dostawa: " + std::to_string(paczka.getId()));
            totalDistance += calculateDistance(prevX, prevY, paczka.getX(), paczka.getY());
            prevX = paczka.getX();
            prevY = paczka.getY();
        }

        totalDistance += calculateDistance(prevX, prevY, magazyn.getX(), magazyn.getY());
        routes.push_back("Powrot do magazynu");
        routes.push_back("Calkowita dlugosc trasy: " + std::to_string(totalDistance) + " jednostek");
    }

    // Obsługa kliknięcia przycisku algorytmu zachłannego
    if (uiElements.buttons["calculateGreedy"].getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))
    && paczki.size() > 0)
    {
        Trasa trasa(&kurier, &magazyn, paczki, &mapa);
        currentRoute = trasa.znajdzTraseAlgorytmZachlanny();
        routes.clear(); // Wyczyszczenie poprzednich wyników
        double totalDistance = 0.0;
        // Początkowe współrzędne (magazyn jako start)
        double prevX = magazyn.getX();
        double prevY = magazyn.getY();

        for (const auto &paczka : currentRoute)
        {
            // Dodaj do listy tras
            routes.push_back("Dostawa: " + std::to_string(paczka.getId()));
            // Oblicz dystans od poprzedniego punktu
            totalDistance += calculateDistance(prevX, prevY, paczka.getX(), paczka.getY());
            // Zaktualizuj współrzędne
            prevX = paczka.getX();
            prevY = paczka.getY();
        }
        // Powrót do magazynu
        totalDistance += calculateDistance(prevX, prevY, magazyn.getX(), magazyn.getY());
        routes.push_back("Powrot do magazynu");

        // Całkowita długość trasy
        routes.push_back("Calkowita dlugosc trasy: " + std::to_string(totalDistance) + " jednostek");
    }

    // Obsługa kliknięcia przycisku algorytmu wyżarzania
    if (uiElements.buttons["calculateAnnealing"].getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y))
    && paczki.size() > 0)
    {
        Trasa trasa(&kurier, &magazyn, paczki, &mapa);
        currentRoute = trasa.znajdzTraseWyzarzanie();

        routes.clear(); // Wyczyszczenie poprzednich wyników

        double totalDistance = 0.0;

        // Początkowe współrzędne (magazyn jako start)
        double prevX = magazyn.getX();
        double prevY = magazyn.getY();

        for (const auto &paczka : currentRoute)
        {
            // Dodaj do listy tras
            routes.push_back("Dostawa: " + std::to_string(paczka.getId()));

            // Oblicz dystans od poprzedniego punktu
            totalDistance += calculateDistance(prevX, prevY, paczka.getX(), paczka.getY());

            // Zaktualizuj współrzędne
            prevX = paczka.getX();
            prevY = paczka.getY();
        }
        // Powrót do magazynu
        totalDistance += calculateDistance(prevX, prevY, magazyn.getX(), magazyn.getY());
        routes.push_back("Powrot do magazynu");

        // Całkowita długość trasy
        routes.push_back("Calkowita dlugosc trasy: " + std::to_string(totalDistance) + " jednostek");
    }

    // Obsługa kliknięcia przycisku "MAPA"
    if (uiElements.buttons["map"].getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
    {
        showMap = true;
        // classDrawMap.drawMapWindow(font, paczki, magazyn, routes, currentRoute); // Dodaj currentRoute
    }

    if (uiElements.buttons["selectFromMap"].getGlobalBounds().contains(static_cast<float>(event.mouseButton.x), static_cast<float>(event.mouseButton.y)))
    {
        showMap = true;          // Otwórz mapę do wyboru punktu
        selectingFromMap = true; // Włącz tryb wybierania z mapy
    }
}
