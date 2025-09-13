# Makefile para Space Invaders - Fase 2
# Proyecto CC3086 - Universidad del Valle de Guatemala

# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g
LDFLAGS = -lncurses -lpthread

# Directorios
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Archivos fuente
SOURCES = $(wildcard $(SRCDIR)/*.cpp) main.cpp
OBJECTS = $(SOURCES:%.cpp=$(OBJDIR)/%.o)
TARGET = $(BINDIR)/space_invaders

# Crear directorios si no existen
$(shell mkdir -p $(OBJDIR) $(OBJDIR)/$(SRCDIR) $(BINDIR))

# Regla principal
all: $(TARGET)

# Compilar el ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "✓ Compilación exitosa! Ejecutable creado en $(TARGET)"

# Compilar archivos objeto
$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Limpiar archivos compilados
clean:
	rm -rf $(OBJDIR) $(BINDIR)
	@echo "✓ Archivos de compilación eliminados"

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Instalar dependencias (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install build-essential libncurses5-dev libncursesw5-dev

# Verificar que ncurses esté instalado
check-deps:
	@echo "Verificando dependencias..."
	@pkg-config --exists ncurses && echo "✓ ncurses encontrado" || echo "✗ ncurses no encontrado - ejecuta 'make install-deps'"

# Debug
debug: CXXFLAGS += -DDEBUG -O0
debug: $(TARGET)

# Release
release: CXXFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)

# Mostrar ayuda
help:
	@echo "Makefile para Space Invaders - Fase 2"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make          - Compilar el proyecto"
	@echo "  make run      - Compilar y ejecutar"
	@echo "  make clean    - Limpiar archivos compilados"
	@echo "  make debug    - Compilar en modo debug"
	@echo "  make release  - Compilar optimizado para release"
	@echo "  make install-deps - Instalar dependencias (Ubuntu/Debian)"
	@echo "  make check-deps   - Verificar dependencias"
	@echo "  make help     - Mostrar esta ayuda"

# Indicar que estos targets no son archivos
.PHONY: all clean run install-deps check-deps debug release help