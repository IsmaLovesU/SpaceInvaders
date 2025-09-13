#ifndef GAMERENDERER_H
#define GAMERENDERER_H

#include <ncurses.h>
#include <vector>
#include <string>

// Forward declaration para evitar dependencias circulares
struct Entity;
struct Player;

class GameRenderer {
private:
    void drawBorder(int width, int height);
    void drawEntity(const Entity& entity);
    void drawBackground();
    
public:
    GameRenderer();
    ~GameRenderer();
    
    void renderGameField(const Player& player, 
                        const std::vector<Entity>& invaders,
                        const std::vector<Entity>& playerBullets,
                        const std::vector<Entity>& invaderBullets,
                        int screenWidth, int screenHeight);
                        
    void renderUI(int score, int lives, int gameMode);
    void renderStartScreen();
    void clearScreen();
};

#endif

