/*  INF3105 - Structures de données et algorithmes
 *  UQAM / Département d'informatique
 *  Automne 2025 / TP2 et TP4
 *  http://ericbeaudry.uqam.ca/INF3105/tp4/
 *
 *  Fichier pointst.h © Éric Beaudry.
 *
*/
#if !defined(_POINTST__H_)
#define _POINTST__H_

#include <iostream>

// Cette classe représente un point sur la surface de la Terre.
class PointST {

  public:
    PointST(){}
    PointST(double latitude_, double longitude_); // angles en radians
    PointST(const PointST&);

  private:
    double latitude;  // angle en radians
    double longitude; // angle en radians

  // Les opérateurs << et >> écrit et lit du texte en degrés et fait la conversion en radians.
  friend std::ostream& operator << (std::ostream&, const PointST&);
  friend std::istream& operator >> (std::istream&, PointST&);

  friend double distance(const PointST&, const PointST&);
};

double distance(const PointST&, const PointST&);

#endif

