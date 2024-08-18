// ConsoleCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "ConsoleHost.h"

using namespace std;

const int ROWS = 20;
const int COLS = 20;

ConsoleHost Host(ROWS, COLS);
Nanochord::Tetris tetris(&Host, ROWS, COLS);

int main()
{
    cout << "Copyright (c) 2024 Nanochord Tetris game library demo\n";
    cout << "Use the arrow keys to navigate blocks.\n\n";

    Host.DrawPlayground();

    int tetrisLevel = tetris.Start();
    KEY_EVENT_RECORD key;

    do
    {
        // TODO: Add game control

        tetrisLevel = tetris.Run();

        ::Sleep(ConsoleHost::CalculateDelayInMs(tetrisLevel));
    } while (1);

    cin.get();
}
