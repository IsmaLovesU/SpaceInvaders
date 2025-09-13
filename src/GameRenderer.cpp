#include "GameRenderer.h"
#include "GameEngine.h"

GameRenderer::GameRenderer() {
    // Constructor vacío
}

GameRenderer::~GameRenderer() {
    // Destructor vacío
}

void GameRenderer::drawBorder(int width, int height) {
    // Borde superior
    move(0, 0);
    for (int i = 0; i < width; i++) {
        addch('=');
    }
    
    // Borde inferior
    move(height - 1, 0);
    for (int i = 0; i < width; i++) {
        addch('=');
    }
    
    // Bordes laterales
    for (int i = 1; i < height - 1; i++) {
        mvaddch(i, 0, '|');
        mvaddch(i, width - 1, '|');
    }
}

void GameRenderer::drawEntity(const Entity& entity) {
    if (entity.active) {
        if (entity.colorPair > 0) {
            attron(COLOR_PAIR(entity.colorPair));
        }
        mvaddch(entity.y, entity.x, entity.symbol);
        if (entity.colorPair > 0) {
            attroff(COLOR_PAIR(entity.colorPair));
        }
    }
}

void GameRenderer::drawBackground() {
    // Dibujar algunas estrellas en el fondo para ambiente espacial
    attron(COLOR_PAIR(4));
    
    static int starPositions[][2] = {
        {10, 5}, {25, 8}, {45, 3}, {60, 12}, {75, 6},
        {15, 15}, {35, 18}, {55, 20}, {70, 16}, {80, 22},
        {5, 25}, {30, 28}, {50, 30}, {65, 27}, {85, 25}
    };
    
    for (int i = 0; i < 15; i++) {
        mvaddch(starPositions[i][1], starPositions[i][0], '.');
    }
    
    attroff(COLOR_PAIR(4));
}

void GameRenderer::renderGameField(const Player& player, 
                                  const std::vector<Entity>& invaders,
                                  const std::vector<Entity>& playerBullets,
                                  const std::vector<Entity>& invaderBullets,
                                  int screenWidth, int screenHeight) {
    
    // Dibujar borde del campo de juego
    drawBorder(screenWidth, screenHeight);
    
    // Dibujar fondo con estrellas
    drawBackground();
    
    // Dibujar jugador
    drawEntity(player.entity);
    
    // Dibujar invasores activos
    for (const auto& invader : invaders) {
        if (invader.active) {
            drawEntity(invader);
        }
    }
    
    // Dibujar proyectiles del jugador
    for (const auto& bullet : playerBullets) {
        drawEntity(bullet);
    }
    
    // Dibujar proyectiles de invasores
    for (const auto& bullet : invaderBullets) {
        drawEntity(bullet);
    }
    
    // Línea de separación para el área de juego
    attron(COLOR_PAIR(4));
    for (int i = 1; i < screenWidth - 1; i++) {
        mvaddch(screenHeight - 5, i, '-');
    }
    attroff(COLOR_PAIR(4));
}

void GameRenderer::renderUI(int score, int lives, int gameMode) {
    int screenWidth, screenHeight;
    getmaxyx(stdscr, screenHeight, screenWidth);
    
    // Área de información del juego
    int uiY = screenHeight - 4;
    
    attron(COLOR_PAIR(4) | A_BOLD);
    
    // Mostrar puntuación
    mvprintw(uiY, 2, "PUNTOS: %d", score);
    
    // Mostrar vidas
    mvprintw(uiY + 1, 2, "VIDAS: ");
    attroff(A_BOLD);
    
    attron(COLOR_PAIR(1));
    for (int i = 0; i < lives; i++) {
        mvaddch(uiY + 1, 9 + i * 2, '*');
    }
    attroff(COLOR_PAIR(1));
    
    // Mostrar modo de juego
    attron(COLOR_PAIR(4) | A_BOLD);
    mvprintw(uiY, screenWidth - 15, "MODO: %d", gameMode);
    
    // Mostrar controles básicos
    attron(COLOR_PAIR(4));
    mvprintw(uiY + 1, screenWidth - 25, "A/D:Mover W:Disparar P:Pausa");
    
    attroff(COLOR_PAIR(4) | A_BOLD);
    attroff(COLOR_PAIR(4));
}

void GameRenderer::renderStartScreen() {
    int screenWidth, screenHeight;
    getmaxyx(stdscr, screenHeight, screenWidth);
    
    clear();
    
    // Título ASCII art simplificado
    std::vector<std::string> title = {
        "╔══════════════════════════════════════╗",
        "║           SPACE INVADERS             ║",
        "║                                      ║",
        "║    Preparándose para la batalla...   ║",
        "║                                      ║",
        "╚══════════════════════════════════════╝"
    };
    
    int startY = screenHeight / 2 - 3;
    int startX = (screenWidth - title[0].length()) / 2;
    
    attron(COLOR_PAIR(5) | A_BOLD);
    for (int i = 0; i < title.size(); i++) {
        mvprintw(startY + i, startX, "%s", title[i].c_str());
    }
    attroff(COLOR_PAIR(5) | A_BOLD);
    
    refresh();
}

void GameRenderer::clearScreen() {
    clear();
}