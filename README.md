# Tetris

Simple Tetris game library.

This library was originally made for an Arduino based GameBox hardware. However, the solution was platform independent, therefore, it can be applied in other solutions, as well.
There can be found demo applications in the repository so as to show how can it be used in different environments.

## How to use?

In C++ simple include the Tetris.h header file. The tetris enums and classes are within the **Nanochord** namespace. The **Nanochord:Host** abastract class must be implemented by the host code.

```cpp
#include "Tetris.h"

const int ROW_COUNT = 20;
const int COL_COUNT = 20;

class MyHost: public Nanochord::Host
{
    // your custom platform imeplementation goes here...
    ...
}

MyHost host;
Nanochord:Tetris tetris(&host, ROW_COUNT, COL_COUNT);

tetris.Start();

while (!tetris.GetGameOver())
{
  // Game control logic goes here...
  // tetris.MoveLeft();
  // tetris.MoveRight();
  // tetris.Rotate();
  // tetris.Drop();
  // tetris.Pause();

  if (is_time_to_run)
    tetris.Run();
}
```

## Demos

![image](./images/consoledemo.png "Console demo")

Coming soon:
* Arduino demo
* C# version with WPF demo
