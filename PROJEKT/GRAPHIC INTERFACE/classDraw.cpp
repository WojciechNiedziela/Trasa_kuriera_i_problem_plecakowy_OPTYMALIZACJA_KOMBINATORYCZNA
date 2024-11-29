#include "classDraw.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classPaczka.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classMagazyn.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classKurier.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classMapa.h"
#include "C:/Users/Admin/Documents/Studia/3 semestr/Programowanie Obiektowe/github/Wojciech_Niedziela_PO/PROJEKT/MECHANICS/classTrasa.h" // Include this for the Trasa class
#include <cmath>
#include <iostream>
#include "classHandlingEvents.h"
#include "classDrawMap.h"

// Konstruktor klasy
classDraw::classDraw() {}

// Implementacja metody MagazynInput
void classDraw::MagazynInput(sf::Font &font, std::vector<std::string> &magazynInputBuffers, int &magazynStep, double &magazynX, double &magazynY)
{
    sf::RenderWindow inputWindow(sf::VideoMode(400, 300), "Wprowadz lokalizacje magazynu");

    sf::Text instructionText("Wprowadz wspolrzedne magazynu:", font, 20);
    instructionText.setPosition(20, 20);

    sf::Text xLabel("X:", font, 20);
    xLabel.setPosition(20, 80);

    sf::Text yLabel("Y:", font, 20);
    yLabel.setPosition(20, 140);

    sf::RectangleShape magazynInputField(sf::Vector2f(1, 30));
    magazynInputField.setFillColor(sf::Color::White);

    while (inputWindow.isOpen())
    {
        sf::Event event;
        while (inputWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                inputWindow.close();

            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128 && magazynStep < 2)
                {
                    char enteredChar = static_cast<char>(event.text.unicode);
                    if (enteredChar == 8) // Backspace
                    {
                        if (!magazynInputBuffers[magazynStep].empty())
                            magazynInputBuffers[magazynStep].pop_back();
                    }
                    else
                    {
                        magazynInputBuffers[magazynStep] += enteredChar;
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                if (magazynStep == 0)
                {
                    try
                    {
                        magazynX = std::stod(magazynInputBuffers[magazynStep]);
                    }
                    catch (const std::invalid_argument &e)
                    {
                        std::cout << "Blad. Ustawiono domyslna wartosc = 0" << std::endl;
                    }
                }
                else if (magazynStep == 1)
                {
                    try
                    {
                        magazynY = std::stod(magazynInputBuffers[magazynStep]);
                    }
                    catch (const std::invalid_argument &e)
                    {
                        std::cout << "Blad. Ustawiono domyslna wartosc = 0" << std::endl;
                    }

                    inputWindow.close();
                }
                magazynStep++;
                if (magazynStep > 1)
                    magazynStep = 1;
            }
        }

        inputWindow.clear();
        inputWindow.draw(instructionText);
        inputWindow.draw(xLabel);
        inputWindow.draw(yLabel);

        for (int i = 0; i < 2; ++i)
        {
            magazynInputField.setPosition(60, 80 + i * 60);
            inputWindow.draw(magazynInputField);

            sf::Text fieldText(magazynInputBuffers[i], font, 20);
            fieldText.setPosition(65, 80 + i * 60);
            inputWindow.draw(fieldText);
        }

        inputWindow.display();
    }
}

double classDraw::calculateDistance(double x1, double y1, double x2, double y2)
{
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void classDraw::MainWindow(sf::RenderWindow &window, sf::Font &font, std::vector<Paczka> &paczki, Magazyn &magazyn, Kurier &kurier, Mapa &mapa)
{
    auto uiElements = initializeUIElements(font); // Inicjalizacja interfejsu użytkownika
    std::vector<std::string> inputBuffers(5, ""); // Bufory wejściowe dla ID, Adres, Waga, X, Y
    int step = 0;                                 // Krok aktualnego pola wejściowego
    std::vector<std::string> routes;              // Lista tras
    bool showMap = false;
    classHandlingEvents classHandlingEvents;
    classDrawMap classDrawMap;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Obsługa zdarzeń
            // handleMainWindowEvents(event, window, paczki, magazyn, kurier, mapa, routes, showMap, uiElements, inputBuffers, step);
            classHandlingEvents.handleMainWindowEvents(event, window, paczki, magazyn, kurier, mapa, routes, showMap, uiElements, inputBuffers, step, currentRoute);
        }
        // Rysowanie głównego okna
        drawMainWindow(window, font, paczki, routes, uiElements);
        if (showMap)
        {
            // cout<<"rute w draw "<<currentRoute.size()<<endl;

            // Otwórz okno mapy
            // showMapWindow(font, paczki, magazyn, routes);
            classDrawMap.showMapWindow(font, paczki, magazyn, routes, currentRoute);
            showMap = false;
        }
    }
}

// Inicjalizacja elementów interfejsu użytkownika

UIElements classDraw::initializeUIElements(sf::Font &font)
{
    UIElements elements;

    elements.inputText.setFont(font);
    elements.inputText.setString("Wprowadz dane paczki:");
    elements.inputText.setPosition(100, 10);
    elements.inputText.setCharacterSize(20);

    // Dodaj etykiety
    std::vector<std::string> labels = {
        "ID Paczki:",
        "Adres:",
        "Waga (kg):",
        "Wspolrzedna X:",
        "Wspolrzedna Y:"};

    for (size_t i = 0; i < labels.size(); ++i)
    {
        sf::Text label;
        label.setFont(font);
        label.setString(labels[i]);
        label.setCharacterSize(20);
        label.setPosition(10, 50 + i * 40); // Pozycjonowanie obok pól
        elements.inputLabels.push_back(label);

        sf::RectangleShape inputField(sf::Vector2f(1, 30));
        inputField.setFillColor(sf::Color::White);
        inputField.setPosition(150, 50 + i * 40); // Pozycjonowanie pól
        elements.inputFields.push_back(inputField);

        sf::Text inputText;
        inputText.setFont(font);
        inputText.setCharacterSize(20);
        inputText.setPosition(155, 50 + i * 40); // Pozycjonowanie tekstu w polach
        elements.inputTexts.push_back(inputText);
    }

    // Przyciski
    auto buttonGenetic = createButton(sf::Vector2f(10, 250), "alg. genetyczny", sf::Color::Green, font);
    elements.buttons["calculateGenetic"] = buttonGenetic.first;
    elements.buttonTexts["calculateGenetic"] = buttonGenetic.second;

    auto buttonGreedy = createButton(sf::Vector2f(220, 250), "alg. zachlanny", sf::Color::Blue, font);
    elements.buttons["calculateGreedy"] = buttonGreedy.first;
    elements.buttonTexts["calculateGreedy"] = buttonGreedy.second;

    auto buttonAnnealing = createButton(sf::Vector2f(430, 250), "alg. wyzarzania", sf::Color::Cyan, font);
    elements.buttons["calculateAnnealing"] = buttonAnnealing.first;
    elements.buttonTexts["calculateAnnealing"] = buttonAnnealing.second;

    auto buttonMap = createButton(sf::Vector2f(640, 10), "MAPA", sf::Color::Magenta, font);
    elements.buttons["map"] = buttonMap.first;
    elements.buttonTexts["map"] = buttonMap.second;

    return elements;
}

std::pair<sf::RectangleShape, sf::Text> classDraw::createButton(const sf::Vector2f &position, const std::string &text, const sf::Color &color, sf::Font &font)
{
    sf::RectangleShape button(sf::Vector2f(200, 50));
    button.setPosition(position);
    button.setFillColor(color);

    sf::Text buttonText;
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(20);
    buttonText.setFillColor(sf::Color::White);
    buttonText.setPosition(position.x + 10, position.y + 10);

    return {button, buttonText};
}

// Rysowanie głównego okna

void classDraw::drawMainWindow(sf::RenderWindow &window, sf::Font &font, std::vector<Paczka> &paczki, std::vector<std::string> &routes, UIElements &uiElements)
{
    window.clear();
    drawUIElements(window, uiElements);

    // Rysowanie pól tekstowych i ich zawartości
    for (size_t i = 0; i < uiElements.inputFields.size(); ++i)
    {
        // Rysowanie prostokątów pól
        window.draw(uiElements.inputFields[i]);

        // Rysowanie tekstu w polach
        window.draw(uiElements.inputTexts[i]);
    }

    drawPackages(window, font, paczki); // Rysowanie paczek

    // Wyświetlanie tytułu sekcji tras
    sf::Text routeTitle("Wyznaczone trasy:", font, 20);
    routeTitle.setPosition(10, 500);
    window.draw(routeTitle);

    drawRoutes(window, font, routes); // Rysowanie tras

    window.display();
}

void classDraw::drawUIElements(sf::RenderWindow &window, UIElements &uiElements)
{
    // Rysowanie tytułu
    window.draw(uiElements.inputText);

    // Rysowanie etykiet i pól wejściowych
    for (size_t i = 0; i < uiElements.inputFields.size(); ++i)
    {
        window.draw(uiElements.inputLabels[i]); // Rysowanie etykiety
        window.draw(uiElements.inputFields[i]); // Rysowanie pola wejściowego
        window.draw(uiElements.inputTexts[i]);  // Rysowanie tekstu w polu
    }

    // Rysowanie przycisków i ich tekstów
    for (auto &[key, button] : uiElements.buttons)
    {
        window.draw(button);
        window.draw(uiElements.buttonTexts[key]);
    }
}

void classDraw::drawPackages(sf::RenderWindow &window, sf::Font &font, std::vector<Paczka> &paczki)
{
    sf::Text packageText("", font, 20);
    packageText.setPosition(10, 300);
    for (size_t i = 0; i < paczki.size(); ++i)
    {
        packageText.setString("Paczka " + std::to_string(paczki[i].getId()) + ": " + paczki[i].getAdres());
        packageText.move(0, 30); // Move down for each package
        window.draw(packageText);
    }
}

void classDraw::drawRoutes(sf::RenderWindow &window, sf::Font &font, std::vector<std::string> &routes)
{
    if (routes.empty())
    {
        return; // Jeśli brak tras, nic nie wyświetlaj
    }

    // Tytuł sekcji tras
    sf::Text routeTitle("Wyznaczone trasy:", font, 20);
    routeTitle.setPosition(10, 500);
    window.draw(routeTitle);

    // Pozycjonowanie początkowe dla listy tras
    sf::Text routeText("", font, 20);
    routeText.setPosition(10, 530); // Start poniżej tytułu

    // Iteracja przez kolejne trasy i wyświetlanie
    for (const auto &route : routes)
    {
        routeText.setString(route);
        window.draw(routeText);
        routeText.move(0, 30); // Przesunięcie w dół dla kolejnej linii
    }
}
