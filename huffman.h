#include <iostream>
#include <fcntl.h>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <string>
#include <sys/stat.h>
#include <sys/mman.h>
#include <cstdint>
#include <algorithm>

extern std::unordered_map<char, int> indiceAparicion;
// Estructura del Nodo del árbol de Huffman
struct Nodo {
    char caracter;
    int frecuencia;
    Nodo* izquierda;
    Nodo* derecha;

    Nodo(char c, int f, Nodo* izq = nullptr, Nodo* der = nullptr)
        : caracter(c), frecuencia(f), izquierda(izq), derecha(der) {}
};

// Comparador para la cola de prioridad
struct Comparar{
    bool operator()(Nodo* a, Nodo* b) {
        if (a->frecuencia == b->frecuencia)
            bool res = indiceAparicion[a->caracter] > indiceAparicion[b->caracter]; // Prioriza el que apareció antes
            return indiceAparicion[a->caracter] > indiceAparicion[b->caracter];
        return a->frecuencia > b->frecuencia;   
    }
    };

// Declaraciones de funciones
Nodo* construirArbolHuffman(const std::unordered_map<char, int>& frecuencias,const std::unordered_map<char, int>& indiceAparicion);
void generarCodigos(Nodo* raiz, std::string codigo, std::unordered_map<char, std::string>& codigos);
void comprimirArchivoHuff(const char* archivo);
void descomprimirArchivoHuff(const char* archivo);
void calcularIndicesAparicion(const std::string& palabra);