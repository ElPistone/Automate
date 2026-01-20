/**
 * @author Gérard Cécé
 * Et utilisation version gratuite de ChatGPT pour rédiger plus
 * rapidement, ou une première version de, certaines parties.
 * @date 2024-09-27
 * @brief Classe Automaton de gestion d'automates finis non déterministe
 *
 *
 */

#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <iostream>
#include "idxset.h"
#include <tuple>
#include <functional>    // Pour std::function
#include <string>        // Pour std::string et std::to_string

using namespace std;

// Pour les epsilon-transitions
const char epsilon = 0;

// Classe Automaton
class Automaton {
private:
    int nb_states;                  // Nombre d'états. L'ensemble des états
    // est {0,..., nb_states -1 }
    IdxSet<int> inits;              // Ensemble des états initiaux
    IdxSet<char> alphabet;
    IdxSet<std::tuple<int, char, int>> transitions; // Ensemble des transitions
    IdxSet<int> finals;             // Ensemble des états finaux

public:
    // Constructeur par défaut : ensembles vides, nb_states = 0
    Automaton() : nb_states(0) {}

    // Constructeur à partir de vectors
    Automaton(const std::vector<int>& init_states,
              const std::vector<std::tuple<int, char, int>>& transitions,
              const std::vector<int>& final_states);

    // Retourne le nombre d'états de l'automate
    int size() const;

    // Ajoute à l'automate un nouvel état et le retourne.
    int newstate();

    // Ajoute un entier e comme état initial de l'automate.
    void add_init(int e);

    // ajoute plusieurs états initiaux via un vector de int
    void add_init(const std::vector<int>& states);

    const IdxSet<int>& get_inits() const;

    const IdxSet<std::tuple<int, char, int>>& get_trans() const;

    // Fonction membre pour ajouter un état final
    void add_final(int e);

    // ajoute plusieurs états finaux via un vector de int
    void add_final(const std::vector<int>& states);

    bool is_final(int q) const;

    // Fonction membre pour ajouter une lettre à l'alphabet
    void add_letter(char c);

    // ajoute plusieurs lettres à l'alphabet via un vector de char
    void add_letter(const std::vector<char>& letters);

    // ajoute plusieurs lettres à l'alphabet via un IdxSet<char>
    void add_letter(const IdxSet<char>& letters);

    const IdxSet<char>& get_alphabet() const;

    // Fonction membre pour ajouter une transition src -c-> dst
    void add_trans(int src, char c, int dst);

    // ajoute plusieurs transitions via un vecteur de tuples
    void add_trans(const std::vector<std::tuple<int, char, int>>& transitions);
    void add_trans(const IdxSet<std::tuple<int, char, int>>& transition);

    // Pour pouvoir écrire add_trans(src, {'a','b','c'},dst)
    void add_trans(int src, const std::vector<char> letters, int dst);

    void add_trans(int src, const IdxSet<char> letters, int dst);

    const IdxSet<int>& get_finals() const;

    // out_letters(q) retourne un IdxSet<char> contenant tous les caractères c
    IdxSet<char> out_letters(int q) const;

    // out_states(q,c) retourne un IdxSet contenant tous les int dst
    IdxSet<int> out_states(int q, char c) const;

    // in_letters(q) retourne un IdxSet contenant tous les caractères c tels qu'ils existent
    // dans l'automate une transition de p-c->q
    IdxSet<char> in_letters(int q) const;

    //in_states (c,q) retourne un IdxSet contenant tous les états tel qu'il existe dans l'automate
    // une transition de p-c->q
    IdxSet<int> in_states(char c, int q) const;

    // Fonction pour afficher l'automate
    void print() const;

    void print(std::function<std::string(int)> st2String) const;

    // Classe d'itérateurs pour parcourir simplement les états d'un automate
    class StateIterator {
    private:
        int current;  // L'état actuel que l'itérateur parcourt

    public:
        // Constructeur de l'itérateur
        StateIterator(int start) : current(start) {}

        // Surcharge de l'opérateur * (déréférencement)
        int operator*() const {
            return current;
        }

        // Surcharge de l'opérateur ++ (incrémentation)
        StateIterator& operator++() {
            ++current;
            return *this;
        }

        // Surcharge de l'opérateur != (comparaison de fin d'itération)
        bool operator!=(const StateIterator& other) const {
            return current != other.current;
        }
    };

    // Méthode begin() pour obtenir l'itérateur au premier état
    StateIterator begin() const;

    // Méthode end() pour obtenir l'itérateur après le dernier état
    StateIterator end() const;
};

#endif // AUTOMATON_H
