#include "RLE.h"
std::string abrir_archivo(const char* archivo) {
    int fd = open(archivo, O_RDONLY);
    if (fd == -1) {
        perror("Error abriendo el archivo");
        return "";
    }
    
    char buffer[1028];
    ssize_t bytes_leidos = read(fd, buffer, sizeof(buffer));
    
    if (bytes_leidos == -1) {
        perror("Error leyendo el archivo");
        close(fd);
        return "";
    }

    close(fd);
    return std::string(buffer, bytes_leidos);
}

// Comprimir archivo
std::string comprimirRLE(const std::string& texto) {
    std::string comprimido;
    int longitud = texto.length();

    for (int i = 0; i < longitud; i++) {
        int contador = 1;
        while (i < longitud - 1 && texto[i] == texto[i + 1]) {
            contador++;
            i++;
        }
        comprimido += texto[i] + std::to_string(contador);
    }

    return comprimido;
}
// Descomprimir archivo
std::string descomprimirRLE(const std::string& texto) {
    std::string descomprimido;
    int longitud = texto.length();
    char letra = '\0';

    for (int i = 0; i < longitud; i++) {
        if (isalpha(texto[i]) || isspace(texto[i])) {
            letra = texto[i];  // Guarda la letra o el espacio
        } 
        else if (isdigit(texto[i])) {
            int cantidad = 0;
            while (i < longitud && isdigit(texto[i])) {
                cantidad = cantidad * 10 + (texto[i] - '0');
                i++; // Avanza mientras haya dígitos
            }
            i--; // Volver atrás porque el último i++ ya pasó al siguiente carácter no numérico

            // Añadir la letra o espacio la cantidad de veces indicada
            descomprimido.append(cantidad, letra);
        }
    }
    return descomprimido;
}
// Función para identificar si un archivo es de texto plano
bool esTextoPlano(const char* archivo) {
    std::ifstream entrada(archivo, std::ios::binary);
    if (!entrada) {
        std::cerr << "Error: No se pudo abrir el archivo " << archivo << std::endl;
        return false;
    }

    char c;
    while (entrada.get(c)) {
        unsigned char uc = static_cast<unsigned char>(c);
        if (uc == 0 || (uc < 32 && uc != '\t' && uc != '\n' && uc != '\r' && uc != ' ')) {
            entrada.close();
            return false; // Es binario
        }
    }

    entrada.close();
    return true; // Es texto plano
}

void mostrarAyuda() {
    std::cout << "Uso: mycommand [opción] <archivo>\n"
              << "Opciones:\n"
              << "  -h, --help        Muestra esta ayuda\n"
              << "  -v, --version     Muestra la versión\n"
              << "  -ch, --compressHuffman    Comprime el archivo usando el algoritmo de huffman\n"
              << "  -cr, --compressRLE    Comprime el archivo usando el algoritmo RLE\n"
              << "  -xh, --decompressHuffman  Descomprime archivos que fueron comprimidos con el algoritmo de huffman\n"
              << "  -xr, --decompressRLE  Descomprime archivos que fueron comprimidos con el RLE de huffman\n";
}

void mostrarVersion() {
    std::cout << "comando versión 1.0\n";
}

void comprimirArchivoRLE(const char* archivo) {
    std::string contenido = abrir_archivo(archivo);
    
    if (!contenido.empty()) {
        std::string comprimido = comprimirRLE(contenido);
        std::string nombre_salida = std::string(archivo) + ".rle";

        // Abrir archivo de salida usando llamadas al sistema
        int fd = open(nombre_salida.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("Error creando el archivo comprimido");
            return;
        }

        // Escribir contenido comprimido en el archivo
        ssize_t bytes_escritos = write(fd, comprimido.c_str(), comprimido.size());
        if (bytes_escritos == -1) {
            perror("Error escribiendo en el archivo comprimido");
            close(fd);
            return;
        }

        close(fd);
        std::cout << "Archivo comprimido guardado como: " << nombre_salida << "\n";
    } else {
        std::cerr << "No se pudo leer el archivo.\n";
    }
}

void descomprimirArchivoRLE(const char* archivo) {
    std::string contenido = abrir_archivo(archivo);
    
    if (!contenido.empty()) {
        std::string descomprimido = descomprimirRLE(contenido);
        std::string nombre_salida = std::string(archivo) + ".txt";

        // Abrir archivo de salida usando llamadas al sistema
        int fd = open(nombre_salida.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            perror("Error creando el archivo descomprimido");
            return;
        }

        // Escribir contenido descomprimido en el archivo
        ssize_t bytes_escritos = write(fd, descomprimido.c_str(), descomprimido.size());
        if (bytes_escritos == -1) {
            perror("Error escribiendo en el archivo descomprimido");
            close(fd);
            return;
        }

        close(fd);
        std::cout << "Archivo descomprimido guardado como: " << nombre_salida << "\n";
    } else {
        std::cerr << "No se pudo leer el archivo.\n";
    }
}
