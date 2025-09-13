#include <iostream>
#include <ncurses.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include "include/GameEngine.h"
#include "include/MenuSystem.h"
#include "include/GameRenderer.h"

using namespace std;

int main() {
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);
    
    // Verificar soporte de colores
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_GREEN, COLOR_BLACK);   // Jugador
        init_pair(2, COLOR_RED, COLOR_BLACK);     // Invasores
        init_pair(3, COLOR_YELLOW, COLOR_BLACK);  // Proyectiles
        init_pair(4, COLOR_CYAN, COLOR_BLACK);    // UI
        init_pair(5, COLOR_MAGENTA, COLOR_BLACK); // Menú
    }
    
    try {
        // Crear instancias principales
        MenuSystem menu;
        GameEngine engine;
        GameRenderer renderer;
        
        bool running = true;
        int option;
        
        while (running) {
            clear();
            
            // Mostrar menú principal
            option = menu.showMainMenu();
            
            switch (option) {
                case 1: // Iniciar juego modo 1 (40 invasores)
                    engine.startGame(1);
                    break;
                    
                case 2: // Iniciar juego modo 2 (50 invasores)
                    engine.startGame(2);
                    break;
                    
                case 3: // Mostrar instrucciones
                    menu.showInstructions();
                    break;
                    
                case 4: // Mostrar puntajes
                    menu.showHighScores();
                    break;
                    
                case 5: // Salir
                    running = false;
                    break;
                    
                default:
                    break;
            }
        }
        
    } catch (const exception& e) {
        endwin();
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    
    // Finalizar ncurses
    endwin();
    cout << "¡Gracias por jugar Space Invaders!" << endl;
    
    return 0;
}