
#pragma warning(disable: 28159)

#include <iostream>
#include <conio.h>
#include "ConsoleHost.h"

const int ROWS = 20;
const int COLS = 20;

ConsoleHost Host(ROWS, COLS);
Nanochord::Tetris tetris(&Host, ROWS, COLS);

int main()
{
    cout << "Copyright (c) 2024 Nanochord Tetris game library demo\n";
    cout << "Use the arrow keys to navigate blocks: Left, Right, Up = Rotate, Down = Drop, P = Pause, Q = Exit\n\n";

    Host.DrawPlayground();

    int tetrisLevel = tetris.Start();

    DWORD lastTickCount = ::GetTickCount();

    for (;;)
    {
        if (tetris.GetGameOver())
        {
            Host.DisplayGameOver();

            cin.get();
            tetris.Start();
        }
        else if (_kbhit())
        {
            char fkey  = _getch();
            
            if (fkey == -32) // arrow key was pressed
            {
                char skey = _getch();
                switch (skey)
                {
                case 80: // Down arrow
                    tetris.Drop();
                    break;
                case 72: // Up arrow
                    tetris.Rotate();
                    break;
                case 75: // Left arrow
                    tetris.MoveLeft();
                    break;
                case 77: // Right arrow
                    tetris.MoveRight();
                    break;
                }
            }
            else
            {
                switch (fkey)
                {
                case 'p':
                case 'P':
                    tetris.Pause();
                    Host.DisplayPaused(tetris.GetIsPaused());
                    break;
                case 'q':
                case 'Q':
                    ConsoleHost::SetColor(15, 0);
                    ConsoleHost::SetCursorPosition(0, ROWS + 4);
                    return 0;
                }
            }
        }

        DWORD m = ::GetTickCount();
        DWORD delay = (11 - tetrisLevel) * 50;
        if ((m - lastTickCount) > delay)
        {
            tetrisLevel = tetris.Run();
            lastTickCount = m;

            ConsoleHost::DisplayValues(tetris.GetActualLevel(), tetris.GetActualPoints());
        }

    }

    return 0;
}
