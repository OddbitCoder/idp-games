using CommandLine;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;

Point pos = new Point(0, 0); 

void Rle2Bmp_DrawLine(Bitmap bmp, Tool tool, Style style, int len)
{
    if (style != Style.Dotted)
    {
        for (int x = pos.X; x < pos.X + len; x++)
        {
            bmp.SetPixel(x, pos.Y, tool == Tool.Pen ? Color.White : Color.Black);
        }
    }
    else
    {
        int alt = 0;
        for (int x = pos.X; x < pos.X + len; x++, alt++)
        {
            bmp.SetPixel(x, pos.Y, alt % 2 == 0 ? Color.White : Color.Black);
        }
    }
    pos.Offset(len, 0);
}

void Rle2Bmp_DrawRow(Bitmap bmp, byte[] rowRle)
{
    var tool = Tool.Pen;
    for (int i = 1; i <= rowRle[0]; i++)
    {
        byte b = rowRle[i];
        var style = (Style)((b & 64) >> 6);
        if ((b & 128) == 0)
        {
            Rle2Bmp_DrawLine(bmp, tool, style, b & 63);
        }
        else 
        {
            var _style = (Style)(((byte)tool >> 1) & (byte)style);
            Rle2Bmp_DrawLine(bmp, tool, _style, (b >> 3) & 7);
            tool = (Tool)(~(byte)tool & 2);
            _style = (Style)(((byte)tool >> 1) & (byte)style);
            Rle2Bmp_DrawLine(bmp, tool, _style, b & 7);
        }
        tool = (Tool)(~(byte)tool & 2);
    }
}

int Rle2Bmp_MeasureRow(byte[] rowRle)
{
    int w = 0;
    for (int i = 1; i <= rowRle[0]; i++)
    {
        byte b = rowRle[i];
        if ((b & 128) == 0)
        {
            w += b & 63;
        }
        else
        {
            w += (b >> 3) & 7;
            w += b & 7;
        }
    }
    return w;
}

(int w, int h) Rle2Bmp_MeasureImage(byte[] rle)
{
    int i = 0;
    int w = 0, h = 0;
    while (i < rle.Length)
    {
        int len = rle[i];
        var row = new byte[len + 1];
        for (int j = i + 1; j < i + 1 + len; j++)
        {
            row[j - i] = rle[j];
        }
        row[0] = (byte)len;
        w = Math.Max(Rle2Bmp_MeasureRow(row), w);
        h++;
        i += len + 1;
    }
    return (w, h);
}

StringBuilder Rle2Code(byte[] rle)
{
    int i = 0;
    var code = new StringBuilder();
    while (i < rle.Length)
    {
        int len = rle[i];
        code.Append($"\"\\x{rle[i]:X2}");
        for (int j = i + 1; j < i + 1 + len; j++)
        {
            code.Append($"\\x{rle[j]:X2}");
        }
        code.AppendLine("\",");
        i += len + 1;
    }
    return code;
}

Bitmap Rle2Bmp(int width, int height, byte[] rle)
{
    int i = 0;
    var bitmap = new Bitmap(width, height, PixelFormat.Format32bppArgb);
    var graphics = Graphics.FromImage(bitmap);
    pos.X = 0; pos.Y = 0;
    while (i < rle.Length)
    {
        int len = rle[i];
        var row = new byte[len + 1];
        for (int j = i + 1; j < i + 1 + len; j++)
        {
            row[j - i] = rle[j];
        }
        row[0] = (byte)len;
        Rle2Bmp_DrawRow(bitmap, row);
        pos.X = 0;
        pos.Offset(0, 1);
        i += len + 1;
    }
    return bitmap;
}

byte[] Bmp2Rle_Sprite(Bitmap bmp)
{
    var rle = new List<byte>();
    for (int y = 0; y < bmp.Height; y++)
    {
        var _px = bmp.GetPixel(0, y);
        int rl = 0;
        var rowRl = new List<(Color color, int len)>();
        // pass 1 (run lengths)
        for (int x = 0; x < bmp.Width; x++)
        {
            var px = bmp.GetPixel(x, y);
            if (px.SameAs(_px)) { rl++; }
            else { rowRl.Add((_px, rl)); rl = 1; }
            _px = px;
        }
        if (rl > 0 && !_px.Transparent())
        {
            rowRl.Add((_px, rl));
        }
        // pass 2 (encoding)
        var rowRle = new List<byte>();
        for (int i = 0; i < rowRl.Count; i++)
        {
            byte b = (byte)((rowRl[i].color.Transparent() ? 0 : 128) + (rowRl[i].color.SameAs(Color.White) ? 64 : 0));
            int len = rowRl[i].len;
            while (len > 0)
            {
                rowRle.Add((byte)(b | Math.Min(63, len)));
                len -= 63;
            }
        }
        if (rowRle.Count > 255) { Console.WriteLine("!!! UNABLE TO ENCODE !!! ROW LENGTH OVER 255 !!!"); }
        rle.Add((byte)rowRle.Count);
        rle.AddRange(rowRle);
    }
    return rle.ToArray();
}

byte[] Bmp2Rle(Bitmap bmp, bool fullLines = true)
{
    var imgRle = new List<byte>();
    for (int y = 0; y < bmp.Height; y++)
    {
        var _px = bmp.GetPixel(0, y);
        int rl = 0;
        var rowRl = new List<(Color color, int len)>();
        if (!_px.SameAs(Color.White)) { rowRl.Add((Color.White, 0)); }
        // pass 1 (basic RL)
        for (int x = 0; x < bmp.Width; x++)
        {
            var px = bmp.GetPixel(x, y);
            if (px.SameAs(_px)) { rl++; }
            else { rowRl.Add((_px, rl)); rl = 1; }
            _px = px;
        }
        if (rl > 0 && (fullLines || _px.SameAs(Color.White))) 
        { 
            rowRl.Add((_px, rl)); 
        }
        if (rowRl.Count == 1 && rowRl[0].len == 0) // optimize empty rows
        {
            rowRl.Clear();
        }
        // pass 2 (dotted runs)
        var rowRlExt = new List<(Color color, bool dotted, int len)>();
        for (int i = 0; i < rowRl.Count; i++)
        {
            rowRlExt.Add((rowRl[i].color, false, rowRl[i].len));
            if (rowRl[i].color.SameAs(Color.White) && rowRl[i].len == 1)
            {
                int altRl = 1;
                for (; i + 1 < rowRl.Count && !rowRl[i].color.SameAs(rowRl[i + 1].color) && rowRl[i + 1].len == 1; altRl++, i++);
                if (altRl > 1)
                {
                    rowRlExt.RemoveAt(rowRlExt.Count - 1);
                    rowRlExt.Add((Color.White, true, altRl));
                    if (i + 1 < rowRl.Count && rowRl[i + 1].color.SameAs(Color.White))
                    {
                        rowRlExt.Add((Color.Black, false, 0));
                    }
                }
            }
        }
        // pass 3 (Partner encoding)
        var rowRle = new List<byte>();
        for (int i = 0; i < rowRlExt.Count; i++)
        {   
            if (i < rowRlExt.Count - 1 && rowRlExt[i].len <= 7 && rowRlExt[i + 1].len <= 7)
            {
                // encode 2 items into 1 byte
                rowRle.Add((byte)(128 | (rowRlExt[i].dotted || rowRlExt[i + 1].dotted ? 64 : 0) | (rowRlExt[i].len << 3) | rowRlExt[i + 1].len));
                i++;
            }
            else
            {
                // encode 1 item into 1 or more bytes
                int maxLen = rowRlExt[i].dotted ? 62 : 63;
                int styleBit = rowRlExt[i].dotted ? 64 : 0; 
                int tail = rowRlExt[i].len % maxLen;
                int n = rowRlExt[i].len / maxLen;
                if (tail == 0 && rowRlExt[i].len > 0) { tail = maxLen; n--; }
                for (int j = 0; j < n; j++)
                {
                    rowRle.Add((byte)(maxLen | styleBit));
                    rowRle.Add(0);
                }
                rowRle.Add((byte)(tail | styleBit));
                if (tail <= 7 && n >= 1) // optimization: encode 2 bytes into 1
                {
                    rowRle.RemoveRange(rowRle.Count - 2, 2);
                    rowRle.Add((byte)(128 | styleBit | tail));
                }
            }
        }
        if (rowRle.Count > 255) { Console.WriteLine("!!! UNABLE TO ENCODE !!! ROW LENGTH OVER 255 !!!"); }
        imgRle.Add((byte)rowRle.Count);
        imgRle.AddRange(rowRle);
    }
    return imgRle.ToArray();
}

ImageFormat DetectGfxFormat(string fn, out bool resolved)
{ 
    var ext = Path.GetExtension(fn).ToUpper();
    resolved = true;
    switch (ext)
    {
        case ".BMP": return ImageFormat.Bmp;
        case ".PNG": return ImageFormat.Png;
        case ".JPG":
        case ".JPEG": return ImageFormat.Jpeg;
        case ".WEBP": return ImageFormat.Webp;
        case ".GIF": return ImageFormat.Gif;
        case ".TIF":
        case ".TIFF": return ImageFormat.Tiff;        
    }
    resolved = false;
    return ImageFormat.Png;
}

var parser = new Parser(settings => 
{
    settings.CaseInsensitiveEnumValues = true;
    settings.HelpWriter = Parser.Default.Settings.HelpWriter;
});

parser.ParseArguments<CliOptions>(args)
    .WithParsed(o =>
    {
        bool imgFl = false;
        if (o.Mode == ConversionMode.Auto)
        {
            DetectGfxFormat(o.InputFile, out imgFl);
        }
        if (imgFl || o.Mode == ConversionMode.Bmp2Rle)
        {
            var img = Image.FromFile(o.InputFile);
            var rle = o.Encoding == RleEncoding.Background
                ? Bmp2Rle((Bitmap)img, !o.CutRows)
                : Bmp2Rle_Sprite((Bitmap)img);
            File.WriteAllBytes(o.OutputFile, rle);
            if (!o.NoCode) // output code
            {
                var codeFn = o.OutputFile + ".code";
                File.WriteAllText(codeFn, Rle2Code(rle).ToString());
            }
        }
        else // Rle2Bmp
        {
            var rle = File.ReadAllBytes(o.InputFile);
            (int w, int h) = Rle2Bmp_MeasureImage(rle);
            var img = Rle2Bmp(w, h, rle);
            img.Save(o.OutputFile, DetectGfxFormat(o.OutputFile, out bool resolv));
        }
    });

enum Tool
{
    Eraser = 0,
    Pen = 2
    
}

enum Style
{ 
    Normal = 0,
    Dotted = 1
}

enum ConversionMode
{ 
    Auto,
    Bmp2Rle,
    Rle2Bmp
}

enum RleEncoding
{
    Background,
    Sprite
}

class CliOptions
{
    [Option('m', "mode", Required = false, HelpText = "Conversion mode (Auto [default], Bmp2Rle, Rle2Bmp).")]
    public ConversionMode Mode { get; set; }
        = ConversionMode.Auto;
    [Option('e', "encoding", Required = false, HelpText = "RLE encoding (Background [default], Sprite).")]
    public RleEncoding Encoding { get; set; }
        = RleEncoding.Background;
    [Option('i', "input", Required = true, HelpText = "Input file name.")]
    public string InputFile { get; set; }
    [Option('o', "output", Required = true, HelpText = "Output file name.")]
    public string OutputFile { get; set; }
    [Option('n', "no-code", Required = false, HelpText = "Do not output code (code output enabled by default).")]
    public bool NoCode { get; set; }
    [Option('c', "cut-rows", Required = false, HelpText = "Cut rows short (full rows by default).")]
    public bool CutRows { get; set; }
}

static class Extensions
{
    public static bool SameAs(this Color c1, Color c2)
    {
        //return c1.ToArgb() == c2.ToArgb();
        // compare with some tolerance
        return Math.Abs(c1.R - c2.R) <= 30 &&
            Math.Abs(c1.G - c2.G) <= 30 &&
            Math.Abs(c1.B - c2.B) <= 30 &&
            Math.Abs(c1.A - c2.A) <= 30;
    }

    public static bool Transparent(this Color c)
    {
        // compare with some tolerance
        return c.A <= 30;
    }
}