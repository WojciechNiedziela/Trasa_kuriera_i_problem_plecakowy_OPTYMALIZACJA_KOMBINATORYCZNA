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

public:
    // Konstruktor
    Paczka(int id, double waga, double x, double y);

    bool operator==(const Paczka& other) const {
        return id_paczki == other.id_paczki && waga == other.waga && x == other.x && y == other.y;
}

    // Gettery
    int getId() const;
    //string getAdres() const;
    double getWaga() const;
    double getX() const;
    double getY() const;

    // bool operator==(const Paczka& other) const {
    //     // this->id_paczki == other.id_paczki;
    //     return id_paczki == other.id_paczki;
    // }
    
    // Metoda do wyświetlania informacji o paczce
    void display() const;
};

#endif // CLASSPACZKA_H
