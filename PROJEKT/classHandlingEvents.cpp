#include "classHandlingEvents.h"
#include "classPaczka.h"
#include "classMagazyn.h"
#include "classKurier.h"
#include "classMapa.h"
#include "classTrasa.h"
#include <cmath>
#include <iostream>
#include <iomanip>

// Konstruktor klasy
classHandlingEvents::classHandlingEvents() {}

double classHandlingEvents::calculateDistance(double x1, double y1, double x2, double y2) {
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}

void classHandlingEvents::handleMainWindow(std::vector<Paczka> &paczki, Magazyn &magazyn, Kurier &kurier, Mapa &mapa) {
    int choice;
    std::vector<std::string> routes; // Przechowuje wygenerowane trasy

    while (true) {
        std::cout << "\n--- MENU GLOWNE ---\n";
        std::cout << "1. Dodaj paczke\n";
        std::cout << "2. Wyswietl paczki\n";
        std::cout << "3. Wyznacz trasy (algorytm genetyczny)\n";
        std::cout << "4. Wyznacz trasy (algorytm zachlanny)\n";
        std::cout << "5. Wyznacz trasy (algorytm wyzarzania)\n";
        std::cout << "6. Wyjdz\n";
        std::cout << "Wybierz opcje: ";
        std::cin >> choice;

        if (std::cin.fail() || choice < 1 || choice > 6) {
            std::cout << "Blad: podaj poprawny numer opcji.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                addPackage(paczki);
                break;
            case 2:
                displayPackages(paczki);
                break;
            case 3:
                displayRoutes(paczki, magazyn, kurier, mapa, routes, "genetyczny");
                break;
            case 4:
                displayRoutes(paczki, magazyn, kurier, mapa, routes, "zachlanny");
                break;
            case 5:
                displayRoutes(paczki, magazyn, kurier, mapa, routes, "wyzarzanie");
                break;
            case 6:
                std::cout << "Wyjscie z programu.\n";
                return;
        }
    }
}

void classHandlingEvents::addPackage(std::vector<Paczka> &paczki) {
    int id;
    std::string adres;
    double waga, x, y;

    std::cout << "Podaj ID paczki: ";
    std::cin >> id;

    std::cout << "Podaj adres paczki: ";
    std::cin.ignore();
    std::getline(std::cin, adres);

    std::cout << "Podaj wage paczki (kg): ";
    std::cin >> waga;

    std::cout << "Podaj wspolrzedne paczki (X Y): ";
    std::cin >> x >> y;

    if (!std::cin.fail()) {
        paczki.emplace_back(id, waga, x, y);
        std::cout << "Paczka dodana pomyslnie.\n";
    } else {
        std::cout << "Blad wprowadzania danych. Sprobuj ponownie.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void classHandlingEvents::displayPackages(const std::vector<Paczka> &paczki) {
    if (paczki.empty()) {
        std::cout << "Brak paczek do wyswietlenia.\n";
        return;
    }

    std::cout << "\n--- Lista Paczek ---\n";
    for (const auto &paczka : paczki) {
        std::cout << "ID: " << paczka.getId()
                  //<< ", Adres: " << paczka.getAdres()
                  << ", Waga: " << paczka.getWaga()
                  << ", Wspolrzedne: (" << paczka.getX() << ", " << paczka.getY() << ")\n";
    }
}

void classHandlingEvents::displayRoutes(std::vector<Paczka> &paczki, Magazyn &magazyn, Kurier &kurier, Mapa &mapa,
                                        std::vector<std::string> &routes, const std::string &algorithm) {
    if (paczki.empty()) {
        std::cout << "Brak paczek do dostarczenia.\n";
        return;
    }

    Trasa trasa(&kurier, &magazyn, paczki, &mapa);
    std::vector<Paczka> route;

    if (algorithm == "genetyczny") {
        route = trasa.znajdzTraseAlgorytmGenetyczny();
    } else if (algorithm == "zachlanny") {
        route = trasa.znajdzTraseAlgorytmZachlanny();
    } else if (algorithm == "wyzarzanie") {
        route = trasa.znajdzTraseWyzarzanie();
    } else {
        std::cout << "Nieznany algorytm: " << algorithm << "\n";
        return;
    }

    std::cout << "\n--- Trasa Wyznaczona (" << algorithm << ") ---\n";
    double totalDistance = 0.0;
    double prevX = magazyn.getX(), prevY = magazyn.getY();

    for (const auto &paczka : route) {
        double distance = calculateDistance(prevX, prevY, paczka.getX(), paczka.getY());
        std::cout << "Dostawa ID: " << paczka.getId()
                  //<< ", Adres: " << paczka.getAdres()
                  << ", Dystans: " << std::fixed << std::setprecision(2) << distance << "\n";
        totalDistance += distance;
        prevX = paczka.getX();
        prevY = paczka.getY();
    }

    totalDistance += calculateDistance(prevX, prevY, magazyn.getX(), magazyn.getY());
    std::cout << "Powrot do magazynu, Dystans: " << std::fixed << std::setprecision(2) << totalDistance << "\n";
}
