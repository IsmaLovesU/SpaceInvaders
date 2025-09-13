# Space Invaders - Fase 2: Entorno Gráfico

**Curso:** CC3086 - Programación de Microprocesadores 
**Fecha:** 12/09/2025  
**Fase:** 2 - Desarrollo del Entorno Gráfico  
**Universidad del Valle de Guatemala**

### 👥 Integrantes del Equipo
- **Denil** - Arquitectura del sistema y configuración
- **Andrés** - Interfaz gráfica ASCII y representación visual
- **Fátima** - Documentación, testing y validación
- **Samuel** - Investigación técnica e implementación de librerías

## 🎮 Descripción del Proyecto

Space Invaders es una implementación del clásico juego arcade desarrollada en C++ utilizando la librería ncurses para la interfaz de consola. Esta es la **Fase 2** del proyecto, enfocada en el desarrollo completo del entorno gráfico y la navegación por menús.

### 🎯 Objetivos de la Fase 2
- ✅ Menú principal funcional con navegación
- ✅ Pantalla de instrucciones detallada
- ✅ Sistema de puntajes destacados
- ✅ Representación visual de todos los elementos del juego
- ✅ Entorno gráfico coherente y estético en consola
- ✅ Demostración funcional de la interfaz

## 🚀 Características Implementadas

### 📺 Interfaz Gráfica
- **Menú Principal:** Navegación completa con opciones de juego
- **ASCII Art:** Representación visual de todos los elementos
- **Colores:** Sistema de colores para mejor experiencia visual
- **Responsive:** Adaptación a diferentes tamaños de consola

### 🎮 Modalidades de Juego
1. **Modo 1:** 40 invasores en grupos de 8
2. **Modo 2:** 50 invasores en grupos de 10

### 🎨 Elementos Visuales
- **Nave del jugador:** `[*]` o `◄►`
- **Invasores:** `▲`, `@`, `W` (diferentes tipos)
- **Proyectiles del jugador:** `^` o `|`
- **Proyectiles enemigos:** `v` o `|`
- **Bordes y marcos:** Caracteres ASCII para delimitación

## 🛠️ Tecnologías Utilizadas

### 📚 Librerías Principales
- **ncurses** - Manejo de interfaz de consola
- **pthread** - Preparación para hilos (Fase 3)
- **chrono** - Control de tiempo y delays
- **iostream/iomanip** - E/O estándar de C++

### 🏗️ Arquitectura del Código
```
src/
├── include/
│   ├── GameEngine.h      # Motor principal del juego
│   ├── MenuSystem.h      # Sistema de menús
│   └── GameRenderer.h    # Renderizado gráfico
├── src/
│   ├── GameEngine.cpp    # Implementación del motor
│   ├── MenuSystem.cpp    # Implementación de menús
│   └── GameRenderer.cpp  # Implementación del renderizado
├── obj/                  # Archivos objeto (generados)
├── bin/                  # Ejecutable (generado)
├── main.cpp             # Punto de entrada
├── Makefile             # Sistema de compilación
└── README.md            # Documentación
```

## 📋 Requisitos del Sistema

### 🖥️ Sistemas Operativos Soportados
- ✅ Linux (Ubuntu, Debian, Fedora, etc.)
- ✅ macOS (con Homebrew)
- ✅ Windows (con WSL o Cygwin)

### 📦 Dependencias
- **g++** (versión 7.0 o superior)
- **make**
- **libncurses-dev** (Ubuntu/Debian) o **ncurses** (macOS)

## 🏗️ Compilación

#### Método 1: Usando Makefile (Recomendado)
```bash

# Ejecutar
make run
```

### 🎯 Comandos Makefile Disponibles
```bash
make            # Compilar el proyecto
make run        # Compilar y ejecutar
make clean      # Limpiar archivos compilados
make debug      # Compilar en modo debug
make release    # Compilar optimizado
make install-deps # Instalar dependencias (Ubuntu/Debian)
make check-deps   # Verificar dependencias
make help       # Mostrar ayuda
```

## 🎮 Cómo Usar

### ⌨️ Controles

#### 📱 Navegación en Menús:
- **W/S** o **↑/↓**: Navegar opciones
- **ENTER**: Seleccionar opción
- **ESC**: Volver al menú anterior

#### 🎮 Controles del Juego:
- **A** o **←**: Mover nave a la izquierda
- **D** o **→**: Mover nave a la derecha
- **W** o **ESPACIO**: Disparar
- **P**: Pausar/reanudar
- **Q** o **ESC**: Salir
- **R**: Reiniciar (al terminar partida)

---

**Nota:** Esta es la implementación de la Fase 2. La funcionalidad completa del juego (movimiento, colisiones, lógica de juego).


¡Gracias por revisar nuestro proyecto! 🚀👾

