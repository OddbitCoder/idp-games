for (int n = 0; n < 60; n++)
{
    var lines = new List<Line>();

    foreach (var list in new[] {
    CreateLines(253, 143, 40),
    CreateLines(254, 143, 40),
    CreateLines(255, 143, 40),
    CreateLines(256, 143, 40),
    CreateLines(257, 143, 39),
    CreateLines(258, 143, 17),
    CreateLines(259, 143, 17)
})
    {
        foreach (var line in list)
        {
            line.Rotate(256, 128, 0.1047 * n);
            line.SnapToGrid();
            //Console.WriteLine(line);
            if (!lines.Contains(line))
            {
                lines.Add(line);
            }
            var l2 = new Line
            {
                pt1 = new Pt { x = line.pt1.x + 1, y = line.pt1.y },
                pt2 = new Pt { x = line.pt2.x + 1, y = line.pt2.y }
            };
            if (!lines.Contains(l2))
            {
                lines.Add(l2);
            }
        }
    }

    Console.WriteLine($"const line hand_minute_{n}[] = {{");
    foreach (var l in lines)
    {
        Console.WriteLine($"\t{l}");
    }
    Console.WriteLine("};");
    Console.WriteLine();
}

Console.WriteLine("const line *hands[] = {");
for (int i = 0; i < 60; i++) {
    Console.WriteLine($"\thand_minute_{i},");
}
Console.WriteLine("};");

static Line[] CreateLines(double x, double y1, double y2)
{ 
    return new Line[] {
        new Line { pt1 = new Pt { x = x, y = y1 }, pt2 = new Pt { x = x, y = y2 } },
        new Line { pt1 = new Pt { x = x + 0.25, y = y1 }, pt2 = new Pt { x = x + 0.25, y = y2 } },
        new Line { pt1 = new Pt { x = x + 0.5, y = y1 }, pt2 = new Pt { x = x + 0.5, y = y2 } },
    };
}

class Pt
{
    public double x;
    public double y;

    public void Rotate(double c1, double c2, double angle)
    {
        double x2 = (x - c1) * Math.Cos(angle) - (y - c2) * Math.Sin(angle) + c1;
        double y2 = (x - c1) * Math.Sin(angle) + (y - c2) * Math.Cos(angle) + c2;
        x = x2;
        y = y2;
    }
}

class Line
{
    public Pt pt1 
        = new();
    public Pt pt2 
        = new();

    public void Rotate(double c1, double c2, double angle)
    {
        pt1.Rotate(c1, c2, angle);
        pt2.Rotate(c1, c2, angle);
    }

    public void SnapToGrid()
    { 
        pt1.y = Math.Round(pt1.y); 
        pt2.y = Math.Round(pt2.y);
        pt1.x = Math.Round(pt1.x * 2);
        pt2.x = Math.Round(pt2.x * 2);
    }

    public override string ToString()
    {
        //return $"({pt1.x},{pt1.y})-({pt2.x},{pt2.y})";
        return $"{{ {{{pt1.x}, {pt1.y}}}, {{{pt2.x}, {pt2.y}}} }},";
    }

    public override bool Equals(object? obj)
    {
        if (obj is not Line) { return false; }
        var other = (Line)obj;
        return pt1.x == other.pt1.x && pt1.y == other.pt1.y
            && pt2.x == other.pt2.x && pt2.y == other.pt2.y;
    }
}

