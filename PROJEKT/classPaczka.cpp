#include "classPaczka.h"
#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

using namespace std;


// Getter dla ID paczki
int Paczka::getId() const {
    return this->id_paczki;
}

// Getter dla wagi paczki
double Paczka::getWaga() const {
    return this->waga;
}

double Paczka::getX() const {
    return this->x;
}

double Paczka::getY() const {
    return this->y;
}

std::string Paczka::getDataWaznosci() const {
    return this->data_waznosci;
}

// Settery
void Paczka::setDataWaznosci(const std::string& data_waznosci="2099-12-31") {
    this->data_waznosci = data_waznosci;
}


// Wyświetlanie informacji o paczce
void Paczka::display() const {
    cout << "Paczka ID: " << id_paczki //<< ", Adres: " << adres_dostawy
         << ", Waga: " << waga << " kg, Wspolrzedne: (" << x << ", " << y << ")" << endl;
}



int Paczka::getDniDoWaznosci() const {
    // Pobierz dzisiejszą datę
    auto dzis = std::chrono::system_clock::now();
    std::time_t teraz = std::chrono::system_clock::to_time_t(dzis);
    std::tm* lokalnyCzas = std::localtime(&teraz);

    // Rozdziel datę ważności paczki
    int rok, miesiac, dzien;
    char delimiter;
    std::istringstream dataStream(data_waznosci);
    dataStream >> rok >> delimiter >> miesiac >> delimiter >> dzien;

    // Zamień datę ważności paczki na czas `std::chrono::time_point`
    std::tm dataTM = {};
    dataTM.tm_year = rok - 1900; // Lata od 1900
    dataTM.tm_mon = miesiac - 1; // Miesiące 0-11
    dataTM.tm_mday = dzien;

    std::time_t czasPaczki = std::mktime(&dataTM);
    auto dataPaczki = std::chrono::system_clock::from_time_t(czasPaczki);

    // Oblicz różnicę w dniach
    auto roznica = std::chrono::duration_cast<std::chrono::hours>(dataPaczki - dzis).count();
    return roznica / 24; // Zamień godziny na dni
}
