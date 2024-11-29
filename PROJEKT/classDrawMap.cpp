#include "classDrawMap.h"
#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cmath>

classDrawMap::classDrawMap() {}

void classDrawMap::showMapWindow(std::vector<Paczka> &paczki, Magazyn &magazyn, std::vector<std::string> &routes, std::vector<Paczka> &currentRoute) {
    while (true) {
        std::cout << "\n--- MENU MAPY ---\n";
        std::cout << "1. Wyświetl paczki na mapie\n";
        std::cout << "2. Dodaj paczkę wybierając punkt\n";
        std::cout << "3. Wyświetl trasy\n";
        std::cout << "4. Wróć do głównego menu\n";
        std::cout << "Wybierz opcję: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail() || choice < 1 || choice > 4) {
            std::cout << "Błąd: Wprowadź poprawny numer opcji.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                displayPackagesOnMap(paczki, magazyn);
                break;
            case 2:
                addPackageTerminal(paczki);
                break;
            case 3:
                displayRoutes(currentRoute, magazyn);
                break;
            case 4:
                return;
        }
    }
}

void classDrawMap::displayPackagesOnMap(const std::vector<Paczka> &paczki, const Magazyn &magazyn) {
    std::cout << "\n--- MAPA PACZEK ---\n";
    std::cout << "Magazyn: (" << magazyn.getX() << ", " << magazyn.getY() << ")\n";

    if (paczki.empty()) {
        std::cout << "Brak paczek do wyświetlenia.\n";
        return;
    }

    for (const auto &paczka : paczki) {
        std::cout << "ID: " << paczka.getId()
                  << ", Współrzędne: (" << paczka.getX() << ", " << paczka.getY() << ")\n";
    }
}

void classDrawMap::addPackageTerminal(std::vector<Paczka> &paczki) {
    int id;
    double x, y;

    std::cout << "\n--- DODAWANIE PACZKI ---\n";
    std::cout << "Podaj ID paczki: ";
    std::cin >> id;

    if (std::cin.fail()) {
        std::cout << "Błąd: Nieprawidłowe ID paczki.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    std::cout << "Podaj współrzędne paczki (X Y): ";
    std::cin >> x >> y;

    if (std::cin.fail()) {
        std::cout << "Błąd: Nieprawidłowe współrzędne paczki.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    paczki.emplace_back(id, "", 0.0, x, y);
    std::cout << "Dodano paczkę o ID " << id << " na współrzędnych (" << x << ", " << y << ").\n";
}

void classDrawMap::displayRoutes(const std::vector<Paczka> &route, const Magazyn &magazyn) {
    if (route.empty()) {
        std::cout << "Brak tras do wyświetlenia.\n";
        return;
    }

    std::cout << "\n--- TRASA ---\n";
    double totalDistance = 0.0;
    double prevX = magazyn.getX(), prevY = magazyn.getY();

    for (const auto &paczka : route) {
        double distance = calculateDistance(prevX, prevY, paczka.getX(), paczka.getY());
        std::cout << "Do paczki ID: " << paczka.getId()
                  << " na współrzędnych (" << paczka.getX() << ", " << paczka.getY() << ")"
                  << ", odległość: " << std::fixed << std::setprecision(2) << distance << "\n";

        totalDistance += distance;
        prevX = paczka.getX();
        prevY = paczka.getY();
    }

    double backDistance = calculateDistance(prevX, prevY, magazyn.getX(), magazyn.getY());
    totalDistance += backDistance;

    std::cout << "Powrót do magazynu, odległość: " << std::fixed << std::setprecision(2) << backDistance << "\n";
    std::cout << "Całkowita odległość: " << totalDistance << "\n";
}

double classDrawMap::calculateDistance(double x1, double y1, double x2, double y2) {
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
