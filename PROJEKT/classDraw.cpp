#include "classDraw.h"
#include "classPaczka.h"
#include "classMagazyn.h"
#include "classKurier.h"
#include "classMapa.h"
#include "classTrasa.h"
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
#include <cmath>
#include "classHandlingEvents.h"
#include <fstream>

classDraw::classDraw() {}

// void classDraw::MagazynInput(double &magazynX, double &magazynY) {
//     std::cout << "Podaj wspolrzedne magazynu (X Y): ";
//     while (true) {
//         std::cin >> magazynX >> magazynY;
//         if (!std::cin.fail()) {
//             break;
//         }
//         std::cout << "Blad: wprowadz poprawne liczby.\n";
//         std::cin.clear();
//         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//     }
//     std::cout << "Magazyn ustawiono na wspolrzedne (" << magazynX << ", " << magazynY << ").\n";
// }

// Funkcja do ustawienia liczby kurierów
void classDraw::SetCouriers(std::vector<Kurier> &kurierzy) {
    int liczbaKurierow;
    std::cout << "Podaj liczbe kurierow: ";
    std::cin >> liczbaKurierow;

    if (liczbaKurierow <= 0) {
        std::cout << "Blad: liczba kurierow musi byc wieksza od 0.\n";
        return;
    }

    kurierzy.clear(); // Wyczyść listę kurierów
    for (int i = 1; i <= liczbaKurierow; ++i) {
        int id;
        double ladownosc;

        std::cout << "Podaj id kuriera " << i << ": ";
        std::cin >> id;
        std::cout << "Podaj ladownosc kuriera " << i << ": ";
        std::cin >> ladownosc;

        kurierzy.emplace_back(id, ladownosc);
    }

    std::cout << "Zapisano " << liczbaKurierow << " kurierow.\n";
}

// Funkcja do wyświetlenia szczegółów kurierów
void classDraw::DisplayCouriers(const std::vector<Kurier> &kurierzy) {
    if (kurierzy.empty()) {
        std::cout << "Brak zarejestrowanych kurierow.\n";
        return;
    }

    std::cout << "\n--- Lista Kurierow ---\n";
    for (const auto &kurier : kurierzy) {
        std::cout << "Id: " << kurier.getId()
                  << ", Ladownosc: " << kurier.getLadownosc() << " kg\n";
    }
}


// Funkcja do wczytywania paczek z pliku
void classDraw::LoadPackagesFromFile(std::vector<Paczka> &paczki, const std::string &fileName)
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cout << "Nie mozna otworzyc pliku: " << fileName << "\n";
        return;
    }

    int id;
    double waga, x, y;
    while (file >> id >> waga >> x >> y)
    {
        paczki.emplace_back(id, waga, x, y);
    }

    file.close();
    std::cout << "Wczytano paczki z pliku: " << fileName << "\n";
}

// Zaktualizowane menu
void classDraw::MainWindow(std::vector<Paczka> &paczki, Magazyn &magazyn, std::vector<Kurier> &kurierzy, Mapa &mapa)
{
    std::vector<Kurier*> kurierzyPtr; // Tymczasowy wektor wskaźników
    for (Kurier &kurier : kurierzy) {
        kurierzyPtr.push_back(&kurier); // Dodaj wskaźnik do każdego kuriera
    }

    classHandlingEvents classHandlingEvents;
    double magazynX = 0.0, magazynY = 0.0;

    //    MagazynInput(magazynX, magazynY);

     int choice;
    while (true) {
        std::cout << "\n--- MENU GLOWNE ---\n";
        std::cout << "1. Dodaj paczke\n";
        std::cout << "2. Wczytaj paczki z pliku\n";
        std::cout << "3. Wyswietl paczki\n";
        std::cout << "4. Ustaw kurierow\n";
        std::cout << "5. Wyswietl kurierow\n";
        std::cout << "6. Wyznacz trasy (algorytm genetyczny)\n";
        std::cout << "7. Wyznacz trasy (algorytm zachlanny)\n";
        std::cout << "8. Wyznacz trasy (wyzarzanie)\n";
        std::cout << "9. Wyjdz\n";
        std::cout << "Wybierz opcje: ";
        std::cin >> choice;

        if (std::cin.fail() || choice < 1 || choice > 9) {
            std::cout << "Blad: podaj poprawny numer opcji.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                AddPackage(paczki);
                break;
            }
            case 2: {
                std::string fileName = "paczki.txt";
                LoadPackagesFromFile(paczki, fileName);
                break;
            }
            case 3: {
                DisplayPackages(paczki);
                break;
            }
            case 4: {
                SetCouriers(kurierzy);
                break;
            }
            case 5: {
                DisplayCouriers(kurierzy);
                break;
            }
            case 6: {
                if (!kurierzy.empty()) {
                    DisplayRoutes(paczki, magazyn, kurierzy, mapa, "genetyczny");
                } else {
                    std::cout << "Brak zarejestrowanych kurierow. Ustaw kurierow najpierw.\n";
                }
                break;
            }
            case 7: {
                if (!kurierzy.empty()) {
                    DisplayRoutes(paczki, magazyn, kurierzy, mapa, "zachlanny");
                } else {
                    std::cout << "Brak zarejestrowanych kurierow. Ustaw kurierow najpierw.\n";
                }
                break;
            }
            case 8: {
                if (!kurierzy.empty()) {
                    DisplayRoutes(paczki, magazyn, kurierzy, mapa, "wyzarzanie");
                } else {
                    std::cout << "Brak zarejestrowanych kurierow. Ustaw kurierow najpierw.\n";
                }
                break;
            }
            case 9: {
                std::cout << "Wyjscie z programu.\n";
                return;
            }
        }
    }
}

void classDraw::AddPackage(std::vector<Paczka> &paczki)
{
    int id;
    std::string adres;
    double waga, x, y;

    std::cout << "Podaj ID paczki: ";
    std::cin >> id;
    // std::cout << "Podaj adres paczki: ";
    // std::cin.ignore();
    // std::getline(std::cin, adres);
    std::cout << "Podaj wage paczki (kg): ";
    std::cin >> waga;
    std::cout << "Podaj wspolrzedne paczki (X Y): ";
    std::cin >> x >> y;

    if (!std::cin.fail())
    {
        paczki.emplace_back(id, waga, x, y);
        std::cout << "Paczka dodana pomyslnie.\n";
    }
    else
    {
        std::cout << "Blad wprowadzania danych.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

void classDraw::DisplayPackages(const std::vector<Paczka> &paczki)
{
    if (paczki.empty())
    {
        std::cout << "Brak paczek do wyswietlenia.\n";
        return;
    }
    std::cout << "\n--- Lista Paczek ---\n";
    for (const auto &paczka : paczki)
    {
        std::cout << "ID: " << paczka.getId()
                  //<< ", Adres: " << paczka.getAdres()
                  << ", Waga: " << paczka.getWaga()
                  << ", Wspolrzedne: (" << paczka.getX() << ", " << paczka.getY() << ")\n";
    }
}

void classDraw::DisplayRoutes(std::vector<Paczka> &paczki, Magazyn &magazyn, std::vector<Kurier> &kurierzy, Mapa &mapa, const std::string &algorithm)
{
    if (paczki.empty())
    {
        std::cout << "Brak paczek do dostarczenia.\n";
        return;
    }

    Trasa trasa(kurierzy, &magazyn, paczki, &mapa);
    std::vector<std::vector<Paczka>> routes;

    if (algorithm == "genetyczny")
    {
        routes = trasa.znajdzTraseAlgorytmGenetyczny();
    }
    else if (algorithm == "zachlanny")
    {
        routes = trasa.znajdzTraseAlgorytmZachlanny();
    }
    else if (algorithm == "wyzarzanie")
    {
        routes = trasa.znajdzTraseAlgorytmWyzarzania();
    }
    else
    {
        std::cout << "Nieznany algorytm: " << algorithm << "\n";
        return;
    }

    // Wyświetlanie tras dla każdego kuriera
    for (size_t i = 0; i < routes.size(); ++i) {
        std::cout << "\n--- Trasa kuriera " << (i + 1) << " ---\n";
        double totalDistance = 0.0;
        double prevX = magazyn.getX(), prevY = magazyn.getY();

        for (const auto &paczka : routes[i]) {
            double distance = calculateDistance(prevX, prevY, paczka.getX(), paczka.getY());
            if (paczka.getId() == -1) {
                std::cout << "Powrot do magazynu, Dystans: " << std::fixed << std::setprecision(2) << distance << "\n";
            } else {
                std::cout << "Dostawa ID: " << paczka.getId()
                          << ", Dystans: " << std::fixed << std::setprecision(2) << distance << "\n";
            }
            totalDistance += distance;
            prevX = paczka.getX();
            prevY = paczka.getY();
        }

        totalDistance += calculateDistance(prevX, prevY, magazyn.getX(), magazyn.getY());
        std::cout << "Calkowity Dystans: " << std::fixed << std::setprecision(2) << totalDistance << "\n";
    }
}

double classDraw::calculateDistance(double x1, double y1, double x2, double y2)
{
    return std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
