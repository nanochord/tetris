﻿using Nanochord;
using System.Collections.ObjectModel;
using System.Windows;
using System.Windows.Input;
using System.Windows.Threading;

namespace WpfTetrisDemo.ViewModels;

public class MainWindowViewModel : BaseViewModel, IHost
{
    #region ctor

    public MainWindowViewModel()
    {
        tetris = new(this, Rows, Columns);

        _Map = new ObservableCollection<BlockViewModel>(
            Enumerable.Range(0, Rows * Columns).Select(i => new BlockViewModel())
            );

        _NextBlock = new ObservableCollection<BlockViewModel>(
            Enumerable.Range(0, 16).Select(i => new BlockViewModel())
        );

        tetrisLevel = tetris.Start();
        lastTickCount = Environment.TickCount;

        timer.Tick += Timer_Tick;
        timer.Start();
        DisplayValues();
    }

    #endregion

    #region Fields

    readonly Tetris tetris;
    readonly Random rand = new(Environment.TickCount);
    readonly DispatcherTimer timer = new()
    {
        Interval = TimeSpan.FromMilliseconds(10)
    };

    int tetrisLevel;
    int lastTickCount;

    public const int Rows = 20;
    public const int Columns = 10;

    #endregion

    #region IHost

    public void ClearPlayfield()
    {
        foreach (var block in _Map)
            block.SetColor(0);
        foreach (var block in _NextBlock)
            block.SetColor(0);
    }

    public void ClearBlock(Block block)
    {
        DrawBlock(block, 0, false);
    }

    public void DrawBlock(Block block)
    {
        DrawBlock(block, block.Color, false);
    }

    public void DrawNextBlock(Block block)
    {
        DrawBlock(block, block.Color, true);
    }

    public void PaintPlayground(Playfield playfield)
    {
        for (int y = 0; y < Rows; y++)
        {
            for (int x = 0; x < Columns; x++)
            {
                DrawPixel(x, y, playfield[y, x], false);
            }
        }
    }

    public void Print(string text) => Console.Write(text);

    public int Random(int max) => rand.Next(0, max);

    public void TetrisEvent(TetrisEventKind kind)
    {
        DisplayValues();

        if (kind == TetrisEventKind.GameOver)
        {
            Message = "Game over";
            timer.Stop();
        }
    }

    #endregion

    #region Event handlers

    private void Timer_Tick(object sender, EventArgs e)
    {
        int m = Environment.TickCount;
        int delay = (11 - tetrisLevel) * 50;
        if ((m - lastTickCount) > delay)
        {
            tetrisLevel = tetris.Run();
            lastTickCount = m;
        }
    }

    #endregion

    #region Commands

    private ICommand moveLeftCommand;
    public ICommand MoveLeftCommand
    {
        get
        {
            return moveLeftCommand ??= new ActionCommand(() =>
            {
                tetris.MoveLeft();
            });
        }
    }

    private ICommand moveRightCommand;
    public ICommand MoveRightCommand
    {
        get
        {
            return moveRightCommand ??= new ActionCommand(() =>
            {
                tetris.MoveRight();
            });
        }
    }

    private ICommand rotateCommand;
    public ICommand RotateCommand
    {
        get
        {
            return rotateCommand ??= new ActionCommand(() =>
            {
                tetris.Rotate();
            });
        }
    }

    private ICommand dropCommand;
    public ICommand DropCommand
    {
        get
        {
            return dropCommand ??= new ActionCommand(() =>
            {
                tetris.Drop();
            });
        }
    }

    private ICommand pauseCommand;
    public ICommand PauseCommand
    {
        get
        {
            return pauseCommand ??= new ActionCommand(() =>
            {
                if (tetris.GameOver)
                    return;

                tetris.Pause();
                if (tetris.IsPaused)
                {
                    timer.Stop();
                    Message = "Game paused";
                }
                else
                {
                    timer.Start();
                    Message = string.Empty;
                }
            });
        }
    }

    private ICommand enterCommand;
    public ICommand EnterCommand
    {
        get
        {
            return enterCommand ??= new ActionCommand(() =>
            {
                if (tetris.GameOver)
                {
                    Message = string.Empty;
                    tetrisLevel = tetris.Start();
                    DisplayValues();
                    timer.Start();
                }
            });
        }
    }

    #endregion

    #region Methods

    void DrawPixel(int x, int y, int color, bool isNextBlock)
    {
        if (isNextBlock)
        {
            if (y >= 0 && y < 4 && x >= 0 && x < 4)
                NextBlock[(3 - y) * 4 + x].SetColor(color);
        }
        else
        {
            if (y >= 0 && y < Rows && x >= 0 && x < Columns)
                Map[((Rows - 1) - y) * Columns + x].SetColor(color);
        }
    }

    void DrawBlock(Block block, int color, bool isNextBlock)
    {
        var bmp = block.GetCurrentBitmap();

        for (int i = 0; i < 4; i++)
        {
            if (isNextBlock)
            {
                DrawPixel(0, i, (bmp[i] & 0x8) != 0 ? color : 0, isNextBlock);
                DrawPixel(1, i, (bmp[i] & 0x4) != 0 ? color : 0, isNextBlock);
                DrawPixel(2, i, (bmp[i] & 0x2) != 0 ? color : 0, isNextBlock);
                DrawPixel(3, i, (bmp[i] & 0x1) != 0 ? color : 0, isNextBlock);
            }
            else
            {
                int yy = block.Y + 1 - i;

                if ((bmp[i] & 0x8) != 0)
                    DrawPixel(block.X - 2, yy, color, isNextBlock);
                if ((bmp[i] & 0x4) != 0)
                    DrawPixel(block.X - 1, yy, color, isNextBlock);
                if ((bmp[i] & 0x2) != 0)
                    DrawPixel(block.X, yy, color, isNextBlock);
                if ((bmp[i] & 0x1) != 0)
                    DrawPixel(block.X + 1, yy, color, isNextBlock);
            }
        }
    }

    void DisplayValues()
    {
        RaisePropertyChanged(nameof(ActualPoints));
        RaisePropertyChanged(nameof(ActualLevel));
        RaisePropertyChanged(nameof(CompletedRows));
    }

    #endregion

    #region Mvvm

    private readonly ObservableCollection<BlockViewModel> _Map;
    public ObservableCollection<BlockViewModel> Map { get { return _Map; } }

    private readonly ObservableCollection<BlockViewModel> _NextBlock;
    public ObservableCollection<BlockViewModel> NextBlock { get { return _NextBlock; } }

    public int ActualPoints { get => tetris.ActualPoints; }
    public int ActualLevel { get => tetris.ActualLevel; }
    public int CompletedRows { get => tetris.LinesCompleted; }

    private string _message = string.Empty;
    public string Message
    {
        get { return _message; }
        set
        {
            SetProperty(ref _message, value);
            ShowMessage = !string.IsNullOrEmpty(value) ? Visibility.Visible : Visibility.Collapsed;
            RaisePropertyChanged(nameof(ShowMessage));
        }
    }

    public Visibility ShowMessage { get; set; } = Visibility.Collapsed;

    #endregion

}
