/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2025 / TP4
 *  http://ericbeaudry.uqam.ca/INF3105/tp4/
 *  Fichier de départ date.cpp © Éric Beaudry.
 *
*/
#include "date.h"
#include <cstdio>
#include <assert.h>

// =========================
//    Constructeurs
// =========================

Date::Date()
    : secondes(0)
{
}

Date::Date(int jours, int heures, int minutes)
{
    secondes = convertirEnSecondes(jours, heures, minutes);
}

// =========================
//    Méthodes statiques
// =========================

long Date::convertirEnSecondes(int jours, int heures, int minutes)
{
    // 1 jour = 24*3600 secondes
    long s = 0;
    s += static_cast<long>(jours) * 24L * 3600L;
    s += static_cast<long>(heures) * 3600L;
    s += static_cast<long>(minutes) * 60L;
    return s;
}

void Date::convertirDepuisSecondes(long sec,
                                   int& jours,
                                   int& heures,
                                   int& minutes)
{
    if (sec < 0) sec = 0; // sécurité

    long restant = sec;
    const long SECS_PAR_JOUR = 24L * 3600L;

    jours = static_cast<int>(restant / SECS_PAR_JOUR);
    restant %= SECS_PAR_JOUR;

    heures = static_cast<int>(restant / 3600L);
    restant %= 3600L;

    minutes = static_cast<int>(restant / 60L);
}

// =========================
//    Opérateurs de comparaison
// =========================

bool Date::operator <(const Date& date) const
{
    return secondes < date.secondes;
}

bool Date::operator <=(const Date& date) const
{
    return secondes <= date.secondes;
}

// Les autres opérateurs sont définis inline dans date.h
// (>, >=, ==, !=)

// =========================
//    Opérateurs d'E/S
// =========================

std::ostream& operator << (std::ostream& os, const Date& d)
{
    int jours, heures, minutes;
    Date::convertirDepuisSecondes(d.secondes, jours, heures, minutes);

    // Format : 0j_00h00m
    os << jours
       << 'j'
       << '_'
       << std::setw(2) << std::setfill('0') << heures
       << 'h'
       << std::setw(2) << std::setfill('0') << minutes
       << 'm';

    // Remettre le remplissage à l'espace par sécurité
    os << std::setfill(' ');

    return os;
}

std::istream& operator >> (std::istream& is, Date& d)
{
    // On lit un format du type : 0j_01h00m
    // déjà préparé dans le squelette
    char chaine[40];
    int jours, heures, minutes;
    char j, m, h, underscore;
    is >> jours >> j >> underscore >> heures >> h >> minutes >> m;

    // Vérifications basiques de format
    assert(j=='j');
    assert(underscore=='_');
    assert(h=='h' && m=='m');

    // Utiliser jours, heures, minutes pour modifier l'objet d.
    d.secondes = Date::convertirEnSecondes(jours, heures, minutes);

    return is;
}