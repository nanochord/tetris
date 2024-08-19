#include <iostream>
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
    KEY_EVENT_RECORD key;

    DWORD lastTickCount = ::GetTickCount();

    for (;;)
    {
        if (tetris.GetGameOver())
        {
            Host.DisplayGameOver();

            cin.get();
            tetris.Start();
        }
        else
        {
            // TODO: Unfortunately, this solution blocks the console application until the user presses a key.
            //       I haven't found so far a better solution...

            if (ConsoleHost::GetKeyEventRecord(key))
            {
                switch (key.wVirtualScanCode)
                {
                case 80: // Down
                    tetris.Drop();
                    break;
                case 72: // Up
                    tetris.Rotate();
                    break;
                case 75: // Left
                    tetris.MoveLeft();
                    break;
                case 77: // Right
                    tetris.MoveRight();
                    break;
                case 25:
                    tetris.Pause();
                    Host.DisplayPaused(tetris.GetIsPaused());
                    break;
                case 16: // Q
                    ConsoleHost::SetColor(15, 0);
                    ConsoleHost::SetCursorPosition(0, ROWS + 4);
                    return 0;
                }
            }
        }

        DWORD m = ::GetTickCount();
        DWORD delay = ConsoleHost::CalculateDelayInMs(tetrisLevel);
        if ((m - lastTickCount) > delay)
        {
            tetrisLevel = tetris.Run();
            lastTickCount = m;

            ConsoleHost::DisplayValues(tetris.GetActualLevel(), tetris.GetActualPoints());
        }

    }

    return 0;
}
