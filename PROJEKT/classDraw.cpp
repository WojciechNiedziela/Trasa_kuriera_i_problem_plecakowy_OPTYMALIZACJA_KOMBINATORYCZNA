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
#include <regex>
#include <filesystem>

classDraw::classDraw() {}

// Funkcja pomocnicza do obliczania odległości między dwoma punktami
static double calculateDistance(double x1, double y1, double x2, double y2)
{
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

// Metoda obliczająca łączną długość trasy
// "trasa" to wektor paczek w kolejności dostawy
// "magazyn" to punkt początkowy i końcowy
// "mapa" (jeśli potrzeba) może być używana do bardziej złożonych obliczeń

double classDraw::obliczDlugoscTrasy(const std::vector<Paczka> &trasa, const Magazyn &magazyn)
{
    double totalDistance = 0.0;

    // Wspórzędne początkowe: magazyn
    double prevX = magazyn.getX();
    double prevY = magazyn.getY();

    // Iteruj przez paczki w trasie
    for (const auto &paczka : trasa)
    {
        double currentX = paczka.getX();
        double currentY = paczka.getY();

        // Dodaj odległość od poprzedniego punktu do obecnego
        totalDistance += calculateDistance(prevX, prevY, currentX, currentY);

        // Ustaw obecne wspórzędne jako poprzednie
        prevX = currentX;
        prevY = currentY;
    }

    // Powrót do magazynu
    totalDistance += calculateDistance(prevX, prevY, magazyn.getX(), magazyn.getY());

    return totalDistance;
}

double classDraw::obliczCalkowitaDlugoscTras(const std::vector<std::vector<Paczka>> &wynikoweTrasy, const Magazyn &magazyn)
{
    double calkowitaDlugosc = 0.0;

    for (const auto &trasa : wynikoweTrasy)
    {
        double dlugoscTrasy = obliczDlugoscTrasy(trasa, magazyn);
        std::cout << "Długość trasy: " << dlugoscTrasy << std::endl; // Wyświetl długość trasy
        calkowitaDlugosc += dlugoscTrasy;
    }

    return calkowitaDlugosc;
}

bool classDraw::walidujDate(const std::string &data)
{
    // Wyrażenie regularne sprawdzające format YYYY-MM-DD
    const std::regex wzorzec("^\\d{4}-\\d{2}-\\d{2}$");

    // Sprawdź, czy format się zgadza
    if (!std::regex_match(data, wzorzec))
    {
        return false;
    }

    // Rozdziel na rok, miesiąc, dzień
    int rok, miesiac, dzien;
    if (sscanf(data.c_str(), "%4d-%2d-%2d", &rok, &miesiac, &dzien) != 3)
    {
        return false;
    }

    // Sprawdź poprawność daty
    if (miesiac < 1 || miesiac > 12 || dzien < 1)
    {
        return false;
    }

    // Liczba dni w każdym miesiącu (rok nieprzestępny)
    const int dniWMiesiacach[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Sprawdź, czy rok jest przestępny
    bool przestepny = (rok % 4 == 0 && rok % 100 != 0) || (rok % 400 == 0);

    // Liczba dni w lutym w roku przestępnym
    if (miesiac == 2 && przestepny)
    {
        if (dzien > 29)
        {
            return false;
        }
    }
    else
    {
        if (dzien > dniWMiesiacach[miesiac - 1])
        {
            return false;
        }
    }

    return true;
}

// Funkcja do ustawienia liczby kurierów
void classDraw::SetCouriers(std::vector<Kurier> &kurierzy)
{
    int liczbaKurierow;
    std::cout << "Podaj liczbe kurierow: ";
    std::cin >> liczbaKurierow;

    if (liczbaKurierow <= 0)
    {
        std::cout << "Blad: liczba kurierow musi byc wieksza od 0.\n";
        return;
    }

    // kurierzy.clear(); // Wyczyść listę kurierów
    for (int i = 1; i <= liczbaKurierow; ++i)
    {
        int id;
        double ladownosc;

        while (true)
        {
            std::cout << "Podaj id kuriera " << i << ": ";
            std::cin >> id;

            // Sprawdź, czy ID jest unikalne
            bool istnieje = false;
            for (const auto &kurier : kurierzy)
            {
                if (kurier.getId() == id) // Zakładamy, że `Kurier` ma metodę `getId()`
                {
                    istnieje = true;
                    break;
                }
            }

            if (istnieje)
            {
                std::cout << "Kurier z podanym ID juz istnieje. Wprowadz inne ID.\n";
            }
            else
            {
                break; // ID jest unikalne
            }
        }

        std::cout << "Podaj ladownosc kuriera " << i << ": ";
        std::cin >> ladownosc;

        kurierzy.emplace_back(id, ladownosc);
    }

    std::cout << "Zapisano " << liczbaKurierow << " kurierow.\n";
}

// Funkcja do wyświetlenia szczegółów kurierów
void classDraw::DisplayCouriers(const std::vector<Kurier> &kurierzy)
{
    if (kurierzy.empty())
    {
        std::cout << "Brak zarejestrowanych kurierow.\n";
        return;
    }

    std::cout << "\n--- Lista Kurierow ---\n";
    for (const auto &kurier : kurierzy)
    {
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
    string data_waznosci;
    while (file >> id >> waga >> x >> y >> data_waznosci)
    {
        paczki.emplace_back(id, waga, x, y, data_waznosci);
    }

    file.close();
    std::cout << "Wczytano paczki z pliku: " << fileName << "\n";
}

// Zaktualizowane menu
void classDraw::MainWindow(std::vector<Paczka> &paczki, Magazyn &magazyn, std::vector<Kurier> &kurierzy, Mapa &mapa)
{
    std::vector<Kurier *> kurierzyPtr; // Tymczasowy wektor wskaźników
    for (Kurier &kurier : kurierzy)
    {
        kurierzyPtr.push_back(&kurier); // Dodaj wskaźnik do każdego kuriera
    }

    classHandlingEvents classHandlingEvents;
    double magazynX = 0.0, magazynY = 0.0;

    //    MagazynInput(magazynX, magazynY);

    int choice;
    while (true)
    {
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
        std::cout << "10. TESTY\n";
        std::cout << "11. TESTY CZAS\n";
        std::cout << "Wybierz opcje: ";
        std::cin >> choice;

        if (std::cin.fail() || choice < 1 || choice > 11)
        {
            std::cout << "Blad: podaj poprawny numer opcji.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            AddPackage(paczki);
            break;
        }
        case 2:
        {
            std::string fileName = "paczki.txt";
            LoadPackagesFromFile(paczki, fileName);
            break;
        }
        case 3:
        {
            DisplayPackages(paczki);
            break;
        }
        case 4:
        {
            SetCouriers(kurierzy);
            break;
        }
        case 5:
        {
            DisplayCouriers(kurierzy);
            break;
        }
        case 6:
        {
            if (!kurierzy.empty())
            {
                DisplayRoutes(paczki, magazyn, kurierzy, mapa, "genetyczny");
            }
            else
            {
                std::cout << "Brak zarejestrowanych kurierow. Ustaw kurierow najpierw.\n";
            }
            break;
        }
        case 7:
        {
            if (!kurierzy.empty())
            {
                DisplayRoutes(paczki, magazyn, kurierzy, mapa, "zachlanny");
            }
            else
            {
                std::cout << "Brak zarejestrowanych kurierow. Ustaw kurierow najpierw.\n";
            }
            break;
        }
        case 8:
        {
            if (!kurierzy.empty())
            {
                DisplayRoutes(paczki, magazyn, kurierzy, mapa, "wyzarzanie");
            }
            else
            {
                std::cout << "Brak zarejestrowanych kurierow. Ustaw kurierow najpierw.\n";
            }
            break;
        }
        case 9:
        {
            std::cout << "Wyjscie z programu.\n";
            return;
        }
        case 10:
        {
            // Wybór algorytmu
            int algorytmWybor;
            std::cout << "Wybierz algorytm:\n";
            std::cout << "1. Algorytm Zachłanny (GRE)\n";
            std::cout << "2. Algorytm Genetyczny (GEN)\n";
            std::cout << "3. Algorytm Wyżarzania (SA)\n";
            std::cout << "Wybór: ";
            std::cin >> algorytmWybor;

            if (algorytmWybor < 1 || algorytmWybor > 3)
            {
                std::cout << "Nieprawidłowy wybór algorytmu.\n";
                break;
            }

            // Wczytaj kurierów z pliku
            std::vector<Kurier> kurierzy;
            std::ifstream fileKurierzy("kurierzy.txt");
            if (!fileKurierzy)
            {
                std::cerr << "Nie można otworzyć pliku kurierzy.txt.\n";
                break;
            }

            int liczbaKurierow;
            fileKurierzy >> liczbaKurierow;
            for (int i = 0; i < liczbaKurierow; ++i)
            {
                int id;
                double ladownosc;
                fileKurierzy >> id >> ladownosc;
                kurierzy.emplace_back(id, ladownosc);
            }
            fileKurierzy.close();

            // Otwórz folder data i przetwórz pliki
            std::string folder = "data/";
            std::string algorytmNazwa = (algorytmWybor == 1) ? "GRE" : (algorytmWybor == 2) ? "GEN"
                                                                                            : "SA";
            std::ofstream results("results_" + algorytmNazwa + ".txt");
            if (!results)
            {
                std::cerr << "Nie można utworzyć pliku wynikowego.\n";
                break;
            }

            for (const auto &entry : std::filesystem::directory_iterator(folder))
            {
                if (!entry.is_regular_file())
                    continue;

                std::string fileName = entry.path().string();
                std::vector<Paczka> paczki;

                // Wczytaj paczki z pliku
                std::ifstream filePaczki(fileName);
                if (!filePaczki)
                {
                    std::cerr << "Nie można otworzyć pliku: " << fileName << "\n";
                    continue;
                }

                int id;
                double waga, x, y;
                std::string data;
                while (filePaczki >> id >> waga >> x >> y >> data)
                {
                    paczki.emplace_back(id, waga, x, y, data);
                }
                filePaczki.close();

                // Ustaw magazyn (przykładowo na (0, 0))
                // Magazyn magazyn(0.0, 0.0);
                Mapa mapa;

                // Uruchom odpowiedni algorytm
                Trasa trasa(kurierzy, &magazyn, paczki, &mapa);
                std::vector<std::vector<Paczka>> wynikoweTrasy;
                if (algorytmWybor == 1)
                {
                    wynikoweTrasy = trasa.znajdzTraseAlgorytmZachlanny();
                }
                else if (algorytmWybor == 2)
                {
                    wynikoweTrasy = trasa.znajdzTraseAlgorytmGenetyczny(100, 50); // Rozmiar populacji i liczba pokoleń
                }
                else if (algorytmWybor == 3)
                {
                    wynikoweTrasy = trasa.znajdzTraseAlgorytmWyzarzania();
                }

                // // Oblicz całkowitą długość trasy
                // double totalDistance = 0.0;

                // for (const auto &trasa : wynikoweTrasy)
                // {
                //     //Trasa trasaObj(magazyn, mapa); // Utwórz instancję klasy Trasa (lub odpowiedni konstruktor)
                //     //totalDistance += trasaObj.obliczDlugoscTrasy(trasa); // Wywołaj metodę na obiekcie trasaObj

                //     //totalDistance += trasa.obliczDlugoscTrasy(trasa);
                //     totalDistance += obliczDlugoscTrasy(trasa);
                // }

                double totalDistance = obliczCalkowitaDlugoscTras(wynikoweTrasy, magazyn);

                // Zapisz wynik do pliku
                results << "Plik: " << fileName << ", Długość trasy: " << std::fixed << std::setprecision(2) << totalDistance << "\n";
            }

            results.close();
            std::cout << "Wyniki zapisano do pliku: results_" << algorytmNazwa << ".txt\n";

            break;
        }
        case 11:
        {
            // Wybór algorytmu
            int algorytmWybor;
            std::cout << "Wybierz algorytm:\n";
            std::cout << "1. Algorytm Zachłanny (GRE)\n";
            std::cout << "2. Algorytm Genetyczny (GEN)\n";
            std::cout << "3. Algorytm Wyżarzania (SA)\n";
            std::cout << "Wybór: ";
            std::cin >> algorytmWybor;

            if (algorytmWybor < 1 || algorytmWybor > 3)
            {
                std::cout << "Nieprawidłowy wybór algorytmu.\n";
                break;
            }

            // Wczytaj kurierów z pliku
            std::vector<Kurier> kurierzy;
            std::ifstream fileKurierzy("kurierzy.txt");
            if (!fileKurierzy)
            {
                std::cerr << "Nie można otworzyć pliku kurierzy.txt.\n";
                break;
            }

            int liczbaKurierow;
            fileKurierzy >> liczbaKurierow;
            for (int i = 0; i < liczbaKurierow; ++i)
            {
                int id;
                double ladownosc;
                fileKurierzy >> id >> ladownosc;
                kurierzy.emplace_back(id, ladownosc);
            }
            fileKurierzy.close();

            // Otwórz folder data i przetwórz pliki
            std::string folder = "data/";
            std::string algorytmNazwa = (algorytmWybor == 1) ? "GRE" : (algorytmWybor == 2) ? "GEN"
                                                                                            : "SA";
            std::ofstream results("results_time_" + algorytmNazwa + ".txt");
            if (!results)
            {
                std::cerr << "Nie można utworzyć pliku wynikowego.\n";
                break;
            }

            for (const auto &entry : std::filesystem::directory_iterator(folder))
            {
                if (!entry.is_regular_file())
                    continue;

                std::string fileName = entry.path().string();
                std::vector<Paczka> paczki;

                // Wczytaj paczki z pliku
                std::ifstream filePaczki(fileName);
                if (!filePaczki)
                {
                    std::cerr << "Nie można otworzyć pliku: " << fileName << "\n";
                    continue;
                }

                int id;
                double waga, x, y;
                std::string data;
                while (filePaczki >> id >> waga >> x >> y >> data)
                {
                    paczki.emplace_back(id, waga, x, y, data);
                }
                filePaczki.close();

                // Ustaw magazyn (przykładowo na (0, 0))
                Mapa mapa;

                Trasa trasa(kurierzy, &magazyn, paczki, &mapa);
                std::vector<std::vector<Paczka>> wynikoweTrasy;

                // Pomiar czasu algorytmu
                auto start = std::chrono::high_resolution_clock::now();

                if (algorytmWybor == 1)
                {
                    wynikoweTrasy = trasa.znajdzTraseAlgorytmZachlanny();
                }
                else if (algorytmWybor == 2)
                {
                    wynikoweTrasy = trasa.znajdzTraseAlgorytmGenetyczny(100, 50); // Rozmiar populacji i liczba pokoleń
                }
                else if (algorytmWybor == 3)
                {
                    wynikoweTrasy = trasa.znajdzTraseAlgorytmWyzarzania();
                }

                auto end = std::chrono::high_resolution_clock::now();
                auto czasWykonania = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

                // Zapisz wynik do pliku
                results << "Plik: " << fileName
                        << ", Czas wykonania: " << czasWykonania << " ms\n";

                std::cout << "Przetworzono plik: " << fileName << ", czas: " << czasWykonania << " ms\n";
            }

            results.close();
            std::cout << "Wyniki czasowe zapisano do pliku: results_time_" << algorytmNazwa << ".txt\n";

            break;
        }
        }
    }
}

void classDraw::AddPackage(std::vector<Paczka> &paczki)
{
    int id;
    std::string adres, data_waznosci;
    double waga, x, y;

    while (true)
    {
        std::cout << "Podaj ID paczki: ";
        std::cin >> id;

        // Sprawdź, czy ID jest unikalne
        bool istnieje = false;
        for (const auto &paczka : paczki)
        {
            if (paczka.getId() == id)
            {
                istnieje = true;
                break;
            }
        }

        if (istnieje)
        {
            std::cout << "Paczka z podanym ID juz istnieje. Wprowadz inne ID.\n";
        }
        else
        {
            break; // ID jest unikalne
        }
    }

    std::cout << "Podaj wage paczki (kg): ";
    std::cin >> waga;
    std::cout << "Podaj wspolrzedne paczki (X Y): ";
    std::cin >> x >> y;

    while (true)
    {
        std::cout << "Podaj date waznosci w formacie YYYY-MM-DD: ";
        std::cin >> data_waznosci;

        if (walidujDate(data_waznosci))
        {
            std::cout << "Poprawna data: " << data_waznosci << std::endl;
            break;
        }
        else
        {
            std::cout << "Niepoprawna data. Podaj ponownie.\n";
        }
    }

    if (!std::cin.fail())
    {
        paczki.emplace_back(id, waga, x, y, data_waznosci);
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
                  << ", Wspolrzedne: (" << paczka.getX() << ", " << paczka.getY() << ")\n"
                  << ", Data waznosci: " << paczka.getDataWaznosci() << "\n";
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

    // // Wyświetlanie tras dla każdego kuriera
    // for (size_t i = 0; i < routes.size(); ++i) {
    //     std::cout << "\n--- Trasa kuriera " << (i + 1) << " ---\n";
    //     double totalDistance = 0.0;
    //     double prevX = magazyn.getX(), prevY = magazyn.getY();

    //     for (const auto &paczka : routes[i]) {
    //         double distance = calculateDistance(prevX, prevY, paczka.getX(), paczka.getY());
    //         if (paczka.getId() == -1) {
    //             std::cout << "Powrot do magazynu, Dystans: " << std::fixed << std::setprecision(2) << distance << "\n";
    //         } else {
    //             std::cout << "Dostawa ID: " << paczka.getId()
    //                       << ", Dystans: " << std::fixed << std::setprecision(2) << distance << "\n";
    //         }
    //         totalDistance += distance;
    //         prevX = paczka.getX();
    //         prevY = paczka.getY();
    //     }

    //     totalDistance += calculateDistance(prevX, prevY, magazyn.getX(), magazyn.getY());
    //     std::cout << "Calkowity Dystans: " << std::fixed << std::setprecision(2) << totalDistance << "\n";
    // }

    // Wyświetlanie tras dla każdego kuriera
    for (size_t i = 0; i < routes.size(); ++i)
    {
        std::cout << "\n--- Trasa kuriera " << (i + 1) << " ---\n";
        double totalDistance = 0.0;
        double prevX = magazyn.getX(), prevY = magazyn.getY();

        for (const auto &paczka : routes[i])
        {
            double distance = calculateDistance(prevX, prevY, paczka.getX(), paczka.getY());

            if (paczka.getId() == -1)
            {
                std::cout << "Powrot do magazynu, Dystans: " << std::fixed << std::setprecision(2) << distance << "\n";
            }
            else
            {
                int dniDoWaznosci = paczka.getDniDoWaznosci();
                std::cout << "Dostawa ID: " << paczka.getId()
                          << ", Dystans: " << std::fixed << std::setprecision(2) << distance
                          << ", Dni do waznosci: " << dniDoWaznosci << "\n";
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
