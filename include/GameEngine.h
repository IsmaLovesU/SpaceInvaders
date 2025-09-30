#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <ncurses.h>
#include <vector>
#include <string>
#include "GameRenderer.h"

// Forward declaration para evitar dependencia circular
class ThreadManager;

// Estructura para representar entidades del juego
struct Entity {
    int x, y;
    char symbol;
    bool active;
    int colorPair;
    
    Entity(int _x = 0, int _y = 0, char _symbol = ' ', int _color = 0) 
        : x(_x), y(_y), symbol(_symbol), active(true), colorPair(_color) {}
};

// Estructura para el jugador
struct Player {
    Entity entity;
    int lives;
    int score;
    
    Player() : lives(3), score(0) {
        entity = Entity(0, 0, '*', 1);
    }
};

class GameEngine {
private:
    GameRenderer* renderer;
    ThreadManager* threadManager;
    
    Player player;
    std::vector<Entity> invaders;
    std::vector<Entity> playerBullets;
    std::vector<Entity> invaderBullets;
    
    int gameMode;
    int screenWidth, screenHeight;
    int gameState; // 0: jugando, 1: pausa, 2: game over, 3: victoria
    bool running;
    bool playerShouldShoot;
    
    void initializeGame();
    void setupInvaders();
    void showGameOverScreen();
    void showVictoryScreen();
    void showPauseScreen();
    
public:
    GameEngine();
    ~GameEngine();
    
    void startGame(int mode);
    void pauseGame();
    void resumeGame();
    void resetGame();
    void render();
    
    // Getters para los hilos
    Player* getPlayer() { return &player; }
    std::vector<Entity>* getInvaders() { return &invaders; }
    std::vector<Entity>* getPlayerBullets() { return &playerBullets; }
    std::vector<Entity>* getInvaderBullets() { return &invaderBullets; }
    ThreadManager* getThreadManager() { return threadManager; }
    
    int getGameState() const { return gameState; }
    void setGameState(int state) { gameState = state; }
    
    int getScreenWidth() const { return screenWidth; }
    int getScreenHeight() const { return screenHeight; }
    
    bool isRunning() const { return running; }
    void setRunning(bool r) { running = r; }
    
    bool shouldPlayerShoot() const { return playerShouldShoot; }
    void setPlayerShoot(bool shoot) { playerShouldShoot = shoot; }
};

#endif