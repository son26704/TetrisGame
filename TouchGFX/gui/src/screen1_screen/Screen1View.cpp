#include <gui/screen1_screen/Screen1View.hpp>
#include "main.h"

Screen1View::Screen1View()
    : tetrominoContainer(TetrominoContainer),
      tetrominoContainerO(TetrominoContainerO),
      tetrominoContainerT(TetrominoContainerT),
      tetrominoContainerS(TetrominoContainerS),
      tetrominoContainerZ(TetrominoContainerZ),
      tetrominoContainerL(TetrominoContainerL),
      tetrominoContainerJ(TetrominoContainerJ),
      // Khởi tạo box con trong TetrominoContainer (I)
      box1(*static_cast<Box*>(TetrominoContainer.getFirstChild())),
      box2(*static_cast<Box*>(box1.getNextSibling())),
      box3(*static_cast<Box*>(box2.getNextSibling())),
      box4(*static_cast<Box*>(box3.getNextSibling())),
      // TetrominoContainerO (O)
      box1_1(*static_cast<Box*>(TetrominoContainerO.getFirstChild())),
      box2_1(*static_cast<Box*>(box1_1.getNextSibling())),
      box3_1(*static_cast<Box*>(box2_1.getNextSibling())),
      box4_1(*static_cast<Box*>(box3_1.getNextSibling())),
      // TetrominoContainerT (T)
      box1_2(*static_cast<Box*>(TetrominoContainerT.getFirstChild())),
      box2_2(*static_cast<Box*>(box1_2.getNextSibling())),
      box3_2(*static_cast<Box*>(box2_2.getNextSibling())),
      box4_2(*static_cast<Box*>(box3_2.getNextSibling())),
      // TetrominoContainerS (S)
      box1_3(*static_cast<Box*>(TetrominoContainerS.getFirstChild())),
      box2_3(*static_cast<Box*>(box1_3.getNextSibling())),
      box3_3(*static_cast<Box*>(box2_3.getNextSibling())),
      box4_3(*static_cast<Box*>(box3_3.getNextSibling())),
      // TetrominoContainerZ (Z)
      box1_4(*static_cast<Box*>(TetrominoContainerZ.getFirstChild())),
      box2_4(*static_cast<Box*>(box1_4.getNextSibling())),
      box3_4(*static_cast<Box*>(box2_4.getNextSibling())),
      box4_4(*static_cast<Box*>(box3_4.getNextSibling())),
      // TetrominoContainerL (L)
      box1_5(*static_cast<Box*>(TetrominoContainerL.getFirstChild())),
      box2_5(*static_cast<Box*>(box1_5.getNextSibling())),
      box3_5(*static_cast<Box*>(box2_5.getNextSibling())),
      box4_5(*static_cast<Box*>(box3_5.getNextSibling())),
      // TetrominoContainerJ (J)
      box1_6(*static_cast<Box*>(TetrominoContainerJ.getFirstChild())),
      box2_6(*static_cast<Box*>(box1_6.getNextSibling())),
      box3_6(*static_cast<Box*>(box2_6.getNextSibling())),
      box4_6(*static_cast<Box*>(box3_6.getNextSibling())),
      // NextTetrominoContainer
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
