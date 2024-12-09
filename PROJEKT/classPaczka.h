#ifndef CLASSPACZKA_H
#define CLASSPACZKA_H

#include <string>
using namespace std;

class Paczka {
private:
    int id_paczki;
    //string adres_dostawy;
    double waga;
    double x;  // współrzędna x paczki
    double y;  // współrzędna y paczki
    std::string data_waznosci; // Pole do przechowywania daty ważności w formacie "YYYY-MM-DD"

public:
    // Konstruktor
    Paczka(int id, double waga, double x, double y, const std::string& data_waznosci="2099-12-31") : id_paczki(id), waga(waga), x(x), y(y), data_waznosci(data_waznosci) {};

    bool operator==(const Paczka& other) const {
        return id_paczki == other.id_paczki && waga == other.waga && x == other.x && y == other.y;
}

    // Gettery
    int getId() const;
    //string getAdres() const;
    double getWaga() const;
    double getX() const;
    double getY() const;
    std::string getDataWaznosci() const; // Getter do daty ważności

    void setDataWaznosci(const std::string& data_waznosci); // Setter do daty ważności

    void display() const;

    int getDniDoWaznosci() const;

};

#endif // CLASSPACZKA_H
