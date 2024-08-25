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

using Nanochord;

namespace Nanochord
{
    /// <summary>
    /// Base class for all Blocks
    /// </summary>
    public abstract class Block
    {
        public int X { get; set; }
        public int Y { get; set; }
        public byte OriIndex { get; set; }
        internal abstract byte[][] OriBitmaps { get; }
        public byte OriCount { get { return (byte)OriBitmaps.Length; } }
        public abstract byte Color { get; set; }
        public bool IsI { get; set; }

        public byte[] GetCurrentBitmap()
        {
            return OriBitmaps[OriIndex];
        }
    }

    /// <summary>
    /// 4x4 block
    /// </summary>
    public class Block_O : Block
    {
        internal override byte[][] OriBitmaps => [[0, 6, 6, 0]];
        public override byte Color { get; set; } = 1;
    }

    /// <summary>
    /// 4x1 I-block
    /// </summary>
    public class Block_I : Block
    {
        public Block_I()
        {
            IsI = true;
        }

        internal override byte[][] OriBitmaps => [[0, 15, 0, 0], [2, 2, 2, 2]];
        public override byte Color { get; set; } = 2;
    }

    /// <summary>
    /// S-block
    /// </summary>
    public class Block_S : Block
    {
        internal override byte[][] OriBitmaps => [[0, 3, 6, 0], [2, 3, 1, 0]];
        public override byte Color { get; set; } = 3;
    }

    /// <summary>
    /// Z-block
    /// </summary>
    public class Block_Z : Block
    {
        internal override byte[][] OriBitmaps => [[0, 6, 3, 0], [1, 3, 2, 0]];
        public override byte Color { get; set; } = 4;
    }

    /// <summary>
    /// L-block
    /// </summary>
    public class Block_L : Block
    {
        internal override byte[][] OriBitmaps => [[0, 7, 4, 0], [2, 2, 3, 0], [1, 7, 0, 0], [6, 2, 2, 0]];
        public override byte Color { get; set; } = 5;
    }

    /// <summary>
    /// J-block
    /// </summary>
    public class Block_J : Block
    {
        internal override byte[][] OriBitmaps => [[0, 7, 1, 0], [3, 2, 2, 0], [4, 7, 0, 0], [2, 2, 6, 0]];
        public override byte Color { get; set; } = 6;
    }

    /// <summary>
    /// T-block
    /// </summary>
    public class Block_T : Block
    {
        internal override byte[][] OriBitmaps => [[0, 7, 2, 0], [2, 3, 2, 0], [2, 7, 0, 0], [2, 6, 2, 0]];
        public override byte Color { get; set; } = 7;
    }

    /// <summary>
    /// Placement result
    /// </summary>
    public enum PlacementTestResult
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
    public enum TetrisEventKind
    {
        GameOver,
        Touchdown,
        RowCompleted,
        LevelChanged
    };

    /// <summary>
    /// Interface to the embedder environment
    /// </summary>
    public interface IHost
    {
        // Clears the entire background
        void ClearPlayfield();

        // Displays the specified block to the output
        void DrawBlock(Block Block);

        // Displays the next block to the background
        void DrawNextBlock(Block Block);

        // Clears the specified block from the output
        void ClearBlock(Block Block);

        // Displays the current content of the playfield
        void PaintPlayground(Playfield Playfield);

        // Prints to the console output the specified text (for debug purposes)
        void Print(string text);

        // Generates a random number between 0 and max.
        int Random(int max);

        // Called when a Tetris game event occurs.
        void TetrisEvent(TetrisEventKind kind);
    }


    /// <summary>
    /// This class represents the playfield and its operations
    /// </summary>
    public class Playfield
    {
        public Playfield(IHost host, int rows, int cols)
        {
            this.host = host;
            this.Rows = rows;
            Columns = cols;

            // Minimum 10x10!
            if (this.Rows < 10)
                this.Rows = 10;
            if (Columns < 10)
                Columns = 10;

            map = new byte[this.Rows][];
            for (int i = 0; i < this.Rows; i++)
                map[i] = new byte[Columns];

            Clear();
        }

        protected readonly IHost host;
        public int Rows { get; protected set; }
        public int Columns { get; protected set; }
        protected readonly byte[][] map;
        public readonly int[] CompletedLines = [0, 0, 0, 0];

        public byte this[int row, int col]
        {
            get
            {
                return map[row][col];
            }
        }

        /// <summary>
        /// Tests whether the specified 4x4 bitmap can be placed at the specified position without overlapping other blocks or
        /// hanging out of the playfield
        /// </summary>
        /// <param name="bitmap"></param>
        /// <param name="x"></param>
        /// <param name="y"></param>
        /// <returns></returns>
        public virtual PlacementTestResult PlacementTest(byte[] bitmap, int x, int y)
        {
            if (bitmap == null)
                return PlacementTestResult.Error;

            int yy = 0;
            PlacementTestResult res = PlacementTestResult.Succeeded;

            for (int i = 0; i < 4; i++)
            {
                yy = y + 1 - i;

                if ((bitmap[i] & 0x8) != 0)
                {
                    res = IsPositionEmpty(x - 2, yy);
                    if (res != PlacementTestResult.Succeeded)
                        return res;
                }
                if ((bitmap[i] & 0x4) != 0)
                {
                    res = IsPositionEmpty(x - 1, yy);
                    if (res != PlacementTestResult.Succeeded)
                        return res;
                }
                if ((bitmap[i] & 0x2) != 0)
                {
                    res = IsPositionEmpty(x, yy);
                    if (res != PlacementTestResult.Succeeded)
                        return res;
                }
                if ((bitmap[i] & 0x1) != 0)
                {
                    res = IsPositionEmpty(x + 1, yy);
                    if (res != PlacementTestResult.Succeeded)
                        return res;
                }
            }

            return PlacementTestResult.Succeeded;
        }

        /// <summary>
        /// Ocupies the area represented by the specified block in the playfield
        /// </summary>
        /// <param name="block"></param>
        public virtual void Occupy(Block block)
        {
            var currBmp = block.GetCurrentBitmap();

            for (byte i = 0; i < 4; i++)
            {
                byte yy = (byte)(block.Y + 1 - i);

                if (yy < Rows)
                {
                    if ((currBmp[i] & 0x8) != 0)
                        map[yy][block.X - 2] = block.Color;

                    if ((currBmp[i] & 0x4) != 0)
                        map[yy][block.X - 1] = block.Color;

                    if ((currBmp[i] & 0x2) != 0)
                        map[yy][block.X] = block.Color;

                    if ((currBmp[i] & 0x1) != 0)
                        map[yy][block.X + 1] = block.Color;
                }
            }
        }

        /// <summary>
        /// Returns the number of completed rows in the playfield
        /// </summary>
        /// <returns></returns>
        public virtual int GetCompletedRows()
        {
            int cnt = 0;

            for (int y = Rows - 1; y >= 0; y--)
            {
                bool isRowFull = true;

                for (int x = 0; x < Columns - 1; x++)
                {

                    if (map[y][x] == 0)
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
        public virtual void Clear()
        {
            for (byte i = 0; i < Rows; i++)
                for (byte j = 0; j < Columns; j++)
                    map[i][j] = 0;
        }

        // Empties the specified row in the playfield
        public virtual void ClearRow(byte y)
        {
            for (byte i = (byte)(y + 1); i < Rows; i++)
                for (byte x = 0; x < Columns; x++)
                    map[i - 1][x] = map[i][x];

            for (byte x = 0; x < Columns; x++)
                map[Rows - 1][x] = 0;
        }

        // Dumps the content of the playfield
        public virtual void Dump()
        {
            if (host != null)
            {
                for (byte i = 0; i < Rows; i++)
                {
                    for (byte j = 0; j < Columns; j++)
                    {
                        host.Print(map[i][j] == 0 ? "0" : "1");
                    }
                    host.Print("\r\n");
                }
            }
        }

        // Checks the specified position in the playfield whether ot is empty or not.
        public virtual PlacementTestResult IsPositionEmpty(int xx, int yy)
        {
            if (xx < 0)
                return PlacementTestResult.StickoutLeft;
            if (xx > Columns - 1)
                return PlacementTestResult.StickoutRight;
            if (yy < 0)
                return PlacementTestResult.Failed;
            if (yy > Rows - 1)
                return PlacementTestResult.Succeeded;

            return map[yy][xx] != 0 ? PlacementTestResult.Failed : PlacementTestResult.Succeeded;
        }
    }



    /// <summary>
    /// This class implements the simple Tetris game logic.
    /// </summary>
    public class Tetris
    {
        public Tetris(IHost host, int rows, int cols)
        {
            mHost = host;
            mPlayfield = new Playfield(host, rows, cols);
        }

        protected readonly Playfield mPlayfield;
        protected Block mCurrentBlock = null;
        protected Block mNextBlock = null;
        protected readonly IHost mHost;

        public byte ActualLevel { get; protected set; } = 1;
        public int ActualPoints { get; protected set; } = 0;
        public int LinesCompleted { get; protected set; } = 0;
        public bool IsPaused { get; protected set; } = false;
        public bool GameOver { get; protected set; } = false;

        /// <summary>
        /// Starts a new game
        /// </summary>
        /// <returns></returns>
        public virtual int Start()
        {
            if (mHost == null)
                return -1;

            mHost.ClearPlayfield();

            mPlayfield.Clear();

            ActualLevel = 1;
            ActualPoints = 0;
            LinesCompleted = 0;
            IsPaused = false;
            GameOver = false;

            mCurrentBlock = CreateNewRandomBlock();
            mNextBlock = CreateNewRandomBlock();

            mHost.DrawBlock(mCurrentBlock);
            mHost.DrawNextBlock(mNextBlock);

            return ActualLevel;
        }

        /// <summary>
        /// Pauses the current game
        /// </summary>
        public virtual void Pause()
        {
            IsPaused = !IsPaused;
        }

        /// <summary>
        /// Runs the game
        /// </summary>
        /// <returns></returns>
        public virtual int Run()
        {
            int interval = Run(out _, false);
            return interval;
        }

        public virtual void MoveLeft()
        {
            if (mCurrentBlock != null && !IsPaused && !GameOver)
            {
                PlacementTestResult res = mPlayfield.PlacementTest(mCurrentBlock.GetCurrentBitmap(), mCurrentBlock.X - 1, mCurrentBlock.Y);
                if (res == PlacementTestResult.Succeeded)
                {
                    mHost.ClearBlock(mCurrentBlock);
                    mCurrentBlock.X--;
                    mHost.DrawBlock(mCurrentBlock);
                }
            }
        }

        public virtual void MoveRight()
        {
            if (mCurrentBlock != null && !IsPaused && !GameOver)
            {
                PlacementTestResult res = mPlayfield.PlacementTest(mCurrentBlock.GetCurrentBitmap(), mCurrentBlock.X + 1, mCurrentBlock.Y);
                if (res == PlacementTestResult.Succeeded)
                {
                    mHost.ClearBlock(mCurrentBlock);
                    mCurrentBlock.X++;
                    mHost.DrawBlock(mCurrentBlock);
                }
            }
        }

        public virtual void Rotate()
        {
            if (mCurrentBlock != null && !IsPaused && !GameOver && mCurrentBlock.OriCount > 1)
            {
                byte idx = (byte)(mCurrentBlock.OriIndex == mCurrentBlock.OriCount - 1 ? 0 : mCurrentBlock.OriIndex + 1);

                PlacementTestResult res = mPlayfield.PlacementTest(mCurrentBlock.OriBitmaps[idx], mCurrentBlock.X, mCurrentBlock.Y);

                if (res == PlacementTestResult.Failed)
                    return;

                int offs = (res == PlacementTestResult.Succeeded ? 0 : -100); //?

                if (res == PlacementTestResult.StickoutLeft)
                {
                    res = mPlayfield.PlacementTest(mCurrentBlock.OriBitmaps[idx], mCurrentBlock.X + 1, mCurrentBlock.Y);

                    if (res == PlacementTestResult.Succeeded)
                    {
                        offs = 1;
                    }
                    else
                    {
                        if (mCurrentBlock.IsI)
                        {
                            res = mPlayfield.PlacementTest(mCurrentBlock.OriBitmaps[idx], mCurrentBlock.X + 2, mCurrentBlock.Y);
                            if (res == PlacementTestResult.Succeeded)
                            {
                                offs = 2;
                            }
                        }
                    }
                }
                else if (res == PlacementTestResult.StickoutRight)
                {
                    res = mPlayfield.PlacementTest(mCurrentBlock.OriBitmaps[idx], mCurrentBlock.X - 1, mCurrentBlock.Y);
                    if (res == PlacementTestResult.Succeeded)
                    {
                        offs = -1;
                    }
                }

                if (offs != -100) //?
                {
                    mHost.ClearBlock(mCurrentBlock);
                    mCurrentBlock.X += offs;
                    mCurrentBlock.OriIndex = idx;
                    mHost.DrawBlock(mCurrentBlock);
                }
            }
        }

        public virtual int Drop()
        {
            int interval = 0;

            if (mCurrentBlock != null && !IsPaused && !GameOver)
            {
                PlacementTestResult res = PlacementTestResult.Succeeded;
                do
                {
                    interval = Run(out res, true);
                } while (res != PlacementTestResult.Failed);
            }

            return interval;
        }

        protected virtual Block CreateNewRandomBlock()
        {
            Block pBlock = null;

            switch (mHost.Random(7))
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

            if (pBlock != null)
            {
                pBlock.Y = mPlayfield.Rows - 1;
                //pBlock.X = m_Playfield.columns / 2;
                pBlock.X = mHost.Random(mPlayfield.Columns - 4) + 2;
            }
            return pBlock;
        }

        protected virtual PlacementTestResult DoRun()
        {
            PlacementTestResult ptr = mPlayfield.PlacementTest(mCurrentBlock.GetCurrentBitmap(), mCurrentBlock.X, mCurrentBlock.Y - 1);

            if (ptr != PlacementTestResult.Succeeded)
            {
                // touchdown
                mPlayfield.Occupy(mCurrentBlock);

                mCurrentBlock = mNextBlock;
                mNextBlock = CreateNewRandomBlock();
                mHost.DrawNextBlock(mNextBlock);

                PlacementTestResult ptr2 = mPlayfield.PlacementTest(mCurrentBlock.GetCurrentBitmap(), mCurrentBlock.X, mCurrentBlock.Y);
                if (ptr2 != PlacementTestResult.Succeeded)
                {
                    GameOver = true;
                    mHost.TetrisEvent(TetrisEventKind.GameOver);
                }
            }
            else
            {
                mHost.ClearBlock(mCurrentBlock);
                mCurrentBlock.Y--;
            }

            if (!GameOver)
            {
                mHost.DrawBlock(mCurrentBlock);
            }

            return ptr;
        }

        protected virtual int Run(out PlacementTestResult pres, bool isDropped)
        {
            pres = PlacementTestResult.Failed;

            if (mCurrentBlock == null)
                return -1;

            if (!IsPaused && !GameOver)
            {
                pres = DoRun();

                // touchdown
                if (pres != PlacementTestResult.Succeeded)
                {
                    ActualPoints++; // TODO: It simply increases the number of points.
                    mHost.TetrisEvent(TetrisEventKind.Touchdown);
                }

                // completed rows test
                int cnt = mPlayfield.GetCompletedRows();

                if (cnt > 0)
                {
                    for (byte i = 0; i < cnt; i++)
                    {
                        mPlayfield.ClearRow((byte)mPlayfield.CompletedLines[i]);
                    }

                    LinesCompleted++;

                    mHost.TetrisEvent(TetrisEventKind.RowCompleted);

                    if (LinesCompleted <= 0)
                    {
                        ActualLevel = 1;
                    }
                    else if ((LinesCompleted >= 1) && (LinesCompleted <= 90))
                    {
                        ActualLevel = (byte)(1 + ((LinesCompleted - 1) / 10));
                        mHost.TetrisEvent(TetrisEventKind.LevelChanged);
                    }
                    else if (LinesCompleted >= 91)
                    {
                        ActualLevel = 10;
                        mHost.TetrisEvent(TetrisEventKind.LevelChanged);
                    }

                    mHost.PaintPlayground(mPlayfield);
                    mHost.DrawBlock(mCurrentBlock);
                }

                return ActualLevel;
            }

            return -1;
        }
    }
}