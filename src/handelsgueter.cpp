#include <iostream>
#include <string>
#include "../include/handelsgueter.hpp"

using namespace std;
using namespace market;
Handelsgueter::Handelsgueter(std::string name_, int anzahl_) : name(name_), anzahl(anzahl_){};

/*int Handelsgueter::getHandelswert()
{
    return handelswert;
}
*/
std::string Handelsgueter::getName()
{
    return name;
}

int Handelsgueter::getAnzahl()
{
    return anzahl;
}

void Handelsgueter::setAnzahl(int neueAnzhal)
{
    anzahl = neueAnzhal;
}