#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "automaton.h"
#include "idxset.h"
#include <iostream>
#include <vector>
#include <tuple>
#include <sstream>

using namespace std;
//===================== A faire 1 ===========================
/**
 * @brief vectMeme Vérifie si un entier est présent dans un vecteur.
 *
 * @param vect Le vecteur d'entiers à parcourir.
 * @param n L'entier à rechercher dans le vecteur.
 * @return true Si l'entier est présent dans le vecteur.
 * @return false Sinon.
 */
bool vectMeme(const vector<int> &vect, int n){
    for (auto v: vect){
        if (v== n) return true;
    }
    return false;
}
//==========================================================


//===================== A faire 2 ===========================
/**
 * @brief vectIntersection Fait l'intersection entre deux vecteurs.
 *
 * @param vect1 le premier vecteur.
 * @param vect2 le deuxième vecteur.
 * @return le vecteur résultant
 */
vector <int> vectIntersection(const vector<int> &vect1, const vector<int> &vect2){
    vector <int> result;
    for (auto v: vect1){
        if (vectMeme(vect2,v))result.push_back(v);
    }
    return result;
}
//===========================================================


//===================== A faire 3 ===========================
/**
 * @brief succesors retourne l'ensemble des états de l'automate
 * qui sont des successeurs de l'un des états de srcs à travers c
 * @param aut l'automate
 * @param srcs les états sources
 * @param c l'etiquette de la transition
 * @return un ensemble d'état successeurs.
 */
IdxSet <int> succesors (const Automaton &aut, const IdxSet<int> &srcs, char c){
    IdxSet <int> result;
    for (int q : srcs){ // parcours des états initiaux
        for (int des : aut.out_states(q,c))result.add(des);
    }
    return result;
}
//===========================================================


//===================== A faire 4 ===========================
/**
 * @brief succesors retourne l'ensemble des états de l'automate
 * qui sont des successeurs de l'un des états de srcs étiquettée par le mot word
 * @param aut l'automate
 * @param srcs les états sources
 * @param word le mot
 * @return un ensemble d'état successeurs
 */
IdxSet <int> succesors (const Automaton &aut, const IdxSet<int> &srcs, const string &word){
    IdxSet <int> current = srcs; // on commence par les états initiaux
    for (char c : word) current = succesors(aut, current, c); // update après chaque lettre
    return current; // états atteints après avoir lu le mot.
}
//===========================================================


//===================== A faire 5 ===========================
/**
 * @brief appartient indique si un mot est reconnue par l'automate
 * @param aut l'automate
 * @param word mot à tester
 * @return true si le mot est reconnu
 * @return false si le mot n'est pas reconnu
 */
bool appartient(const Automaton &aut, const string &word){
    IdxSet <int> reachables = succesors(aut, aut.get_inits(), word);
    for (auto q : reachables){
        if (aut.is_final(q))return true;
    }
    return false;
}
//===========================================================


//===================== A faire 6 ===========================
/**
 * @brief succesorsStar trouve l'ensemble des états accessibles par les états contenus dsans srscs
 * @param aut l'automate
 * @param srcs  etats sources
 * @return ensemble des états accessibles
 */
IdxSet<int> succesorsStar(const Automaton &aut, const IdxSet<int> &srcs){
    IdxSet<int> reachables = srcs;// état de départ
    IdxSet<int> toBeTreated = srcs;// état à traiter
    while(!toBeTreated.is_empty()){
        int q = toBeTreated.choose();
        for (auto c: aut.out_letters(q)){// Les lettres sortantes
            for (auto qPrime : aut.out_states(q,c)){// Les états de destinations
                if (!reachables.mem(qPrime)){
                    reachables.add(qPrime);
                    toBeTreated.add(qPrime);
                }
            }
        }
    }
    return reachables;
}
//===========================================================


//===================== A faire 7 ===========================
/**
 * @brief emptyLanguage indique si le language reconnu par l'automate est vide
 * @param aut l'automate
 * @return  true si le language est vide
 * @return false si le language n'est pas vide
 */
bool emptyLanguage(Automaton aut){
    // Calculer tous les états accessibles à partir des états initiaux
    IdxSet<int> reachables = succesorsStar(aut, aut.get_inits());

    // Vérifier si au moins un état accessible est final
    for (int q : reachables) {
        if (aut.is_final(q)) {
            return false; // Le langage n'est pas vide
        }
    }

    return true; // Le langage est vide
}
//===========================================================


//===================== A faire 9 ===========================
/**
 * @brief predecessorsStar trouve l'ensemble des états co-accessibles contenus dans srcs
 * @param aut l'automate
 * @param srcs état source
 * @return
 */
IdxSet<int> predecessorsStar(const Automaton &aut, const IdxSet<int> &srcs) {
    IdxSet<int> reachables = srcs;// état de départ
    IdxSet<int> toBeTreated = srcs;// état à traiter

    while (!toBeTreated.is_empty()) {
        int q = toBeTreated.choose();

        for (char a : aut.in_letters(q)) {// lettre Entrante
            for (int q_prime : aut.in_states(a, q)) {// Etat de provenance
                if (!reachables.mem(q_prime)) {
                    reachables.add(q_prime);
                    toBeTreated.add(q_prime);
                }
            }
        }
    }
    return reachables;
}
/**
 * @brief trim trouve un automate dont tous les états sont accessibles et co-accessibles
 * @brief suppression des états inutiles
 * @param aut l'automate
 * @return un automate
 */
Automaton trim(const Automaton &aut) {
    IdxSet<int> accessible = succesorsStar(aut, aut.get_inits());
    IdxSet<int> coaccessible = predecessorsStar(aut, aut.get_finals());

    Automaton result;

    // Ajout des états utiles (intersection des accessibles et co-accessibles)
    // On conserve les identifiants d'origine pour simplifier la copie des
    // transitions.
    for (int s : accessible) {
        if (coaccessible.mem(s)) {
            if (aut.get_inits().mem(s))
                result.add_init(s);
            if (aut.get_finals().mem(s))
                result.add_final(s);
        }
    }

    // Ajout des transitions entre états utiles
    for (const auto &t : aut.get_trans()) {
        int src = get<0>(t);// état source de t
        int dst = get<2>(t);// état de destination de t
        char c = get<1>(t);// caractère étiquettant t

        // Une transition est conservée si ses deux extrémités sont utiles
        if (accessible.mem(src) && coaccessible.mem(src) && accessible.mem(dst) &&
            coaccessible.mem(dst)) {
            result.add_trans(src, c, dst);
        }
    }

    return result;
}

//===========================================================


//===================== A faire 10 ==========================
/**
 * @brief intersection donne l'automate d'intersection de deux automates
 * @param aut1 premier automate
 * @param aut2 deuxième automate
 * @return l'automate résultant
 */
Automaton intersection(const Automaton &aut1, const Automaton &aut2) {
    Automaton result;
    IdxSet<pair<int, int>>
        pairs; // Mappe les paires d'états vers un nouvel index unique
    IdxSet<int> to_be_treated; // Liste de travail des index d'états à traiter
    IdxSet<int> treated;       // Ensemble des index d'états déjà traités

    // Initialisation avec les paires d'états initiaux
    for (int init1 : aut1.get_inits()) {
        for (int init2 : aut2.get_inits()) {
            int p = pairs.addindex(make_pair(init1, init2));
            result.add_init(p);
            to_be_treated.add(p);
        }
    }

    while (!to_be_treated.is_empty()) {
        int p_idx = to_be_treated.choose();
        if (treated.mem(p_idx))
            continue;
        treated.add(p_idx);

        pair<int, int> p_pair = pairs.at(p_idx);
        int p1 = p_pair.first;
        int p2 = p_pair.second;

        // Un état produit est final si les deux états composants sont finaux
        if (aut1.is_final(p1) && aut2.is_final(p2)) {
            result.add_final(p_idx);
        }

        // Calcul des transitions possibles
        IdxSet<char> alpha1 = aut1.out_letters(p1);
        IdxSet<char> alpha2 = aut2.out_letters(p2);

        for (char c : alpha1) {
            if (alpha2.mem(c)) { // Si la lettre est commune
                IdxSet<int> next1 = aut1.out_states(p1, c);
                IdxSet<int> next2 = aut2.out_states(p2, c);

                for (int n1 : next1) {
                    for (int n2 : next2) {
                        int next_idx = pairs.addindex(make_pair(n1, n2));
                        result.add_trans(p_idx, c, next_idx);

                        // Si le nouvel état n'a pas encore été traité, on l'ajoute à la
                        // liste
                        if (!treated.mem(next_idx) && !to_be_treated.mem(next_idx)) {
                            to_be_treated.add(next_idx);
                        }
                    }
                }
            }
        }
    }

    return result;
}

//===========================================================

//==================== A faire 11 ===========================
/**
 * Déterminise un automate non déterministe (NFA) en utilisant l'algorithme des
 * sous-ensembles (subset construction). Chaque état du nouvel automate
 * correspond à un ensemble d'états de l'automate d'origine.
 * @param aut L'automate à déterminiser.
 * @return Un automate déterministe équivalent.
 */
Automaton determinize(const Automaton &aut) {
    Automaton det;
    IdxSet<IdxSet<int>>
        statesets; // Mappe un ensemble d'états vers un nouvel index unique
    IdxSet<int> to_be_treated;
    IdxSet<int> treated;

    // L'état initial du DFA est l'ensemble des états initiaux du NFA
    IdxSet<int> inits = aut.get_inits();
    int init_idx = statesets.addindex(inits);
    det.add_init(init_idx);
    to_be_treated.add(init_idx);

    while (!to_be_treated.is_empty()) {
        int curr_idx = to_be_treated.choose();
        if (treated.mem(curr_idx))
            continue;
        treated.add(curr_idx);

        IdxSet<int> curr_set = statesets.at(curr_idx);// ensemble

        // Un état du DFA est final s'il contient au moins un état final du NFA
        for (int s : curr_set) {
            if (aut.is_final(s)) {
                det.add_final(curr_idx);
                break;
            }
        }

        // Pour chaque lettre de l'alphabet
        for (char c : aut.get_alphabet()) {
            IdxSet<int> next_set; // ensemble des états atteints depuis tous les états de curr_set
            // On calcule l'union des transitions pour tous les états du sous-ensemble
            // courant
            for (int s : curr_set) {
                next_set.add(aut.out_states(s, c));
            }

            if (!next_set.is_empty()) {
                int next_idx = statesets.addindex(next_set);
                det.add_trans(curr_idx, c, next_idx);

                if (!treated.mem(next_idx) && !to_be_treated.mem(next_idx)) {
                    to_be_treated.add(next_idx);
                }
            }
        }
    }

    return det;
}

//==========================================================

//========================== A faire 12 =====================
Automaton complement(const Automaton &aut){
    // 1. determinisation
    Automaton deter = determinize(aut);

    // 2. Complétion de l'automate : ajout du puit pour les transitions manquantes
    int puit = deter.size(); // Nouvel état puit
    bool puit_needed = false;
    for (int q = 0; q < deter.size(); ++q) {
        for (char c : deter.get_alphabet()) {
            if (deter.out_states(q, c).is_empty()) {
                deter.add_trans(q, c, puit);
                puit_needed = true;
            }
        }
    }
    // Ajout de la boucle sur le puit
    if (puit_needed) {
        for (char c : deter.get_alphabet()) {
            deter.add_trans(puit, c, puit); // Le puits boucle sur lui-même
        }
    }
    // 3. Inversion des étas finaux
    Automaton complement;
    int total_etat = deter.size() + (puit_needed ? 1 : 0);
    for (int q = 0; q < total_etat; q++){
        if (deter.get_inits().mem(q))complement.add_init(q);// les états initiaux restent initiaux
        if (!deter.is_final(q))complement.add_final(q);// les états non finaux deviennent finaux
    }
    // On copie toutes les transistions
    complement.add_trans(deter.get_trans());

    return complement;
}
//============================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connecter les boutons aux slots
    connect(ui->testAutomatonButton, &QPushButton::clicked,
            this, &MainWindow::on_testAutomatonButton_clicked);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_testAutomatonButton_clicked()
{
    testAutomaton();
}

void MainWindow::testAutomaton()
{
    ui->textOutput->clear();

    // Test 1: Création d'un automate simple
    ui->textOutput->appendPlainText("\t\tTest 1: Automate simple");cout<< endl;cout<< endl;



    Automaton aut1({1},{{1,'a',2},{2,'b',3}, {3,'c',4}},{4});
    aut1.add_trans(1,{'a','b','c'},1);
    aut1.add_trans(4,{'a','b','c'},4);

    // Capture la sortie cout
    std::stringstream buffer;
    streambuf* old = cout.rdbuf(buffer.rdbuf());
    aut1.print();
    cout<< "=======================================================" <<endl;cout<< endl;

    // Test d'appartenance
    cout << "\t\t Test 2: Appartenance de mot à l'automate"<< endl;cout<< endl;


    cout<< (appartient(aut1,"abbabca")
            ? "abbabca appartient à l'automate"
            : "abbabca n'appartient pas à l'automate ")
         << endl;

    cout<< (appartient(aut1,"aaaabcbb")
                 ? "aaaabcbb appartient à l'automate"
                 : "aaaabcbb n'appartient pas à l'automate ")
         << endl;

    cout<< (appartient(aut1,"aaaabbb")
                 ? "aaaabbb appartient à l'automate"
                 : "aaaabbb n'appartient pas à l'automate ")
         << endl;
    cout<< "=======================================================" <<endl;cout<< endl;

    cout<< "\t\tTest 3: Language Vide"<< endl;cout<< endl;

    cout<< (emptyLanguage(aut1)
                 ? "le language est vide"
                 : "le language n'est pas vide ")
         << endl;
    cout<< "\t\tTest 4: Fonction Trim"<< endl;cout<< endl;
    Automaton aut_trim = trim(aut1);
    aut_trim.print();

    cout<< "\t\tTest 5: Fonction Intersection d'automate"<< endl;cout<< endl;
    Automaton aut_empty({1}, { {1,'d',1}, {1,'e',1} },{1});
    Automaton aut_non_empty({1}, { {1,'a',2}, {2,'b',3}, {3,'c',4} },{4});

    Automaton intersect = intersection(aut1, aut_empty);
    cout<< "L'intersection d'automate"<<endl;
    intersect.print();

    Automaton intersect1 = intersection(aut1, aut_non_empty);
    intersect1.print();

    cout<< "\t\tTest 6: Determinisation"<< endl;cout<< endl;
    Automaton determiniser = determinize(aut1);
    determiniser.print();

    cout<< "\t\tTest 7: Complementaire de l'automate"<< endl;cout<< endl;
    Automaton complementaire = complement(aut1);
    complementaire.print();


    cout.rdbuf(old);
    ui->textOutput->appendPlainText(QString::fromStdString(buffer.str()));

}
