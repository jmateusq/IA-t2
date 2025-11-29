#ifndef MATERIA_H
#define MATERIA_H

#include <string>
#include <iostream>

class Materia {
private:
    int id;
    std::string nome;
    int dificuldade; // 0: Livre, 1: Leve, 3: Difícil

public:
    Materia(); // Construtor padrão
    Materia(int id, std::string nome, int dificuldade);

    int getId() const;
    std::string getNome() const;
    int getDificuldade() const;

    // Sobrecarga de operador para comparar matérias facilmente
    bool operator==(const Materia& other) const;
    bool operator!=(const Materia& other) const;
};

#endif