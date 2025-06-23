#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/Unicode.hpp>
#include <cmsis_os.h>
#include "main.h"

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View();
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void buttonClicked();
    virtual void handleTickEvent();
private:
    uint32_t tickCount = 0;
    bool isPaused = false;
    bool isGameOver = false;
    bool isFastDropping = false;
    uint8_t board[20][10];

    struct Tetromino {
        int8_t cells[4][2];
        uint8_t color;
    };
    Tetromino currentPiece;
    Tetromino nextPiece;
    int8_t currentX = 3;
    int8_t currentY = -1;

    const Tetromino tetrominoes[7] = {
        {{{0,0},{0,1},{0,2},{0,3}}, 1}, // I
        {{{0,0},{1,0},{0,1},{1,1}}, 2}, // O
        {{{0,0},{1,0},{2,0},{1,1}}, 3}, // T
        {{{1,0},{2,0},{0,1},{1,1}}, 4}, // S
        {{{0,0},{1,0},{1,1},{2,1}}, 5}, // Z
        {{{0,0},{0,1},{0,2},{1,2}}, 6}, // L
        {{{1,0},{1,1},{1,2},{0,2}}, 7}  // J
    };

    const uint32_t pieceColors[8] = {
        touchgfx::Color::getColorFromRGB(0, 0, 0),      // 0: Empty
        touchgfx::Color::getColorFromRGB(0, 255, 255),  // 1: Cyan (I)
        touchgfx::Color::getColorFromRGB(255, 255, 0),  // 2: Yellow (O)
        touchgfx::Color::getColorFromRGB(128, 0, 128),  // 3: Purple (T)
        touchgfx::Color::getColorFromRGB(0, 255, 0),    // 4: Green (S)
        touchgfx::Color::getColorFromRGB(255, 0, 0),    // 5: Red (Z)
        touchgfx::Color::getColorFromRGB(255, 165, 0),  // 6: Orange (L)
        touchgfx::Color::getColorFromRGB(0, 0, 255)     // 7: Blue (J)
    };

    Box boardCells[20][10];

    Container& tetrominoContainer;
    Container& tetrominoContainerO;
    Container& tetrominoContainerT;
    Container& tetrominoContainerS;
    Container& tetrominoContainerZ;
    Container& tetrominoContainerL;
    Container& tetrominoContainerJ;
    Container* tetrominoContainers[7];

    Box& box1; Box& box2; Box& box3; Box& box4;
    Box& box1_1; Box& box2_1; Box& box3_1; Box& box4_1;
    Box& box1_2; Box& box2_2; Box& box3_2; Box& box4_2;
    Box& box1_3; Box& box2_3; Box& box3_3; Box& box4_3;
    Box& box1_4; Box& box2_4; Box& box3_4; Box& box4_4;
    Box& box1_5; Box& box2_5; Box& box3_5; Box& box4_5;
    Box& box1_6; Box& box2_6; Box& box3_6; Box& box4_6;

    Container& nextTetrominoContainer;
    Box& nextCell1;
    Box& nextCell2;
    Box& nextCell3;
    Box& nextCell4;

    uint32_t score = 0;
    uint32_t highScore = 0;

    // Effect variables
    bool isLineClearEffect = false;
    uint8_t lineClearRows[4] = {0}; // Store up to 4 cleared rows
    uint8_t numLinesCleared = 0;
    uint32_t lineClearEffectTick = 0;
    bool isGameOverEffect = false;
    uint32_t gameOverEffectTick = 0;
    bool isScoreHighlight = false;
    uint32_t scoreHighlightTick = 0;

    bool checkCollision(int8_t newX, int8_t newY);
    void placePiece();
    void updateBoardDisplay();
    void updateTetrominoDisplay();
    void handleControlInput();
    int checkAndClearLines();
    void updateScoreDisplay();
    void updateNextPieceDisplay();
    void resetGame();
    void rotatePiece();
    void updateCurrentTetrominoUI();

    // New effect functions
    void startLineClearEffect(int rows[], int count);
    void updateLineClearEffect();
    void startGameOverEffect();
    void updateGameOverEffect();
    void highlightScore();
    void updateScoreHighlight();

    TextAreaWithOneWildcard& gameOverText;

    void readHighScoreFromFlash();
    void writeHighScoreToFlash(uint32_t newScore);
};

#endif // SCREEN1VIEW_HPP
