/*  INF3105 - Structures de données et algorithmes *
 *  UQAM - Département d'informatique              *
 *  Automne 2025 - TP4                             *
 *  Auteur(s):                                     *
 *                                                 *
 *                                                 */

#if !defined(__SUCC_H__)
#define __SUCC_H__

#include <string>
#include "arbreavl.h"
#include "date.h"
#include "pointst.h"

using namespace std;

class Succursale{
  public:
    Succursale();
    Succursale(const string& nom,
               const PointST& position,
               int nbvoitures_initial,
               int nbplaceslibres_initial);

    const string& getNom() const { return nom; }
    const PointST& getPosition() const { return position; }
    int getCapacite() const { return nbplaces; }

    // ----- Cas origine = destination -----
    // Vérifie s'il reste au moins 1 voiture sur [debut, fin)
    bool verifierMemeSuccursale(const Date& debut, const Date& fin) const;

    // Applique la réservation : -1 voiture sur [debut, fin)
    void enregistrerMemeSuccursale(const Date& debut, const Date& fin);

    // ----- Cas origine != destination -----
    // Vérifie qu'il reste >= 1 voiture à partir de 'debut' jusqu'à +inf
    bool verifierDepart(const Date& debut) const;

    // Applique le départ : à partir de 'debut', -1 voiture
    void enregistrerDepart(const Date& debut);

    // Vérifie qu'il y a < nbplaces voitures à partir de 'fin' jusqu'à +inf
    bool verifierArrivee(const Date& fin) const;

    // Applique l'arrivée : à partir de 'fin', +1 voiture
    void enregistrerArrivee(const Date& fin);

  private:
    // Point dans l'agenda : à partir de 'date', il y a 'voitures' voitures.
    struct PointAgenda {
        Date date;
        int voitures;

        bool operator<(const PointAgenda& autre) const {
            return date < autre.date;
        }
    };

    string  nom;
    PointST position;
    int     nbplaces;               // nombre total de places (voitures + places libres)
    ArbreAVL<PointAgenda> agenda;   // agenda des voitures présentes

    // S'assure qu'il existe une entrée à la date donnée.
    // Si elle n'existe pas, on crée une clé à 'date' avec la valeur précédente.
    void assurerPoint(const Date& date);

    // Vérifie que sur [debut, fin) ou [debut, +inf) le nombre de voitures
    // reste dans [minVoitures, maxVoitures].
    // fin == nullptr => +inf
    bool verifierIntervalle(const Date& debut,
                            const Date* fin,
                            int minVoitures,
                            int maxVoitures) const;

    // Applique un delta (+1 ou -1) sur [debut, fin) ou [debut, +inf)
    void appliquerDelta(const Date& debut,
                        const Date* fin,
                        int delta);
};

#endif