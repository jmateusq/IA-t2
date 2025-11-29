#include "Materia.h"

Materia::Materia() : id(0), nome("Livre"), dificuldade(0) {}

Materia::Materia(int id, std::string nome, int dificuldade) 
    : id(id), nome(nome), dificuldade(dificuldade) {}

int Materia::getId() const {
    return id;
}

std::string Materia::getNome() const {
    return nome;
}

int Materia::getDificuldade() const {
    return dificuldade;
}

bool Materia::operator==(const Materia& other) const {
    return this->id == other.id;
}

bool Materia::operator!=(const Materia& other) const {
    return !(*this == other);
}