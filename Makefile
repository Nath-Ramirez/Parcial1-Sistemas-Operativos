# Nombre del compilador
CXX = g++

# Flags de compilación
CXXFLAGS = -Wall -Wextra -std=c++11

# Archivos fuente y objetos
SRC = Main.cpp huffman.cpp RLE.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = Mycompressor

# Ruta de instalación
INSTALL_PATH = /usr/local/bin/$(TARGET)

# Regla para compilar el ejecutable
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

# Regla para compilar los archivos objeto
%.o: %.cpp huffman.h RLE.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para instalar el ejecutable en /usr/local/bin/
install: $(TARGET)
	sudo mv $(TARGET) $(INSTALL_PATH)
	sudo chmod +x $(INSTALL_PATH)

# Regla para limpiar los archivos generados
clean:
	rm -f $(OBJ) $(TARGET)

# Regla para desinstalar el ejecutable
uninstall:
	sudo rm -f $(INSTALL_PATH)
