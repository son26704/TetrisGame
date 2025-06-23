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

    // Các tần số note cơ bản (8-bit style) - giá trị từ 0-255
    static const uint8_t NOTE_C4 = 50;   // ~262 Hz
    static const uint8_t NOTE_D4 = 60;   // ~294 Hz
    static const uint8_t NOTE_E4 = 70;   // ~330 Hz
    static const uint8_t NOTE_F4 = 75;   // ~349 Hz
    static const uint8_t NOTE_G4 = 85;   // ~392 Hz
    static const uint8_t NOTE_A4 = 95;   // ~440 Hz
    static const uint8_t NOTE_B4 = 105;  // ~494 Hz
    static const uint8_t NOTE_C5 = 110;  // ~523 Hz
    static const uint8_t NOTE_D5 = 120;  // ~587 Hz
    static const uint8_t NOTE_E5 = 130;  // ~659 Hz
    static const uint8_t NOTE_F5 = 135;  // ~698 Hz
    static const uint8_t NOTE_G5 = 145;  // ~784 Hz
    static const uint8_t NOTE_A5 = 155;  // ~880 Hz
    static const uint8_t NOTE_B5 = 165;  // ~988 Hz
    static const uint8_t NOTE_C6 = 175;  // ~1047 Hz

    // Background music state
    bool backgroundMusicEnabled = true;
    uint8_t currentMelodyIndex = 0;
    uint32_t musicTick = 0;

    // Tetris theme melody (simplified)
    struct MusicNote {
        uint8_t frequency;
        uint16_t duration; // Sửa thành uint16_t để hỗ trợ giá trị lớn
    };

    static const MusicNote tetrisTheme[32]; // Chỉ khai báo, định nghĩa trong .cpp

    // Hàm phát âm thanh đơn giản
    void playSound(uint8_t frequency, uint16_t duration)
    {
        uint16_t soundData = (frequency << 8) | (duration & 0xFF);
        osMessageQueuePut(soundQueueHandle, &soundData, 0, 0);
    }

    // Hàm dừng âm thanh
    void stopSound()
    {
        uint16_t soundData = 0; // frequency = 0 để dừng
        osMessageQueuePut(soundQueueHandle, &soundData, 0, 0);
    }

    // Các âm thanh game cụ thể
    void playMoveSound()
    {
        playSound(NOTE_C4, 50);
    }

    void playRotateSound()
    {
        playSound(NOTE_E4, 80);
    }

    void playDropSound()
    {
        playSound(NOTE_G4, 60);
    }

    void playLineClearSound()
    {
        // Phát một chuỗi âm thanh ngắn
        playSound(NOTE_C5, 100);
        osDelay(50);
        playSound(NOTE_E5, 100);
        osDelay(50);
        playSound(NOTE_G5, 150);
    }

    void playGameOverSound()
    {
        // Âm thanh game over - từ cao xuống thấp
        playSound(NOTE_C6, 200);
        osDelay(100);
        playSound(NOTE_A5, 200);
        osDelay(100);
        playSound(NOTE_F5, 200);
        osDelay(100);
        playSound(NOTE_C5, 400);
    }

    void playLevelUpSound()
    {
        // Âm thanh tăng level - từ thấp lên cao
        playSound(NOTE_C4, 100);
        osDelay(50);
        playSound(NOTE_E4, 100);
        osDelay(50);
        playSound(NOTE_G4, 100);
        osDelay(50);
        playSound(NOTE_C5, 200);
    }

    void playFastDropSound()
    {
        playSound(NOTE_A4, 30);
    }

    // Background music control
    void playBackgroundMusic()
    {
        if (!backgroundMusicEnabled) return;

        musicTick++;
        if (musicTick % 60 == 0) // Mỗi giây chơi một note
        {
            MusicNote note = tetrisTheme[currentMelodyIndex];
            playSound(note.frequency, note.duration);

            currentMelodyIndex++;
            if (currentMelodyIndex >= 32)
            {
                currentMelodyIndex = 0; // Lặp lại
            }
        }
    }

    void toggleBackgroundMusic()
    {
        backgroundMusicEnabled = !backgroundMusicEnabled;
        if (!backgroundMusicEnabled)
        {
            stopSound();
        }
    }

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
};

#endif // SCREEN1VIEW_HPP
