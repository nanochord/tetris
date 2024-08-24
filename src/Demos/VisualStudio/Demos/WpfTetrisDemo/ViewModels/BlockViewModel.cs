using System.Windows.Media;

namespace WpfTetrisDemo.ViewModels;

public class BlockViewModel : BaseViewModel
{
    static readonly Brush[] MainColors = [
        Brushes.Transparent,
        new SolidColorBrush(Color.FromArgb(0xff, 0xcc, 0xcc, 0x00)), // O
        new SolidColorBrush(Color.FromArgb(0xff, 0x00, 0xcc, 0xcc)), // I
        new SolidColorBrush(Color.FromArgb(0xff, 0x00, 0xcc, 0x00)), // S
        new SolidColorBrush(Color.FromArgb(0xff, 0xcc, 0x00, 0x00)), // Z
        new SolidColorBrush(Color.FromArgb(0xff, 0xcc, 0x66, 0x00)), // L
        new SolidColorBrush(Color.FromArgb(0xff, 0x00, 0x00, 0xcc)), // J
        new SolidColorBrush(Color.FromArgb(0xff, 0x99, 0x00, 0xcc)), // T
    ];

    static readonly Brush[] TopLeftBorderColors = [
        Brushes.Transparent,
        new SolidColorBrush(Color.FromArgb(0xff, 0xff, 0xff, 0x00)), // O
        new SolidColorBrush(Color.FromArgb(0xff, 0x00, 0xff, 0xff)), // I
        new SolidColorBrush(Color.FromArgb(0xff, 0x00, 0xff, 0x00)), // S
        new SolidColorBrush(Color.FromArgb(0xff, 0xff, 0x00, 0x00)), // Z
        new SolidColorBrush(Color.FromArgb(0xff, 0xff, 0x88, 0x00)), // L
        new SolidColorBrush(Color.FromArgb(0xff, 0x00, 0x00, 0xff)), // J
        new SolidColorBrush(Color.FromArgb(0xff, 0xcc, 0x00, 0xff)), // T
    ];

    static readonly Brush[] BottomRightColors = [
        Brushes.Transparent,
        new SolidColorBrush(Color.FromArgb(0xff, 0x99, 0x99, 0x00)), // O
        new SolidColorBrush(Color.FromArgb(0xff, 0x00, 0x99, 0x99)), // I
        new SolidColorBrush(Color.FromArgb(0xff, 0x00, 0x99, 0x00)), // S
        new SolidColorBrush(Color.FromArgb(0xff, 0x99, 0x00, 0x00)), // Z
        new SolidColorBrush(Color.FromArgb(0xff, 0x99, 0x44, 0x00)), // L
        new SolidColorBrush(Color.FromArgb(0xff, 0x00, 0x00, 0x99)), // J
        new SolidColorBrush(Color.FromArgb(0xff, 0x66, 0x00, 0x99)), // TZ
    ];

    private Brush _MainColor = Brushes.Black;
    public Brush MainColor
    {
        get { return _MainColor; }
        set { SetProperty(ref _MainColor, value); }
    }

    private Brush _TopLeftColor = Brushes.Black;
    public Brush TopLeftColor
    {
        get { return _TopLeftColor; }
        set { SetProperty(ref _TopLeftColor, value); }
    }

    private Brush _BottomRightColor = Brushes.Black;
    public Brush BottomRightColor
    {
        get { return _BottomRightColor; }
        set { SetProperty(ref _BottomRightColor, value); }
    }

    public void SetColor(int colorIndex)
    {
        if (colorIndex < 0 || colorIndex >= MainColors.Length)
            throw new IndexOutOfRangeException(nameof(colorIndex));

        MainColor = MainColors[colorIndex];
        TopLeftColor = TopLeftBorderColors[colorIndex];
        BottomRightColor = BottomRightColors[colorIndex];
    }
}
