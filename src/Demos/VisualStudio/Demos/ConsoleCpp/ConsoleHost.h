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
    int m_indend;

public:
    ConsoleHost(int rows, int cols)
    {
        m_rows = rows;
        m_cols = cols;

        m_indend = (80 - m_cols - 4) / 2;

        std::srand(time(0));
    }


    // Host interface

    void ClearBackground() override
    {
        for (int r = 0; r < m_rows; r++)
        {
            ConsoleHost::SetCursorPosition(m_indend + 1, 4 + r);
            cout << string(m_cols, ' ');
        }

        ConsoleHost::SetCursorPosition(0, 0);
    }

    void DrawBlock(const Nanochord::Block* pBlock) override
    {
        DrawBlock(pBlock, pBlock->Color, m_indend + 1, 3);
    }

    void DrawNextBlock(const Nanochord::Block* pBlock) override
    {
        for (int i = 0; i < 4; i++)
        {
            ConsoleHost::SetCursorPosition(3, 8 + i);
            SetColor(0, 0);
            cout << ' ';
        }
        DrawBlock(pBlock, 3, 2, 6);
    }

    void ClearBlock(const Nanochord::Block* pBlock) override
    {
        DrawBlock(pBlock, 0, m_indend + 1, 3);
    }

    void PaintPlayground(const Nanochord::Playfield* pPlayfield) override
    {
        for (byte y = 0; y < m_rows; y++)
        {
            for (byte x = 0; x < m_cols; x++)
            {
                DrawPixel(x, y, pPlayfield->Map[y][x], m_indend + 1, 3);
            }
        }
    }

    void Print(const char* text) override
    {
        cout << text;
    }

    int Random(int max) override
    {
        return rand() % max;
    }

    // Functions

    static void SetCursorPosition(char x, char y)
    {
        static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        std::cout.flush();
        COORD coord = { (SHORT)x, (SHORT)y };
        SetConsoleCursorPosition(hOut, coord);
    }

    static int CalculateDelayInMs(int tetrisLevel)
    {
        return (11 - tetrisLevel) * 50;
    }

    static bool GetKeyEventRecord(KEY_EVENT_RECORD& krec)
    {
        DWORD cc;
        INPUT_RECORD irec[128];
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

        if (hOut != NULL)
        {
            BOOL f = ReadConsoleInput(hOut, irec, 128, &cc);

            if (f) // TODO
                //&& irec.EventType == KEY_EVENT && ((KEY_EVENT_RECORD&)irec.Event).bKeyDown)
            {
                krec = (KEY_EVENT_RECORD&)irec[0].Event;
                return true;
            }
        }

        return false;
    }

    void DrawPlayground() const
    {
        cout << string(m_indend, ' ') << '+' << string(m_cols, '-') << '+' << endl;

        for (int r = 0; r < m_rows; r++)
        {
            cout << string(m_indend, ' ') << '|' << string(m_cols, ' ') << '|' << endl;
        }

        cout << string(m_indend, ' ') << '+' << string(m_cols, '-') << '+' << endl;

        ConsoleHost::SetCursorPosition(2, 3);
        cout << "Level:  " << (int)1;

        ConsoleHost::SetCursorPosition(2, 4);
        cout << "Points: " << 0;

        SetCursorPosition(0, 7);
        cout << "  +----+\n";
        cout << "  |    |\n";
        cout << "  |    |\n";
        cout << "  +----+";
    }

    static void SetColor(int textColor, int bgColor)
    {
        HANDLE hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
    }

    void DrawBlock(const Nanochord::Block* pBlock, byte color, int xorigin, int yorigin)
    {
        const byte* bmp = pBlock->GetCurrentBitmap();

        for (int i = 0; i < 4; i++)
        {
            byte yy = pBlock->Y + 1 - i;

            if ((bmp[i] & 0x8) != 0)
                DrawPixel(pBlock->X - 2, yy, color, xorigin, yorigin);
            if ((bmp[i] & 0x4) != 0)
                DrawPixel(pBlock->X - 1, yy, color, xorigin, yorigin);
            if ((bmp[i] & 0x2) != 0)
                DrawPixel(pBlock->X, yy, color, xorigin, yorigin);
            if ((bmp[i] & 0x1) != 0)
                DrawPixel(pBlock->X + 1, yy, color, xorigin, yorigin);
        }

        ConsoleHost::SetCursorPosition(80, 0);
    }

    void DrawPixel(byte x, byte y, byte color, int xorigin, int yorigin)
    {
        if (x >= 0 && y >= 0 && x < m_cols && y < m_rows)
        {
            SetColor(color, color);
            ConsoleHost::SetCursorPosition(xorigin + x, yorigin + (m_rows - y));
            cout << ' ';
        }
    }
};
