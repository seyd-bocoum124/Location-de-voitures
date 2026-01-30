/* Squelette pour classe générique ArbreAVL<T>.
 * Lab6 et Lab7 -- Arbres AVL
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 *
 */
#if !defined(_ARBREAVL___H_)
#define _ARBREAVL___H_

#include <cassert>
#include <iostream>
#include "pile.h"

template <class T>
class ArbreAVL {
  private:
    struct Noeud{
        Noeud(const T& c)
            : contenu(c), equilibre(0), gauche(nullptr), droite(nullptr) {}
        T contenu;
        int equilibre;   // hauteur(gauche) - hauteur(droite)
        Noeud *gauche,
              *droite;
    };

    Noeud* racine;

    // Fonctions internes
    bool inserer(Noeud*&, const T&);
    void rotationGaucheDroite(Noeud*&);
    void rotationDroiteGauche(Noeud*&);
    void vider(Noeud*&);
    void copier(const Noeud*, Noeud*&) const;

    int hauteur(const Noeud*) const;
    int compter(const Noeud*) const;
    void preparer_afficher(const Noeud* n, int profondeur, int& rang,
                           T* elements, int* profondeurs) const;

  public:
    // ==================== Iterateur ====================
    class Iterateur{
      public:
        Iterateur(const ArbreAVL& a)
            : arbre_associe(a), courant(nullptr) {}

        Iterateur(const Iterateur& a)
            : arbre_associe(a.arbre_associe),
              courant(a.courant),
              chemin(a.chemin) {}

        // Construit un itérateur positionné sur le noeud c
        // en reconstruisant le chemin depuis la racine.
        Iterateur(const ArbreAVL& a, Noeud* c)
            : arbre_associe(a), courant(c)
        {
            if(!c) return; // fin()

            Noeud* n = a.racine;
            while(n && n != c){
                chemin.empiler(n);          // ancêtre
                if(c->contenu < n->contenu)
                    n = n->gauche;
                else
                    n = n->droite;
            }
        }

        operator bool() const { return courant!=nullptr; }
        bool operator!() const { return courant==nullptr; }

        bool operator==(const Iterateur& o) const {
            assert(&arbre_associe==&o.arbre_associe);
            return courant==o.courant;
        }
        bool operator!=(const Iterateur& o) const {
            assert(&arbre_associe==&o.arbre_associe);
            return courant!=o.courant;
        }

        const T& operator*() const {
            assert(courant);
            return courant->contenu;
        }

        Iterateur& operator++();    // in-order, O(1) amorti
        Iterateur operator++(int){ Iterateur copie(*this); ++(*this); return copie; }

        Iterateur& operator=(const Iterateur& autre){
            assert(&arbre_associe==&autre.arbre_associe);
            courant = autre.courant;
            chemin  = autre.chemin;
            return *this;
        }

      private:
        const ArbreAVL& arbre_associe;
        Noeud* courant;
        // chemin = ancêtres de "courant" (du plus proche au plus lointain)
        Pile<Noeud*> chemin;

      friend class ArbreAVL;
    };

  public:
    ArbreAVL() : racine(nullptr) {}
    ArbreAVL(const ArbreAVL& autre) : racine(nullptr) { copier(autre.racine, racine); }
    ~ArbreAVL() { vider(racine); }

    bool vide() const { return racine==nullptr; }
    bool contient(const T&) const;
    void inserer(const T&);
    void vider() { vider(racine); }

    void enlever(const T&);  // enlèvement simple (sans rééquilibrage complet)

    ArbreAVL& operator=(const ArbreAVL& autre){
        if(this!=&autre){
            vider(racine);
            copier(autre.racine, racine);
        }
        return *this;
    }

    int compter() const { return compter(racine); }
    int hauteur() const { return hauteur(racine); }
    void afficher() const;

    // Itérateurs et recherches
    Iterateur debut() const;
    Iterateur fin() const { return Iterateur(*this,nullptr); }
    Iterateur rechercher(const T&) const;
    Iterateur rechercherEgalOuSuivant(const T&) const;
    Iterateur rechercherEgalOuPrecedent(const T&) const;

    const T& operator[](const Iterateur& it) const {
        assert(&it.arbre_associe == this);
        assert(it.courant);
        return it.courant->contenu;
    }
    T& operator[](const Iterateur& it){
        assert(&it.arbre_associe == this);
        assert(it.courant);
        return it.courant->contenu;
    }
};

// ===============================================================
//                  DÉFINITIONS ARBRE AVL
// ===============================================================

template <class T>
bool ArbreAVL<T>::contient(const T& element) const
{
    Noeud* n = racine;
    while(n){
        if(element < n->contenu)
            n = n->gauche;
        else if(n->contenu < element)
            n = n->droite;
        else
            return true;
    }
    return false;
}

template <class T>
void ArbreAVL<T>::inserer(const T& element)
{
    inserer(racine, element);
}

template <class T>
bool ArbreAVL<T>::inserer(Noeud*& noeud, const T& element)
{
    if(noeud==nullptr){
        noeud = new Noeud(element);
        return true;
    }

    if(element < noeud->contenu){
        if(inserer(noeud->gauche, element)){
            noeud->equilibre++;
            if(noeud->equilibre == 0)
                return false;
            if(noeud->equilibre == 1)
                return true;
            // equilibre == 2
            if(noeud->gauche->equilibre < 0)
                rotationDroiteGauche(noeud->gauche);
            rotationGaucheDroite(noeud);
        }
        return false;
    }else if(noeud->contenu < element){
        if(inserer(noeud->droite, element)){
            noeud->equilibre--;
            if(noeud->equilibre == 0)
                return false;
            if(noeud->equilibre == -1)
                return true;
            // equilibre == -2
            if(noeud->droite->equilibre > 0)
                rotationGaucheDroite(noeud->droite);
            rotationDroiteGauche(noeud);
        }
        return false;
    }

    // element == noeud->contenu : mise à jour du contenu
    noeud->contenu = element;
    return false;
}

template <class T>
void ArbreAVL<T>::rotationGaucheDroite(Noeud*& b)
{
    Noeud* a = b->gauche;
    int ea = a->equilibre;
    int eb = b->equilibre;
    int neb = eb - 1 - (ea > 0 ? ea : 0);
    int nea = ea - 1 + (neb < 0 ? neb : 0);

    b->gauche = a->droite;
    a->droite = b;
    b->equilibre = neb;
    a->equilibre = nea;
    b = a;
}

template <class T>
void ArbreAVL<T>::rotationDroiteGauche(Noeud*& b)
{
    Noeud* a = b->droite;
    int ea = a->equilibre;
    int eb = b->equilibre;
    int neb = eb + 1 - (ea < 0 ? -ea : 0);
    int nea = ea + 1 + (neb > 0 ? neb : 0);

    b->droite = a->gauche;
    a->gauche = b;
    b->equilibre = neb;
    a->equilibre = nea;
    b = a;
}

template <class T>
void ArbreAVL<T>::vider(Noeud*& noeud)
{
    if(noeud){
        vider(noeud->gauche);
        vider(noeud->droite);
        delete noeud;
        noeud = nullptr;
    }
}

template <class T>
void ArbreAVL<T>::copier(const Noeud* source, Noeud*& noeud) const
{
    if(!source){
        noeud = nullptr;
        return;
    }
    noeud = new Noeud(source->contenu);
    noeud->equilibre = source->equilibre;
    copier(source->gauche, noeud->gauche);
    copier(source->droite, noeud->droite);
}

template <class T>
int ArbreAVL<T>::hauteur(const Noeud* n) const
{
    if(!n) return 0;
    int hg = hauteur(n->gauche);
    int hd = hauteur(n->droite);
    return (hg>hd ? hg : hd) + 1;
}

template <class T>
int ArbreAVL<T>::compter(const Noeud* n) const
{
    if(!n) return 0;
    return 1 + compter(n->gauche) + compter(n->droite);
}

// ====================== Enlever (simple) ======================
template <class T>
void ArbreAVL<T>::enlever(const T& element)
{
    Noeud** lien = &racine;
    Noeud* n = racine;

    while(n){
        if(element < n->contenu){
            lien = &n->gauche;
            n = n->gauche;
        }else if(n->contenu < element){
            lien = &n->droite;
            n = n->droite;
        }else
            break;
    }
    if(!n) return;

    if(!n->gauche || !n->droite){
        Noeud* enfant = n->gauche ? n->gauche : n->droite;
        *lien = enfant;
        delete n;
        return;
    }

    // Deux enfants : on prend le successeur (min du sous-arbre droit)
    Noeud** lienSucc = &n->droite;
    Noeud* succ = n->droite;
    while(succ->gauche){
        lienSucc = &succ->gauche;
        succ = succ->gauche;
    }
    n->contenu = succ->contenu;
    n->equilibre = succ->equilibre;
    *lienSucc = succ->droite;
    delete succ;
}

// ====================== Affichage (fourni) ====================

template <class T>
void ArbreAVL<T>::afficher() const{
    int n = compter();
    T* elements = new T[n];
    int* profondeurs = new int[n];
    int rang = 0;
    preparer_afficher(racine, 0, rang, elements, profondeurs);
    int col = 0;
    for(int ligne=0;;ligne++){
        int i;
        for(i=0;i<n;i++){
            if(ligne==profondeurs[i]){
                while(col++<i*2)
                    std::cout << ' ';
                col += 2;
                std::cout << elements[i];
            }
        }
        if(i==n) break;
        std::cout << std::endl;
    }
    delete[] elements;
    delete[] profondeurs;
}

template <class T>
void ArbreAVL<T>::preparer_afficher(const Noeud* n, int profondeur,
                                    int& rang, T* elements, int* profondeurs) const{
    if(n==nullptr) return;
    preparer_afficher(n->gauche, profondeur+1, rang, elements, profondeurs);
    elements[rang] = n->contenu;
    profondeurs[rang] = profondeur;
    rang++;
    preparer_afficher(n->droite, profondeur+1, rang, elements, profondeurs);
}

// ===============================================================
//                    ITÉRATEUR & RECHERCHES
// ===============================================================

template <class T>
typename ArbreAVL<T>::Iterateur ArbreAVL<T>::debut() const
{
    Iterateur it(*this);
    Noeud* n = racine;

    // Construire chemin jusqu'au plus petit élément
    while(n){
        it.chemin.empiler(n);
        n = n->gauche;
    }

    if(!it.chemin.vide())
        it.courant = it.chemin.depiler();
    else
        it.courant = nullptr;

    return it;
}

template <class T>
typename ArbreAVL<T>::Iterateur
ArbreAVL<T>::rechercher(const T& e) const
{
    Iterateur it(*this);
    Noeud* n = racine;

    while(n){
        if(e < n->contenu){
            it.chemin.empiler(n);      // n devient un ancêtre potentiel
            n = n->gauche;
        }else if(n->contenu < e){
            it.chemin.empiler(n);
            n = n->droite;
        }else{
            it.courant = n;            // trouvé
            return it;
        }
    }

    // pas trouvé
    it.courant = nullptr;
    it.chemin = Pile<Noeud*>();        // on vide proprement la pile
    return it;
}

template <class T>
typename ArbreAVL<T>::Iterateur
ArbreAVL<T>::rechercherEgalOuSuivant(const T& e) const
{
    Noeud* n = racine;
    Noeud* candidat = nullptr;
    while(n){
        if(e < n->contenu){
            candidat = n;
            n = n->gauche;
        }else if(n->contenu < e){
            n = n->droite;
        }else{
            candidat = n;
            break;
        }
    }
    return Iterateur(*this, candidat);
}

template <class T>
typename ArbreAVL<T>::Iterateur
ArbreAVL<T>::rechercherEgalOuPrecedent(const T& e) const
{
    Noeud* n = racine;
    Noeud* candidat = nullptr;
    while(n){
        if(e < n->contenu){
            n = n->gauche;
        }else if(n->contenu < e){
            candidat = n;
            n = n->droite;
        }else{
            candidat = n;
            break;
        }
    }
    return Iterateur(*this, candidat);
}

// ++ optimisé : successeur in-order, O(1) amorti, en utilisant
// uniquement empiler/depiler de Pile (pas de sommet()).
template <class T>
typename ArbreAVL<T>::Iterateur&
ArbreAVL<T>::Iterateur::operator++()
{
    if(!courant) return *this;

    // 1) S'il y a un sous-arbre droit, le successeur est le plus petit
    //    élément de ce sous-arbre.
    if(courant->droite){
        // IMPORTANT : le courant devient aussi un ancêtre
        chemin.empiler(courant);
        Noeud* n = courant->droite;
        while(n){
            chemin.empiler(n);
            n = n->gauche;
        }
        courant = chemin.depiler();   // plus à gauche du sous-arbre droit
        return *this;
    }

    // 2) Sinon, on remonte par la pile d'ancêtres jusqu'à trouver
    //    un parent pour lequel on vient du fils gauche.
    while(!chemin.vide()){
        Noeud* parent = chemin.depiler();
        if(parent->gauche == courant){
            courant = parent;
            return *this;
        }
        courant = parent;
    }

    // 3) Plus de successeur : fin()
    courant = nullptr;
    return *this;
}

#endif