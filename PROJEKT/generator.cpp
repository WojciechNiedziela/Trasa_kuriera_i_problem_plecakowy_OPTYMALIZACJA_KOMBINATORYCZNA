#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

struct Paczka {
    int id;
    std::string adres;
    double waga;
    double x;
    double y;
};

void generujDanePaczki(std::vector<Paczka> &paczki, int liczbaPaczek) {
    const double maxWaga = 50.0; // Maksymalna waga paczki
    const double maxX = 100.0;  // Maksymalna współrzędna Xgit 
    const double maxY = 100.0;  // Maksymalna współrzędna Y

    for (int i = 0; i < liczbaPaczek; ++i) {
        Paczka paczka;
        paczka.id = i + 1;
        paczka.waga = (std::rand() % static_cast<int>(maxWaga * 100)) / 100.0; // Waga z dokładnością do dwóch miejsc po przecinku
        paczka.x = (std::rand() % static_cast<int>(maxX * 100)) / 100.0;
        paczka.y = (std::rand() % static_cast<int>(maxY * 100)) / 100.0;

        paczki.push_back(paczka);
    }
}

void zapiszDaneDoPliku(const std::vector<Paczka> &paczki, const std::string &nazwaPliku) {
    std::ofstream plik(nazwaPliku);
    if (!plik) {
        std::cerr << "Nie można otworzyć pliku do zapisu: " << nazwaPliku << std::endl;
        return;
    }

    for (const auto &paczka : paczki) {
        plik << paczka.id << " "
             //<< paczka.adres << " "
             << paczka.waga << " "
             << paczka.x << " "
             << paczka.y << "\n";
    }

    plik.close();
    std::cout << "Dane paczek zapisano do pliku: " << nazwaPliku << std::endl;
}

int main() {
    std::srand(std::time(nullptr)); // Inicjalizacja generatora liczb losowych
    int liczbaPaczek;
    std::cout << "Podaj liczbe paczek do wygenerowania: ";
    std::cin >> liczbaPaczek;

    if (liczbaPaczek <= 0) {
        std::cerr << "Liczba paczek musi być większa od 0.\n";
        return 1;
    }

    std::vector<Paczka> paczki;
    generujDanePaczki(paczki, liczbaPaczek);
    zapiszDaneDoPliku(paczki, "paczki.txt");

    return 0;
}
