#include "classTrasa.h"
#include <iostream>
#include <cmath>
#include <random>
#include <limits>
#include <algorithm>
#include <vector>

using namespace std;

// Konstruktor
Trasa::Trasa(Kurier* kurier, Magazyn* magazyn, const vector<Paczka>& paczki, Mapa* mapa) {
    this->kurier = kurier;
    this->magazyn = magazyn;
    this->paczki = paczki;
    this->mapa = mapa;
}

// Dynamiczny problem plecakowy (DPP)
vector<Paczka> Trasa::rozwiazProblemPlecakowy(const vector<Paczka>& dostepnePaczki, double ladownosc) {
    int n = dostepnePaczki.size();
    vector<vector<double>> dp(n + 1, vector<double>(static_cast<int>(ladownosc) + 1, 0));
    vector<vector<bool>> wybory(n + 1, vector<bool>(static_cast<int>(ladownosc) + 1, false));

    for (int i = 1; i <= n; ++i) {
        for (int w = 1; w <= static_cast<int>(ladownosc); ++w) {
            double wagaPaczki = dostepnePaczki[i - 1].getWaga();
            double koszt = mapa->odleglosc(magazyn->getX(), magazyn->getY(), dostepnePaczki[i - 1].getX(), dostepnePaczki[i - 1].getY());

            if (wagaPaczki <= w) {
                double bezTego = dp[i - 1][w];
                double zTym = koszt + dp[i - 1][w - static_cast<int>(wagaPaczki)];
                if (zTym > bezTego) {
                    dp[i][w] = zTym;
                    wybory[i][w] = true;
                } else {
                    dp[i][w] = bezTego;
                }
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    vector<Paczka> wybranePaczki;
    int w = static_cast<int>(ladownosc);
    for (int i = n; i > 0; --i) {
        if (wybory[i][w]) {
            wybranePaczki.push_back(dostepnePaczki[i - 1]);
            w -= static_cast<int>(dostepnePaczki[i - 1].getWaga());
        }
    }

    return wybranePaczki;
}


// Oblicz długość trasy
double Trasa::obliczDlugoscTrasy(const vector<Paczka>& trasa) const {
    double dlugosc = 0.0;
    double x = magazyn->getX();
    double y = magazyn->getY();

    for (const auto& paczka : trasa) {
        dlugosc += mapa->odleglosc(x, y, paczka.getX(), paczka.getY());
        x = paczka.getX();
        y = paczka.getY();
    }

    dlugosc += mapa->odleglosc(x, y, magazyn->getX(), magazyn->getY());
    return dlugosc;
}

vector<Paczka> Trasa::znajdzTraseAlgorytmZachlanny() {
    vector<Paczka> wynikowaTrasa;                 // Wynikowy wektor paczek w trasie
    vector<Paczka> paczkiDoDostarczenia = paczki; // Kopia paczek do dostarczenia

    while (!paczkiDoDostarczenia.empty()) {
        // Rozwiąż problem plecakowy dla aktualnych paczek
        vector<Paczka> aktualnaTrasa = rozwiazProblemPlecakowy(paczkiDoDostarczenia, kurier->getLadownosc());

        // Obsługa przypadku, gdy żadna paczka nie zmieściła się w ładowności
        if (aktualnaTrasa.empty()) {
            std::cerr << "Brak paczek możliwych do dostarczenia – zakończenie trasy.\n";
            break;
        }

        // Dodaj magazyn jako początek trasy
        wynikowaTrasa.emplace_back(Paczka(-1, 0.0, magazyn->getX(), magazyn->getY()));

        // Dodaj wybrane paczki do wynikowej trasy
        wynikowaTrasa.insert(wynikowaTrasa.end(), aktualnaTrasa.begin(), aktualnaTrasa.end());

        // Dodaj magazyn jako koniec tej części trasy
        wynikowaTrasa.emplace_back(Paczka(-1, 0.0, magazyn->getX(), magazyn->getY()));

        // Usuń paczki dostarczone w tej iteracji
        for (const auto& paczka : aktualnaTrasa) {
            auto it = std::find(paczkiDoDostarczenia.begin(), paczkiDoDostarczenia.end(), paczka);
            if (it != paczkiDoDostarczenia.end()) {
                paczkiDoDostarczenia.erase(it);
            }
        }
    }

    return wynikowaTrasa; // Zwróć jedną trasę uwzględniającą wszystkie paczki
}








// Algorytm genetyczny z uwzględnieniem ładowności
// vector<Paczka> Trasa::znajdzTraseAlgorytmGenetyczny(int rozmiarPopulacji, int liczbaPokolen) {
//     vector<Paczka> wybranePaczki = rozwiazProblemPlecakowy(kurier->getLadownosc());
//     vector<vector<Paczka>> populacja(rozmiarPopulacji);

//     random_device rd;
//     mt19937 generator(rd());

//     for (int i = 0; i < rozmiarPopulacji; ++i) {
//         vector<Paczka> trasa = wybranePaczki;
//         shuffle(trasa.begin(), trasa.end(), generator);
//         populacja[i] = trasa;
//     }

//     for (int pokolenie = 0; pokolenie < liczbaPokolen; ++pokolenie) {
//         sort(populacja.begin(), populacja.end(), [this](const vector<Paczka>& a, const vector<Paczka>& b) {
//             return obliczDlugoscTrasy(a) < obliczDlugoscTrasy(b);
//         });

//         vector<vector<Paczka>> nowaPopulacja(populacja.begin(), populacja.begin() + rozmiarPopulacji / 2);

//         while (nowaPopulacja.size() < rozmiarPopulacji) {
//             int i1 = rand() % (rozmiarPopulacji / 2);
//             int i2 = rand() % (rozmiarPopulacji / 2);
//             vector<Paczka> dziecko = krzyzowanie(populacja[i1], populacja[i2]);

//             if (rand() % 100 < 10) {
//                 mutacja(dziecko);
//             }

//             nowaPopulacja.push_back(dziecko);
//         }

//         populacja = nowaPopulacja;
//     }

//     // Zwracamy najlepszą trasę z populacji
//     return populacja[0];
// }

// // Algorytm wyżarzania z uwzględnieniem ładowności
// vector<Paczka> Trasa::znajdzTraseAlgorytmWyzarzania() {
//     vector<Paczka> wybranePaczki = rozwiazProblemPlecakowy(kurier->getLadownosc());
//     vector<Paczka> optymalnaTrasa = wybranePaczki;
//     double temperatura = 100.0;
//     double wspolczynnikChlodzenia = 0.999;
//     double minimalnaTemperatura = 0.0001;

//     while (temperatura > minimalnaTemperatura) {
//         int i = rand() % optymalnaTrasa.size();
//         int j = rand() % optymalnaTrasa.size();

//         double aktualnaDlugosc = obliczDlugoscTrasy(optymalnaTrasa);
//         swap(optymalnaTrasa[i], optymalnaTrasa[j]);
//         double nowaDlugosc = obliczDlugoscTrasy(optymalnaTrasa);

//         double prawdopodobienstwo = exp((aktualnaDlugosc - nowaDlugosc) / temperatura);
//         if (prawdopodobienstwo < static_cast<double>(rand()) / RAND_MAX) {
//             swap(optymalnaTrasa[i], optymalnaTrasa[j]);
//         }

//         temperatura *= wspolczynnikChlodzenia;
//     }

//     return optymalnaTrasa;
// }

// // Wyświetlanie tras
// void Trasa::displayTrasy(const vector<vector<Paczka>>& trasy) {
//     for (size_t i = 0; i < trasy.size(); ++i) {
//         cout << "Trasa " << i + 1 << ":\n";
//         for (const auto& paczka : trasy[i]) {
//             cout << "- Paczka ID: " << paczka.getId()
//                  << ", Waga: " << paczka.getWaga()
//                  << ", Współrzędne: (" << paczka.getX() << ", " << paczka.getY() << ")\n";
//         }
//         cout << "Długość trasy: " << obliczDlugoscTrasy(trasy[i]) << " jednostek.\n\n";
//     }
// }

// // Funkcja krzyżowania tras (jednolite krzyżowanie)
// vector<Paczka> Trasa::krzyzowanie(const vector<Paczka>& rodzic1, const vector<Paczka>& rodzic2) {
//     vector<Paczka> dziecko = rodzic1;
//     size_t n = rodzic1.size();

//     random_device rd;
//     mt19937 generator(rd());
//     uniform_int_distribution<size_t> dist(0, n - 1);

//     for (size_t i = 0; i < n; ++i) {
//         if (dist(generator) % 2 == 0 && find(dziecko.begin(), dziecko.end(), rodzic2[i]) == dziecko.end()) {
//             dziecko[i] = rodzic2[i];
//         }
//     }

//     return dziecko;
// }

// // Funkcja mutacji (zamiana dwóch paczek)
// void Trasa::mutacja(vector<Paczka>& trasa) {
//     size_t n = trasa.size();

//     if (n < 2) return; // Brak wystarczającej liczby elementów do mutacji

//     random_device rd;
//     mt19937 generator(rd());
//     uniform_int_distribution<size_t> dist(0, n - 1);

//     size_t i = dist(generator);
//     size_t j = dist(generator);

//     while (i == j) {
//         j = dist(generator); // Zapewnij, że indeksy są różne
//     }

//     swap(trasa[i], trasa[j]);
// }