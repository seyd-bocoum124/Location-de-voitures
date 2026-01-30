/* Squelette pour classe générique ArbreMap<K, V>.
 * Lab8 -- Dictionnaires et Arbres binaires de recherche
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 * http://ericbeaudry.uqam.ca/INF3105/lab8/
 *
 */
#if !defined(__ARBREMAP_H__)
#define __ARBREMAP_H__

#include "arbreavl.h"
#include <cassert>

template <class K, class V>
class ArbreMap {

  private:
    // Entrée (clé, valeur) stockée dans l'AVL.
    struct Entree {
        K cle;
        V valeur;

        Entree() : cle(), valeur() {}
        Entree(const K& c) : cle(c), valeur() {}
        Entree(const K& c, const V& v) : cle(c), valeur(v) {}

        // Comparaison basée uniquement sur la clé (pour l'AVL)
        bool operator<(const Entree& autre) const {
            return cle < autre.cle;
        }
    };

    ArbreAVL<Entree> entrees;

  public:
    bool contient(const K&) const;

    void enlever(const K&);
    void vider();
    bool vide() const;

    const V& operator[] (const K&) const;
    V& operator[] (const K&);
};

template <class K, class V>
void ArbreMap<K,V>::vider(){
    entrees.vider();
}

template <class K, class V>
bool ArbreMap<K,V>::vide() const{
    return entrees.vide();
}

template <class K, class V>
void ArbreMap<K,V>::enlever(const K& c)
{
    Entree e(c);
    entrees.enlever(e);
}

template <class K, class V>
bool ArbreMap<K,V>::contient(const K& c) const
{
    Entree e(c);
    return entrees.contient(e);
}

template <class K, class V>
const V& ArbreMap<K,V>::operator[] (const K& c) const
{
    Entree e(c);
    typename ArbreAVL<Entree>::Iterateur it = entrees.rechercher(e);
    assert(it);  // En const, on suppose que la clé existe
    const Entree& entree = entrees[it];
    return entree.valeur;
}

template <class K, class V>
V& ArbreMap<K,V>::operator[] (const K& c) 
{
    Entree e(c);
    typename ArbreAVL<Entree>::Iterateur it = entrees.rechercher(e);

    if(!it){
        // La clé n'existe pas : on insère une entrée avec valeur par défaut
        entrees.inserer(e);
        it = entrees.rechercher(e);
        assert(it);
    }

    Entree& entree = entrees[it];
    return entree.valeur;
}

#endif