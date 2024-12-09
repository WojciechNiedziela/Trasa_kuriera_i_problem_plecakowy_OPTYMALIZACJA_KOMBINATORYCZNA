#include "classTrasa.h"
#include <iostream>
#include <cmath>
#include <random>
#include <limits>
#include <algorithm>
#include <vector>

using namespace std;

// Konstruktor
Trasa::Trasa(vector<Kurier> kurierzy, Magazyn* magazyn, const vector<Paczka>& paczki, Mapa* mapa) {
    this->kurierzy = kurierzy;
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


// Kryterium sortowania paczek według daty ważności
void sortujPaczkiPoDacie(std::vector<Paczka>& paczki) {
    std::sort(paczki.begin(), paczki.end(), [](const Paczka& a, const Paczka& b) {
        int dniA = a.getDniDoWaznosci(); // Zakładamy, że metoda getDniDoWaznosci() zwraca liczbę dni do daty ważności
        int dniB = b.getDniDoWaznosci();
        if (dniA <= 2 && dniB > 2) return true;  // Priorytet dla paczek z datą < 2 dni
        if (dniA <= 5 && dniB > 5) return true;  // Priorytet dla paczek z datą < 5 dni
        if (dniA > 5 && dniB > 5) return false;  // Przy odległych datach decyduje odległość
        return dniA < dniB;                      // Sortuj po liczbie dni rosnąco
    });
}

std::vector<std::vector<Paczka>> Trasa::znajdzTraseAlgorytmZachlanny() {
    std::vector<std::vector<Paczka>> wynikoweTrasy(kurierzy.size());
    std::vector<Paczka> paczkiDoDostarczenia = paczki;

    // Sortuj paczki po dacie ważności
    sortujPaczkiPoDacie(paczkiDoDostarczenia);

    while (!paczkiDoDostarczenia.empty()) {
        bool jakakolwiekDostawa = false;

        for (size_t i = 0; i < kurierzy.size(); ++i) {
            if (paczkiDoDostarczenia.empty()) break;

            // Rozwiąż problem plecakowy dla aktualnego kuriera
            std::vector<Paczka> aktualnaTrasa = rozwiazProblemPlecakowy(paczkiDoDostarczenia, kurierzy[i].getLadownosc());

            // Jeśli kurier nie może zabrać żadnej paczki, przejdź do następnego kuriera
            if (aktualnaTrasa.empty()) {
                continue;
            }

            // Dodaj magazyn jako początek trasy
            wynikoweTrasy[i].emplace_back(Paczka(-1, 0.0, magazyn->getX(), magazyn->getY(), "2099-12-31"));

            // Dodaj wybrane paczki do trasy
            wynikoweTrasy[i].insert(wynikoweTrasy[i].end(), aktualnaTrasa.begin(), aktualnaTrasa.end());

            // Dodaj magazyn jako koniec trasy
            wynikoweTrasy[i].emplace_back(Paczka(-1, 0.0, magazyn->getX(), magazyn->getY(), "2099-12-31"));

            // Usuń dostarczone paczki z listy paczek do dostarczenia
            for (const auto& paczka : aktualnaTrasa) {
                auto it = std::find(paczkiDoDostarczenia.begin(), paczkiDoDostarczenia.end(), paczka);
                if (it != paczkiDoDostarczenia.end()) {
                    paczkiDoDostarczenia.erase(it);
                }
            }

            jakakolwiekDostawa = true;
        }

        // Jeśli w tej iteracji żaden kurier nie dostarczył paczek, przerwij pętlę
        if (!jakakolwiekDostawa) {
            std::cerr << "Nie można dostarczyć więcej paczek. Pozostałe paczki nie mieszczą się w żadnym kurierze.\n";
            break;
        }
    }

    return wynikoweTrasy;
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

// Funkcja krzyżowania tras (jednolite krzyżowanie)
vector<Paczka> Trasa::krzyzowanie(const vector<Paczka>& rodzic1, const vector<Paczka>& rodzic2) {
    vector<Paczka> dziecko = rodzic1;
    size_t n = rodzic1.size();

    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<size_t> dist(0, n - 1);

    for (size_t i = 0; i < n; ++i) {
        if (dist(generator) % 2 == 0 && find(dziecko.begin(), dziecko.end(), rodzic2[i]) == dziecko.end()) {
            dziecko[i] = rodzic2[i];
        }
    }

    return dziecko;
}

// Funkcja mutacji (zamiana dwóch paczek)
void Trasa::mutacja(vector<Paczka>& trasa) {
    size_t n = trasa.size();

    if (n < 2) return; // Brak wystarczającej liczby elementów do mutacji

    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<size_t> dist(0, n - 1);

    size_t i = dist(generator);
    size_t j = dist(generator);

    while(i == j) {
        j = dist(generator); // Zapewnij, że indeksy są różne
    }

    swap(trasa[i], trasa[j]);
}



std::vector<std::vector<Paczka>> Trasa::znajdzTraseAlgorytmGenetyczny(int rozmiarPopulacji, int liczbaPokolen) {
    std::vector<std::vector<Paczka>> wynikoweTrasy(kurierzy.size());
    std::vector<Paczka> paczkiDoDostarczenia = paczki;

    sortujPaczkiPoDacie(paczkiDoDostarczenia);

    while (!paczkiDoDostarczenia.empty()) {
        for (size_t i = 0; i < kurierzy.size(); ++i) {
            if (paczkiDoDostarczenia.empty()) break;

            // Rozwiąż problem plecakowy
            std::vector<Paczka> wybranePaczki = rozwiazProblemPlecakowy(paczkiDoDostarczenia, kurierzy[i].getLadownosc());

            if (wybranePaczki.empty()) continue;

            // Tworzenie populacji
            std::vector<std::vector<Paczka>> populacja(rozmiarPopulacji);
            random_device rd;
            mt19937 generator(rd());
            for (int j = 0; j < rozmiarPopulacji; ++j) {
                std::vector<Paczka> trasa = wybranePaczki;
                std::shuffle(trasa.begin(), trasa.end(), generator);
                populacja[j] = trasa;
            }

            // Ewolucja
            for (int pokolenie = 0; pokolenie < liczbaPokolen; ++pokolenie) {
                std::sort(populacja.begin(), populacja.end(), [this](const std::vector<Paczka>& a, const std::vector<Paczka>& b) {
                    return obliczDlugoscTrasy(a) < obliczDlugoscTrasy(b);
                });

                // Krzyżowanie i mutacje
                while (populacja.size() < static_cast<size_t>(rozmiarPopulacji)) {
                    int i1 = rand() % (rozmiarPopulacji / 2);
                    int i2 = rand() % (rozmiarPopulacji / 2);
                    std::vector<Paczka> dziecko = krzyzowanie(populacja[i1], populacja[i2]);
                    if (rand() % 100 < 10) {
                        mutacja(dziecko);
                    }
                    populacja.push_back(dziecko);
                }
            }

            // Wybór najlepszej trasy
            wynikoweTrasy[i].emplace_back(Paczka(-1, 0.0, magazyn->getX(), magazyn->getY()));
            wynikoweTrasy[i].insert(wynikoweTrasy[i].end(), populacja[0].begin(), populacja[0].end());
            wynikoweTrasy[i].emplace_back(Paczka(-1, 0.0, magazyn->getX(), magazyn->getY()));

            // Usuń dostarczone paczki
            for (const auto& paczka : populacja[0]) {
                auto it = std::find(paczkiDoDostarczenia.begin(), paczkiDoDostarczenia.end(), paczka);
                if (it != paczkiDoDostarczenia.end()) {
                    paczkiDoDostarczenia.erase(it);
                }
            }
        }
    }

    return wynikoweTrasy;
}



std::vector<std::vector<Paczka>> Trasa::znajdzTraseAlgorytmWyzarzania() {
    std::vector<std::vector<Paczka>> wynikoweTrasy(kurierzy.size());
    std::vector<Paczka> paczkiDoDostarczenia = paczki;


    sortujPaczkiPoDacie(paczkiDoDostarczenia);


    while (!paczkiDoDostarczenia.empty()) {
        for (size_t i = 0; i < kurierzy.size(); ++i) {
            if (paczkiDoDostarczenia.empty()) break;

            std::vector<Paczka> wybranePaczki = rozwiazProblemPlecakowy(paczkiDoDostarczenia, kurierzy[i].getLadownosc());
            if (wybranePaczki.empty()) continue;

            // Optymalizacja wyżarzania
            std::vector<Paczka> optymalnaTrasa = wybranePaczki;
            double temperatura = 100.0;
            double wspolczynnikChlodzenia = 0.999;
            while (temperatura > 0.0001) {
                int i1 = rand() % optymalnaTrasa.size();
                int i2 = rand() % optymalnaTrasa.size();
                double dlugoscPrzed = obliczDlugoscTrasy(optymalnaTrasa);
                std::swap(optymalnaTrasa[i1], optymalnaTrasa[i2]);
                double dlugoscPo = obliczDlugoscTrasy(optymalnaTrasa);
                if (exp((dlugoscPrzed - dlugoscPo) / temperatura) < static_cast<double>(rand()) / RAND_MAX) {
                    std::swap(optymalnaTrasa[i1], optymalnaTrasa[i2]);
                }
                temperatura *= wspolczynnikChlodzenia;
            }

            wynikoweTrasy[i].emplace_back(Paczka(-1, 0.0, magazyn->getX(), magazyn->getY()));
            wynikoweTrasy[i].insert(wynikoweTrasy[i].end(), optymalnaTrasa.begin(), optymalnaTrasa.end());
            wynikoweTrasy[i].emplace_back(Paczka(-1, 0.0, magazyn->getX(), magazyn->getY()));

            // Usuń dostarczone paczki
            for (const auto& paczka : optymalnaTrasa) {
                auto it = std::find(paczkiDoDostarczenia.begin(), paczkiDoDostarczenia.end(), paczka);
                if (it != paczkiDoDostarczenia.end()) {
                    paczkiDoDostarczenia.erase(it);
                }
            }
        }
    }

    return wynikoweTrasy;
}


// // Wyświetlanie tras
void Trasa::displayTrasy(const vector<vector<Paczka>>& trasy) {
    for (size_t i = 0; i < trasy.size(); ++i) {
        cout << "Trasa " << i + 1 << ":\n";
        for (const auto& paczka : trasy[i]) {
            cout << "- Paczka ID: " << paczka.getId()
                 << ", Waga: " << paczka.getWaga()
                 << ", Współrzędne: (" << paczka.getX() << ", " << paczka.getY() << ")\n";
        }
        cout << "Długość trasy: " << obliczDlugoscTrasy(trasy[i]) << " jednostek.\n\n";
    }
}

