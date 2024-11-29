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
classDrawMap::classDrawMap() {}

// Rysowanie mapy

void classDrawMap::showMapWindow(sf::Font &font, std::vector<Paczka> &paczki, Magazyn &magazyn, std::vector<std::string> &routes, std::vector<Paczka> &currentRoute)
{
    sf::VideoMode fullScreenMode = sf::VideoMode::getDesktopMode();
    sf::RenderWindow mapWindow(fullScreenMode, "Mapa paczek", sf::Style::Default);

    while (mapWindow.isOpen())
    {

        sf::Event event; // Deklaracja zmiennej event
        while (mapWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                mapWindow.close();
            }


            // Obsługa kliknięcia na mapę, gdy flaga selectingFromMap jest włączona
            if (selectingFromMap && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                // Oblicz współrzędne kliknięcia
                float clickedX = (event.mouseButton.x - mapWindow.getSize().x / 2) / 50.0f;
                float clickedY = (mapWindow.getSize().y / 2 - event.mouseButton.y) / 50.0f;

                // Otwórz okno szczegółów paczki
                addPackageDetails(font, paczki, clickedX, clickedY);

                //selectingFromMap = false; // Wyłącz tryb wybierania z mapy
                mapWindow.close();        // Zamknij mapę po wybraniu punktu
            }
        }

        // Rysowanie mapy
        mapWindow.clear(sf::Color::White);
        drawMapWindow(mapWindow, font, paczki, magazyn, routes);
        if (!currentRoute.empty())
        {
            //cout<<"Rysowanie aktualnej trasy"<<endl;
            drawRoutesOnMap(mapWindow, magazyn, currentRoute); // Rysowanie aktualnej trasy
        }
        mapWindow.display();
    }
}

void classDrawMap::addPackageDetails(sf::Font &font, std::vector<Paczka> &paczki, float x, float y)
{
    sf::RenderWindow detailWindow(sf::VideoMode(400, 300), "Dodaj szczegoly paczki");

    // Etykiety
    sf::Text idLabel("ID Paczki:", font, 20);
    idLabel.setPosition(20, 20);

    sf::Text adresLabel("Adres:", font, 20);
    adresLabel.setPosition(20, 80);

    sf::Text wagaLabel("Waga (kg):", font, 20);
    wagaLabel.setPosition(20, 140);

    sf::Text wspolrzedneLabel("Wspolrzedne: (" + std::to_string(x) + ", " + std::to_string(y) + ")", font, 20);
    wspolrzedneLabel.setPosition(20, 200);

    // Pola tekstowe
    std::vector<std::string> inputBuffers(2, ""); // Bufory dla ID i Adresu
    int step = 0;                                 // Aktualnie edytowane pole

    sf::RectangleShape inputField(sf::Vector2f(1, 30));
    inputField.setFillColor(sf::Color::White);
    inputField.setOutlineThickness(1);
    inputField.setOutlineColor(sf::Color::Black);

    while (detailWindow.isOpen())
    {
        sf::Event event;
        while (detailWindow.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                detailWindow.close();

            if (event.type == sf::Event::TextEntered && step < 2)
            {
                char enteredChar = static_cast<char>(event.text.unicode);
                if (enteredChar == '\b') // Backspace
                {
                    if (!inputBuffers[step].empty())
                        inputBuffers[step].pop_back();
                }
                else if (enteredChar >= 32 && enteredChar <= 126) // Printable characters
                {
                    inputBuffers[step] += enteredChar;
                }
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
            {
                if (step == 1) // Ostatnie pole
                {
                    // Dodaj paczkę do listy
                    int paczkaId = std::stoi(inputBuffers[0]);
                    std::string adres = inputBuffers[1];
                    double waga = 0.0; // Default weight (can be changed)
                    paczki.emplace_back(paczkaId, adres, waga, x, y);

                    detailWindow.close(); // Zamknij okno po dodaniu paczki
                }
                else
                {
                    step++;
                }
            }
        }

        detailWindow.clear();

        // Rysowanie etykiet
        detailWindow.draw(idLabel);
        detailWindow.draw(adresLabel);
        detailWindow.draw(wagaLabel);
        detailWindow.draw(wspolrzedneLabel);

        // Rysowanie pola aktywnego
        inputField.setPosition(130, 20 + step * 60);
        detailWindow.draw(inputField);

        // Wyświetlanie tekstu w polach
        for (int i = 0; i < 2; ++i)
        {
            sf::Text fieldText(inputBuffers[i], font, 20);
            fieldText.setPosition(130, 25 + i * 60);
            fieldText.setFillColor(sf::Color::White);
            detailWindow.draw(fieldText);
        }

        detailWindow.display();
    }
}

void classDrawMap::drawMapWindow(sf::RenderWindow &mapWindow, sf::Font &font, std::vector<Paczka> &paczki, Magazyn &magazyn, std::vector<std::string> &routes)
{
    mapWindow.clear(sf::Color::White);

    drawGrid(mapWindow);
    drawAxis(mapWindow);
    drawPackagesOnMap(mapWindow, font, paczki);
    drawWarehouse(mapWindow, font, magazyn);
}

void classDrawMap::drawGrid(sf::RenderWindow &mapWindow)
{
    sf::VertexArray gridLines(sf::Lines);
    float gridSize = 50.0f;
    float centerX = mapWindow.getSize().x / 2.0f;
    float centerY = mapWindow.getSize().y / 2.0f;

    // Rysowanie poziomych linii (Y)
    for (float y = centerY; y >= 0; y -= gridSize)
    {
        gridLines.append(sf::Vertex(sf::Vector2f(0, y), sf::Color(200, 200, 200)));
        gridLines.append(sf::Vertex(sf::Vector2f(mapWindow.getSize().x, y), sf::Color(200, 200, 200)));
    }
    for (float y = centerY; y <= mapWindow.getSize().y; y += gridSize)
    {
        gridLines.append(sf::Vertex(sf::Vector2f(0, y), sf::Color(200, 200, 200)));
        gridLines.append(sf::Vertex(sf::Vector2f(mapWindow.getSize().x, y), sf::Color(200, 200, 200)));
    }

    // Rysowanie pionowych linii (X)
    for (float x = centerX; x >= 0; x -= gridSize)
    {
        gridLines.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color(200, 200, 200)));
        gridLines.append(sf::Vertex(sf::Vector2f(x, mapWindow.getSize().y), sf::Color(200, 200, 200)));
    }
    for (float x = centerX; x <= mapWindow.getSize().x; x += gridSize)
    {
        gridLines.append(sf::Vertex(sf::Vector2f(x, 0), sf::Color(200, 200, 200)));
        gridLines.append(sf::Vertex(sf::Vector2f(x, mapWindow.getSize().y), sf::Color(200, 200, 200)));
    }

    // Rysowanie siatki na oknie
    mapWindow.draw(gridLines);
}

void classDrawMap::drawAxis(sf::RenderWindow &mapWindow)
{
    // Example implementation for drawing axes
    sf::VertexArray axes(sf::Lines, 4);
    float centerX = mapWindow.getSize().x / 2;
    float centerY = mapWindow.getSize().y / 2;

    axes[0].position = sf::Vector2f(centerX, 0); // Y-axis
    axes[1].position = sf::Vector2f(centerX, mapWindow.getSize().y);
    axes[2].position = sf::Vector2f(0, centerY); // X-axis
    axes[3].position = sf::Vector2f(mapWindow.getSize().x, centerY);

    axes[0].color = axes[1].color = axes[2].color = axes[3].color = sf::Color::Black;

    mapWindow.draw(axes);
}

void classDrawMap::drawPackagesOnMap(sf::RenderWindow &mapWindow, sf::Font &font, std::vector<Paczka> &paczki)
{
    // Example implementation for drawing packages
    for (const auto &paczka : paczki)
    {
        sf::CircleShape point(5);
        point.setFillColor(sf::Color::Red);

        // Convert package coordinates to window coordinates
        float pixelX = paczka.getX() * 50 + mapWindow.getSize().x / 2;
        float pixelY = mapWindow.getSize().y / 2 - paczka.getY() * 50;

        point.setPosition(pixelX - 5, pixelY - 5); // Adjust for circle center
        mapWindow.draw(point);

        // Draw package ID
        sf::Text idText(std::to_string(paczka.getId()), font, 15);
        idText.setPosition(pixelX + 10, pixelY - 10);
        idText.setFillColor(sf::Color::Black);
        mapWindow.draw(idText);
    }
}

void classDrawMap::drawWarehouse(sf::RenderWindow &mapWindow, sf::Font &font, Magazyn &magazyn)
{
    // Example implementation for drawing the warehouse
    sf::CircleShape warehousePoint(10);
    warehousePoint.setFillColor(sf::Color::Green);

    // Convert warehouse coordinates to window coordinates
    float warehouseX = magazyn.getX() * 50 + mapWindow.getSize().x / 2;
    float warehouseY = mapWindow.getSize().y / 2 - magazyn.getY() * 50;

    warehousePoint.setPosition(warehouseX - 10, warehouseY - 10); // Adjust for circle center
    mapWindow.draw(warehousePoint);

    // Draw warehouse label
    sf::Text warehouseText("Magazyn", font, 15);
    warehouseText.setPosition(warehouseX + 15, warehouseY - 10);
    warehouseText.setFillColor(sf::Color::Black);
    mapWindow.draw(warehouseText);
}

void classDrawMap::drawRoutesOnMap(sf::RenderWindow &mapWindow, Magazyn &magazyn, const std::vector<Paczka> &optimalRoute)
{
    if (optimalRoute.empty())
        return; // Jeśli brak punktów w trasie, nic nie rysujemy

    sf::VertexArray routeLines(sf::LinesStrip);

    // Start od magazynu
    float prevX = magazyn.getX();
    float prevY = magazyn.getY();

    // Dodaj magazyn jako pierwszy punkt trasy
    float startX = prevX * 50 + mapWindow.getSize().x / 2.0f;
    float startY = mapWindow.getSize().y / 2.0f - prevY * 50;
    routeLines.append(sf::Vertex(sf::Vector2f(startX, startY), sf::Color::Blue));

    // Iteracja przez punkty w trasie
    for (const auto &paczka : optimalRoute)
    {
        // Oblicz współrzędne kolejnego punktu
        float pixelX = paczka.getX() * 50 + mapWindow.getSize().x / 2.0f;
        float pixelY = mapWindow.getSize().y / 2.0f - paczka.getY() * 50;

        // Dodaj punkt do linii
        routeLines.append(sf::Vertex(sf::Vector2f(pixelX, pixelY), sf::Color::Blue));
    }

    // Dodaj powrót do magazynu
    float endX = magazyn.getX() * 50 + mapWindow.getSize().x / 2.0f;
    float endY = mapWindow.getSize().y / 2.0f - magazyn.getY() * 50;
    routeLines.append(sf::Vertex(sf::Vector2f(endX, endY), sf::Color::Blue));

    // Rysowanie linii
    mapWindow.draw(routeLines);
}
