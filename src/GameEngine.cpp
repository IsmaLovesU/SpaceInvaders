#include "GameEngine.h"
#include <chrono>
#include <thread>
#include <algorithm>

GameEngine::GameEngine() : renderer(nullptr), gameMode(1), gameState(0), running(false) {
    getmaxyx(stdscr, screenHeight, screenWidth);
    renderer = new GameRenderer();
}

GameEngine::~GameEngine() {
    delete renderer;
}

void GameEngine::startGame(int mode) {
    gameMode = mode;
    running = true;
    gameState = 0;
    
    initializeGame();
    
    // Bucle principal del juego
    while (running) {
        handleInput();
        
        if (gameState == 0) { // Jugando
            updateGame();
            checkCollisions();
        }
        
        renderGame();
        
        // Control de FPS (aproximadamente 30 FPS)
        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
}

void GameEngine::initializeGame() {
    getmaxyx(stdscr, screenHeight, screenWidth);
    
    // Inicializar jugador
    player.lives = 3;
    player.score = 0;
    player.entity.x = screenWidth / 2;
    player.entity.y = screenHeight - 3;
    player.entity.symbol = '*';
    player.entity.colorPair = 1;
    player.entity.active = true;
    
    // Limpiar vectores
    invaders.clear();
    playerBullets.clear();
    invaderBullets.clear();
    
    // Configurar invasores según el modo
    setupInvaders();
}

void GameEngine::setupInvaders() {
    invaders.clear();
    
    int invaderCount = (gameMode == 1) ? 40 : 50;
    int groupSize = (gameMode == 1) ? 8 : 10;
    int groups = invaderCount / groupSize;
    
    int startX = 5;
    int startY = 3;
    int spacing = 3;
    
    for (int group = 0; group < groups; group++) {
        for (int i = 0; i < groupSize; i++) {
            int x = startX + (i * spacing);
            int y = startY + (group * 2);
            
            // Alternar símbolos para variedad visual
            char symbol = (group % 3 == 0) ? 'W' : ((group % 3 == 1) ? '@' : '^');
            
            Entity invader(x, y, symbol, 2);
            invaders.push_back(invader);
        }
    }
}

void GameEngine::handleInput() {
    int ch = getch();
    
    switch (ch) {
        case 'a':
        case 'A':
        case KEY_LEFT:
            if (player.entity.x > 1) {
                player.entity.x--;
            }
            break;
            
        case 'd':
        case 'D':
        case KEY_RIGHT:
            if (player.entity.x < screenWidth - 2) {
                player.entity.x++;
            }
            break;
            
        case 'w':
        case 'W':
        case ' ':
            // Limitar número de proyectiles simultáneos
            if (playerBullets.size() < 3) {
                Entity bullet(player.entity.x, player.entity.y - 1, '^', 3);
                playerBullets.push_back(bullet);
            }
            break;
            
        case 'p':
        case 'P':
            if (gameState == 0) {
                pauseGame();
            } else if (gameState == 1) {
                resumeGame();
            }
            break;
            
        case 'q':
        case 'Q':
        case 27: // ESC
            running = false;
            break;
            
        case 'r':
        case 'R':
            if (gameState == 2 || gameState == 3) {
                resetGame();
            }
            break;
            
        default:
            break;
    }
}

void GameEngine::updateGame() {
    // Actualizar proyectiles del jugador
    for (auto it = playerBullets.begin(); it != playerBullets.end();) {
        it->y--;
        if (it->y < 1) {
            it = playerBullets.erase(it);
        } else {
            ++it;
        }
    }
    
    // Actualizar proyectiles de invasores (simulación básica)
    for (auto it = invaderBullets.begin(); it != invaderBullets.end();) {
        it->y++;
        if (it->y >= screenHeight - 1) {
            it = invaderBullets.erase(it);
        } else {
            ++it;
        }
    }
    
    // Simular disparo ocasional de invasores
    static int shootTimer = 0;
    shootTimer++;
    if (shootTimer > 60 && !invaders.empty()) { // Aproximadamente cada 2 segundos
        shootTimer = 0;
        int randomInvader = rand() % invaders.size();
        if (invaders[randomInvader].active) {
            Entity bullet(invaders[randomInvader].x, invaders[randomInvader].y + 1, 'v', 2);
            invaderBullets.push_back(bullet);
        }
    }
}

void GameEngine::checkCollisions() {
    // Colisiones proyectiles del jugador vs invasores
    for (auto& bullet : playerBullets) {
        for (auto& invader : invaders) {
            if (bullet.active && invader.active &&
                bullet.x == invader.x && bullet.y == invader.y) {
                bullet.active = false;
                invader.active = false;
                player.score += 10;
            }
        }
    }
    
    // Colisiones proyectiles de invasores vs jugador
    for (auto& bullet : invaderBullets) {
        if (bullet.active && player.entity.active &&
            bullet.x == player.entity.x && bullet.y == player.entity.y) {
            bullet.active = false;
            player.lives--;
            if (player.lives <= 0) {
                gameState = 2; // Game Over
            }
        }
    }
    
    // Verificar condición de victoria
    bool allInvadersDestroyed = true;
    for (const auto& invader : invaders) {
        if (invader.active) {
            allInvadersDestroyed = false;
            break;
        }
    }
    
    if (allInvadersDestroyed) {
        gameState = 3; // Victoria
    }
    
    // Remover entidades inactivas
    playerBullets.erase(
        std::remove_if(playerBullets.begin(), playerBullets.end(),
                      [](const Entity& e) { return !e.active; }),
        playerBullets.end());
        
    invaderBullets.erase(
        std::remove_if(invaderBullets.begin(), invaderBullets.end(),
                      [](const Entity& e) { return !e.active; }),
        invaderBullets.end());
}

void GameEngine::renderGame() {
    clear();
    
    if (gameState == 0) { // Jugando
        renderer->renderGameField(player, invaders, playerBullets, invaderBullets, screenWidth, screenHeight);
        renderer->renderUI(player.score, player.lives, gameMode);
        
    } else if (gameState == 1) { // Pausa
        showPauseScreen();
        
    } else if (gameState == 2) { // Game Over
        showGameOverScreen();
        
    } else if (gameState == 3) { // Victoria
        showVictoryScreen();
    }
    
    refresh();
}

void GameEngine::showPauseScreen() {
    int centerY = screenHeight / 2;
    int centerX = screenWidth / 2;
    
    std::vector<std::string> pauseText = {
        "██████   █████  ██    ██ ███████  █████  ",
        "██   ██ ██   ██ ██    ██ ██      ██   ██ ",
        "██████  ███████ ██    ██ ███████ ███████ ",
        "██      ██   ██ ██    ██      ██ ██   ██ ",
        "██      ██   ██  ██████  ███████ ██   ██ ",
        "",
        "Presiona P para continuar",
        "Presiona Q para salir"
    };
    
    attron(COLOR_PAIR(4) | A_BOLD);
    for (int i = 0; i < pauseText.size(); i++) {
        mvprintw(centerY - 4 + i, centerX - pauseText[i].length() / 2, "%s", pauseText[i].c_str());
    }
    attroff(COLOR_PAIR(4) | A_BOLD);
}

void GameEngine::showGameOverScreen() {
    int centerY = screenHeight / 2;
    int centerX = screenWidth / 2;
    
    std::vector<std::string> gameOverText = {
        "██████   █████  ███    ███ ███████      ██████  ██    ██ ███████ ██████  ",
        "██       ██   ██ ████  ████ ██          ██    ██ ██    ██ ██      ██   ██ ",
        "██   ███ ███████ ██ ████ ██ █████       ██    ██ ██    ██ █████   ██████  ",
        "██    ██ ██   ██ ██  ██  ██ ██          ██    ██  ██  ██  ██      ██   ██ ",
        " ██████  ██   ██ ██      ██ ███████      ██████    ████   ███████ ██   ██ ",
        "",
        "Puntuación final: " + std::to_string(player.score),
        "",
        "Presiona R para reiniciar",
        "Presiona Q para salir al menú"
    };
    
    attron(COLOR_PAIR(2) | A_BOLD);
    for (int i = 0; i < gameOverText.size(); i++) {
        mvprintw(centerY - 5 + i, centerX - gameOverText[i].length() / 2, "%s", gameOverText[i].c_str());
    }
    attroff(COLOR_PAIR(2) | A_BOLD);
}

void GameEngine::showVictoryScreen() {
    int centerY = screenHeight / 2;
    int centerX = screenWidth / 2;
    
    std::vector<std::string> victoryText = {
        "██    ██ ██  ██████ ████████  ██████  ██████  ██  █████  ",
        "██    ██ ██ ██         ██    ██    ██ ██   ██ ██ ██   ██ ",
        "██    ██ ██ ██         ██    ██    ██ ██████  ██ ███████ ",
        " ██  ██  ██ ██         ██    ██    ██ ██   ██ ██ ██   ██ ",
        "  ████   ██  ██████    ██     ██████  ██   ██ ██ ██   ██ ",
        "",
        "¡Has salvado la Tierra!",
        "Puntuación final: " + std::to_string(player.score),
        "",
        "Presiona R para jugar de nuevo",
        "Presiona Q para salir al menú"
    };
    
    attron(COLOR_PAIR(1) | A_BOLD);
    for (int i = 0; i < victoryText.size(); i++) {
        mvprintw(centerY - 5 + i, centerX - victoryText[i].length() / 2, "%s", victoryText[i].c_str());
    }
    attroff(COLOR_PAIR(1) | A_BOLD);
}

void GameEngine::pauseGame() {
    gameState = 1;
}

void GameEngine::resumeGame() {
    gameState = 0;
}

void GameEngine::resetGame() {
    initializeGame();
    gameState = 0;
}