#include "automaton.h"
#include <iostream>
#include <vector>

using namespace std;

// Constructeur à partir de vectors
Automaton::Automaton(const vector<int>& init_states,
                     const vector<tuple<int, char, int>>& transitions,
                     const vector<int>& final_states) : nb_states(0) {
    add_init(init_states);
    add_trans(transitions);
    add_final(final_states);
}

// Retourne le nombre d'états de l'automate
int Automaton::size() const {
    return nb_states;
}

// Ajoute un nouvel état
int Automaton::newstate() {
    nb_states++;
    return nb_states - 1;
}

// Ajoute un état initial
void Automaton::add_init(int e) {
    inits.add(e);
    if (e >= nb_states) {
        nb_states = e + 1;
    }
}

// Ajoute plusieurs états initiaux
void Automaton::add_init(const vector<int>& states) {
    for (int e : states) {
        add_init(e);
    }
}

const IdxSet<int>& Automaton::get_inits() const {
    return inits;
}

const IdxSet<tuple<int, char, int>>& Automaton::get_trans() const {
    return transitions;
}

// Ajoute un état final
void Automaton::add_final(int e) {
    finals.add(e);
    if (e >= nb_states) {
        nb_states = e + 1;
    }
}

// Ajoute plusieurs états finaux
void Automaton::add_final(const vector<int>& states) {
    for (int e : states) {
        add_final(e);
    }
}

// Vérifie si un état est final
bool Automaton::is_final(int q) const {
    return finals.mem(q);
}

// Ajoute une lettre à l'alphabet
void Automaton::add_letter(char c) {
    alphabet.add(c);
}

// Ajoute plusieurs lettres à l'alphabet
void Automaton::add_letter(const vector<char>& letters) {
    for (char c : letters) {
        add_letter(c);
    }
}

// Ajoute plusieurs lettres à l'alphabet via IdxSet
void Automaton::add_letter(const IdxSet<char>& letters) {
    for (char c : letters) {
        add_letter(c);
    }
}

const IdxSet<char>& Automaton::get_alphabet() const {
    return alphabet;
}

// Ajoute une transition
void Automaton::add_trans(int src, char c, int dst) {
    transitions.add({src, c, dst});

    if (src >= nb_states) {
        nb_states = src + 1;
    }
    if (dst >= nb_states) {
        nb_states = dst + 1;
    }
    add_letter(c);
}

// Ajoute plusieurs transitions via un vecteur de tuples
void Automaton::add_trans(const vector<tuple<int, char, int>>& trans) {
    for (const auto& t : trans) {
        add_trans(get<0>(t), get<1>(t), get<2>(t));
    }
}
void Automaton::add_trans(const IdxSet<std::tuple<int, char, int>>& trans) {
    for (const auto& t : trans) {
        // Appelle la version à trois arguments : add_trans(source, label, destination)
        add_trans(std::get<0>(t), std::get<1>(t), std::get<2>(t));
    }
}

// Ajoute plusieurs transitions avec les mêmes source et destination
void Automaton::add_trans(int src, const vector<char> letters, int dst) {
    for (char c : letters) {
        add_trans(src, c, dst);
    }
}

void Automaton::add_trans(int src, const IdxSet<char> letters, int dst) {
    for (char c : letters) {
        add_trans(src, c, dst);
    }
}

const IdxSet<int>& Automaton::get_finals() const {
    return finals;
}

// Retourne les lettres sortantes d'un état
IdxSet<char> Automaton::out_letters(int q) const {
    IdxSet<char> letters;
    for (const auto& t : transitions) {
        if (get<0>(t) == q) {
            letters.add(get<1>(t));
        }
    }
    return letters;
}

// Retourne les états accessibles depuis un état avec une lettre donnée
IdxSet<int> Automaton::out_states(int q, char c) const {
    IdxSet<int> states;
    for (const auto& t : transitions) {
        if (get<0>(t) == q && get<1>(t) == c) {
            states.add(get<2>(t));
        }
    }
    return states;
}
//===================== A faire 8 =============
IdxSet<char> Automaton::in_letters(int q)const {
    IdxSet<char> result;
    for(auto t : transitions){
        if (get<2>(t) == q) result.add(get<1>(t));
    }
    return result;
}

IdxSet<int> Automaton::in_states(char c, int q) const {
    IdxSet<int> result;
    for (auto t : transitions) {
        if (get<1>(t) == c && get<2>(t) == q) result.add(get<0>(t));
    }
    return result;
}
//===============================================
// Fonctions d'affichage
void Automaton::print() const {
    cout << "Number of states: " << nb_states << endl;
    cout << "Initial states: " << inits << endl;
    cout << "Alphabet: " << alphabet << endl;
    cout << "Transitions: {" << endl;
    for (const auto& t : transitions) {
        cout << "  " << get<0>(t) << " -" << get<1>(t)
        << "-> " << get<2>(t) << endl;
    }
    cout << "}" << endl;
    cout << "Final states: " << finals << endl;
}

void Automaton::print(function<string(int)> st2String) const {
    cout << "Number of states: " << nb_states << endl;

    cout << "Initial states: {";
    bool first = true;
    for (const auto& q : inits) {
        if (!first) cout << ", ";
        cout << st2String(q);
        first = false;
    }
    cout << "}" << endl;

    cout << "Alphabet: " << alphabet << endl;

    cout << "Transitions: {" << endl;
    for (const auto& t : transitions) {
        cout << "  " << st2String(get<0>(t)) << " -" << get<1>(t)
        << "-> " << st2String(get<2>(t)) << endl;
    }
    cout << "}" << endl;

    cout << "Final states: {";
    first = true;
    for (const auto& q : finals) {
        if (!first) cout << ", ";
        cout << st2String(q);
        first = false;
    }
    cout << "}" << endl;
}


// Méthodes d'itérateur
Automaton::StateIterator Automaton::begin() const {
    return StateIterator(0);
}

Automaton::StateIterator Automaton::end() const {
    return StateIterator(nb_states);
}
