# Parcial 1 Sistemas Operativos

## 1. Introducción
En este primer parcial de Sistemas Operativos, implementamos las llamadas al sistema usando Kali Linux y C++ para construír algoritmos de compresión.
Los algoritmos elegidos para la compresión de archivos son:
- Huffman
- Run-Length Encoding (RLE)

---
## 2. Main.cpp
```cpp

```
---

---
## 3. Menú de ayuda
```cpp

```
---

---
## 4. Run-Length Encoding (RLE)
Este algoritmo de compresión se basa en eliminar redundancias, reemplazando secuencias repetitivas de caracteres por una representación más corta en la que indica el caracter seguido del númeor de veces que se repite consecutivamente.

## 5. Ejemplo paso a paso
Texto de entrada **"Hoolaaa"**

1. **Contar las repeticiones:**
   ```
   H: 1
   o: 2
   l: 1
   a: 3
   ```
2. **Generar la cadena comprimida:**
   ```
   H1o2l1a3
   ```
3. **Resultado final:**
   ```
   Entrada:  "Hoolaaa"
   Salida comprimida:  "H1o2l1a3"
   ```

## 6. Pseudocódigo del Algoritmo RLE
```plaintext
1. Inicializar una cadena vacía para almacenar la salida comprimida.
2. Recorrer el texto de entrada carácter por carácter.
3. Contar cuántas veces se repite el carácter consecutivamente.
4. Almacenar el carácter y el número de repeticiones en la salida comprimida.
5. Continuar hasta el final del texto.
6. Retornar la cadena comprimida.
```

## 7. Algoritmo Run-Length (RLE) en C++
1. Función para identificar si un archivo es texto plano 
```cpp
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
```
- 
---

---
Huffman
---
