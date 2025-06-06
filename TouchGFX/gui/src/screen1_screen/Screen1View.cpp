#include <gui/screen1_screen/Screen1View.hpp>
#include "main.h"

// Định nghĩa mảng tetrisTheme
const Screen1View::MusicNote Screen1View::tetrisTheme[32] = {
    {NOTE_E5, 200}, {NOTE_B4, 100}, {NOTE_C5, 100}, {NOTE_D5, 200}, {NOTE_C5, 100}, {NOTE_B4, 100},
    {NOTE_A4, 200}, {NOTE_A4, 100}, {NOTE_C5, 100}, {NOTE_E5, 200}, {NOTE_D5, 100}, {NOTE_C5, 100},
    {NOTE_B4, 300}, {NOTE_C5, 100}, {NOTE_D5, 200}, {NOTE_E5, 200},
    {NOTE_C5, 200}, {NOTE_A4, 200}, {NOTE_A4, 400},
    {NOTE_D5, 200}, {NOTE_F5, 100}, {NOTE_A5, 200}, {NOTE_G5, 100}, {NOTE_F5, 100},
    {NOTE_E5, 300}, {NOTE_C5, 100}, {NOTE_E5, 200}, {NOTE_D5, 100}, {NOTE_C5, 100},
    {NOTE_B4, 200}, {NOTE_B4, 100}, {NOTE_C5, 100}
};

Screen1View::Screen1View()
    : tetrominoContainer(TetrominoContainer),
      tetrominoContainerO(TetrominoContainerO),
      tetrominoContainerT(TetrominoContainerT),
      tetrominoContainerS(TetrominoContainerS),
      tetrominoContainerZ(TetrominoContainerZ),
      tetrominoContainerL(TetrominoContainerL),
      tetrominoContainerJ(TetrominoContainerJ),
      box1(*static_cast<Box*>(TetrominoContainer.getFirstChild())),
      box2(*static_cast<Box*>(box1.getNextSibling())),
      box3(*static_cast<Box*>(box2.getNextSibling())),
      box4(*static_cast<Box*>(box3.getNextSibling())),
      box1_1(*static_cast<Box*>(TetrominoContainerO.getFirstChild())),
      box2_1(*static_cast<Box*>(box1_1.getNextSibling())),
      box3_1(*static_cast<Box*>(box2_1.getNextSibling())),
      box4_1(*static_cast<Box*>(box3_1.getNextSibling())),
      box1_2(*static_cast<Box*>(TetrominoContainerT.getFirstChild())),
      box2_2(*static_cast<Box*>(box1_2.getNextSibling())),
      box3_2(*static_cast<Box*>(box2_2.getNextSibling())),
      box4_2(*static_cast<Box*>(box3_2.getNextSibling())),
      box1_3(*static_cast<Box*>(TetrominoContainerS.getFirstChild())),
      box2_3(*static_cast<Box*>(box1_3.getNextSibling())),
      box3_3(*static_cast<Box*>(box2_3.getNextSibling())),
      box4_3(*static_cast<Box*>(box3_3.getNextSibling())),
      box1_4(*static_cast<Box*>(TetrominoContainerZ.getFirstChild())),
      box2_4(*static_cast<Box*>(box1_4.getNextSibling())),
      box3_4(*static_cast<Box*>(box2_4.getNextSibling())),
      box4_4(*static_cast<Box*>(box3_4.getNextSibling())),
      box1_5(*static_cast<Box*>(TetrominoContainerL.getFirstChild())),
      box2_5(*static_cast<Box*>(box1_5.getNextSibling())),
      box3_5(*static_cast<Box*>(box2_5.getNextSibling())),
      box4_5(*static_cast<Box*>(box3_5.getNextSibling())),
      box1_6(*static_cast<Box*>(TetrominoContainerJ.getFirstChild())),
      box2_6(*static_cast<Box*>(box1_6.getNextSibling())),
      box3_6(*static_cast<Box*>(box2_6.getNextSibling())),
      box4_6(*static_cast<Box*>(box3_6.getNextSibling())),
      nextTetrominoContainer(NextTetrominoContainer),
      nextCell1(*static_cast<Box*>(NextTetrominoContainer.getFirstChild())),
      nextCell2(*static_cast<Box*>(nextCell1.getNextSibling())),
      nextCell3(*static_cast<Box*>(nextCell2.getNextSibling())),
      nextCell4(*static_cast<Box*>(nextCell3.getNextSibling()))
{
    for (int row = 0; row < 20; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            boardCells[row][col] = Box();
            boardCells[row][col].setWidthHeight(12, 12);
            boardCells[row][col].setVisible(false);
            add(boardCells[row][col]);
        }
    }

    tetrominoContainers[0] = &tetrominoContainer;
    tetrominoContainers[1] = &tetrominoContainerO;
    tetrominoContainers[2] = &tetrominoContainerT;
    tetrominoContainers[3] = &tetrominoContainerS;
    tetrominoContainers[4] = &tetrominoContainerZ;
    tetrominoContainers[5] = &tetrominoContainerL;
    tetrominoContainers[6] = &tetrominoContainerJ;
}

Screen1View::~Screen1View()
{
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
    resetGame();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::buttonClicked()
{
    if (isGameOver)
    {
        resetGame();
        return;
    }
    isPaused = !isPaused;
    updateScoreDisplay();
}

void Screen1View::handleTickEvent()
{
    Screen1ViewBase::handleTickEvent();

    // Phát nhạc nền
    if (!isPaused && !isGameOver)
    {
        playBackgroundMusic();
    }

    if (isPaused || isGameOver) return;

    tickCount++;

    // Xử lý input mỗi tick
    handleControlInput();

    uint32_t dropSpeed = isFastDropping ? 3 : 30;
    if (tickCount % dropSpeed == 0)
    {
        if (!checkCollision(currentX, currentY + 1))
        {
            currentY++;
            updateCurrentTetrominoUI();
        }
        else
        {
            int pieceIndex = currentPiece.color - 1;
            tetrominoContainers[pieceIndex]->setVisible(false);
            tetrominoContainers[pieceIndex]->invalidate();

            placePiece();
            checkAndClearLines();
            updateBoardDisplay();
            updateScoreDisplay();

            currentPiece = nextPiece;
            currentX = 3;
            currentY = -1;

            if (checkCollision(currentX, currentY))
            {
                isGameOver = true;
                updateScoreDisplay();
                return;
            }

            updateTetrominoDisplay();

            uint8_t pieceIndexNew;
            if (osMessageQueueGet(pieceQueueHandle, &pieceIndexNew, NULL, 0) == osOK)
            {
                nextPiece = tetrominoes[pieceIndexNew];
            }
            else
            {
                nextPiece = tetrominoes[0];
            }
            updateNextPieceDisplay();
        }
    }
}

void Screen1View::handleControlInput()
{
    if (isGameOver) return;

    uint8_t controlSignal;
    if (osMessageQueueGet(controlQueueHandle, &controlSignal, NULL, 0) == osOK)
    {
        if (controlSignal == 1 && !checkCollision(currentX - 1, currentY)) // Trái
        {
            currentX--;
            updateCurrentTetrominoUI();
            playMoveSound();
        }
        else if (controlSignal == 2 && !checkCollision(currentX + 1, currentY)) // Phải
        {
            currentX++;
            updateCurrentTetrominoUI();
            playMoveSound();
        }
        else if (controlSignal == 3) // Xoay
        {
            rotatePiece();
            playRotateSound();
        }
        else if (controlSignal == 4) // Rơi nhanh
        {
            isFastDropping = true;
            playFastDropSound();
        }
        else if (controlSignal == 5) // Nhả rơi nhanh
        {
            isFastDropping = false;
        }
    }
}

void Screen1View::placePiece()
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
    playDropSound();
}

int Screen1View::checkAndClearLines()
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
        if (linesCleared >= 4)
        {
            playLevelUpSound();
        }
        else
        {
            playLineClearSound();
        }
    }
    return linesCleared;
}

void Screen1View::updateScoreDisplay()
{
    if (isGameOver)
    {
        Unicode::snprintf(scoreTextBuffer, SCORETEXT_SIZE, "Game Over");
        playGameOverSound();
    }
    else
    {
        Unicode::snprintf(scoreTextBuffer, SCORETEXT_SIZE, isPaused ? "Paused" : "%d", score);
    }
    scoreText.invalidate();
}

void Screen1View::updateNextPieceDisplay()
{
    NextTetrominoContainer.setXY(186, 80);
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

void Screen1View::resetGame()
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
    currentMelodyIndex = 0;
    musicTick = 0;
    backgroundMusicEnabled = true;

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

void Screen1View::updateBoardDisplay()
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

void Screen1View::updateTetrominoDisplay()
{
    for (int i = 0; i < 7; i++)
    {
        tetrominoContainers[i]->setVisible(false);
        tetrominoContainers[i]->invalidate();
    }
    updateCurrentTetrominoUI();
}

bool Screen1View::checkCollision(int8_t newX, int8_t newY)
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

void Screen1View::rotatePiece()
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

void Screen1View::updateCurrentTetrominoUI()
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
