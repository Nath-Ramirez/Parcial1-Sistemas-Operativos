#include "huffman.h"
std::string nodoInfo(Nodo* nodo) {
    return (nodo->caracter == '\0') ? "Nodo interno (" + std::to_string(nodo->frecuencia) + ")" 
                                    : "'" + std::string(1, nodo->caracter) + "' (" + std::to_string(nodo->frecuencia) + ")";
}

void calcularIndicesAparicion(const std::string& palabra) {
    for (int i = 0; i < palabra.size(); i++) {
        if (indiceAparicion.find(palabra[i]) == indiceAparicion.end()) {
            indiceAparicion[palabra[i]] = i;
        }
    }
}

std::unordered_map<char, int> indiceAparicion;
std::unordered_map<char, int> indiceAparicion2;
void destruirArbol(Nodo* raiz) {
    if (!raiz) return;
    
    destruirArbol(raiz->izquierda);
    destruirArbol(raiz->derecha);
    
    delete raiz;
}

Nodo* construirArbolHuffman(const std::unordered_map<char, int>& frecuencias) {
    std::priority_queue<Nodo*, std::vector<Nodo*>, Comparar> cola;
    for (const auto& par : frecuencias) {
        Nodo* nuevoNodo = new Nodo(par.first, par.second);
        cola.push(nuevoNodo);
        
    }

    while (cola.size() > 1) {
        Nodo* izq = cola.top(); cola.pop();
        Nodo* der = cola.top(); cola.pop();
        Nodo* nuevo = new Nodo('\0', izq->frecuencia + der->frecuencia, izq, der);
        cola.push(nuevo);
    }
    return cola.top();
}

void generarCodigos(Nodo* raiz, std::string codigo, std::unordered_map<char, std::string>& codigos) {
    if (!raiz) return;
    if (!raiz->izquierda && !raiz->derecha) {
        codigos[raiz->caracter] = codigo;
    }
    generarCodigos(raiz->izquierda, codigo + "0", codigos);
    generarCodigos(raiz->derecha, codigo + "1", codigos);
}





// Función global para construir la tabla de descompresión
void construirTabla(Nodo* nodo, std::string codigo, std::unordered_map<std::string, char>& tabla) {
    if (!nodo) return;
    if (!nodo->izquierda && !nodo->derecha) {  // Nodo hoja
        tabla[codigo] = nodo->caracter;
    }
    construirTabla(nodo->izquierda, codigo + "0", tabla);
    construirTabla(nodo->derecha, codigo + "1", tabla);
}


void comprimirArchivoHuff(const char* archivo) {
    int fd_entrada = open(archivo, O_RDONLY);
    if (fd_entrada == -1) {
        std::cerr << "Error: No se pudo abrir el archivo " << archivo << std::endl;
        return;
    }

    // Calcular frecuencias e índices de aparición
    std::unordered_map<char, int> frecuencias;
    char buffer[4096];
    ssize_t bytesLeidos;
    std::string contenido;
    while ((bytesLeidos = read(fd_entrada, buffer, sizeof(buffer))) > 0) {
        contenido.append(buffer, bytesLeidos);
        for (ssize_t i = 0; i < bytesLeidos; i++) {
            frecuencias[buffer[i]]++;
        }
    }
    
    // Volver al inicio del archivo
    lseek(fd_entrada, 0, SEEK_SET);
    calcularIndicesAparicion(contenido);

    // Construir árbol de Huffman
    Nodo* raiz = construirArbolHuffman(frecuencias);
    
    // Generar códigos
    std::unordered_map<char, std::string> codigos;
    generarCodigos(raiz, "", codigos);
    
    // Crear archivo de salida
    std::string nombreSalida = std::string(archivo) + ".huff";
    int fd_salida = open(nombreSalida.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_salida == -1) {
        std::cerr << "Error: No se pudo crear el archivo de salida" << std::endl;
        close(fd_entrada);
        return;
    }

    // Escribir el número total de caracteres diferentes
    int numCaracteres = frecuencias.size();
    write(fd_salida, &numCaracteres, sizeof(int));

    std::vector<std::pair<char, int>> vecFrecuencias(frecuencias.begin(), frecuencias.end());
    std::sort(vecFrecuencias.begin(), vecFrecuencias.end(), 
    [&](const std::pair<char, int>& a, const std::pair<char, int>& b) {
        if (a.second != b.second) {
            return a.second > b.second;  // Ordenar por frecuencia (descendente)
        }
        return indiceAparicion.at(a.first) < indiceAparicion.at(b.first);  // Desempatar por índice de aparición (ascendente)
    }
);
    
    // Escribir la tabla de frecuencias e índices en modo binario
    for (const auto& par : vecFrecuencias) {
        write(fd_salida, &par.first, sizeof(char));  // Carácter
        write(fd_salida, &par.second, sizeof(int));  // Frecuencia
        int indice = indiceAparicion[par.first];     // Índice de primera aparición
        write(fd_salida, &indice, sizeof(int));
    }

    // Escribir la cantidad total de bits válidos
    std::string bits;
    lseek(fd_entrada, 0, SEEK_SET);
    
    while ((bytesLeidos = read(fd_entrada, buffer, sizeof(buffer))) > 0) {
        for (ssize_t i = 0; i < bytesLeidos; i++) {
            bits += codigos[buffer[i]];
        }
    }

    int totalBits = bits.size();
    write(fd_salida, &totalBits, sizeof(
        totalBits));

    // Convertir la cadena de bits en bytes y escribirlos
    for (size_t i = 0; i < bits.size(); i += 8) {
        std::string byteStr = bits.substr(i, 8);
        if (byteStr.size() < 8)
            byteStr.append(8 - byteStr.size(), '0'); // Rellenar con ceros
        char byte = static_cast<char>(std::bitset<8>(byteStr).to_ulong());
        write(fd_salida, &byte, sizeof(byte));
    }
    destruirArbol(raiz);
    close(fd_entrada);
    close(fd_salida);
    std::cout << "Archivo comprimido como " << nombreSalida << std::endl;
}



void descomprimirArchivoHuff(const char* archivo) {
    int fd = open(archivo, O_RDONLY);
    if (fd == -1) {
        std::cerr << "Error: No se pudo abrir el archivo comprimido" << std::endl;
        return;
    }

    // Leer el número de caracteres diferentes
    int numCaracteres;
    read(fd, &numCaracteres, sizeof(int));

    // Leer la tabla de frecuencias e índices
    std::unordered_map<char, int> frecuencias;
    
    for (int i = 0; i < numCaracteres; i++) {
        char caracter;
        int frecuencia;
        int indice;
        
        read(fd, &caracter, sizeof(char));
        read(fd, &frecuencia, sizeof(int));
        read(fd, &indice, sizeof(int));
        
        frecuencias[caracter] = frecuencia;
        indiceAparicion[caracter] = indice;
    }


    // Reconstruir el árbol de Huffman
    Nodo* raiz = construirArbolHuffman(frecuencias);
    // Generar tabla de descompresión
    std::unordered_map<std::string, char> tablaDescompresion;
    construirTabla(raiz, "", tablaDescompresion);
    
    // Leer la cantidad total de bits válidos
    
    int totalBits;
    read(fd, &totalBits, sizeof(int));

    // Leer los datos comprimidos
    std::string bits;
    char byte;
    while (read(fd, &byte, sizeof(char)) > 0) {
        bits += std::bitset<8>(byte).to_string();
    }

    // Truncar los bits extra (que se agregaron al final para completar bytes)
    bits = bits.substr(0, totalBits);

    // Decodificar los bits usando la tabla de descompresión
    std::string mensaje;
    std::string codigoActual;
    for (char bit : bits) {
        codigoActual += bit;
        if (tablaDescompresion.find(codigoActual) != tablaDescompresion.end()) {
            mensaje += tablaDescompresion[codigoActual];
            codigoActual.clear();
        }
    }

    // Guardar el archivo descomprimido
    std::string nombreSalida = std::string(archivo) + ".dec";
    int fd_salida = open(nombreSalida.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd_salida == -1) {
        std::cerr << "Error: No se pudo crear el archivo descomprimido" << std::endl;
        return;
    }

    write(fd_salida, mensaje.c_str(), mensaje.size());
    close(fd_salida);
    close(fd);

    std::cout << "Archivo descomprimido como " << nombreSalida << std::endl;
}







