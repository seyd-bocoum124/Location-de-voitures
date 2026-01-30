/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2025 / TP4
 *  Fichier de départ date.h © Éric Beaudry.
 *
*/
#if !defined(__DATE_H__)
#define __DATE_H__

#include <iostream>
#include <iomanip>
#include <cstdlib>

class Date{
  public:
    // Constructeur par défaut => 0j_00h00m
    Date();

    // Constructeur explicite (surtout utile pour tests)
    Date(int jours, int heures, int minutes);

    // Comparateurs (ESSENTIELS pour ArbreAVL / ArbreMap)
    bool operator < (const Date& date) const;
    bool operator <=(const Date& date) const;
    bool operator > (const Date& date) const { return date < *this; }
    bool operator >=(const Date& date) const { return !(*this < date); }
    bool operator ==(const Date& date) const { return secondes == date.secondes; }
    bool operator !=(const Date& date) const { return secondes != date.secondes; }

  private:
    // Toute la date en secondes depuis 0j_00h00m
    long secondes;

    // Convertit j/h/m en secondes
    static long convertirEnSecondes(int jours, int heures, int minutes);

    // Convertit secondes -> j/h/m
    static void convertirDepuisSecondes(long sec,
                                        int& jours,
                                        int& heures,
                                        int& minutes);

    // Pour affichage
    friend std::ostream& operator << (std::ostream&, const Date& date);

    // Pour lecture (0j_00h00m)
    friend std::istream& operator >> (std::istream&, Date& date);
};

#endif