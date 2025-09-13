#ifndef MENUSYSTEM_H
#define MENUSYSTEM_H

#include <ncurses.h>
#include <vector>
#include <string>

class MenuSystem {
private:
    int selectedOption;
    std::vector<std::string> mainMenuOptions;
    int screenWidth, screenHeight;
    
    void drawBorder();
    void drawTitle();
    void drawOptions(const std::vector<std::string>& options, int selected);
    void drawFooter();
    void centerText(int y, const std::string& text, int colorPair = 0);
    
public:
    MenuSystem();
    ~MenuSystem();
    
    int showMainMenu();
    void showInstructions();
    void showHighScores();
    void waitForKey();
};

#endif