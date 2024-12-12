#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iomanip>
#include <filesystem>

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

void generujDanePaczki(std::vector<Paczka>& paczki, int liczbaPaczek, double maxWaga, double maxX, double maxY) {
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

void zapiszDaneDoPliku(const std::vector<Paczka>& paczki, const std::string& folder, int liczbaPaczek, double maxX, double maxY, double maxWaga) {
    // Tworzenie nazwy pliku
    std::ostringstream nazwaPlikuStream;
    nazwaPlikuStream << folder << "/paczki_" << liczbaPaczek << "_" << maxX << "x" << maxY << "_" << maxWaga << ".txt";
    std::string nazwaPliku = nazwaPlikuStream.str();

    // Tworzenie folderu, jeśli nie istnieje
    std::filesystem::create_directories(folder);

    std::ofstream plik(nazwaPliku);
    if (!plik) {
        std::cerr << "Nie można otworzyć pliku do zapisu: " << nazwaPliku << std::endl;
        return;
    }

    for (const auto& paczka : paczki) {
        plik << paczka.id << " "
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

    // Konfiguracja kroków i maksymalnych wartości
    const int liczbaPaczekStart = 100, liczbaPaczekMax = 1000, liczbaPaczekKrok = 100;
    const double maxWagaStart = 10.0, maxWagaMax = 100.0, maxWagaKrok = 10.0;
    const double maxXStart = 50.0, maxXMax = 150.0, maxXKrok = 10.0;
    const double maxYStart = 50.0, maxYMax = 150.0, maxYKrok = 10.0;
    const std::string folder = "data-duzo-paczek"; // Folder do zapisu danych

    for (int liczbaPaczek = liczbaPaczekStart; liczbaPaczek <= liczbaPaczekMax; liczbaPaczek += liczbaPaczekKrok) {
        for (double maxWaga = maxWagaStart; maxWaga <= maxWagaMax; maxWaga += maxWagaKrok) {
            for (double maxX = maxXStart; maxX <= maxXMax; maxX += maxXKrok) {
                for (double maxY = maxYStart; maxY <= maxYMax; maxY += maxYKrok) {
                    std::vector<Paczka> paczki;
                    generujDanePaczki(paczki, liczbaPaczek, maxWaga, maxX, maxY);
                    zapiszDaneDoPliku(paczki, folder, liczbaPaczek, maxX, maxY, maxWaga);
                }
            }
        }
    }

    return 0;
}
