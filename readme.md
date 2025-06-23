Tetris Game on STM32F429I-DISCOVERY 🎮
This project implements the classic Tetris game on the STM32F429I-DISCOVERY board, powered by the STM32F429ZIT6 microcontroller with an ARM Cortex-M4 core. The game features a vibrant graphical user interface (GUI) designed with TouchGFX, displayed on a 2.4-inch TFT LCD, and includes physical button controls, touch input, and sound effects via a passive buzzer. FreeRTOS ensures smooth real-time task management for an engaging gameplay experience.
📋 Project Overview

Team Members:
Nguyễn Trung Sơn (20225225)
Nguyễn Quang Huy (20225336)
Ngân Văn Thiện (20215647)


Instructor: Dr. Ngô Lam Trung
Platform: STM32F429I-DISCOVERY board
Development Tools: STM32CubeIDE, TouchGFX
Hardware: 2.4-inch TFT LCD (ILI9341), passive buzzer (KY-006), tactile buttons
Objective: Create an interactive Tetris game with intuitive controls, dynamic visuals, and sound effects, showcasing embedded system programming, GUI development, and hardware-software integration.

✨ Features
Functional Requirements

Gameplay Mechanics 🕹️:
Classic Tetris with a 10x20 grid and seven standard tetromino shapes (I, O, T, S, Z, L, J).
Random tetromino generation using a "bag" system with hardware RNG.
Player controls: move left (PG2), move right (PG3), rotate clockwise (PC4), soft drop (PC5), start/pause (PA0 or touch button).
Game states: play, pause, game over with restart functionality.


Input Handling ⌨️:
Four tactile buttons and one onboard button (PA0) for gameplay controls.
Touch input on LCD for start/pause functionality.
Debouncing for reliable button presses.


Display 📺:
240x320 pixel LCD showing the game grid, current/next tetromino, score, high score, and game status.
Unique colors for each tetromino type.
Visual effects for line clears and game over.


Sound 🔊:
Sound effects for movement, rotation, drop, line clear, game over, level-up, and soft drop.
Tetris theme music played via passive buzzer (PA9, PWM TIM1_CH2).
Toggle music on/off.


Data Storage 💾:
High score saved in flash memory (sector 11).


Real-Time Operation ⏱️:
FreeRTOS tasks manage game logic, input, GUI updates, and sound.
Smooth gameplay with responsive controls and consistent frame updates.



Non-Functional Requirements

Reliability 🛡️: Stable operation without crashes during extended play.
Performance ⚡: ~30 FPS GUI updates, adjustable tetromino drop speed (normal: 30 ticks, soft drop: 3 ticks).
Hardware Durability 🔧: Secure button and buzzer connections.
Usability 😊: Intuitive interface with clear visual and auditory feedback.
Portability 🎒: Compact design within the STM32F429I-DISCOVERY and test board.
Maintainability 📝: Modular code with clear documentation, adhering to STM32CubeIDE and TouchGFX conventions.

🛠️ Hardware Setup
Components

STM32F429I-DISCOVERY Board:
MCU: STM32F429ZIT6 (180 MHz, 2 MB Flash, 256 KB SRAM).
LCD: 2.4-inch TFT (240x320, ILI9341) with touch panel.
Onboard button: PA0 (start/pause).
SDRAM: IS42S16400J (64 Mbit).


Test Board:
Four tactile buttons: PG2 (left), PG3 (right), PC4 (rotate), PC5 (soft drop).
Passive buzzer (KY-006): PA9 (PWM TIM1_CH2), 1.5–2.5 kHz, 3–5V.



Connections

Buttons: Connected to GPIO pins with pull-up resistors (except PA0), sharing a common ground.
Buzzer: Signal to PA9, VCC to 3.3V, GND to common ground.
LCD: SPI5 (~5.625 MHz), chip select (PC2), data/command (PD13), touch panel via I2C3 (100 kHz).
SDRAM: FMC Bank 2 (90 MHz).

💻 Software Architecture
Modules

Game Logic (Screen1View.cpp) 🎲:
Manages tetromino movement, collision detection, line clearing, and scoring.
Uses a 20x10 grid array and tetromino structure.


GUI (Screen1ViewBase.cpp, Screen1View.cpp) 🖼️:
Displays game elements using TouchGFX containers and boxes.
Handles visual effects and touch input.


Input Handling (main.c: StartInputTask) 🖱️:
Polls buttons every 5 ms with debouncing, sends control signals via FreeRTOS queue.


Sound (main.c: StartSoundTask, Screen1View.cpp) 🎵:
Generates PWM signals (200–2000 Hz) for effects and music.


Tetromino Generation (main.c: StartGameTask) 🔢:
Shuffles tetromino bag using hardware RNG.


High Score Management (Screen1View.cpp) 🏆:
Reads/writes high score to flash memory.


Task Management (main.c) ⏳:
FreeRTOS tasks: defaultTask, GUI_Task, gameTask, inputTask, soundTask.
Queues: pieceQueue, controlQueue, soundQueue.



Development Tools

STM32CubeIDE: For coding, debugging, and configuring MCU peripherals.
TouchGFX: For designing and rendering the GUI.
FreeRTOS: For real-time task scheduling.

🚀 Installation and Setup

Clone the Repository:
git clone https://github.com/your-username/tetris-stm32.git


Open Project:

Import the project into STM32CubeIDE.
Ensure TouchGFX is installed and configured.


Hardware Connections:

Connect buttons and buzzer to the test board as specified.
Ensure the STM32F429I-DISCOVERY board is powered and connected to the PC.


Build and Flash:

Build the project in STM32CubeIDE.
Flash the firmware to the STM32F429I-DISCOVERY board.


Run the Game:

Power on the board.
Use buttons (PG2, PG3, PC4, PC5, PA0) or touch input to play.



🎮 Usage

Controls:
Left (PG2): Move tetromino left ⬅️.
Right (PG3): Move tetromino right ➡️.
Rotate (PC4): Rotate tetromino clockwise 🔄.
Soft Drop (PC5): Speed up tetromino fall ⬇️.
Start/Pause (PA0 or touch button): Start or pause the game ⏯️.


Scoring:
Clear 1 line: 100 points 🥉.
Clear 2 lines: 300 points 🥈.
Clear 3 lines: 600 points 🥇.
Clear 4 lines: 1000 points 🏅.


Music: Toggle background music (Tetris theme) on/off via game logic 🎶.
High Score: Automatically saved to flash memory 💾.

✅ Evaluation

Achievements:
Fully functional Tetris gameplay with all required mechanics.
Responsive input handling with debouncing.
Smooth GUI at ~30 FPS with clear visual effects.
Reliable sound effects and toggleable music.
Fair tetromino distribution via RNG.
Persistent high score storage.


Non-Functional Goals:
Stable and crash-free operation ✅.
Durable hardware connections 🔩.
Intuitive and portable design 🎒.
Modular and maintainable code 📜.



🔮 Future Improvements

Replace passive buzzer with active buzzer or DAC for richer audio 🎙️.
Implement touch-based controls for movement and rotation 👆.
Add power-saving mode (e.g., dim LCD during pause) 🔋.
Design a custom PCB for better button and buzzer integration 🛠️.

🙏 Acknowledgments

Dr. Ngô Lam Trung for guidance and support.
STMicroelectronics for STM32 hardware and tools.
TouchGFX team for GUI development framework.
