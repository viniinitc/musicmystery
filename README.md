# Musicmystery




## 🛠️ How to compile

💻 **How to play on Linux**

1. Make sure you have GCC, make and git:
   ```
   sudo apt install build-essential git
   ```

2. Install all needed libraries:
   ```
   sudo apt install libasound2-dev libx11-dev libxrandr-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxcursor-dev libxinerama-dev libwayland-dev libxkbcommon-dev
   ```

3. Download Raylib and get in the folder:
   ```
   git clone --depth 1 https://github.com/raysan5/raylib.git raylib
   cd raylib/src/
   ```

4. Compile Raylib:
   ```
   make
   ```

5. Install Raylib:
   ```
   sudo make install
   cd ~
   ```

6. Clone the game repository:
   ```
   git clone https://github.com/GMiranda21ML/Jogo-PIF.git
   cd musicmystery
   ```

7. Compile:
   ```
   make

8. Run the game:

   cd bin/Debug/
   ./musicmystery
   ```



Caso ocorra algum problema, consulte a documentação da Raylib para Linux: [Raylib para Linux](https://github.com/raysan5/raylib/wiki/Working-on-GNU-Linux).


---

## 📜 Main menu

A tela de menu oferece as seguintes opções:

![Menu](assets\img\menujogo.png)

- 🟡 **Endless**: To start the game.
- 🏆 **Highscore**: To check your highest score.
- ⌨️ **Credits**: To check credits.
- ❌ **Exit**: To close the game.

---

## 🕹️ How to play

### Keybinds:

- Use the arrow keys to control the player:
  - **W** — To go left downwards
  - **A** — To go right downwards
  - **D** — To go right upwards
  - **L** — To go left upwards
  - **Space** — To shoot
  - **1** — To switch between notes
  - **Esc** — To pause


---

## 🎬 Demonstring how to execute the game

[![Assista ao vídeo no YouTube](https://img.youtube.com/vi/CFFP_qnYuyY/maxresdefault.jpg)](https://youtu.be/CFFP_qnYuyY)

---

## 🗺️ What the game is about

**Music mystery** is a turn/grid based game where you have to kill all the enemies to save yourself from this twisted dream. Good for you that the enemies can only move when you move. But be careful, certain enemies can only hear certain sounds. How long can you survive in this messed up dream.

---


### 🗝️ Objective

- Get to the stair without letting the enemies reach you or your musical soul, and get the maximum point you can.

---


## 📊 Presentation

[Veja os slides no Canva.](https://www.canva.com/design/DAGoTto-qis/6Rn61_Hm-M_6Z0kySZ2rDg/view?utm_content=DAGoTto-qis&utm_campaign=designshare&utm_medium=link2&utm_source=sharebutton)


## 👥 Developer

- [**Vinicius Tenorio**](https://github.com/viniinitc) 
