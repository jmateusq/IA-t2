#include "Grade.h"
#include <cstdlib> // rand
#include <algorithm> // swap

Grade::Grade(const std::vector<Materia>& catalogoMaterias) {
    // Preenche a grade aleatoriamente com matérias do catálogo
    for (unsigned long i = 0; i < SLOTS_TOTAL; i++) {
        int indiceAleatorio = rand() % catalogoMaterias.size();
        slots.push_back(catalogoMaterias[indiceAleatorio]);
    }
    atualizarPontuacao();
}

Grade::Grade(const Grade& outra) {
    this->slots = outra.slots;
    this->pontuacao = outra.pontuacao;
}

int Grade::getPontuacao() const {
    return pontuacao;
}

void Grade::atualizarPontuacao() {
    int score = 0;

    for (unsigned long dia = 0; dia < DIAS; dia++) {
        for (unsigned long turno = 0; turno < TURNOS; turno++) {
            unsigned long index = dia * TURNOS + turno;
            Materia atual = slots[index];

            // REGRA 1: Premia matérias difíceis (Dif=3) de manhã (Turno 0)
            if (turno == 0) {
                if (atual.getDificuldade() == 3) score += 20;
                else if (atual.getId() == 0) score -= 10; // ID 0 é Livre
            }

            // REGRA 2: Penaliza matérias difíceis à noite (Turno 2)
            if (turno == 2) { 
                if (atual.getDificuldade() == 3) score -= 20;
                else if (atual.getId() == 0) score += 10; // Descanso à noite
            }

            // REGRA 3: Penaliza repetição no mesmo dia
            if (turno > 0) {
                if (slots[index] == slots[index - 1] && atual.getId() != 0) {
                    score -= 50; 
                }
            }
        }
    }
    this->pontuacao = score;
}

Grade Grade::gerarVizinho() const {
    Grade vizinho(*this); // Cria cópia
    
    unsigned long i = rand() % SLOTS_TOTAL;
    unsigned long j = rand() % SLOTS_TOTAL;

    std::swap(vizinho.slots[i], vizinho.slots[j]);
    
    vizinho.atualizarPontuacao(); // Recalcula score do vizinho
    return vizinho;
}

void Grade::imprimir() const {
    std::cout << "---------------------------------------------------\n";
    std::cout << "      | Seg    | Ter    | Qua    | Qui    | Sex    |\n";
    std::cout << "---------------------------------------------------\n";
    
    const std::string turnosNome[] = {"Manha", "Tarde", "Noite"};
    
    for (unsigned long t = 0; t < TURNOS; t++) {
        std::cout << turnosNome[t] << " | ";
        for (unsigned long d = 0; d < DIAS; d++) {
            unsigned long index = d * TURNOS + t;
            // Formatação simples para manter alinhamento
            std::string nome = slots[index].getNome();
            while(nome.length() < 7) nome += " "; 
            std::cout << nome << "| ";
        }
        std::cout << "\n";
    }
    std::cout << "---------------------------------------------------\n";
    std::cout << "PONTUACAO TOTAL: " << pontuacao << "\n\n";
}