/*
    Nanochord simple Tetris library demo for Arduino

    Copyright (c) 2024 Norbert László

    This demo is written for Arduino Uno but it can easily be ported to any other boards as Nano, Mega, etc.
    using the necessary modifications.
    A 3.5" TFT LCD for Arduino Uno shield is used to display the Tetris playfield.
    This demo uses the following libraries to manage the TFT LCD display:
    - Adafruit GFX Library
    - MCUFRIEND_kbv
    - UTFTGLUE

    The game can be controlled through the serial input monitor using the following characters sent as message to the board:
    'q' = move block left
    'w' = move block right
    'e' = rotate block
    ' ' = drop block
    'p' = pause game
    's' = restart game after game over

    NOTE: Ensure that the Tetris.h header file is copied next to this sketch in the same folder.
*/


#include <MCUFRIEND_kbv.h>
#include <UTFTGLUE.h>
#include "Tetris.h"

#define ROW_COUNT    20
#define COLUMN_COUNT 10

// RRRRRGGG GGGBBBBB
#define COLOR565(r, g, b) (((r & 0xf8) << 8) | ((g & 0xfc) << 3) | ((b & 0xf8) >> 3))
#define BLOCKSIZE 16

UTFTGLUE gTFT(0x9326, A2, A1, A3, A4, A0);

int DWIDTH = gTFT.getDisplayXSize() * 2; // the real size is two times of what returned by the UTFTGLUE for somehow
int DHEIGHT = gTFT.getDisplayYSize();

// Declare which fonts we will be using
#if !defined(SmallFont)
extern uint8_t SmallFont[];
#endif

void DisplayGameOver();

class TetrisHost : public Nanochord::Host {
public:
  void SetColor(uint16_t color) {
    //rrrrrggg gggbbbbb
    gTFT.setColor(
      (color >> 8) & 0xf8,
      (color >> 3) & 0xfc,
      (color << 3) & 0xf8);
  }

  void SetBackColor(uint16_t color) {
    gTFT.setBackColor(
      (color >> 8) & 0xf8,
      (color >> 3) & 0xfc,
      (color << 3) & 0xf8);
  }

  void ClearBackground() override {
    gTFT.setColor(0);
    gTFT.fillRect(0, 0, BLOCKSIZE * ROW_COUNT, BLOCKSIZE * COLUMN_COUNT);
  }

  void DrawBlock(const Nanochord::Block* pBlock) override {
    byte* bmp = pBlock->GetCurrentBitmap();
    for (byte i = 0; i < 4; i++) {
      byte yy = pBlock->Y + 1 - i;

      if ((bmp[i] & 0x8) != 0)
        DrawPixel(pBlock->X - 2, yy, pBlock->Color);
      if ((bmp[i] & 0x4) != 0)
        DrawPixel(pBlock->X - 1, yy, pBlock->Color);
      if ((bmp[i] & 0x2) != 0)
        DrawPixel(pBlock->X, yy, pBlock->Color);
      if ((bmp[i] & 0x1) != 0)
        DrawPixel(pBlock->X + 1, yy, pBlock->Color);
    }
  }

  void DrawPixel(byte x, byte y, byte color, int xoffs = 0) {
    if (x >= 0 && y >= 0 && x < 10 && y < 20) {
      SetColor(GetBlockColor(color));
      int xdelta = xoffs + x * BLOCKSIZE;
      int ydelta = y * BLOCKSIZE;
      gTFT.fillRect(xdelta, DHEIGHT - ydelta, BLOCKSIZE + xdelta - 2, DHEIGHT - (BLOCKSIZE + ydelta - 0));
    }
  }

  uint16_t GetBlockColor(byte idx) {
    switch (idx) {
      case 0:
        return 0;
      default:
      case 1:
        return COLOR565(255, 0, 0);
      case 2:
        return COLOR565(0, 255, 0);
      case 3:
        return COLOR565(0, 0, 255);
      case 4:
        return COLOR565(255, 255, 0);
      case 5:
        return COLOR565(0, 255, 255);
      case 6:
        return COLOR565(255, 0, 255);
      case 7:
        return COLOR565(255, 128, 255);
    }
  }

  void DrawNextBlock(const Nanochord::Block* pBlock) override {
    SetColor(0);
    gTFT.fillRect(BLOCKSIZE * COLUMN_COUNT + 5, DHEIGHT - BLOCKSIZE * 4, DWIDTH - 1, DHEIGHT - 1);

    byte* bmp = pBlock->GetCurrentBitmap();
    for (byte i = 0; i < 4; i++) {
      DrawPixel(pBlock->X - 2, i, (bmp[i] & 0x8) != 0 ? pBlock->Color : 0, 300);
      DrawPixel(pBlock->X - 1, i, (bmp[i] & 0x4) != 0 ? pBlock->Color : 0, 300);
      DrawPixel(pBlock->X, i, (bmp[i] & 0x2) != 0 ? pBlock->Color : 0, 300);
      DrawPixel(pBlock->X + 1, i, (bmp[i] & 0x1) != 0 ? pBlock->Color : 0, 300);
    }
  }

  void ClearBlock(const Nanochord::Block* pBlock) override {
    byte* bmp = pBlock->GetCurrentBitmap();
    for (byte i = 0; i < 4; i++) {
      byte yy = pBlock->Y + 1 - i;

      if ((bmp[i] & 0x8) != 0)
        DrawPixel(pBlock->X - 2, yy, 0);
      if ((bmp[i] & 0x4) != 0)
        DrawPixel(pBlock->X - 1, yy, 0);
      if ((bmp[i] & 0x2) != 0)
        DrawPixel(pBlock->X, yy, 0);
      if ((bmp[i] & 0x1) != 0)
        DrawPixel(pBlock->X + 1, yy, 0);
    }
  }

  void PaintPlayground(const Nanochord::Playfield* pPlayfield) override {
    for (byte y = 0; y < ROW_COUNT; y++)
      for (byte x = 0; x < COLUMN_COUNT; x++) {
        DrawPixel(x, y, pPlayfield->Map[y][x]);
      }
  }

  void TetrisEvent(Nanochord::TetrisEventKind kind) override {
    if (kind == Nanochord::TetrisEventKind::GameOver)
      DisplayGameOver();
  }

  int Random(int max) override {
    return random(max);
  }

  void Print(const char* text) override {
    Serial.print(text);
  }
};

TetrisHost host;
Nanochord::Tetris tetris(&host, ROW_COUNT, COLUMN_COUNT);
long int lastTickCount = 0;
long int tetrisLevel = 1;

void DisplayValues(int level, int points)
{
  gTFT.setTextColor(COLOR565(255, 255, 255));

  gTFT.print("(c) 2024 Nanochord Tetris game library", 170, 10);

  gTFT.print("Actual level: ", 280, 50, 0);
  gTFT.print("Actual points: ", 280, 70, 0);

  gTFT.printNumI(level, 400, 50);
  gTFT.printNumI(points, 400, 70);

  gTFT.setColor(COLOR565(128, 128, 128));
  gTFT.drawLine(BLOCKSIZE * COLUMN_COUNT + 5, 0, BLOCKSIZE * COLUMN_COUNT + 5, DHEIGHT - 1);
}

void DisplayPaused()
{
  gTFT.setTextColor(COLOR565(255, 255, 0));
  if (tetris.GetIsPaused())
  {
    gTFT.print("** GAME PAUSED **", 280, 120);
  }
  else
  {
    gTFT.print("                 ", 280, 120);
  }
}

void DisplayGameOver()
{
  gTFT.setTextColor(COLOR565(0xff, 0, 0xaa));
  if (tetris.GetGameOver())
  {
     gTFT.print("*************", 280, 120);
     gTFT.print("*           *", 280, 130);
     gTFT.print("* GAME OVER *", 280, 140);
     gTFT.print("*           *", 280, 150);
     gTFT.print("*************", 280, 160);
  }
  else
  {
    gTFT.setColor(0);
    gTFT.fillRect(280, 120, DWIDTH - 1, 170);
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Nanochord simple Tetris demo game for Arduino has started");

  randomSeed(analogRead(0));

  gTFT.InitLCD();
  gTFT.setFont(SmallFont);

  gTFT.fillScreen(0);

  tetrisLevel = tetris.Start();
  DisplayValues(tetris.GetActualLevel(), tetris.GetActualPoints());

  lastTickCount = millis();
}

void loop() {
  if (Serial.available()) {
    char ch = Serial.read();

    if (tetris.GetGameOver()) {
      if (ch == 's')
      {
        tetris.Start();
        DisplayGameOver();
        DisplayValues(tetris.GetActualLevel(), tetris.GetActualPoints());
      }
    } else {
      switch (ch) {
        case 'q':
          tetris.MoveLeft();
          break;
        case 'w':
          tetris.MoveRight();
          break;
        case 'e':
          tetris.Rotate();
          break;
        case ' ':
          tetris.Drop();
          break;
        case 'p':
          tetris.Pause();
          DisplayPaused();
          break;
      }
    }
  }

  long int m = millis();
  int periodTimeMs = (11 - tetrisLevel) * 50;

  if ((m - lastTickCount) > periodTimeMs) {
    tetrisLevel = tetris.Run();
    lastTickCount = m;
    DisplayValues(tetris.GetActualLevel(), tetris.GetActualPoints());
  }
}
