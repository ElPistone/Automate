/**
 * @author Gérard Cécé
 * Et utilisation version gratuite de ChatGPT pour rédiger plus
 * rapidement, ou une première version de, certaines parties.
 * @date 2024-09-27
 * @brief Classe IdxSet de manipulation simplifiée d'ensemble dont les éléments sont indexés.
 *
 *
 */

#ifndef IDXSET_H
#define IDXSET_H

#include <iostream>
#include <vector>
#include <stdexcept> // Pour std::out_of_range
#include <algorithm> // Pour std::find

// Pour tout objet mis dans un IdxSet, l'opérateur == DOIT avoir été redéfini
// pour ne pas simplement tester une égalité d'adresses mémoires mais une
// égalité des contenus des objets.
template <typename T>
class IdxSet {
private:
    std::vector<T> elements;  // Stockage interne des éléments de l'ensemble

public:
    // Constructeur par défaut, l'ensemble est vide.
    IdxSet() {}

    // Constructeur à partir d'un vector<T>
    IdxSet(const std::vector<T>& vec) {
        add(vec); // Défini ci-dessous
    }

    // Constructeur à partir d'un autre IdxSet<T>
    IdxSet(const IdxSet<T>& set) {
        elements = set.elements; // Il s'agit ici d'une copie
        // des éléments.
    }

    // Fonction mem(e) utilisant l'éventuelle redéfinition de ==
    bool mem(const T& e) const {
        for (const T& elem : elements) {
            if (elem == e) {   // Attention, == doit avoir été redéfini pour
                // ne pas être égal à une simple identité d'adresse
                return true; // L'élément est trouvé dans l'ensemble
            }
        }
        return false; // L'élément n'a pas été trouvé
    }

    // Fonction add(e)
    void add(const T& e) {
        if (!mem(e)) {
            elements.push_back(e); // Ajoute l'élément s'il n'est pas déjà présent
        }
    }

    // Pour ajouter le contenu d'un vector
    void add(const std::vector<T>& vec) {
        for (const T& e : vec) {
            add(e);  // Ajoute chaque élément du vector à l'ensemble via la méthode add
        }
    }

    // Nouvelle variante de add pour ajouter le contenu d'un
    // autre IdxSet. Equivaut à une union.
    void add(const IdxSet<T>& other) {
        for (const T& elem : other.elements) { // Itérer sur chaque élément de l'autre IdxSet
            add(elem); // utilise la méthode add d'un élément.
        }
    }

    // Retire un élément de l'ensemble sous-jacent
    void remove(const T& e) {
        // Trouver l'élément e dans le vecteur elements
        auto it = std::find(elements.begin(), elements.end(), e);

        // Si l'élément est trouvé, on le supprime
        if (it != elements.end()) {
            elements.erase(it);
        }
    }

    // Retire de l'ensemble sous-jacent tous les éléments du vector v.
    void remove(const std::vector<T>& v) {
        for (const T& e : v) {
            remove(e);  // Appelle la méthode remove(e) pour chaque élément du vector
        }
    }

    // Retire de l'ensemble sous-jacent tous les éléments de l'ensemble set.
    void remove(const IdxSet<T>& set) {
        for (const T& e : set.elements) {
            remove(e);  // Appelle la méthode remove(e) pour chaque élément du IdxSet
        }
    }

    // Retourne le nombre d'éléments dans l'ensemble
    size_t size() const {
        return elements.size();  // Utilise la méthode size() du std::vector
        // pour renvoyer le nombre d'éléments
    }

    // retourne le ième élément de l'ensemble
    // Envoie l'exception std::out_of_range si les bornes ne sont pas
    // respectée : ensemble non vide et 0 < i < size()
    const T& at(size_t i) const {
        return elements.at(i);
    }

    // Est-ce que l'ensemble est vide ?
    bool is_empty() const {
        return elements.empty();  // Utilise la méthode empty() du std::vector pour vérifier si l'ensemble est vide
    }


    // Fonction index(e) utilisant equals : retourne l'index de e ou lève
    // l'exception std::out_of_range si e ne fait pas partie de l'ensemble.
    // DEVRAIT ETRE INVALIDE SI REMOVE d'un élément qui n'était pas le dernier.
    int index(const T& e) const {
        for (size_t i = 0; i < elements.size(); ++i) {
            if (elements[i] == e) {
                return static_cast<int>(i); // Retourne l'index de e
            }
        }
        throw std::out_of_range("L'élément n'est pas dans l'ensemble."); // Lève une exception si e n'est pas trouvé
    }


    // Ajoute un élément à l'ensemble avant de retourner son index.
    // Si l'élément fait déjà parti de l'ensemble, retourne simplement
    // son index. S'il ne fait pas encore parti de l'ensemble, l'ajoute
    // avant de retourner son index.
    int addindex(const T& e) {
        try {
            return index(e);
            // Si cela a fonctionné, l'élément faisait déjà parti de l'ensemble.
        }
        catch (const std::out_of_range& ex) {
            elements.push_back(e); // Ajoute l'élément car il n'est pas déjà présent
            return (size() - 1);  // son index est donc le dernier, on commence à 0.
        }
    }


    // Vide l'ensemble.
    void clear() {
        elements.clear();
    }



    // Fonction choose() : retire et retourne le dernier élément
    T choose() {
        if (elements.empty()) {
            throw std::out_of_range("L'ensemble est vide, aucun élément à retirer.");
        }
        T lastElement = elements.back(); // Récupère le dernier élément
        elements.pop_back(); // Retire le dernier élément
        return lastElement; // Retourne le dernier élément retiré
    }


    // Fonction see_one() : retourne, sans le supprimmer, un élément de l'ensemble
    // exeption out_of_range si l'ensemble est vide.
    T see_one() const {
        if (elements.empty()) {
            throw std::out_of_range("L'ensemble est vide, aucun élément à voir.");
        }
        return elements.back(); // Retourne le dernier élément
    }


    // Redéfinition de ==. Deux ensembles sont égaux si ils ont le même nombre
    // d'éléments et tout élément du premier appartient au second.
    // Cette redéfinition est nécessaire si nous devons créer des ensembles
    // d'ensembles, comme : IdxSet<IdxSet<int>> ensens;
    bool operator==(const IdxSet<T>& other) const {
        // Vérifier si les deux ensembles ont le même nombre d'éléments
        if (elements.size() != other.elements.size()) {
            return false;
        }

        // Vérifier si tous les éléments de *this sont dans other
        for (const T& elem : elements) {
            if (!other.mem(elem)) { // Utiliser la méthode mem pour vérifier la présence
                return false;
            }
        }

        // Si toutes les conditions sont remplies, les ensembles sont égaux
        return true;
    }


    // Pour les itérateurs, notamment dans les for
    // Méthode pour obtenir le début de l'ensemble
    typename std::vector<T>::iterator begin() {
        return elements.begin();
    }

    // Méthode pour obtenir la fin de l'ensemble
    typename std::vector<T>::iterator end() {
        return elements.end();
    }

    // Dans les cas de parcours de l'ensemble par une fonction qualifiée de const
    // Méthode pour obtenir le début de l'ensemble (const)
    typename std::vector<T>::const_iterator begin() const {
        return elements.begin();
    }

    // Méthode pour obtenir la fin de l'ensemble (const)
    typename std::vector<T>::const_iterator end() const {
        return elements.end();
    }


    // Pour pouvoir écrire cout << ens pour ens un IdxSet<T> dès qu'on peut
    // écrire cout << e pour e un T.
    //template <typename T>
    friend std::ostream& operator<<(std::ostream& os, const IdxSet<T>& set) {
        os << "{";
        bool first = true;
        for (const auto& elem : set.elements) {
            if (!first) {
                os << ", ";  // Séparateur entre les éléments
            }
            os << elem;  // Affichage de l'élément (requiert que T ait une surcharge de <<)
            first = false;
        }
        os << "}";
        return os;
    }


    // Méthode pour afficher les éléments de l'ensemble
    void print() const {
        std::cout << "{ ";
        for (const T& elem : elements) {
            std::cout << elem << " "; // Supposer que l'opérateur << est surchargé pour T
        }
        std::cout << "}" << std::endl;
    }
};

#endif // IDXSET_H
