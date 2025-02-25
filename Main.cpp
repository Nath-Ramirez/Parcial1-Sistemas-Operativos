#include "huffman.h"
#include "RLE.h"
#include <iostream>
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: uso incorrecto de los comandos intente: -h   o  --help" << std::endl;
        return 1;
    }

    if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        mostrarAyuda();
        return 0;
    } else if (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0) {
        mostrarVersion();
        return 0;
    } else if ((strcmp(argv[1], "-cr") == 0 || strcmp(argv[1], "--compressRLE") == 0) && argc == 3) {
        if (esTextoPlano(argv[2]))
            comprimirArchivoRLE(argv[2]);
        else
            std::cerr << "Error: El archivo no es de texto plano" << std::endl;
        return 0;
    } else if ((strcmp(argv[1], "-xr") == 0 || strcmp(argv[1], "--decompressRLE") == 0) && argc == 3) {
        descomprimirArchivoRLE(argv[2]);
        return 0;
    } else if ((strcmp(argv[1], "-ch") == 0 || strcmp(argv[1], "--compressHuffman") == 0) && argc == 3) {
        comprimirArchivoHuff(argv[2]);
        return 0;
    } else if ((strcmp(argv[1], "-xh") == 0 || strcmp(argv[1], "--decompressHuffman") == 0) && argc == 3) {
        descomprimirArchivoHuff(argv[2]);
        return 0;
    }
    else {
        std::cerr << "Opción no válida. Use --help para más información.\n";
        return 1;
    }
    return 0;
}