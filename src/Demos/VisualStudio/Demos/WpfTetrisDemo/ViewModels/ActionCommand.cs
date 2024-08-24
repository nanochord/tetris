using System.Windows.Input;

namespace WpfTetrisDemo.ViewModels;

public class ActionCommand(Action action) : ICommand
{
    private readonly Action _action = action;

    public void Execute(object parameter)
    {
        _action();
    }

    public bool CanExecute(object parameter)
    {
        return true;
    }

    public event EventHandler CanExecuteChanged;
}
