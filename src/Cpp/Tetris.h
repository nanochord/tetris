/*
    Nanochord.Tetris
 
    Simple Tetris game library
 
    MIT License

    Copyright (c) 2024 Norbert László

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
 */

#ifndef _Nanochord_Tetris_
#define _Nanochord_Tetris_

#ifndef ARDUINO
typedef unsigned char byte;
#endif

namespace Nanochord
{
    class Playfield;

    /// <summary>
    /// Base class for all Blocks
    /// </summary>
    class Block
    {
    public:
        Block()
        {
            OriCount = 0;
            OriIndex = 0;
            X = 5;
            Y = 20;
            IsI = false;
        }

    public:
        int X;
        int Y;
        byte OriIndex;
        const byte* OriBitmaps[4] = { 0 };
        byte OriCount;
        byte Color;
        bool IsI;

    public:
        const byte* GetCurrentBitmap() const
        {
            return OriBitmaps[OriIndex];
        }
    };

    /// <summary>
    /// 4x4 block
    /// </summary>
    class Block_O : public Block
    {
        const byte bmp1[4] = { 0, 6, 6, 0 };

    public:
        Block_O()
        {
            OriCount = 1;
            OriBitmaps[0] = bmp1;
            Color = 1;
        }
    };

    /// <summary>
    /// 4x1 I-block
    /// </summary>
    class Block_I : public Block
    {
        const byte bmp1[4] = { 0, 15, 0, 0 };
        const byte bmp2[4] = { 2, 2, 2, 2 };

    public:
        Block_I()
        {
            OriCount = 2;
            OriBitmaps[0] = bmp1;
            OriBitmaps[1] = bmp2;
            Color = 2;
            IsI = true;
        }
    };

    /// <summary>
    /// S-block
    /// </summary>
    class Block_S : public Block
    {
        const byte bmp1[4] = { 0, 3, 6, 0 };
        const byte bmp2[4] = { 2, 3, 1, 0 };

    public:
        Block_S()
        {
            OriCount = 2;
            OriBitmaps[0] = bmp1;
            OriBitmaps[1] = bmp2;
            Color = 3;
        }
    };

    /// <summary>
    /// Z-block
    /// </summary>
    class Block_Z : public Block
    {
        const byte bmp1[4] = { 0, 6, 3, 0 };
        const byte bmp2[4] = { 1, 3, 2, 0 };

    public:
        Block_Z()
        {
            OriCount = 2;
            OriBitmaps[0] = bmp1;
            OriBitmaps[1] = bmp2;
            Color = 4;
        }
    };

    /// <summary>
    /// L-block
    /// </summary>
    class Block_L : public Block
    {
        const byte bmp1[4] = { 0, 7, 4, 0 };
        const byte bmp2[4] = { 2, 2, 3, 0 };
        const byte bmp3[4] = { 1, 7, 0, 0 };
        const byte bmp4[4] = { 6, 2, 2, 0 };

    public:
        Block_L()
        {
            OriCount = 4;
            OriBitmaps[0] = bmp1;
            OriBitmaps[1] = bmp2;
            OriBitmaps[2] = bmp3;
            OriBitmaps[3] = bmp4;
            Color = 5;
        }
    };
    
    /// <summary>
    /// J-block
    /// </summary>
    class Block_J : public Block
    {
        const byte bmp1[4] = { 0, 7, 1, 0 };
        const byte bmp2[4] = { 3, 2, 2, 0 };
        const byte bmp3[4] = { 4, 7, 0, 0 };
        const byte bmp4[4] = { 2, 2, 6, 0 };

    public:
        Block_J()
        {
            OriCount = 4;
            OriBitmaps[0] = bmp1;
            OriBitmaps[1] = bmp2;
            OriBitmaps[2] = bmp3;
            OriBitmaps[3] = bmp4;
            Color = 6;
        }
    };

    /// <summary>
    /// T-block
    /// </summary>
    class Block_T : public Block
    {
        const byte bmp1[4] = { 0, 7, 2, 0 };
        const byte bmp2[4] = { 2, 3, 2, 0 };
        const byte bmp3[4] = { 2, 7, 0, 0 };
        const byte bmp4[4] = { 2, 6, 2, 0 };

    public:
        Block_T()
        {
            OriCount = 4;
            OriBitmaps[0] = bmp1;
            OriBitmaps[1] = bmp2;
            OriBitmaps[2] = bmp3;
            OriBitmaps[3] = bmp4;
            Color = 7;
        }
    };


    /// <summary>
    /// Placement result
    /// </summary>
    enum PlacementTestResult
    {
        Error = -1,
        Succeeded = 0,
        Failed = 1,
        StickoutLeft = 2,
        StickoutRight = 3,
    };

    /// <summary>
    /// Different kind of Tetris events
    /// </summary>
    enum TetrisEventKind
    {
        GameOver,
        Touchdown,
        RowCompleted
    };

    /// <summary>
    /// Abstract class, interface to the embedder environment
    /// </summary>
    class Host
    {
    public:
        // Clears the entire background
        virtual void ClearBackground() = 0;

        // Displays the specified block to the output
        virtual void DrawBlock(const Block* Block) = 0;

        // Displays the next block to the background
        virtual void DrawNextBlock(const Block* Block) = 0;

        // Clears the specified block from the output
        virtual void ClearBlock(const Block* Block) = 0;

        // Displays the current content of the playfield
        virtual void PaintPlayground(const Playfield* Playfield) = 0;

        // Prints to the console output the specified text (for debug purposes)
        virtual void Print(const char* text) = 0;

        // Generates a random number between 0 and max.
        virtual int Random(int max) = 0;

        // Called when a Tetris game event occurs.
        virtual void TetrisEvent(TetrisEventKind kind) = 0;
    };


    /// <summary>
    /// This class represents the playfield and its operations
    /// </summary>
    class Playfield
    {
        friend class Tetris;

    public:
        Playfield(Host* pHost, int rows, int cols)
        {
            m_pHost = pHost;
            m_Rows = rows;
            m_Columns = cols;

            // Minimum 10x10!
            if (m_Rows < 10)
                m_Rows = 10;
            if (m_Columns < 10)
                m_Columns = 10;

            Map = new byte*[m_Rows];
            for (int r = 0; r < m_Rows; r++)
                Map[r] = new byte[m_Columns];

            Clear();
        }

        ~Playfield()
        {
            for (int r = 0; r < m_Rows; r++)
                delete[] Map[r];
            delete[] Map;
        }

        byte **Map;
        int CompletedLines[4] = { 0 };

    protected:
        int m_Columns;
        int m_Rows;
        Host* m_pHost;

    public:
        // Tests whether the specified 4x4 bitmap can be placed at the specified position without overlapping other blocks or
        // hanging out of the playfield
        virtual PlacementTestResult PlacementTest(const byte* bitmap, int x, int y)
        {
            if (bitmap == NULL)
                return PlacementTestResult::Error;

            int yy = 0;
            PlacementTestResult res = PlacementTestResult::Succeeded;

            for (int i = 0; i < 4; i++)
            {
                yy = y + 1 - i;

                if ((bitmap[i] & 0x8) != 0)
                {
                    res = IsPositionEmpty(x - 2, yy);
                    if (res != PlacementTestResult::Succeeded)
                        return res;
                }
                if ((bitmap[i] & 0x4) != 0)
                {
                    res = IsPositionEmpty(x - 1, yy);
                    if (res != PlacementTestResult::Succeeded)
                        return res;
                }
                if ((bitmap[i] & 0x2) != 0)
                {
                    res = IsPositionEmpty(x, yy);
                    if (res != PlacementTestResult::Succeeded)
                        return res;
                }
                if ((bitmap[i] & 0x1) != 0)
                {
                    res = IsPositionEmpty(x + 1, yy);
                    if (res != PlacementTestResult::Succeeded)
                        return res;
                }
            }

            return PlacementTestResult::Succeeded;
        }

        // Ocupies the area represented by the specified block in the playfield
        virtual void Occupy(const Block* pBlock)
        {
            const byte* currBmp = pBlock->GetCurrentBitmap();

            for (byte i = 0; i < 4; i++)
            {
                byte yy = pBlock->Y + 1 - i;

                if (yy < m_Rows)
                {
                    if ((currBmp[i] & 0x8) != 0)
                        Map[yy][pBlock->X - 2] = pBlock->Color;

                    if ((currBmp[i] & 0x4) != 0)
                        Map[yy][pBlock->X - 1] = pBlock->Color;

                    if ((currBmp[i] & 0x2) != 0)
                        Map[yy][pBlock->X] = pBlock->Color;

                    if ((currBmp[i] & 0x1) != 0)
                        Map[yy][pBlock->X + 1] = pBlock->Color;
                }
            }
        }

        // Returns the number of completed rows in the playfield
        virtual int GetCompletedRows()
        {
            int cnt = 0;

            for (int y = m_Rows - 1; y >= 0; y--)
            {
                bool isRowFull = true;

                for (int x = 0; x < m_Columns - 1; x++)
                {

                    if (Map[y][x] == 0)
                    {
                        isRowFull = false;
                        break;
                    }
                }

                if (isRowFull)
                {
                    CompletedLines[cnt] = y;
                    cnt++;
                }
            }

            return cnt;
        }

        // Empties the playfield
        virtual void Clear()
        {
            for (byte i = 0; i < m_Rows; i++)
                for (byte j = 0; j < m_Columns; j++)
                    Map[i][j] = 0;
        }

        // Empties the specified row in the playfield
        virtual void ClearRow(byte y)
        {
            for (byte i = y + 1; i < m_Rows; i++)
                for (byte x = 0; x < m_Columns; x++)
                    Map[i - 1][x] = Map[i][x];

            for (byte x = 0; x < m_Columns; x++)
                Map[m_Rows - 1][x] = 0;
        }

        // Dumps the content of the playfield
        virtual void Dump()
        {
            if (m_pHost != NULL)
            {
                for (byte i = 0; i < m_Rows; i++)
                {
                    for (byte j = 0; j < m_Columns; j++)
                    {
                        m_pHost->Print(Map[i][j] == 0 ? "0" : "1");
                    }
                    m_pHost->Print("\r\n");
                }
            }
        }

        // Checks the specified position in the playfield whether ot is empty or not.
        virtual PlacementTestResult IsPositionEmpty(int xx, int yy)
        {
            if (xx < 0)
                return PlacementTestResult::StickoutLeft;
            if (xx > m_Columns - 1)
                return PlacementTestResult::StickoutRight;
            if (yy < 0)
                return PlacementTestResult::Failed;
            if (yy > m_Rows - 1)
                return PlacementTestResult::Succeeded;

            return Map[yy][xx] != 0 ? PlacementTestResult::Failed : PlacementTestResult::Succeeded;
        }
    };


    /// <summary>
    /// This class implements the simple Tetris game logic.
    /// </summary>
    class Tetris
    {
    public:
        Tetris(Host* pHost, int rows, int cols) : m_Playfield(pHost, rows, cols)
        {
            m_pHost = pHost;
        }

    protected:
        Playfield m_Playfield;
        Block* m_pCurrentBlock = NULL;
        Block* m_pNextBlock = NULL;
        byte m_ActualLevel = 1;
        int m_ActualPoints = 0;
        int m_LinesCompleted = 0;
        bool m_IsPaused = false;
        bool m_GameOver = false;
        Host* m_pHost;

    public:
        // Starts a new game
        virtual int Start()
        {
            if (m_pHost == NULL)
                return -1;

            m_pHost->ClearBackground();

            m_Playfield.Clear();

            m_ActualLevel = 1;
            m_ActualPoints = 0;
            m_LinesCompleted = 0;
            m_IsPaused = false;
            m_GameOver = false;

            m_pCurrentBlock = CreateNewRandomBlock();
            m_pNextBlock = CreateNewRandomBlock();

            m_pHost->DrawBlock(m_pCurrentBlock);
            m_pHost->DrawNextBlock(m_pNextBlock);

            return m_ActualLevel;
        }

        byte GetActualLevel() const { return m_ActualLevel; }
        int GetActualPoints() const { return m_ActualPoints; }
        int GetLinesCompleted() const { return m_LinesCompleted; }
        bool GetIsPaused() const { return m_IsPaused; }
        bool GetGameOver() const { return m_GameOver; }

        // Pauses the current game
        virtual void Pause()
        {
            m_IsPaused = !m_IsPaused;
        }

        // Runs the game
        virtual int Run()
        {
            PlacementTestResult res;
            int interval = Run(&res, false);
            return interval;
        }

        virtual void MoveLeft()
        {
            if (m_pCurrentBlock != NULL && !m_IsPaused && !m_GameOver)
            {
                PlacementTestResult res = m_Playfield.PlacementTest(m_pCurrentBlock->GetCurrentBitmap(), m_pCurrentBlock->X - 1, m_pCurrentBlock->Y);
                if (res == PlacementTestResult::Succeeded)
                {
                    m_pHost->ClearBlock(m_pCurrentBlock);
                    m_pCurrentBlock->X--;
                    m_pHost->DrawBlock(m_pCurrentBlock);
                }
            }
        }

        virtual void MoveRight()
        {
            if (m_pCurrentBlock != NULL && !m_IsPaused && !m_GameOver)
            {
                PlacementTestResult res = m_Playfield.PlacementTest(m_pCurrentBlock->GetCurrentBitmap(), m_pCurrentBlock->X + 1, m_pCurrentBlock->Y);
                if (res == PlacementTestResult::Succeeded)
                {
                    m_pHost->ClearBlock(m_pCurrentBlock);
                    m_pCurrentBlock->X++;
                    m_pHost->DrawBlock(m_pCurrentBlock);
                }
            }
        }

        virtual void Rotate()
        {
            if (m_pCurrentBlock != NULL && !m_IsPaused && !m_GameOver && m_pCurrentBlock->OriCount > 1)
            {
                byte idx = (m_pCurrentBlock->OriIndex == m_pCurrentBlock->OriCount - 1 ? 0 : m_pCurrentBlock->OriIndex + 1);

                PlacementTestResult res = m_Playfield.PlacementTest(m_pCurrentBlock->OriBitmaps[idx], m_pCurrentBlock->X, m_pCurrentBlock->Y);

                if (res == PlacementTestResult::Failed)
                    return;

                byte offs = (res == PlacementTestResult::Succeeded ? 0 : -100); //?

                if (res == PlacementTestResult::StickoutLeft)
                {
                    res = m_Playfield.PlacementTest(m_pCurrentBlock->OriBitmaps[idx], m_pCurrentBlock->X + 1, m_pCurrentBlock->Y);

                    if (res == PlacementTestResult::Succeeded)
                    {
                        offs = 1;
                    }
                    else
                    {
                        if (m_pCurrentBlock->IsI)
                        {
                            res = m_Playfield.PlacementTest(m_pCurrentBlock->OriBitmaps[idx], m_pCurrentBlock->X + 2, m_pCurrentBlock->Y);
                            if (res == PlacementTestResult::Succeeded)
                            {
                                offs = 2;
                            }
                        }
                    }
                }
                else if (res == PlacementTestResult::StickoutRight)
                {
                    res = m_Playfield.PlacementTest(m_pCurrentBlock->OriBitmaps[idx], m_pCurrentBlock->X - 1, m_pCurrentBlock->Y);
                    if (res == PlacementTestResult::Succeeded)
                    {
                        offs = -1;
                    }
                }

                if (offs != -100) //?
                {
                    m_pHost->ClearBlock(m_pCurrentBlock);
                    m_pCurrentBlock->X += offs;
                    m_pCurrentBlock->OriIndex = idx;
                    m_pHost->DrawBlock(m_pCurrentBlock);
                }
            }
        }

        virtual int Drop()
        {
            int interval = 0;

            if (m_pCurrentBlock != NULL && !m_IsPaused && !m_GameOver)
            {
                PlacementTestResult res = PlacementTestResult::Succeeded;
                do
                {
                    interval = Run(&res, true);
                } while (res != PlacementTestResult::Failed);
            }

            return interval;
        }

    protected:

        virtual Block* CreateNewRandomBlock()
        {
            Block* pBlock = NULL;

            switch (m_pHost->Random(8))
            {
            default:
            case 0:
                pBlock = new Block_O();
                break;
            case 1:
                pBlock = new Block_I();
                break;
            case 2:
                pBlock = new Block_S();
                break;
            case 3:
                pBlock = new Block_Z();
                break;
            case 4:
                pBlock = new Block_J();
                break;
            case 5:
                pBlock = new Block_L();
                break;
            case 6:
                pBlock = new Block_T();
                break;
            }

            if (pBlock)
            {
                pBlock->Y = m_Playfield.m_Rows - 1;
                pBlock->X = m_Playfield.m_Columns / 2;
                //pBlock->X = m_pHost->Random(m_Playfield.m_Columns - 4) + 2;
            }

            return pBlock;
        }

        virtual PlacementTestResult DoRun()
        {
            PlacementTestResult ptr = m_Playfield.PlacementTest(m_pCurrentBlock->GetCurrentBitmap(), m_pCurrentBlock->X, m_pCurrentBlock->Y - 1);

            if (ptr != PlacementTestResult::Succeeded)
            {
                // touchdown
                m_Playfield.Occupy(m_pCurrentBlock);

                delete m_pCurrentBlock;
                m_pCurrentBlock = m_pNextBlock;
                m_pNextBlock = CreateNewRandomBlock();
                m_pHost->DrawNextBlock(m_pNextBlock);

                PlacementTestResult ptr2 = m_Playfield.PlacementTest(m_pCurrentBlock->GetCurrentBitmap(), m_pCurrentBlock->X, m_pCurrentBlock->Y);
                if (ptr2 != PlacementTestResult::Succeeded)
                {
                    m_GameOver = true;
                    m_pHost->TetrisEvent(TetrisEventKind::GameOver);
                }
            }
            else
            {
                m_pHost->ClearBlock(m_pCurrentBlock);
                m_pCurrentBlock->Y--;
            }

            if (!m_GameOver)
            {
                m_pHost->DrawBlock(m_pCurrentBlock);
            }

            return ptr;
        }

        virtual int Run(PlacementTestResult* pres, bool isDropped)
        {
            if (m_pCurrentBlock == NULL)
                return -1;

            if (!m_IsPaused && !m_GameOver)
            {
                *pres = DoRun();

                // touchdown
                if (*pres != PlacementTestResult::Succeeded)
                {
                    m_ActualPoints++; // TODO: It simply increases the number of points.
                    m_pHost->TetrisEvent(TetrisEventKind::Touchdown);
                }

                // completed rows test
                byte cnt = m_Playfield.GetCompletedRows();

                if (cnt > 0)
                {
                    for (byte i = 0; i < cnt; i++)
                    {
                        m_Playfield.ClearRow(m_Playfield.CompletedLines[i]);
                    }

                    m_LinesCompleted++;

                    if (m_LinesCompleted <= 0)
                    {
                        m_ActualLevel = 1;
                    }
                    else if ((m_LinesCompleted >= 1) && (m_LinesCompleted <= 90))
                    {
                        m_ActualLevel = 1 + ((m_LinesCompleted - 1) / 10);
                    }
                    else if (m_LinesCompleted >= 91)
                    {
                        m_ActualLevel = 10;
                    }

                    m_pHost->TetrisEvent(TetrisEventKind::RowCompleted);
                    m_pHost->PaintPlayground(&m_Playfield);
                    m_pHost->DrawBlock(m_pCurrentBlock);
                }

                return m_ActualLevel;
            }

            return -1;
        }
    };
}

#endif