#include "MenuSystem.h"
#include <chrono>
#include <thread>

MenuSystem::MenuSystem() : selectedOption(0) {
    getmaxyx(stdscr, screenHeight, screenWidth);
    
    mainMenuOptions = {
        "1. Iniciar Juego - Modo 1 (40 Invasores)",
        "2. Iniciar Juego - Modo 2 (50 Invasores)", 
        "3. Instrucciones",
        "4. Puntajes Destacados",
        "5. Salir"
    };
}

MenuSystem::~MenuSystem() {
    // Destructor vacío
}

void MenuSystem::drawBorder() {
    // Dibujar borde superior
    move(0, 0);
    for (int i = 0; i < screenWidth; i++) {
        addch('=');
    }
    
    // Dibujar borde inferior
    move(screenHeight - 1, 0);
    for (int i = 0; i < screenWidth; i++) {
        addch('=');
    }
    
    // Dibujar bordes laterales
    for (int i = 1; i < screenHeight - 1; i++) {
        mvaddch(i, 0, '|');
        mvaddch(i, screenWidth - 1, '|');
    }
}

void MenuSystem::drawTitle() {
    std::vector<std::string> title = {
        "  ███████ ██████   █████   ██████ ███████     ██ ███    ██ ██    ██  █████  ██████  ███████ ██████  ███████ ",
        "  ██      ██   ██ ██   ██ ██      ██          ██ ████   ██ ██    ██ ██   ██ ██   ██ ██      ██   ██ ██      ",
        "  ███████ ██████  ███████ ██      █████       ██ ██ ██  ██ ██    ██ ███████ ██   ██ █████   ██████  ███████ ",
        "       ██ ██      ██   ██ ██      ██          ██ ██  ██ ██  ██  ██  ██   ██ ██   ██ ██      ██   ██      ██ ",
        "  ███████ ██      ██   ██  ██████ ███████     ██ ██   ████   ████   ██   ██ ██████  ███████ ██   ██ ███████ "
    };
    
    int startY = 3;
    attron(COLOR_PAIR(5));
    for (int i = 0; i < title.size(); i++) {
        centerText(startY + i, title[i], 5);
    }
    attroff(COLOR_PAIR(5));
}

void MenuSystem::drawOptions(const std::vector<std::string>& options, int selected) {
    int startY = screenHeight / 2 - 1;
    
    for (int i = 0; i < options.size(); i++) {
        if (i == selected) {
            attron(A_REVERSE);
            attron(COLOR_PAIR(4));
            centerText(startY + i, "> " + options[i] + " <");
            attroff(COLOR_PAIR(4));
            attroff(A_REVERSE);
        } else {
            centerText(startY + i, "  " + options[i] + "  ");
        }
    }
}

void MenuSystem::drawFooter() {
    std::string controls = "Usar W/S o flechas para navegar, ENTER para seleccionar";
    centerText(screenHeight - 3, controls, 4);
}

void MenuSystem::centerText(int y, const std::string& text, int colorPair) {
    int x = (screenWidth - text.length()) / 2;
    if (colorPair > 0) {
        attron(COLOR_PAIR(colorPair));
    }
    mvprintw(y, x, "%s", text.c_str());
    if (colorPair > 0) {
        attroff(COLOR_PAIR(colorPair));
    }
}

int MenuSystem::showMainMenu() {
    int ch;
    selectedOption = 0;
    
    while (true) {
        clear();
        getmaxyx(stdscr, screenHeight, screenWidth);
        
        drawBorder();
        drawTitle();
        drawOptions(mainMenuOptions, selectedOption);
        drawFooter();
        
        refresh();
        
        ch = getch();
        
        switch (ch) {
            case 'w':
            case 'W':
            case KEY_UP:
                selectedOption = (selectedOption - 1 + mainMenuOptions.size()) % mainMenuOptions.size();
                break;
                
            case 's':
            case 'S':
            case KEY_DOWN:
                selectedOption = (selectedOption + 1) % mainMenuOptions.size();
                break;
                
            case '\n':
            case '\r':
                return selectedOption + 1;
                
            case 27: // ESC
                return 5; // Salir
                
            default:
                break;
        }
        
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

void MenuSystem::showInstructions() {
    clear();
    getmaxyx(stdscr, screenHeight, screenWidth);
    
    drawBorder();
    
    std::vector<std::string> instructions = {
    "===============================================================",
    "                    INSTRUCCIONES DE SPACE INVADERS",
    "===============================================================",
    "",
    "OBJETIVO:",
    "  * Destruir todos los invasores espaciales antes de que lleguen al suelo",
    "  * Evitar ser alcanzado por los proyectiles enemigos",
    "  * Proteger la Tierra de la invasion alienigena",
    "",
    "CONTROLES:",
    "  * A o <- : Mover nave hacia la izquierda",
    "  * D o -> : Mover nave hacia la derecha", 
    "  * W o ESPACIO : Disparar proyectil",
    "  * P : Pausar/reanudar juego",
    "  * Q o ESC : Salir del juego",
    "  * R : Reiniciar partida (al terminar)",
    "",
    "ELEMENTOS DEL JUEGO:",
    "  * Nave del jugador: [*] o <->",
    "  * Invasores: ^ o @ o W",
    "  * Proyectiles del jugador: | o ^",
    "  * Proyectiles enemigos: | o v",
    "",
    "PUNTUACION:",
    "  * Cada invasor destruido: +10 puntos",
    "  * Bonificacion por oleada completada",
    "",
    "MODALIDADES:",
    "  * Modo 1: 40 invasores en grupos de 8",
    "  * Modo 2: 50 invasores en grupos de 10",
    "",
    "Buena suerte, comandante!",
    "",
    "Presiona cualquier tecla para regresar al menu principal..."
};
    
    int startY = 2;
    for (int i = 0; i < instructions.size(); i++) {
        if (i < 3 || instructions[i].find("OBJETIVO:") != std::string::npos ||
            instructions[i].find("CONTROLES:") != std::string::npos ||
            instructions[i].find("ELEMENTOS") != std::string::npos ||
            instructions[i].find("PUNTUACIÓN:") != std::string::npos ||
            instructions[i].find("MODALIDADES:") != std::string::npos) {
            attron(COLOR_PAIR(4) | A_BOLD);
        }
        
        centerText(startY + i, instructions[i]);
        
        if (i < 3 || instructions[i].find(":") != std::string::npos) {
            attroff(COLOR_PAIR(4) | A_BOLD);
        }
    }
    
    refresh();
    waitForKey();
}

void MenuSystem::showHighScores() {
    clear();
    getmaxyx(stdscr, screenHeight, screenWidth);
    
    drawBorder();
    
    std::vector<std::string> scores = {
    "===============================================================",
    "                        PUNTAJES DESTACADOS",
    "===============================================================",
    "",
    "+-------------------------------------------------------------+",
    "|  RANK  |    JUGADOR    |   PUNTOS   |    MODO    |   FECHA   |",
    "+-------------------------------------------------------------+",
    "|   1°   |   CHAMPION    |   99999    |   MODO 2   | 12/09/25  |",
    "|   2°   |   ACE_PILOT   |   85420    |   MODO 2   | 11/09/25  |",
    "|   3°   |   DEFENDER    |   76850    |   MODO 1   | 10/09/25  |",
    "|   4°   |   ROOKIE_01   |   65230    |   MODO 1   | 09/09/25  |",
    "|   5°   |   SPACE_WAR   |   54170    |   MODO 2   | 08/09/25  |",
    "|   6°   |   GUARDIAN    |   48960    |   MODO 1   | 07/09/25  |",
    "|   7°   |   NOVA_STAR   |   42340    |   MODO 1   | 06/09/25  |",
    "|   8°   |   COSMIC_01   |   38720    |   MODO 2   | 05/09/25  |",
    "|   9°   |   BLASTER     |   35140    |   MODO 1   | 04/09/25  |",
    "|  10°   |   NEWBIE_X    |   28560    |   MODO 1   | 03/09/25  |",
    "+-------------------------------------------------------------+",
    "",
    "* Los puntajes se mantienen durante la sesion actual",
    "* Para aparecer en esta tabla, debes superar al menos 25,000 puntos",
    "",
    "Presiona cualquier tecla para regresar al menu principal..."
};
    
    int startY = 2;
    for (int i = 0; i < scores.size(); i++) {
        if (i < 3) {
            attron(COLOR_PAIR(4) | A_BOLD);
        } else if (i >= 5 && i <= 17) {
            attron(COLOR_PAIR(3));
        }
        
        centerText(startY + i, scores[i]);
        
        if (i < 3) {
            attroff(COLOR_PAIR(4) | A_BOLD);
        } else if (i >= 5 && i <= 17) {
            attroff(COLOR_PAIR(3));
        }
    }
    
    refresh();
    waitForKey();
}

void MenuSystem::waitForKey() {
    nodelay(stdscr, FALSE);
    getch();
    nodelay(stdscr, TRUE);
}