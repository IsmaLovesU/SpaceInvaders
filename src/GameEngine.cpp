#include "GameEngine.h"
#include "ThreadManager.h"
#include <chrono>
#include <thread>
#include <algorithm>

GameEngine::GameEngine() 
    : renderer(nullptr), threadManager(nullptr), gameMode(1), 
      gameState(0), running(false), playerShouldShoot(false) {
    getmaxyx(stdscr, screenHeight, screenWidth);
    renderer = new GameRenderer();
    threadManager = new ThreadManager(this);
}

GameEngine::~GameEngine() {
    if (threadManager && threadManager->isRunning()) {
        threadManager->stopThreads();
    }
    delete threadManager;
    delete renderer;
}

void GameEngine::startGame(int mode) {
    gameMode = mode;
    running = true;
    gameState = 0;
    playerShouldShoot = false;
    
    initializeGame();
    
    // Iniciar todos los hilos
    threadManager->startThreads();
    
    // Bucle principal - ahora solo espera a que running sea false
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Detener hilos cuando se sale del juego
    threadManager->stopThreads();
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

void GameEngine::render() {
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
        "====================================",
        "               PAUSA",
        "====================================",
        "",
        "Presiona P para continuar",
        "Presiona Q para salir"
    };
    
    attron(COLOR_PAIR(4) | A_BOLD);
    for (size_t i = 0; i < pauseText.size(); i++) {
        mvprintw(centerY - 4 + i, centerX - pauseText[i].length() / 2, "%s", pauseText[i].c_str());
    }
    attroff(COLOR_PAIR(4) | A_BOLD);
}

void GameEngine::showGameOverScreen() {
    int centerY = screenHeight / 2;
    int centerX = screenWidth / 2;
    
    std::vector<std::string> gameOverText = {
        "====================================",
        "            GAME OVER",
        "====================================",
        "",
        "Puntuacion final: " + std::to_string(player.score),
        "",
        "Presiona R para reiniciar",
        "Presiona Q para salir al menu"
    };
    
    attron(COLOR_PAIR(2) | A_BOLD);
    for (size_t i = 0; i < gameOverText.size(); i++) {
        mvprintw(centerY - 5 + i, centerX - gameOverText[i].length() / 2, "%s", gameOverText[i].c_str());
    }
    attroff(COLOR_PAIR(2) | A_BOLD);
}

void GameEngine::showVictoryScreen() {
    int centerY = screenHeight / 2;
    int centerX = screenWidth / 2;
    
    std::vector<std::string> victoryText = {
        "====================================",
        "             VICTORIA!",
        "====================================",
        "",
        "Has salvado la Tierra!",
        "Puntuacion final: " + std::to_string(player.score),
        "",
        "Presiona R para jugar de nuevo",
        "Presiona Q para salir al menu"
    };
    
    attron(COLOR_PAIR(1) | A_BOLD);
    for (size_t i = 0; i < victoryText.size(); i++) {
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
    // Solo reinicializar el estado del juego sin detener hilos
    pthread_mutex_lock(threadManager->getEntityMutex());
    pthread_mutex_lock(threadManager->getGameStateMutex());
    
    // Reinicializar jugador
    player.lives = 3;
    player.score = 0;
    player.entity.x = screenWidth / 2;
    player.entity.y = screenHeight - 3;
    player.entity.active = true;
    playerShouldShoot = false;
    
    // Limpiar vectores
    invaders.clear();
    playerBullets.clear();
    invaderBullets.clear();
    
    // Configurar invasores según el modo
    setupInvaders();
    
    // Cambiar estado a jugando
    gameState = 0;
    
    pthread_mutex_unlock(threadManager->getGameStateMutex());
    pthread_mutex_unlock(threadManager->getEntityMutex());
}