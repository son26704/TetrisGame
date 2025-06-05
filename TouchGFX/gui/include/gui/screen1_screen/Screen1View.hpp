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
protected:
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
        {{{0,0},{0,1},{0,2},{0,3}}, 1}, // I: (0,0), (0,12), (0,24), (0,36)
        {{{0,0},{1,0},{0,1},{1,1}}, 2}, // O: (0,0), (12,0), (0,12), (12,12)
        {{{0,0},{1,0},{2,0},{1,1}}, 3}, // T: (0,0), (12,0), (24,0), (12,12)
        {{{1,0},{2,0},{0,1},{1,1}}, 4}, // S: (12,0), (24,0), (0,12), (12,12)
        {{{0,0},{1,0},{1,1},{2,1}}, 5}, // Z: (0,0), (12,0), (12,12), (24,12)
        {{{0,0},{0,1},{0,2},{1,2}}, 6}, // L: (0,0), (0,12), (0,24), (12,24)
        {{{1,0},{1,1},{1,2},{0,2}}, 7}  // J: (12,0), (12,12), (12,24), (0,24)
    };

    const uint32_t pieceColors[8] = {
        touchgfx::Color::getColorFromRGB(0, 0, 0),      // 0: Không dùng (trống)
        touchgfx::Color::getColorFromRGB(0, 255, 255),  // 1: Cyan (I)
        touchgfx::Color::getColorFromRGB(255, 255, 0),  // 2: Yellow (O)
        touchgfx::Color::getColorFromRGB(128, 0, 128),  // 3: Purple (T)
        touchgfx::Color::getColorFromRGB(0, 255, 0),    // 4: Green (S)
        touchgfx::Color::getColorFromRGB(255, 0, 0),    // 5: Red (Z)
        touchgfx::Color::getColorFromRGB(255, 165, 0),  // 6: Orange (L)
        touchgfx::Color::getColorFromRGB(0, 0, 255)     // 7: Blue (J)
    };

    Box boardCells[20][10];

    // Container cho các khối
    Container& tetrominoContainer;   // I
    Container& tetrominoContainerO;  // O
    Container& tetrominoContainerT;  // T
    Container& tetrominoContainerS;  // S
    Container& tetrominoContainerZ;  // Z
    Container& tetrominoContainerL;  // L
    Container& tetrominoContainerJ;  // J
    Container* tetrominoContainers[7];

    // Box con trong mỗi container
    Box& box1; Box& box2; Box& box3; Box& box4;         // TetrominoContainer (I)
    Box& box1_1; Box& box2_1; Box& box3_1; Box& box4_1; // TetrominoContainerO (O)
    Box& box1_2; Box& box2_2; Box& box3_2; Box& box4_2; // TetrominoContainerT (T)
    Box& box1_3; Box& box2_3; Box& box3_3; Box& box4_3; // TetrominoContainerS (S)
    Box& box1_4; Box& box2_4; Box& box3_4; Box& box4_4; // TetrominoContainerZ (Z)
    Box& box1_5; Box& box2_5; Box& box3_5; Box& box4_5; // TetrominoContainerL (L)
    Box& box1_6; Box& box2_6; Box& box3_6; Box& box4_6; // TetrominoContainerJ (J)

    Container& nextTetrominoContainer;
    Box& nextCell1;
    Box& nextCell2;
    Box& nextCell3;
    Box& nextCell4;

    uint32_t score = 0;

    bool checkCollision(int8_t newX, int8_t newY)
    {
        for (int i = 0; i < 4; i++)
        {
            int8_t cellX = newX + currentPiece.cells[i][0];
            int8_t cellY = newY + currentPiece.cells[i][1];
            if (cellX < 0 || cellX >= 10 || cellY >= 20)
                return true;
            if (cellY >= 0 && board[cellY][cellX] != 0)
                return true;
        }
        return false;
    }

    void placePiece()
    {
        for (int i = 0; i < 4; i++)
        {
            int8_t cellX = currentX + currentPiece.cells[i][0];
            int8_t cellY = currentY + currentPiece.cells[i][1];
            if (cellY >= 0 && cellY < 20 && cellX >= 0 && cellX < 10)
            {
                board[cellY][cellX] = currentPiece.color;
            }
        }
    }

    void updateBoardDisplay()
    {
        for (int row = 0; row < 20; row++)
        {
            for (int col = 0; col < 10; col++)
            {
                if (board[row][col] == 0)
                {
                    boardCells[row][col].setVisible(false);
                    boardCells[row][col].invalidate();
                }
                else
                {
                    boardCells[row][col].setVisible(true);
                    boardCells[row][col].setXY(col * 12 + 60, row * 12 + 40);
                    boardCells[row][col].setWidthHeight(12, 12);
                    boardCells[row][col].setColor(pieceColors[board[row][col]]);
                    boardCells[row][col].invalidate();
                }
            }
        }
    }

    void updateTetrominoDisplay()
    {
        for (int i = 0; i < 7; i++)
        {
            tetrominoContainers[i]->setVisible(false);
            tetrominoContainers[i]->invalidate();
        }
        updateCurrentTetrominoUI();
    }

    void handleControlInput()
    {
        if (isGameOver) return;
        uint8_t controlSignal;
        if (osMessageQueueGet(controlQueueHandle, &controlSignal, NULL, 0) == osOK)
        {
            if (controlSignal == 1 && !checkCollision(currentX - 1, currentY)) // Trái
            {
                currentX--;
                updateCurrentTetrominoUI();
            }
            else if (controlSignal == 2 && !checkCollision(currentX + 1, currentY)) // Phải
            {
                currentX++;
                updateCurrentTetrominoUI();
            }
            else if (controlSignal == 3) // Xoay
            {
                rotatePiece();
            }
            else if (controlSignal == 4) // Rơi nhanh
            {
                isFastDropping = true;
            }
            else if (controlSignal == 5) // Nhả rơi nhanh
            {
                isFastDropping = false;
            }
        }
    }

    int checkAndClearLines()
    {
        int linesCleared = 0;

        for (int row = 19; row >= 0; row--)
        {
            bool isFull = true;
            for (int col = 0; col < 10; col++)
            {
                if (board[row][col] == 0)
                {
                    isFull = false;
                    break;
                }
            }

            if (isFull)
            {
                linesCleared++;
                for (int r = row; r > 0; r--)
                {
                    for (int col = 0; col < 10; col++)
                    {
                        board[r][col] = board[r-1][col];
                    }
                }
                for (int col = 0; col < 10; col++)
                {
                    board[0][col] = 0;
                }
                row++;
            }
        }

        int points[] = {0, 100, 300, 600, 1000};
        if (linesCleared > 0)
        {
            score += points[linesCleared];
        }

        return linesCleared;
    }

    void updateScoreDisplay()
    {
        if (isGameOver)
        {
            Unicode::snprintf(scoreTextBuffer, SCORETEXT_SIZE, "Game Over");
        }
        else
        {
            Unicode::snprintf(scoreTextBuffer, SCORETEXT_SIZE, isPaused ? "Paused" : "%d", score);
        }
        scoreText.invalidate();
    }

    void updateNextPieceDisplay()
    {
        NextTetrominoContainer.setXY(196, 80);
        for (int i = 0; i < 4; i++)
        {
            int8_t relativeX = nextPiece.cells[i][0];
            int8_t relativeY = nextPiece.cells[i][1];
            switch (i)
            {
                case 0:
                    nextCell1.setXY(relativeX * 12, relativeY * 12);
                    nextCell1.setColor(pieceColors[nextPiece.color]);
                    nextCell1.invalidate();
                    break;
                case 1:
                    nextCell2.setXY(relativeX * 12, relativeY * 12);
                    nextCell2.setColor(pieceColors[nextPiece.color]);
                    nextCell2.invalidate();
                    break;
                case 2:
                    nextCell3.setXY(relativeX * 12, relativeY * 12);
                    nextCell3.setColor(pieceColors[nextPiece.color]);
                    nextCell3.invalidate();
                    break;
                case 3:
                    nextCell4.setXY(relativeX * 12, relativeY * 12);
                    nextCell4.setColor(pieceColors[nextPiece.color]);
                    nextCell4.invalidate();
                    break;
            }
        }
        NextTetrominoContainer.invalidate();
    }

    void resetGame()
    {
        for (int i = 0; i < 7; i++)
        {
            tetrominoContainers[i]->setVisible(false);
            tetrominoContainers[i]->invalidate();
        }

        for (int row = 0; row < 20; row++)
        {
            for (int col = 0; col < 10; col++)
            {
                board[row][col] = 0;
                boardCells[row][col].setVisible(false);
                boardCells[row][col].invalidate();
            }
        }

        updateBoardDisplay();

        score = 0;
        isGameOver = false;
        isPaused = false;

        uint8_t pieceIndex;
        if (osMessageQueueGet(pieceQueueHandle, &pieceIndex, NULL, 0) == osOK)
        {
            currentPiece = tetrominoes[pieceIndex];
        }
        else
        {
            currentPiece = tetrominoes[0];
        }

        currentX = 3;
        currentY = -1;
        updateTetrominoDisplay();

        if (osMessageQueueGet(pieceQueueHandle, &pieceIndex, NULL, 0) == osOK)
        {
            nextPiece = tetrominoes[pieceIndex];
        }
        else
        {
            nextPiece = tetrominoes[0];
        }

        updateNextPieceDisplay();
        updateScoreDisplay();

        invalidate();
    }

    void rotatePiece()
    {
        int8_t originalCells[4][2];
        for (int i = 0; i < 4; i++)
        {
            originalCells[i][0] = currentPiece.cells[i][0];
            originalCells[i][1] = currentPiece.cells[i][1];
        }

        // Xoay 90 độ
        for (int i = 0; i < 4; i++)
        {
            int8_t x = currentPiece.cells[i][0];
            int8_t y = currentPiece.cells[i][1];
            currentPiece.cells[i][0] = -y;
            currentPiece.cells[i][1] = x;
        }

        // Dịch về góc 0,0 nhỏ nhất
        int8_t minX = currentPiece.cells[0][0];
        int8_t minY = currentPiece.cells[0][1];
        for (int i = 1; i < 4; i++)
        {
            if (currentPiece.cells[i][0] < minX) minX = currentPiece.cells[i][0];
            if (currentPiece.cells[i][1] < minY) minY = currentPiece.cells[i][1];
        }
        for (int i = 0; i < 4; i++)
        {
            currentPiece.cells[i][0] -= minX;
            currentPiece.cells[i][1] -= minY;
        }

        if (checkCollision(currentX, currentY))
        {
            for (int i = 0; i < 4; i++)
            {
                currentPiece.cells[i][0] = originalCells[i][0];
                currentPiece.cells[i][1] = originalCells[i][1];
            }
        }

        updateCurrentTetrominoUI();
    }

    void updateCurrentTetrominoUI()
    {
        int pieceIndex = currentPiece.color - 1;
        Container* cont = tetrominoContainers[pieceIndex];

        // Chọn đúng 4 box cho loại tetromino
        Box* boxes[4];
        switch (pieceIndex)
        {
            case 0: boxes[0] = &box1;   boxes[1] = &box2;   boxes[2] = &box3;   boxes[3] = &box4;   break; // I
            case 1: boxes[0] = &box1_1; boxes[1] = &box2_1; boxes[2] = &box3_1; boxes[3] = &box4_1; break; // O
            case 2: boxes[0] = &box1_2; boxes[1] = &box2_2; boxes[2] = &box3_2; boxes[3] = &box4_2; break; // T
            case 3: boxes[0] = &box1_3; boxes[1] = &box2_3; boxes[2] = &box3_3; boxes[3] = &box4_3; break; // S
            case 4: boxes[0] = &box1_4; boxes[1] = &box2_4; boxes[2] = &box3_4; boxes[3] = &box4_4; break; // Z
            case 5: boxes[0] = &box1_5; boxes[1] = &box2_5; boxes[2] = &box3_5; boxes[3] = &box4_5; break; // L
            case 6: boxes[0] = &box1_6; boxes[1] = &box2_6; boxes[2] = &box3_6; boxes[3] = &box4_6; break; // J
            default: return;
        }

        for (int i = 0; i < 4; i++)
        {
            int16_t cellX = currentPiece.cells[i][0];
            int16_t cellY = currentPiece.cells[i][1];
            boxes[i]->setXY(cellX * 12, cellY * 12);
            boxes[i]->setVisible(true);
            boxes[i]->invalidate();
        }
        cont->setXY(currentX * 12 + 60, currentY * 12 + 40);
        cont->setVisible(true);
        cont->invalidate();
    }
};

#endif // SCREEN1VIEW_HPP
