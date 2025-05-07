using System.Text.RegularExpressions;

string inputPath = "C:\\Work\\idp-games\\src\\softporn\\parts456.h"; 
string outputPath = "C:\\Work\\idp-games\\src\\softporn\\parts456_out.h";

string source = File.ReadAllText(inputPath);

var stringLiterals = new List<string>();
int index = 0;

string pattern = @"(?<!\\)""((?:\\.|[^""\\])*)""";

string result = Regex.Replace(source, pattern, match =>
{
    string extracted = Regex.Unescape(match.Groups[1].Value);

    bool replace = extracted.Length > 5;

    if (replace) { stringLiterals.Add(extracted); }
    return replace ? $"loadText({index++})" : $"\"{extracted}\"";
});

File.WriteAllText(outputPath, result);

int offset = 0;
for (int i = 0; i < stringLiterals.Count; i++)
{
    var item = stringLiterals[i];
    Console.WriteLine($"\t/* {i} */ {{ {offset}, {item.Length} }}, // {item.Replace("\r", "").Replace("\n", "\n\t// ")}");
    offset += item.Replace("\r", "").Replace("\n", "\n\r").Length;
}

// output texts.bin

var texts_bin = "";

foreach (var _item in stringLiterals)
{
    var item = _item.Replace("\r", "").Replace("\n", "\n\r");
    texts_bin += item;
}

File.WriteAllText("C:\\Work\\idp-games\\src\\softporn\\texts.bin", texts_bin);