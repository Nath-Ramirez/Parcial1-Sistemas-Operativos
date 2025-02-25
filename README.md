# Parcial1-Sistemas-Operativos

## 1. Introducción
En este primer parcial de Sistemas Operativos, implementamos las llamadas al sistema usando Kali Linux y C++ para construír algoritmos de compresión.
Los algoritmos elegidos para la compresión de archivos son:
- Huffman
- Run-Length Encoding (RLE)

---
## 2. Run-Length Encoding (RLE)
Este algoritmo de compresión se basa en eliminar redundancias, reemplazando secuencias repetitivas de caracteres por una representación más corta en la que indica el caracter seguido del númeor de veces que se repite consecutivamente.

## 3. Ejemplo paso a paso
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

## 4. Pseudocódigo del Algoritmo RLE
```plaintext
1. Inicializar una cadena vacía para almacenar la salida comprimida.
2. Recorrer el texto de entrada carácter por carácter.
3. Contar cuántas veces se repite el carácter consecutivamente.
4. Almacenar el carácter y el número de repeticiones en la salida comprimida.
5. Continuar hasta el final del texto.
6. Retornar la cadena comprimida.
```

## 6. Algoritmo Run-Length (RLE) en C++

---

---
Huffman
---
