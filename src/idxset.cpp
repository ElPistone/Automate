#include "idxset.h"
#include <iostream>
#include <vector>

// Ce fichier peut être utilisé pour des spécialisations explicites si nécessaire
// Pour la plupart des cas, le template dans le header suffit

// Exemple de spécialisation pour std::tuple<int, char, int>
// pour résoudre les problèmes potentiels d'opérateur <<
namespace std {
std::ostream& operator<<(std::ostream& os, const tuple<int, char, int>& t) {
    os << "(" << get<0>(t) << "," << get<1>(t) << "," << get<2>(t) << ")";
    return os;
}
}
