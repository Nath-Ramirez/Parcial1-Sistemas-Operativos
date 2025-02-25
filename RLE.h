#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <queue>
#include <unordered_map>
#include <vector>
#include <bitset>
#include <string>
#include <cctype>
std::string abrir_archivo(const char* archivo);
std::string comprimirRLE(const std::string& texto);
std::string descomprimirRLE(const std::string& texto);
void comprimirArchivoRLE(const char* archivo);
void descomprimirArchivoRLE(const char* archivo);
bool esTextoPlano(const char* archivo);
void mostrarAyuda();
void mostrarVersion();