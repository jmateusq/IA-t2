#include "Grade.h"
#include <cstdlib> // rand
#include <algorithm> // swap
#include <iomanip> // setw para a impressão bonita

// Construtor
Grade::Grade(const std::vector<Materia>& catalogoMaterias, Configuracao Config) : config(Config) {
    int dias = config.getDias();
    int turnos = config.getTurnos();
    int aulasPorTurno = config.getAulaTurno();
    
    // CORREÇÃO: Calculo total considerando aulas por turno
    unsigned long int totalSlots =(unsigned long int)(dias * turnos * aulasPorTurno);

    slots.reserve(totalSlots); 

    // PASSO 1: Garantir pelo menos uma ocorrência de cada matéria
    for (const auto& mat : catalogoMaterias) {
        if (slots.size() < totalSlots) {
            slots.push_back(mat);
        }
    }

    // PASSO 2: Preencher o restante aleatoriamente
    while (slots.size() < totalSlots) {
        unsigned long indiceAleatorio = (unsigned long)rand() % catalogoMaterias.size();
        slots.push_back(catalogoMaterias[indiceAleatorio]);
    }

    // PASSO 3: Embaralhar
    for (size_t i = 0; i < slots.size(); i++) {
        size_t j = (unsigned long)rand() % slots.size();
        std::swap(slots[i], slots[j]);
    }

    atualizarPontuacao();
}

// Construtor de Cópia
Grade::Grade(const Grade& outra) : config(outra.config) {
    this->slots = outra.slots;
    this->pontuacao = outra.pontuacao;
}

int Grade::getPontuacao() const {
    return pontuacao;
}

// LÓGICA DE PONTUAÇÃO ATUALIZADA
void Grade::atualizarPontuacao() {
    int dias = config.getDias();
    int turnos = config.getTurnos();
    int aulasPorTurno = config.getAulaTurno();
    int score = 0;

    // Iteramos hierarquicamente: Dia > Turno > Aula
    for (int d = 0; d < dias; d++) {
        for (int t = 0; t < turnos; t++) {
            for (int a = 0; a < aulasPorTurno; a++) {
                
                // CÁLCULO DO ÍNDICE LINEAR CORRETO
                // Quantas aulas já passaram nos dias anteriores? (d * (turnos * aulasPorTurno))
                // Quantas aulas já passaram neste dia? (t * aulasPorTurno)
                // Qual a aula atual? (+ a)
                unsigned long int index =(unsigned long int) ((d * turnos * aulasPorTurno) + (t * aulasPorTurno) + a);
                
                if (index >= slots.size()) break; // Segurança

                Materia atual = slots[index];

                // REGRA 1: Preferência por matérias difíceis de manhã (Turno 0)
                if (t == 0) {
                    if (atual.getDificuldade() == 3) score += 15;
                    // Bônus extra se for a primeira aula do dia (mente fresca)
                    if (a == 0 && atual.getDificuldade() == 3) score += 10;
                }

                // REGRA 2: Penaliza matérias difíceis à noite (Turno 2, se existir)
                if (t == 2) { 
                    if (atual.getDificuldade() == 3) score -= 20;
                    else if (atual.getId() == 0) score += 10; // "Livre" à noite é bom
                }

                // REGRA 3: Evitar repetição seguida da MESMA matéria (exceto se for "Livre")
                // Verifica slot anterior dentro do mesmo dia
                if (index > 0) {
                    // Verifica se não estamos na virada do dia (aula 0 do turno 0 do dia > 0)
                    bool mesmoDia = (index % (unsigned long int)(turnos * aulasPorTurno)) != 0;
                    
                    if (mesmoDia) {
                        if (slots[index] == slots[index - 1] && atual.getId() != 0) { // Assumindo ID 0 = Livre
                            score -= 50; // Penalidade pesada por dobradinha chata
                        }
                    }
                }
            }
        }
    }
    this->pontuacao = score;
}

Grade Grade::gerarVizinho() const {
    Grade vizinho(*this); // Copia a grade atual (incluindo config)
    
    // Usa o tamanho real do vetor para garantir limites válidos
    unsigned long totalSlots = vizinho.slots.size(); 
    
    if (totalSlots > 0) {
        unsigned long i = (unsigned long)rand() % totalSlots;
        unsigned long j = (unsigned long)rand() % totalSlots;
        std::swap(vizinho.slots[i], vizinho.slots[j]);
    }
    
    vizinho.atualizarPontuacao();
    return vizinho;
}

Grade& Grade::operator=(const Grade& other) {
    if (this != &other) {
        this->slots = other.slots;
        this->pontuacao = other.pontuacao;
        this->config = other.config; // Importante copiar a config também
    }
    return *this;
}

// LÓGICA DE IMPRESSÃO ATUALIZADA (TABELA DETALHADA)
void Grade::imprimir() const {
    int dias = config.getDias();
    int turnos = config.getTurnos();
    int aulasPorTurno = config.getAulaTurno();

    std::cout << "\n=================== GRADE GERADA ===================\n";
    std::cout << "Score: " << pontuacao << "\n";
    std::cout << "----------------------------------------------------\n";

    // Cabeçalho dos Dias
    std::cout << std::left << std::setw(10) << "TURNO";
    for (int d = 0; d < dias; d++) {
        std::string diaStr = "DIA " + std::to_string(d + 1);
        std::cout << "| " << std::setw(12) << diaStr;
    }
    std::cout << "|\n";
    std::cout << "----------------------------------------------------\n";

    const std::string nomesTurnos[] = {"MANHA", "TARDE", "NOITE"};

    for (int t = 0; t < turnos; t++) {
        // Para cada aula dentro do turno, criamos uma linha na tabela
        for (int a = 0; a < aulasPorTurno; a++) {
            
            // Coluna do Turno (Imprime o nome apenas na primeira aula do bloco, senão vazio)
            std::string nomeTurnoDisplay = (a == 0) ? nomesTurnos[t % 3] : "";
            std::cout << std::left << std::setw(10) << nomeTurnoDisplay;

            // Colunas dos Dias
            for (int d = 0; d < dias; d++) {
                unsigned long int index = (unsigned long int)((d * turnos * aulasPorTurno) + (t * aulasPorTurno) + a);
                
                std::string nomeMat = "";
                if (index < slots.size()) {
                    nomeMat = slots[index].getNome();
                    // Corta string se for muito longa para não quebrar a tabela
                    if (nomeMat.size() > 11) nomeMat = nomeMat.substr(0, 11);
                }
                
                std::cout << "| " << std::setw(12) << nomeMat;
            }
            std::cout << "|\n";
        }
        // Separador entre turnos para facilitar leitura
        std::cout << "----------";
        for(int k=0; k<dias; k++) std::cout << "--------------";
        std::cout << "-\n";
    }
    std::cout << "\n";
}