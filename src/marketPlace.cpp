#include <iostream>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <math.h>

#include "../include/marketPlace.hpp"
#include "../include/nutzer.hpp"
#include "../include/handelsgueter.hpp"

using namespace std;
using namespace handelsplatz;

int MarketPlace::getKontostand(int id)
{
    for (auto [name, structur] : usersInformation)
        if (name == getNutzer(id).getBenutzername())
            return structur.user.getKontostand();
    return 0;
}

std::vector<string> MarketPlace::getMyOwnOffers(int id)
{
    std::vector<string> rueckgabe;
    for (auto [verkäuferName, alleInfos] : angeboteVonNutzern)
    {
        if (verkäuferName == getNutzer(id).getBenutzername())
        {
            for (int i = 0; i < alleInfos.angebote.size(); i++)
            {
                rueckgabe.push_back("Name: " + alleInfos.angebote[i].getName());

                rueckgabe.push_back("Preis pro Handelsgut: " + std::to_string(alleInfos.preis[i]));

                rueckgabe.push_back("Anzahl: " + std::to_string(alleInfos.anzahl[i]));
            }
        }
    }
    return rueckgabe;
}

vector<string> MarketPlace::printAllYourItems(int id)
{

    for (auto [name, structur] : usersInformation)
    {
        if (name == getNutzer(id).getBenutzername())
        {
            return structur.user.getGuterVorratName();
        }
    }
    vector<string> d = {"NULL"};

    return d;
}

vector<int> MarketPlace::printAllYourItemsAnzahl(int id)
{

    for (auto [name, structur] : usersInformation)
        if (name == getNutzer(id).getBenutzername())
            return structur.user.getGuterVorratAnzahl();

    vector<int> d = {0};

    return d;
}

MarketPlace::MarketPlace()
{

    // the constructor provides our trade goods that we want to sell
    vector<string> nameDerverkaufendeProdukte = {"Koelsch", "Helles", "Kamera", "Puma", "Film", "Spiele", "Buecher", "VodkaE", "KastenBier", "Wolle"};
    vector<int> wertDerverkaufendeProdukte = {3, 2, 30, 75, 13, 28, 8, 5, 20, 1};

    // the for loop creates the given goods in the vertor as instances
    // of the class Handelsgueter
    for (int i = 0; i < 10; i++)
    {

        Handelsgueter neu = Handelsgueter(nameDerverkaufendeProdukte[i], 1);
        hUp.handelsgut = neu;
        hUp.preis = wertDerverkaufendeProdukte[i];

        angebotVomStaat[neu.getName()] = hUp;
    }
}

int MarketPlace::getID(string name_)
{
    for (const auto &[name, passwordAndUser] : usersInformation)
        if (name_ == name)
            return passwordAndUser.id;

    return 0;
}

bool MarketPlace::newUser(string newName, string newPassword)
{
    if (newName == "NULL")
        return false;

    for (const auto &[name, passwordAndUser] : usersInformation)
        if (name == newName)
            return false;

    nutzer newUser = nutzer(newName, newPassword, 1000);

    pwu.password = newPassword;
    pwu.user = newUser;
    pwu.id = rand() % 1000000;
    usersInformation[newName] = pwu;

    return true;
}

nutzer MarketPlace::login(string userName, string userPassword)
{

    for (const auto &[name, passwordAndUser] : usersInformation)
        if (name == userName && passwordAndUser.password == userPassword)
            return passwordAndUser.user;

    nutzer nullUSER = nutzer("NULL", "NUll", 0);
    return nullUSER;
}

bool MarketPlace::buyFromMarketPlace(string handelsgut, int anzahl, int id)
{

    if (getNutzer(id).getBenutzername() == "NULL")
        return false;

    // guckt, ob Angebot von Staat vorhanden ist
    for (auto &[angebot, hUp] : angebotVomStaat)
    {
        for (auto &[name, infos] : usersInformation)
        {
            if (name == getNutzer(id).getBenutzername())
            {
                if (angebot == handelsgut)
                {
                    if (infos.user.getKontostand() < hUp.preis * anzahl)
                        return false;

                    // Betrag vom Konto des Käufers abziehen
                    int kontostand = getNutzer(id).getKontostand();
                    kontostand = kontostand - (hUp.preis * anzahl);

                    infos.user.setKontostand(kontostand);
                    infos.user.addHandelsgut(handelsgut, anzahl);

                    usersInformation[infos.user.getBenutzername()] = infos;
                    return true;
                }
            }
        }
    }

    return false;
}

bool MarketPlace::buyFromUser(string handelsgut, string verkaufer, int anzahl, int id)
{
    if (getNutzer(id).getBenutzername() == "NULL")
        return false;

    int preis = 0;
    bool verkäuferBool = false;

    // iterriert durch angebote von Nutzern map
    for (auto [verKauferNutzer, alleInfos] : angeboteVonNutzern)
    {
        for (auto [käufer, userInfo] : usersInformation)
        {
            // Wenn Käufer übereinstimmt und wenn Käufer übereinstimmt
            if (verKauferNutzer == verkaufer && käufer == getNutzer(id).getBenutzername())
            {
                for (int i = 0; i < alleInfos.angebote.size(); i++)
                {
                    if (alleInfos.angebote[i].getName() == handelsgut)
                    {
                        if (käufer == getNutzer(id).getBenutzername())
                        {
                            // wenn zu viele Handelsgüter gekauft werden sollen, return
                            if (alleInfos.anzahl[i] < anzahl)
                                return false;

                            // wenn der Käufer zu wenig Geld hat, um den Preis für das Handelsgut zu bezahlen, return
                            else if (userInfo.user.getKontostand() < alleInfos.preis[i] * anzahl)
                                return false;

                            /* KÄUFER aktionen */

                            // Preis zwischen speichern
                            preis = alleInfos.preis[i];

                            // Käufer wird dem Preis vom Konto abgezogen
                            int kontostand = userInfo.user.getKontostand();

                            kontostand = kontostand - preis * anzahl;
                            userInfo.user.setKontostand(kontostand);
                            // Käufer bekommt das Handelsgut zugeschrieben
                            userInfo.user.addHandelsgut(handelsgut, anzahl);

                            // Käufer Information wird gespeichert
                            usersInformation[käufer] = userInfo;

                            /* VERKÄUFER akitoen*/

                            // anzahl,preis und handelsgut werden aus den Vecotren an der richtigen stelle gelöscht,
                            // da alles aufgekauft wurde
                            if (alleInfos.anzahl[i] - anzahl == 0)
                            {
                                alleInfos.anzahl.erase(alleInfos.anzahl.begin() + i);
                                alleInfos.preis.erase(alleInfos.preis.begin() + i);
                                alleInfos.angebote.erase(alleInfos.angebote.begin() + i);
                            }

                            // wenn nur ein Teil der Handelsgueter gekauft werden, wird die anzahl angepasst
                            else
                                alleInfos.anzahl[i] = alleInfos.anzahl[i] - anzahl;

                            angeboteVonNutzern[verKauferNutzer] = alleInfos;
                            verkäuferBool = true;
                        }
                    }
                }
            }
        }
    }

    if (verkäuferBool == true)
    {
        for (auto [käufer, userInfo] : usersInformation)
        {
            if (käufer == verkaufer)
            {
                // Verkäufer Kontostand ändern
                int kontostand = userInfo.user.getKontostand();
                userInfo.user.setKontostand(kontostand + preis * anzahl);

                // Verkäufer bekommt Handelsgut abgezogen
                userInfo.user.removeHandelsgut(handelsgut, anzahl);

                // Verkäufer Information wird gespeichert
                usersInformation[verkaufer] = userInfo;
                return true;
            }
        }
    }
    return false;
}

bool MarketPlace::sellToMarketPlace(string handelsgut, int anzahl, int id)
{
    if (getNutzer(id).getBenutzername() == "NULL")
        return false;

    // durch Iterration durch die Angebote vom Staat
    for (auto [nameDesHandelsgut, handelsgutUndPreis] : angebotVomStaat)
    {
        for (auto [nameVomVerkaufer, infos] : usersInformation)
        {
            if (nameVomVerkaufer == getNutzer(id).getBenutzername())
            {
                // wenn das zuverkaufenden Handelsgut vorhanden ist
                if (nameDesHandelsgut == handelsgut)
                {
                    // Wenn Verkäufer mehr verkaufen will, als vorhanden return false
                    if (infos.user.handelsgutAnzahl(handelsgut) < anzahl)
                        return false;

                    // Errechnet den Erlös und fügt ihn dem Verkäuferkonto zu
                    int erloes = anzahl * handelsgutUndPreis.preis;
                    int kontostand = infos.user.getKontostand();
                    infos.user.setKontostand(kontostand + erloes);

                    // Entfertn die Vorräte vom Verkäufer
                    infos.user.removeHandelsgut(handelsgut, anzahl);

                    // Speichert die veränderungen
                    usersInformation[nameVomVerkaufer] = infos;

                    // NutzerAngebot muss noch angepasst werden
                    for (auto [nameDesVerkaufers, alleInfos] : angeboteVonNutzern)
                    {
                        if (nameDesVerkaufers == getNutzer(id).getBenutzername())
                        {
                            for (int i = 0; i < alleInfos.angebote.size(); i++)
                            {
                                if (alleInfos.angebote[i].getName() == handelsgut)
                                {
                                    // Wenn die Anzahl vom angebotVonNutzer höher ist, als die Anzahl im eigenen Inventar muss das Angebot gelöscht werden
                                    if (alleInfos.anzahl[i] > infos.user.handelsgutAnzahl(handelsgut))
                                    {
                                        angeboteVonNutzern.erase(nameDesVerkaufers);
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                    // Falls der Nutzer kein Angebot in angebote von Nutzer hat oder
                    // die Anzahl von Handelsgüter kleiner ist, als die Handeslgüter in meinem eigenen Inventar:
                    // dann kann true returnt werden
                    return true;
                }
            }
        }
    }
    return false;
}

bool MarketPlace::selltoUser(string zuverkaufendesProdukt, int anzahl, int preis, int id)
{
    if (getNutzer(id).getBenutzername() == "NULL")
        return false;

    // überprüft ob nutzer schon ein angebot hat
    for (auto [nutzerName, alleInfos] : angeboteVonNutzern)
    {
        for (auto [ichSelber, infos] : usersInformation)
        {
            if (ichSelber == getNutzer(id).getBenutzername())
            {
                if (nutzerName == getNutzer(id).getBenutzername())
                {
                    for (int i = 0; i < alleInfos.angebote.size(); i++)
                    {
                        if (alleInfos.angebote[i].getName() == zuverkaufendesProdukt)
                        {

                            std::cout << "wie viel hat der nutzer davon" << infos.user.handelsgutAnzahl(zuverkaufendesProdukt) << std::endl;

                            // Wenn anzahl == 0, dann angebot löschen und returnen
                            if (anzahl == 0)
                            {
                                angeboteVonNutzern.erase(nutzerName);
                                return true;
                            }
                            else if (infos.user.handelsgutAnzahl(zuverkaufendesProdukt) < anzahl)
                                return false;

                            // Anzahl und Preis aktualisieren

                            alleInfos.anzahl[i] = anzahl;
                            alleInfos.preis[i] = preis;
                            angeboteVonNutzern[nutzerName] = alleInfos;
                            return true;
                        }
                    }
                    if (infos.user.handelsgutAnzahl(zuverkaufendesProdukt) < anzahl)
                        return false;

                    // wenn  kein Angebot vorhanden, neues Angebot erstellen
                    Handelsgueter h = Handelsgueter(zuverkaufendesProdukt, anzahl);

                    alleInfos.angebote.push_back(h);
                    alleInfos.preis.push_back(preis);
                    alleInfos.anzahl.push_back(anzahl);

                    angeboteVonNutzern[getNutzer(id).getBenutzername()] = alleInfos;
                    return true;
                }
            }
        }
    }

    for (auto [ichSelber, infos] : usersInformation)
    {
        if (ichSelber == getNutzer(id).getBenutzername())
        {
            if (infos.user.handelsgutAnzahl(zuverkaufendesProdukt) < anzahl)
                return false;

            struct alleInfos d;
            Handelsgueter h2 = Handelsgueter(zuverkaufendesProdukt, anzahl);

            d.angebote.push_back(h2);
            d.preis.push_back(preis);
            d.anzahl.push_back(anzahl);

            angeboteVonNutzern[getNutzer(id).getBenutzername()] = d;
            return true;
        }
    }
    return false;
}

vector<string> MarketPlace::getAllStaatOffers()
{

    vector<string> returnVecotr;
    for (const auto &[name, handelsgutUndPreis] : angebotVomStaat)
    {
        returnVecotr.push_back(name);
        returnVecotr.push_back(to_string(handelsgutUndPreis.preis));
    }

    return returnVecotr;
}

vector<string> MarketPlace::getAllNutzerOffers()
{

    vector<string> returnVecotr;
    for (auto [nutzerName, alleInfos] : angeboteVonNutzern)
    {
        returnVecotr.push_back(nutzerName + " verkauft:");

        for (int i = 0; i < alleInfos.angebote.size(); i++)
        {
            returnVecotr.push_back(to_string(alleInfos.anzahl[i]) + "x " + alleInfos.angebote[i].getName() + " für: " + to_string(alleInfos.preis[i]) + " pro Handeslgut");
        }
        returnVecotr.push_back("\n");
    }

    return returnVecotr;
}

int MarketPlace::getPriceOfMarketPlace(string handelsgut)
{

    for (const auto &[name, handelsgutUndPreis] : angebotVomStaat)
        if (name == handelsgut)
            return handelsgutUndPreis.preis;

    return 0;
}

int MarketPlace::getPriceOfUser(string handelsgut, string verkaufer)
{

    for (const auto &[nutzerName, alleInfos] : angeboteVonNutzern)
        if (nutzerName == verkaufer)

        {
            for (int i = 0; i < alleInfos.angebote.size(); i++)
            {
                if (alleInfos.angebote[i].getName() == handelsgut)
                    return alleInfos.preis[i];
            }
        }
    return 0;
}

void MarketPlace::preisanpassung()
{

    for (auto [name, preisUndNutzer] : angebotVomStaat)
    {

        int startpreis = preisUndNutzer.preis;
        int min = 1;
        int max = 100;
        double zeit = 0.2;
        double tend = 0.3 * (rand() % 100);
        int y = rand() % max;

        preisUndNutzer.preis = startpreis + (1.0 + tend * zeit + 0.5 * sqrt(zeit) * y);
        angebotVomStaat[name] = preisUndNutzer;
    }
}

nutzer MarketPlace::getNutzer(int id)
{
    for (auto [name, userInfos] : usersInformation)
        if (userInfos.id == id)
            return userInfos.user;

    return nutzer("NULL", "NULL", 0);
}
