# Space Invaders con Hilos

Un clon del clásico Space Invaders hecho en C++ usando ncurses y pthreads. Este proyecto fue desarrollado para el curso CC3086 de la Universidad del Valle de Guatemala.

## ¿Qué es esto?

Básicamente es Space Invaders corriendo en la terminal. Usa ncurses para los gráficos ASCII y pthreads para manejar todo el juego con múltiples hilos simultáneos. La idea era aprender sobre concurrencia y sincronización mientras hacíamos algo divertido.

## El equipo

- **Denil** - Se encargó de la arquitectura y configuración del proyecto
- **Andrés** - Hizo toda la interfaz gráfica en ASCII y cómo se ve todo
- **Fátima** - Documentación, pruebas y validación del código
- **Samuel** - Investigó librerías y las implementó

## ¿Qué hace diferente este proyecto?

En vez de hacer todo en un solo bucle como normalmente se haría, separamos cada funcionalidad en su propio hilo:

- **Hilo 1:** Movimiento del jugador (valida límites y actualiza posición)
- **Hilo 2:** Sistema de disparos del jugador
- **Hilo 3:** Movimiento de los invasores (van de lado a lado y bajan)
- **Hilo 4:** Los invasores disparan aleatoriamente
- **Hilo 5:** Actualiza posiciones de todos los proyectiles
- **Hilo 6:** Detecta colisiones entre todo
- **Hilo 7:** Renderiza todo en pantalla (~30 FPS)
- **Hilo 8:** Maneja el input del teclado
- **Hilo 9:** Actualiza puntajes y estadísticas
- **Hilo 10:** Gestiona estados del juego (jugando, pausa, game over, victoria)

### Sincronización

Para que los hilos no se vuelvan locos accediendo a las mismas variables, usamos:

- 4 mutexes (para entidades, puntaje, estado del juego, y renderizado)
- 2 semáforos (uno para acciones del jugador, otro para invasores)
- 1 barrera (para sincronizar todos los hilos cada frame)
- 1 variable de condición (para el renderizado)

## Requisitos

Necesitas tener instalado:
- g++ (versión 7 o más nueva)
- make
- ncurses

### En Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install build-essential libncurses5-dev libncursesw5-dev
```

### En macOS:
```bash
brew install ncurses
```

## ¿Cómo compilar?

Super fácil, solo usa el Makefile:

```bash
make
```

Si quieres compilar y ejecutar de una:
```bash
make run
```

Para limpiar archivos compilados:
```bash
make clean
```

## Estructura del proyecto

```
.
├── include/
│   ├── GameEngine.h         # Motor principal
│   ├── ThreadManager.h      # Manejo de hilos
│   ├── MenuSystem.h         # Menús
│   └── GameRenderer.h       # Renderizado
├── src/
│   ├── GameEngine.cpp
│   ├── ThreadManager.cpp
│   ├── MenuSystem.cpp
│   └── GameRenderer.cpp
├── main.cpp                 # Punto de entrada
├── Makefile                 # Para compilar
└── README.md               # Este archivo
```

## Controles

### En los menús:
- **W/S** o flechas: navegar
- **Enter**: seleccionar
- **ESC**: volver

### En el juego:
- **A/D** o flechas: mover la nave
- **W** o **Espacio**: disparar
- **P**: pausar
- **Q** o **ESC**: salir
- **R**: reiniciar (cuando termina la partida)

## Modos de juego

- **Modo 1:** 40 invasores organizados en grupos de 8
- **Modo 2:** 50 invasores organizados en grupos de 10

El modo 2 es más difícil, básicamente.

## Cómo funciona el juego

Los invasores se mueven de izquierda a derecha, y cuando llegan al borde, bajan y cambian de dirección. De vez en cuando disparan hacia abajo. Tu objetivo es destruirlos todos antes de que:

1. Te quiten todas las vidas (empiezas con 3)
2. Lleguen hasta abajo de la pantalla

Cada invasor que destruyes te da 10 puntos.

## Elementos visuales

- **Tu nave:** `[*]`
- **Invasores:** `^`, `@`, `W` (hay 3 tipos diferentes)
- **Tus disparos:** `^`
- **Disparos enemigos:** `v`
- **Estrellas de fondo:** `.`

## Comandos útiles del Makefile

```bash
make               # Compila el proyecto
make run           # Compila y ejecuta
make clean         # Limpia archivos compilados
make debug         # Compila con símbolos de debug
make release       # Compila optimizado
make threads-info  # Muestra info de los hilos implementados
make help          # Muestra todos los comandos
```

## Problemas comunes

**El juego se ve raro o con caracteres extraños:**
- Asegúrate de que tu terminal soporte colores
- Prueba redimensionar la ventana de tu terminal

**No compila:**
- Verifica que tengas ncurses instalado: `make check-deps`
- Asegúrate de tener g++ actualizado

**El juego va muy lento:**
- Esto puede pasar si tu terminal es muy lenta renderizando
- Intenta con una terminal diferente (tilix, gnome-terminal, kitty)

## Cosas técnicas interesantes

La parte más compleja fue sincronizar los 10 hilos para que no se pisen entre sí. Por ejemplo:

- El hilo de colisiones necesita acceso exclusivo a las entidades mientras revisa
- El hilo de renderizado necesita leer todo pero no puede modificar nada
- Los hilos de movimiento necesitan modificar posiciones pero coordinados

Usamos una barrera de sincronización para que todos los hilos esperen al final de cada frame antes de empezar el siguiente. Esto mantiene todo consistente.

## Estado actual

Este es la **Fase 3** del proyecto. Ya funciona todo:
- ✅ Menú principal completo
- ✅ Dos modos de juego
- ✅ Sistema de puntuación
- ✅ Detección de colisiones
- ✅ Game Over y pantalla de victoria
- ✅ Sistema de pausa
- ✅ 10 hilos funcionando en paralelo

## Créditos

Proyecto desarrollado para CC3086 - Programación de Microprocesadores
Universidad del Valle de Guatemala
Septiembre 2025

---


