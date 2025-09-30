# Makefile para Space Invaders - Fase 3
# Proyecto CC3086 - Universidad del Valle de Guatemala

# Compilador y flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -pthread
LDFLAGS = -lncurses -lpthread

# Directorios
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin

# Archivos fuente y objetos
SOURCES = main.cpp \
          $(SRCDIR)/GameEngine.cpp \
          $(SRCDIR)/ThreadManager.cpp \
          $(SRCDIR)/MenuSystem.cpp \
          $(SRCDIR)/GameRenderer.cpp

OBJECTS = $(OBJDIR)/main.o \
          $(OBJDIR)/src/GameEngine.o \
          $(OBJDIR)/src/ThreadManager.o \
          $(OBJDIR)/src/MenuSystem.o \
          $(OBJDIR)/src/GameRenderer.o

TARGET = $(BINDIR)/space_invaders

# Crear directorios si no existen
$(shell mkdir -p $(OBJDIR) $(OBJDIR)/$(SRCDIR) $(BINDIR))

# Regla principal
all: $(TARGET)

# Compilar el ejecutable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)
	@echo "✓ Compilación exitosa! Ejecutable creado en $(TARGET)"
	@echo "✓ Fase 3 implementada con 10 hilos"

# Compilar main.cpp
$(OBJDIR)/main.o: main.cpp
	$(CXX) $(CXXFLAGS) -I$(INCDIR) -c $< -o $@

# Compilar archivos de src/
$(OBJDIR)/src/%.o: $(SRCDIR)/%.cpp
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

# Verificar que ncurses y pthreads estén disponibles
check-deps:
	@echo "Verificando dependencias..."
	@pkg-config --exists ncurses && echo "✓ ncurses encontrado" || echo "✗ ncurses no encontrado - ejecuta 'make install-deps'"
	@echo "✓ pthreads está incluido en el sistema"

# Debug
debug: CXXFLAGS += -DDEBUG -O0
debug: clean $(TARGET)

# Release
release: CXXFLAGS += -O3 -DNDEBUG
release: clean $(TARGET)

# Mostrar información de hilos
threads-info:
	@echo "========================================="
	@echo "Información de Hilos - Fase 3"
	@echo "========================================="
	@echo ""
	@echo "Hilos implementados:"
	@echo "  1. playerMovementFunc() - Movimiento del jugador"
	@echo "  2. playerShootingFunc() - Disparos del jugador"
	@echo "  3. invaderMovementFunc() - Movimiento de invasores"
	@echo "  4. invaderShootingFunc() - Disparos de invasores"
	@echo "  5. bulletUpdateFunc() - Actualización de proyectiles"
	@echo "  6. collisionDetectionFunc() - Detección de colisiones"
	@echo "  7. renderFunc() - Renderizado"
	@echo "  8. inputHandlerFunc() - Manejo de entrada"
	@echo "  9. scoreUpdateFunc() - Actualización de puntaje"
	@echo "  10. gameStateFunc() - Gestión de estado del juego"
	@echo ""
	@echo "Mecanismos de sincronización:"
	@echo "  - pthread_mutex (4 instancias)"
	@echo "  - sem_t semáforo (2 instancias)"
	@echo "  - pthread_barrier (1 instancia)"
	@echo "  - pthread_cond (1 instancia)"
	@echo ""
	@echo "Total: 10 hilos + 4 mecanismos de sincronización"
	@echo "========================================="

# Verificar estructura del proyecto
check-structure:
	@echo "Verificando estructura del proyecto..."
	@echo ""
	@test -f include/GameEngine.h && echo "✓ include/GameEngine.h" || echo "✗ include/GameEngine.h"
	@test -f include/ThreadManager.h && echo "✓ include/ThreadManager.h" || echo "✗ include/ThreadManager.h"
	@test -f include/MenuSystem.h && echo "✓ include/MenuSystem.h" || echo "✗ include/MenuSystem.h"
	@test -f include/GameRenderer.h && echo "✓ include/GameRenderer.h" || echo "✗ include/GameRenderer.h"
	@test -f src/GameEngine.cpp && echo "✓ src/GameEngine.cpp" || echo "✗ src/GameEngine.cpp"
	@test -f src/ThreadManager.cpp && echo "✓ src/ThreadManager.cpp" || echo "✗ src/ThreadManager.cpp"
	@test -f src/MenuSystem.cpp && echo "✓ src/MenuSystem.cpp" || echo "✗ src/MenuSystem.cpp"
	@test -f src/GameRenderer.cpp && echo "✓ src/GameRenderer.cpp" || echo "✗ src/GameRenderer.cpp"
	@test -f main.cpp && echo "✓ main.cpp" || echo "✗ main.cpp"
	@echo ""

# Mostrar ayuda
help:
	@echo "Makefile para Space Invaders - Fase 3"
	@echo ""
	@echo "Comandos disponibles:"
	@echo "  make               - Compilar el proyecto"
	@echo "  make run           - Compilar y ejecutar"
	@echo "  make clean         - Limpiar archivos compilados"
	@echo "  make debug         - Compilar en modo debug"
	@echo "  make release       - Compilar optimizado para release"
	@echo "  make install-deps  - Instalar dependencias (Ubuntu/Debian)"
	@echo "  make check-deps    - Verificar dependencias"
	@echo "  make threads-info  - Mostrar información de hilos implementados"
	@echo "  make check-structure - Verificar que todos los archivos existan"
	@echo "  make help          - Mostrar esta ayuda"

# Indicar que estos targets no son archivos
.PHONY: all clean run install-deps check-deps debug release help threads-info check-structure