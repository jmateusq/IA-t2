#include <iostream>
#include <vector>
#include <ctime>
#include "Materia.h"
#include "Otimizador.h"

int main() {
    srand((unsigned int)time(NULL));

    // Definição do Catálogo de Matérias
    // ID, Nome, Dificuldade (0=Livre, 1=Leve, 2=Médio, 3=Dificil)
    std::vector<Materia> catalogo;
    catalogo.push_back(Materia(0, "Livre", 0));
    catalogo.push_back(Materia(1, "Matemat", 3));
    catalogo.push_back(Materia(2, "Hist",    1));
    catalogo.push_back(Materia(3, "Fisica",  3));
    catalogo.push_back(Materia(4, "Ingles",  1));

    // Instancia o otimizador com o catálogo e nº de iterações
    Otimizador hillClimbing(catalogo, 1000);
    
    hillClimbing.executar();

    return 0;
}