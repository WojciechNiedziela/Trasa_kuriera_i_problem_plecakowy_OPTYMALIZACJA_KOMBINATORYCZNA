#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iomanip>

struct Paczka {
    int id;
    std::string adres;
    double waga;
    double x;
    double y;
    std::string data;
};

// Funkcja generująca datę w formacie "YYYY-MM-DD" w zakresie od dzisiejszego dnia do 14 dni w przód
std::string generujDate(int przesuniecie) {
    std::time_t teraz = std::time(nullptr);
    teraz += przesuniecie * 24 * 60 * 60; // Przesunięcie o `przesuniecie` dni
    std::tm* lokalnyCzas = std::localtime(&teraz);

    std::ostringstream dataStream;
    dataStream << (1900 + lokalnyCzas->tm_year) << "-"
               << std::setw(2) << std::setfill('0') << (1 + lokalnyCzas->tm_mon) << "-"
               << std::setw(2) << std::setfill('0') << lokalnyCzas->tm_mday;

    return dataStream.str();
}

void generujDanePaczki(std::vector<Paczka> &paczki, int liczbaPaczek) {
    const double maxWaga = 50.0; // Maksymalna waga paczki
    const double maxX = 100.0;  // Maksymalna współrzędna X
    const double maxY = 100.0;  // Maksymalna współrzędna Y

    for (int i = 0; i < liczbaPaczek; ++i) {
        Paczka paczka;
        paczka.id = i + 1;
        paczka.waga = (std::rand() % static_cast<int>(maxWaga * 100)) / 100.0; // Waga z dokładnością do dwóch miejsc po przecinku
        paczka.x = (std::rand() % static_cast<int>(maxX * 100)) / 100.0;
        paczka.y = (std::rand() % static_cast<int>(maxY * 100)) / 100.0;

        // Generowanie daty z losowym przesunięciem od 0 do 14 dni
        int przesuniecie = std::rand() % 15; // Liczba dni od dzisiaj do 14 dni w przód
        paczka.data = generujDate(przesuniecie);

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
             << paczka.y << " "
             << paczka.data << "\n";
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
