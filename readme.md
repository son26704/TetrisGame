# 🎮 Tetris Game on STM32F429I-DISCOVERY

A classic **Tetris game** implemented on the **STM32F429I-DISCOVERY** board using **TouchGFX**, **FreeRTOS**, and various peripherals like tactile buttons, touch input, and a passive buzzer.

---

## 📋 Project Overview

- **Team Members**:
  - Nguyễn Trung Sơn – 20225225  
  - Nguyễn Quang Huy – 20225336  
  - Ngân Văn Thiện – 20215647  

- **Instructor**: Dr. Ngô Lam Trung  
- **Platform**: STM32F429I-DISCOVERY (STM32F429ZIT6, ARM Cortex-M4)  
- **Tools**: STM32CubeIDE, TouchGFX  
- **Hardware**:
  - 2.4" TFT LCD (ILI9341) with touch
  - Passive buzzer (KY-006)
  - Tactile buttons

- **Goal**: Build an interactive Tetris game with smooth graphics, responsive controls, sound effects, and high score saving — demonstrating embedded systems and GUI design.

---

## ✨ Features

### 🎮 Gameplay
- 10x20 Tetris grid with standard 7 tetrominoes (I, O, T, S, Z, L, J)
- Random piece generation using hardware RNG ("bag" system)
- Game states: **play**, **pause**, **game over**, **restart**
- Controls:
  - Move left: PG2
  - Move right: PG3
  - Rotate clockwise: PC4
  - Soft drop: PC5
  - Start/pause: PA0 or touch button

### ⌨️ Input Handling
- Four tactile buttons + onboard PA0
- Touch input for start/pause
- Debouncing logic for accurate presses

### 📺 Display (LCD)
- Resolution: 240x320 pixels
- Displays: game board, next tetromino, score, high score, game status
- Visual effects for line clears and game over

### 🔊 Sound
- Sound effects for:
  - Movement, rotation, drop
  - Line clear, game over, level up
- Tetris theme music via passive buzzer (PA9, PWM TIM1_CH2)
- Music toggle available

### 💾 Data Storage
- High score stored in flash memory (sector 11)

### ⏱️ Real-Time Operation (FreeRTOS)
- Separate tasks for:
  - Game logic
  - Input polling
  - GUI rendering
  - Sound generation
- Smooth ~30 FPS updates
- Responsive control handling

---

## 📦 Hardware Setup

### 🧩 STM32F429I-DISCOVERY Board
- **MCU**: STM32F429ZIT6 (180 MHz, 2 MB Flash, 256 KB RAM)
- **LCD**: 2.4" TFT (ILI9341, 240x320) with touch panel
- **Onboard button**: PA0 (Start/Pause)
- **SDRAM**: IS42S16400J

### 🧪 Test Board Components
- Tactile Buttons:
  - PG2 – Left
  - PG3 – Right
  - PC4 – Rotate
  - PC5 – Soft drop
- Passive Buzzer (KY-006):
  - Signal: PA9 (TIM1_CH2 PWM)
  - Voltage: 3–5V, Frequency: ~2 kHz

### 🔌 Connections
- **Buttons**: Connected to GPIO with pull-ups, common GND
- **Buzzer**: PA9 (signal), 3.3V (VCC), GND
- **LCD**: SPI5 (5.6 MHz), PC2 (CS), PD13 (DC), I2C3 for touch
- **SDRAM**: FMC Bank 2 (90 MHz)

---

## 🧠 Software Architecture

### 🧩 Modules

- **Game Logic** (`Screen1View.cpp`):
  - Handles movement, collision, scoring, line clear
  - 20x10 grid, each piece as structure

- **GUI** (`Screen1View.cpp`, `Screen1ViewBase.cpp`):
  - Draws grid, next piece, score, and states
  - TouchGFX containers and custom visuals

- **Input** (`main.c` – `StartInputTask`):
  - Button polling every 5 ms with debounce
  - Sends control signals via FreeRTOS queue

- **Sound** (`main.c` – `StartSoundTask`, `Screen1View.cpp`):
  - PWM tone generation (200–2000 Hz)
  - Music toggle control

- **Tetromino RNG** (`main.c` – `StartGameTask`):
  - Uses hardware RNG to shuffle tetromino bag

- **High Score** (`Screen1View.cpp`):
  - Stored in Flash memory (sector 11)

- **RTOS Tasks**:
  - `defaultTask`, `GUI_Task`, `gameTask`, `inputTask`, `soundTask`
  - Queues: `pieceQueue`, `controlQueue`, `soundQueue`

---

## 🚀 How to Run

1. **Clone the Repository**
   ```bash
   git clone https://github.com/your-username/tetris-stm32.git
   ```

2. **Open the Project**
   - Import the project into STM32CubeIDE
   - Ensure TouchGFX is installed and properly configured

3. **Connect the Hardware**
   - Wire the buttons and buzzer according to the Hardware Setup section

4. **Build and Flash**
   - Build the project using STM32CubeIDE
   - Flash the firmware to the STM32F429I-DISCOVERY board

5. **Play the Game**
   - Power on the board
   - Use the buttons or touchscreen to play

---

## 🎮 Controls

| Action         | Pin/Button       |
|----------------|------------------|
| Move Left      | PG2              |
| Move Right     | PG3              |
| Rotate         | PC4              |
| Soft Drop      | PC5              |
| Start / Pause  | PA0 / Touch      |

---

## 🧮 Scoring

| Lines Cleared | Points |
|----------------|--------|
| 1 line         | 100    |
| 2 lines        | 300    |
| 3 lines        | 600    |
| 4 lines        | 1000   |

- 🎵 Background music (Tetris theme) can be toggled on/off  
- 💾 High score is automatically saved to Flash memory (sector 11)

---

## ✅ Evaluation

- ✅ Full-featured and stable Tetris gameplay  
- ✅ Responsive and debounced button input  
- ✅ Smooth GUI rendering (~30 FPS)  
- ✅ Realistic sound effects and music toggle  
- ✅ Fair tetromino distribution via hardware RNG  
- ✅ High score persistence across resets  

---
## Demo
- https://drive.google.com/file/d/148uGUMPHRMTenNse1cBFuDzeimCEjjtp/view

## 🔮 Future Improvements

- Use DAC or active buzzer for richer sound output  
- Implement full touch-based controls (swipe/rotate)  
- Add power-saving mode (e.g., dim screen during pause)  
- Design a custom PCB to integrate buttons and buzzer more cleanly  

---

## 🙏 Acknowledgments

- Dr. Ngô Lam Trung – for guidance and support  
- STMicroelectronics – for hardware and development tools  
- TouchGFX Team – for the GUI framework  

