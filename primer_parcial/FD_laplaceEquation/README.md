# Proyecto: FD_LaplaceEquation
# Descripción: Resuelve la ecuación de Laplace por diferencias finitas.

# Compilador
CXX = g++
# Flags de compilación (puedes añadir más si es necesario, como -Wall -Wextra -O2)
CXXFLAGS = -std=c++17 -Wall -Wextra
# Directorios de inclusión
INC_DIR = include
# Directorios de código fuente
SRC_DIR = src
# Nombre del ejecutable
TARGET = FD_LaplaceEquation

# Archivos de código fuente
SRCS = $(SRC_DIR)/laplaceEquation.cpp $(SRC_DIR)/laplaceEquationMain.cpp
# Archivos de encabezado
HDRS = $(INC_DIR)/laplaceEquation.h
# Todos los archivos objeto
OBJS = $(SRCS:.cpp=.o)

# Regla principal: compila el ejecutable
all: $(TARGET)

# Regla para compilar los archivos objeto (.o)
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/laplaceEquation.h
	@echo "Compilando $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INC_DIR)

# Regla para linkear los archivos objeto y crear el ejecutable
$(TARGET): $(OBJS)
	@echo "Enlazando $@"
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@

# Regla para limpiar los archivos objeto y el ejecutable
clean:
	@echo "Limpiando..."
	rm -f $(TARGET) $(OBJS) generate_files/*

# Regla para ejecutar el programa
run: all
	@echo "Ejecutando el programa..."
	./$(TARGET)

.PHONY: all clean run


