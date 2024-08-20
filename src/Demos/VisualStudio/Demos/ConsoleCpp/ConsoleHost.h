#pragma once

#include "Tetris.h"
#include <cstdlib>
#include <time.h>
#include <iostream> 
#include <Windows.h>

using namespace std;

class ConsoleHost : public Nanochord::Host
{
private:
    int m_rows;
    int m_cols;
    int m_indent;

public:
    ConsoleHost(int rows, int cols)
    {
        m_rows = rows;
        m_cols = cols;

        m_indent = (80 - m_cols - 4) / 2;

        srand(time(0));
    }


    // Host interface

    void ClearBackground() override
    {
        SetColor(0, 0);
        for (int r = 0; r < m_rows; r++)
        {
            SetCursorPosition(m_indent + 1, 4 + r);
            cout << string(m_cols, ' ');
        }

        SetCursorPosition(0, 0);
    }

    void DrawBlock(const Nanochord::Block* pBlock) override
    {
        DrawBlock(pBlock, pBlock->Color, m_indent + 1, 3, m_rows);
    }

    void DrawNextBlock(const Nanochord::Block* pBlock) override
    {
        const byte* bmp = pBlock->GetCurrentBitmap();

        for (int i = 0; i < 4; i++)
        {
            DrawPixel(0, i, (bmp[i] & 0x8) != 0 ? pBlock->Color : 0, 4, 7, 4);
            DrawPixel(1, i, (bmp[i] & 0x4) != 0 ? pBlock->Color : 0, 4, 7, 4);
            DrawPixel(2, i, (bmp[i] & 0x2) != 0 ? pBlock->Color : 0, 4, 7, 4);
            DrawPixel(3, i, (bmp[i] & 0x1) != 0 ? pBlock->Color : 0, 4, 7, 4);               
        }
    }

    void ClearBlock(const Nanochord::Block* pBlock) override
    {
        DrawBlock(pBlock, 0, m_indent + 1, 3, m_rows);
    }

    void PaintPlayground(const Nanochord::Playfield* pPlayfield) override
    {
        for (byte y = 0; y < m_rows; y++)
        {
            for (byte x = 0; x < m_cols; x++)
            {
                DrawPixel(x, y, pPlayfield->Map[y][x], m_indent + 1, 3, m_rows);
            }
        }
    }

    void Print(const char* text) override
    {
        cerr << text;
    }

    int Random(int max) override
    {
        return rand() % max;
    }

    void TetrisEvent(Nanochord::TetrisEventKind kind) override
    {
        // Add custom event handling here...
    }

    // Functions

    static void SetColor(int textColor, int bgColor)
    {
        HANDLE hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
        ::SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
    }

    static void SetCursorPosition(char x, char y)
    {
        HANDLE hOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
        std::cout.flush();
        COORD coord = { (SHORT)x, (SHORT)y };
        ::SetConsoleCursorPosition(hOut, coord);
    }

    void DrawPlayground() const
    {
        SetColor(15, 0);

        cout << string(m_indent, ' ') << '+' << string(m_cols, '-') << '+' << endl;

        for (int r = 0; r < m_rows; r++)
        {
            cout << string(m_indent, ' ') << '|' << string(m_cols, ' ') << '|' << endl;
        }

        cout << string(m_indent, ' ') << '+' << string(m_cols, '-') << '+' << endl;

        SetCursorPosition(2, 3);
        cout << "Level:";

        SetCursorPosition(2, 4);
        cout << "Points:";

        SetCursorPosition(2, 7);
        cout << "Next block:";

        DisplayValues(1, 0);
    }

    static void DisplayValues(int level, int points)
    {
        SetColor(15, 0);

        SetCursorPosition(10, 3);
        cout << level;

        SetCursorPosition(10, 4);
        cout << points;

        SetCursorPosition(80, 0);
    }

    void DrawBlock(const Nanochord::Block* pBlock, byte color, int xorigin, int yorigin, int rows)
    {
        const byte* bmp = pBlock->GetCurrentBitmap();

        for (int i = 0; i < 4; i++)
        {
            byte yy = pBlock->Y + 1 - i;

            if ((bmp[i] & 0x8) != 0)
                DrawPixel(pBlock->X - 2, yy, color, xorigin, yorigin, rows);
            if ((bmp[i] & 0x4) != 0)
                DrawPixel(pBlock->X - 1, yy, color, xorigin, yorigin, rows);
            if ((bmp[i] & 0x2) != 0)
                DrawPixel(pBlock->X, yy, color, xorigin, yorigin, rows);
            if ((bmp[i] & 0x1) != 0)
                DrawPixel(pBlock->X + 1, yy, color, xorigin, yorigin, rows);
        }

        SetCursorPosition(80, 0);
    }

    void DrawPixel(byte x, byte y, byte color, int xorigin, int yorigin, int rows) const
    {
        if (x >= 0 && y >= 0 && x < m_cols && y < m_rows)
        {
            SetColor(color, color);
            SetCursorPosition(xorigin + x, yorigin + (rows - y));
            cout << ' ';
        }
    }

    void DisplayPaused(bool show)
    {
        SetColor(6, 0);
        SetCursorPosition(2, 17);

        if (show)
        {
            cout << "** GAME PAUSED **";
        }
        else
        {
            cout << "                 ";
        }
    }

    void DisplayGameOver()
    {
        SetColor(0, 15);

        int x = m_indent + (m_cols - 15) / 2 + 1;

        SetCursorPosition(x, 3 + m_rows / 2 - 0);
        cout << "***************";
        SetCursorPosition(x, 3 + m_rows / 2 - 1);
        cout << "*             *";
        SetCursorPosition(x, 3 + m_rows / 2 - 2);
        cout << "*  Game Over  *";
        SetCursorPosition(x, 3 + m_rows / 2 - 3);
        cout << "*             *";
        SetCursorPosition(x, 3 + m_rows / 2 - 4);
        cout << "***************";
    }
};
