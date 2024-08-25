using System.IO;
using System.Xml.Serialization;

namespace WpfTetrisDemo.Models;

public class TetrisDataModel
{
    public int HighScore { get; set; } = 0;

    public static TetrisDataModel Load(string path)
    {
        if (!File.Exists(path))
            return new TetrisDataModel();

        var serializer = new XmlSerializer(typeof(TetrisDataModel));
        using var reader = new StreamReader(path);
        var data = serializer.Deserialize(reader) as TetrisDataModel;

        return data;
    }

    public void Save(string path)
    {
        var folder = Path.GetDirectoryName(path);
        if (!Directory.Exists(folder))
            Directory.CreateDirectory(folder);

        var serializer = new XmlSerializer(typeof(TetrisDataModel));
        using var writer = new StreamWriter(path);
        serializer.Serialize(writer, this);
    }
}
