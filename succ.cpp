/*  INF3105 - Structures de données et algorithmes *
 *  UQAM - Département d'informatique              *
 *  Automne 2025 - TP4                             *
 *                                                 */

#include "succ.h"
#include <cassert>

// ----------------------------------------------------------------------
// Constructeurs
// ----------------------------------------------------------------------

Succursale::Succursale()
    : nom(), position(), nbplaces(0)
{
}

Succursale::Succursale(const std::string& n,
                       const PointST& pos,
                       int nbvoitures_initial,
                       int nbplaceslibres_initial)
    : nom(n),
      position(pos),
      nbplaces(nbvoitures_initial + nbplaceslibres_initial)
{
    // Au temps 0j_00h00m, il y a nbvoitures_initial voitures à la succursale.
    PointAgenda p;
    p.date = Date(0, 0, 0);   // 0 jour, 0h00
    p.voitures = nbvoitures_initial;
    agenda.inserer(p);
}

// ----------------------------------------------------------------------
// Vérification des réservations
// ----------------------------------------------------------------------

// Cas origine = destination
bool Succursale::verifierMemeSuccursale(const Date& debut, const Date& fin) const
{
    // On veut qu'il reste au moins 1 voiture dans la succursale
    // pour toute la durée [debut, fin).
    return verifierIntervalle(debut, &fin, 1, nbplaces);
}

void Succursale::enregistrerMemeSuccursale(const Date& debut, const Date& fin)
{
    // On consomme 1 voiture sur [debut, fin)
    appliquerDelta(debut, &fin, -1);
}

// Cas origine != destination : départ
bool Succursale::verifierDepart(const Date& debut) const
{
    // À partir de 'debut' jusqu'à +inf, il doit rester >= 1 voiture
    return verifierIntervalle(debut, nullptr, 1, nbplaces);
}

void Succursale::enregistrerDepart(const Date& debut)
{
    // À partir de 'debut', on perd une voiture
    appliquerDelta(debut, nullptr, -1);
}

// Cas origine != destination : arrivée
bool Succursale::verifierArrivee(const Date& fin) const
{
    // À partir de 'fin' jusqu'à +inf, il doit y avoir < nbplaces voitures,
    // donc voitures ∈ [0, nbplaces - 1].
    return verifierIntervalle(fin, nullptr, 0, nbplaces - 1);
}

void Succursale::enregistrerArrivee(const Date& fin)
{
    // À partir de 'fin', on gagne une voiture
    appliquerDelta(fin, nullptr, +1);
}

// ----------------------------------------------------------------------
// Gestion interne de l'agenda
// ----------------------------------------------------------------------

// S'assure qu'il existe une entrée à la date donnée.
// Si elle n'existe pas, on crée une clé à 'date' avec la valeur précédente.
void Succursale::assurerPoint(const Date& date)
{
    PointAgenda cle;
    cle.date = date;
    cle.voitures = 0;

    // On cherche la clé égale ou précédente
    ArbreAVL<PointAgenda>::Iterateur it = agenda.rechercherEgalOuPrecedent(cle);

    // Si on trouve exactement la date, rien à faire
    if (it) {
        const PointAgenda& p = *it;
        if (!(p.date < date) && !(date < p.date)) {
            // p.date == date
            return;
        }
    }

    int voitures = 0;
    if (it) {
        // On copie la valeur de la date précédente
        voitures = (*it).voitures;
    } else {
        // Pas de date précédente : on utilise la première entrée si elle existe
        ArbreAVL<PointAgenda>::Iterateur it2 = agenda.debut();
        if (it2)
            voitures = (*it2).voitures;
        else
            voitures = 0;
    }

    PointAgenda nouv;
    nouv.date = date;
    nouv.voitures = voitures;
    agenda.inserer(nouv);
}

// Vérifie que sur [debut, fin) ou [debut, +inf), le nombre de voitures
// est toujours entre minVoitures et maxVoitures (inclus).
bool Succursale::verifierIntervalle(const Date& debut,
                                    const Date* fin,
                                    int minVoitures,
                                    int maxVoitures) const
{
    if (agenda.vide()) {
        int v = 0;
        return (v >= minVoitures && v <= maxVoitures);
    }

    PointAgenda cle;
    cle.date = debut;
    cle.voitures = 0;

    ArbreAVL<PointAgenda>::Iterateur it =
        agenda.rechercherEgalOuPrecedent(cle);

    // Si aucun prédécesseur, on commence au début
    if (!it)
        it = agenda.debut();

    while (it) {
        const PointAgenda& p = *it;

        // Si on a une borne supérieure fin, on arrête dès que p.date >= fin
        if (fin && ! (p.date < *fin))
            break;

        int v = p.voitures;
        if (v < minVoitures || v > maxVoitures)
            return false;

        ++it;
    }

    return true;
}

// Applique un delta (+1 ou -1) sur [debut, fin) ou [debut, +inf).
// VERSION OPTIMISÉE : on modifie *en place* sans réinsérer dans l'arbre.
void Succursale::appliquerDelta(const Date& debut,
                                const Date* fin,
                                int delta)
{
    // On force la présence des points de rupture
    assurerPoint(debut);
    if (fin)
        assurerPoint(*fin);

    PointAgenda cle;
    cle.date = debut;
    cle.voitures = 0;

    // On commence exactement à 'debut'
    ArbreAVL<PointAgenda>::Iterateur it =
        agenda.rechercherEgalOuSuivant(cle);

    while (it) {
        PointAgenda& p = agenda[it];

        if (fin && ! (p.date < *fin))
            break;

        p.voitures += delta;

        ++it;
    }
}