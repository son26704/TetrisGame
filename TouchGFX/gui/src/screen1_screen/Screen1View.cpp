#include <gui/screen1_screen/Screen1View.hpp>
#include "main.h"
#define FLASH_HIGH_SCORE_ADDRESS 0x080E0000

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
      nextCell4(*static_cast<Box*>(nextCell3.getNextSibling())),
      gameOverText(Screen1ViewBase::gameOverText)
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

    readHighScoreFromFlash();
}

Screen1View::~Screen1View()
{
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
    Unicode::snprintf(gameOverTextBuffer, GAMEOVERTEXT_SIZE, "HIGH SCORE: %d", highScore);
    gameOverText.invalidate();
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

    if (!isPaused && !isGameOver)
    {
        playBackgroundMusic();
    }

    handleControlInput();

    if (isPaused || isGameOver)
    {
        if (isGameOverEffect) updateGameOverEffect();
        return;
    }

    if (isLineClearEffect)
    {
        updateLineClearEffect();
        return;
    }

    if (isScoreHighlight)
    {
        updateScoreHighlight();
    }

    tickCount++;

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
            int linesCleared = checkAndClearLines();
            if (linesCleared > 0)
            {
                highlightScore();
            }
            updateBoardDisplay();
            updateScoreDisplay();

            currentPiece = nextPiece;
            currentX = 3;
            currentY = -1;

            if (checkCollision(currentX, currentY))
            {
                isGameOver = true;
                startGameOverEffect();
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
    uint8_t controlSignal;

    while (osMessageQueueGet(controlQueueHandle, &controlSignal, NULL, 0) == osOK)
    {
        if (controlSignal == 6)
        {
            buttonClicked();
            continue;
        }

        if (isPaused || isGameOver)
        {
            continue;
        }

        if (controlSignal == 1 && !checkCollision(currentX - 1, currentY))
        {
            currentX--;
            updateCurrentTetrominoUI();
            playMoveSound();
        }
        else if (controlSignal == 2 && !checkCollision(currentX + 1, currentY))
        {
            currentX++;
            updateCurrentTetrominoUI();
            playMoveSound();
        }
        else if (controlSignal == 3)
        {
            rotatePiece();
            playRotateSound();
        }
        else if (controlSignal == 4)
        {
            isFastDropping = true;
            playFastDropSound();
        }
        else if (controlSignal == 5)
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
    int clearedRows[4] = {0};

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
            clearedRows[linesCleared] = row;
            linesCleared++;
        }
    }

    int points[] = {0, 100, 300, 600, 1000};
    if (linesCleared > 0)
    {
        score += points[linesCleared];
        startLineClearEffect(clearedRows, linesCleared);
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


void Screen1View::startLineClearEffect(int rows[], int count)
{
    isLineClearEffect = true;
    lineClearEffectTick = 0;
    numLinesCleared = count;
    for (int i = 0; i < count; i++)
    {
        lineClearRows[i] = rows[i];
        for (int col = 0; col < 10; col++)
        {
            boardCells[rows[i]][col].setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
            boardCells[rows[i]][col].setVisible(true);
            boardCells[rows[i]][col].invalidate();
        }
    }
}

void Screen1View::updateLineClearEffect()
{
    lineClearEffectTick++;
    if (lineClearEffectTick < 15)
    {
        bool visible = (lineClearEffectTick % 3) == 0;
        for (int i = 0; i < numLinesCleared; i++)
        {
            for (int col = 0; col < 10; col++)
            {
                boardCells[lineClearRows[i]][col].setVisible(visible);
                boardCells[lineClearRows[i]][col].invalidate();
            }
        }
    }
    else
    {
        // Tạo một bảng tạm mới và chỉ copy lại các hàng không bị xóa
        uint8_t newBoard[20][10] = {0};
        int newRow = 19;

        // Duyệt từ dưới lên
        for (int row = 19; row >= 0; row--)
        {
            bool isCleared = false;
            for (int i = 0; i < numLinesCleared; i++)
            {
                if (row == lineClearRows[i])
                {
                    isCleared = true;
                    break;
                }
            }

            if (!isCleared)
            {
                for (int col = 0; col < 10; col++)
                {
                    newBoard[newRow][col] = board[row][col];
                }
                newRow--;
            }
        }

        // Gán lại
        for (int row = 0; row < 20; row++)
        {
            for (int col = 0; col < 10; col++)
            {
                board[row][col] = newBoard[row][col];
            }
        }

        isLineClearEffect = false;
        updateBoardDisplay();
    }
}


void Screen1View::startGameOverEffect()
{
    isGameOverEffect = true;
    gameOverEffectTick = 0;
    for (int row = 0; row < 20; row++)
    {
        for (int col = 0; col < 10; col++)
        {
            if (board[row][col] != 0)
            {
                boardCells[row][col].setColor(touchgfx::Color::getColorFromRGB(255, 0, 0));
                boardCells[row][col].invalidate();
            }
        }
    }
}

void Screen1View::updateGameOverEffect()
{
    gameOverEffectTick++;
    if (gameOverEffectTick < 30)
    {
        uint8_t red = 255 - (gameOverEffectTick * 8);
        for (int row = 0; row < 20; row++)
        {
            for (int col = 0; col < 10; col++)
            {
                if (board[row][col] != 0)
                {
                    boardCells[row][col].setColor(touchgfx::Color::getColorFromRGB(red, 0, 0));
                    boardCells[row][col].invalidate();
                }
            }
        }
    }
    else
    {
        isGameOverEffect = false;
        updateBoardDisplay();
    }
}

void Screen1View::highlightScore()
{
    isScoreHighlight = true;
    scoreHighlightTick = 0;
    scoreText.setColor(touchgfx::Color::getColorFromRGB(255, 255, 0));
    scoreText.invalidate();
}

void Screen1View::updateScoreHighlight()
{
    scoreHighlightTick++;
    if (scoreHighlightTick >= 10)
    {
        isScoreHighlight = false;
        scoreText.setColor(touchgfx::Color::getColorFromRGB(0, 0, 255));
        scoreText.invalidate();
    }
}

void Screen1View::updateScoreDisplay()
{
    if (isGameOver)
    {
        Unicode::snprintf(scoreTextBuffer, SCORETEXT_SIZE, "%d", score);
        if (score > highScore)
        {
            writeHighScoreToFlash(score);
            Unicode::snprintf(gameOverTextBuffer, GAMEOVERTEXT_SIZE, "NEW HIGH SCORE!");
        }
        else
        {
            Unicode::snprintf(gameOverTextBuffer, GAMEOVERTEXT_SIZE, "GAME OVER");
        }
        gameOverText.invalidate();
        playGameOverSound();
    }
    else
    {
        Unicode::snprintf(scoreTextBuffer, SCORETEXT_SIZE, isPaused ? "Paused" : "%d", score);
        if (score > highScore)
        {
            Unicode::snprintf(gameOverTextBuffer, GAMEOVERTEXT_SIZE, "NEW HIGH SCORE!");
        }
        else
        {
            Unicode::snprintf(gameOverTextBuffer, GAMEOVERTEXT_SIZE, "HIGH SCORE: %d", highScore);
        }
        gameOverText.invalidate();
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
    isFastDropping = false;
    isLineClearEffect = false;
    isGameOverEffect = false;
    isScoreHighlight = false;

    currentMelodyIndex = 0;
    musicTick = 0;
    backgroundMusicEnabled = true;

    uint8_t dummy;
    while (osMessageQueueGet(controlQueueHandle, &dummy, NULL, 0) == osOK) {}

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

    Unicode::snprintf(gameOverTextBuffer, GAMEOVERTEXT_SIZE, "HIGH SCORE: %d", highScore);
    gameOverText.invalidate();

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

    for (int i = 0; i < 4; i++)
    {
        int8_t x = currentPiece.cells[i][0];
        int8_t y = currentPiece.cells[i][1];
        currentPiece.cells[i][0] = -y;
        currentPiece.cells[i][1] = x;
    }

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

    Box* boxes[4];
    switch (pieceIndex)
    {
        case 0: boxes[0] = &box1;   boxes[1] = &box2;   boxes[2] = &box3;   boxes[3] = &box4;   break;
        case 1: boxes[0] = &box1_1; boxes[1] = &box2_1; boxes[2] = &box3_1; boxes[3] = &box4_1; break;
        case 2: boxes[0] = &box1_2; boxes[1] = &box2_2; boxes[2] = &box3_2; boxes[3] = &box4_2; break;
        case 3: boxes[0] = &box1_3; boxes[1] = &box2_3; boxes[2] = &box3_3; boxes[3] = &box4_3; break;
        case 4: boxes[0] = &box1_4; boxes[1] = &box2_4; boxes[2] = &box3_4; boxes[3] = &box4_4; break;
        case 5: boxes[0] = &box1_5; boxes[1] = &box2_5; boxes[2] = &box3_5; boxes[3] = &box4_5; break;
        case 6: boxes[0] = &box1_6; boxes[1] = &box2_6; boxes[2] = &box3_6; boxes[3] = &box4_6; break;
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

void Screen1View::readHighScoreFromFlash()
{
    highScore = *(volatile uint32_t*)FLASH_HIGH_SCORE_ADDRESS;
    if (highScore == 0xFFFFFFFF)
    {
        highScore = 0;
    }
}

void Screen1View::writeHighScoreToFlash(uint32_t newScore)
{
    HAL_FLASH_Unlock();
    FLASH_EraseInitTypeDef eraseInit = {0};
    uint32_t sectorError = 0;

    eraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    eraseInit.Sector = FLASH_SECTOR_11;
    eraseInit.NbSectors = 1;
    eraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    if (HAL_FLASHEx_Erase(&eraseInit, &sectorError) == HAL_OK)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_HIGH_SCORE_ADDRESS, newScore) == HAL_OK)
        {
            highScore = newScore;
        }
    }
    HAL_FLASH_Lock();
}

void Screen1View::playSound(uint8_t frequency, uint16_t duration)
{
    uint16_t soundData = (frequency << 8) | (duration & 0xFF);
    osMessageQueuePut(soundQueueHandle, &soundData, 0, 0);
}

void Screen1View::stopSound()
{
    uint16_t soundData = 0;
    osMessageQueuePut(soundQueueHandle, &soundData, 0, 0);
}

void Screen1View::playMoveSound()
{
    playSound(NOTE_C4, 50);
}

void Screen1View::playRotateSound()
{
    playSound(NOTE_E4, 80);
}

void Screen1View::playDropSound()
{
    playSound(NOTE_G4, 60);
}

void Screen1View::playLineClearSound()
{
    playSound(NOTE_C5, 100);
    osDelay(50);
    playSound(NOTE_E5, 100);
    osDelay(50);
    playSound(NOTE_G5, 150);
}

void Screen1View::playGameOverSound()
{
    playSound(NOTE_C6, 200);
    osDelay(100);
    playSound(NOTE_A5, 200);
    osDelay(100);
    playSound(NOTE_F5, 200);
    osDelay(100);
    playSound(NOTE_C5, 400);
}

void Screen1View::playLevelUpSound()
{
    playSound(NOTE_C4, 100);
    osDelay(50);
    playSound(NOTE_E4, 100);
    osDelay(50);
    playSound(NOTE_G4, 100);
    osDelay(50);
    playSound(NOTE_C5, 200);
}

void Screen1View::playFastDropSound()
{
    playSound(NOTE_A4, 30);
}

void Screen1View::playBackgroundMusic()
{
    if (!backgroundMusicEnabled) return;

    musicTick++;
    if (musicTick % 60 == 0)
    {
        MusicNote note = tetrisTheme[currentMelodyIndex];
        playSound(note.frequency, note.duration);

        currentMelodyIndex++;
        if (currentMelodyIndex >= 32)
        {
            currentMelodyIndex = 0;
        }
    }
}

void Screen1View::toggleBackgroundMusic()
{
    backgroundMusicEnabled = !backgroundMusicEnabled;
    if (!backgroundMusicEnabled)
    {
        stopSound();
    }
}
