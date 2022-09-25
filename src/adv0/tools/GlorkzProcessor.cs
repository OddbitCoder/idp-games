var lines = File.ReadAllLines(@"C:\Work\idp-games\src\adv0\glorkz");

var crlf = "\n\r";

var msg = "";
var msgLen = 0;
int prevMsgId = 0;
int offset = 0;

int sect = 1;

var vocWriter = new BinaryWriter(File.Open("C:\\Work\\idp-games\\src\\adv0\\voc.bin", FileMode.Create));
var txtWriter = new BinaryWriter(File.Open("C:\\Work\\idp-games\\src\\adv0\\text.bin", FileMode.Create));

int[] cond = new int[141];

var trav = new List<byte>[141];
for (int i = 0; i < 141; i++) {
    trav[i] = new List<byte>();
}

int[] fixd = new int[101];
int[] plac = new int[101];

int[] actspk = new int[35];

foreach (var line in lines.Select(l => l.Trim())) 
{
    var split = line.Split(new[] { ' ', '\t' }, 2);
    if (split.Length == 1)
    {
        if (line != "-1")
        {
            sect = Convert.ToInt32(line);
            Console.WriteLine();
            Console.WriteLine($"*** {sect} ***");
            Console.WriteLine();
            if (sect != -1) { continue; }
        }
    }

    if (sect == 1 || sect == 2 || sect == 5 || sect == 6 || sect == 12)
    {
        int msgId = Convert.ToInt32(split[0]);
        if ((prevMsgId != msgId) && !(sect == 5 && (msgId == 0 || msgId >= 100)))
        {
            Console.WriteLine(msgLen > 0 
                ? $"/* {prevMsgId} */ {{ {offset}, {msgLen} }}, /* {msg} */"
                : $"/* {prevMsgId} */ {{ {offset}, {msgLen} }},"
            );
            if (msgId != -1)
            {
                for (int i = prevMsgId; i < msgId - 1; i++)
                {
                    Console.WriteLine($"{{ {offset + msgLen}, 0 }},");
                }
            }
            txtWriter.Write(msg.Select(ch => (byte)ch).ToArray());
            offset += msgLen;
            msg = "";
            prevMsgId = msgId;
            msgLen = 0;
        }
        if (msgId != -1)
        {
            var msgPart = split[1].Trim();
            if (sect == 5 && (msgId == 0 || msgId >= 100))
            {
                msgPart = $"{msgId.ToString("000")}\t{msgPart}";
            }
            msgLen += (msg == "" ? 0 : crlf.Length) + msgPart.Length;
            msg += (msg == "" ? "" : crlf) + msgPart;
        }
        else
        {
            prevMsgId = 0;
        }
    }
    else if (sect == 4)
    {
        int wordId = Convert.ToInt32(split[0]);
        if (wordId != -1)
        {
            var word = split[1].Trim();
            vocWriter.Write((byte)word.Length);
            vocWriter.Write((ushort)wordId);
            vocWriter.Write(word.Select(ch => (byte)ch).ToArray());
        }
    }
    else if (sect == 3)
    {
        int id = Convert.ToInt32(split[0]);
        if (id != -1)
        {
            var parts = split[1].Split('\t');
            int n1 = Convert.ToInt32(parts[0]);
            int n = n1 % 1000; // "tloc"
            int m = n1 / 1000; // "conditions"
            foreach (var part in parts.Skip(1).Select(x => Convert.ToInt32(x)))
            {
                if (part >= 128 || part == 0) { throw new Exception(part.ToString()); }
            }
            int tverb_count = parts.Skip(1).Count();
            var tverbs = parts.Skip(1).Select(x => Convert.ToInt32(x));
            // write trav table as bytes (optimized)
            // add first tverb (with '| 128')
            trav[id].Add((byte)(tverbs.First() | 128));
            // add tloc
            trav[id].Add((byte)(n & 0xFF));
            trav[id].Add((byte)((n >> 8) & 0xFF));
            // add cond
            trav[id].Add((byte)(m & 0xFF));
            trav[id].Add((byte)((m >> 8) & 0xFF));
            // add tverbs
            foreach (byte tverb in tverbs.Skip(1))
            {
                trav[id].Add(tverb);
            }
        }
    }
    else if (sect == 9)
    {
        int bit = Convert.ToInt32(split[0]);
        if (bit != -1)
        {
            var parts = split[1].Split('\t');
            foreach (var idx in parts.Select(x => Convert.ToInt32(x)))
            {
                cond[idx] |= 2 ^ bit;
            }
        }
    }
    else if (sect == 7)
    {
        int idx = Convert.ToInt32(split[0]);
        if (idx != -1)
        {
            var parts = split[1].Split('\t');
            plac[idx] = Convert.ToInt32(parts[0]);
            fixd[idx] = parts.Length == 2 ? Convert.ToInt32(parts[1]) : 0;
        }
    }
    else if (sect == 8)
    { 
        int idx = Convert.ToInt32(split[0]);
        if (idx != -1)
        {
            actspk[idx] = Convert.ToInt32(split[1]);
        }
    }
}

vocWriter.Close();
txtWriter.Close();

Console.WriteLine();
Console.WriteLine("*** 7 fixd ***");
Console.WriteLine();

foreach (var item in fixd)
{
    Console.Write($"{item}, ");
}
Console.WriteLine();

Console.WriteLine();
Console.WriteLine("*** 7 plac ***");
Console.WriteLine();

foreach (var item in plac)
{
    Console.Write($"{item}, ");
}
Console.WriteLine();

Console.WriteLine();
Console.WriteLine("*** 9 cond ***");
Console.WriteLine();

foreach (var item in cond)
{
    Console.Write($"{item}, ");
}
Console.WriteLine();

Console.WriteLine();
Console.WriteLine("*** 8 actspk ***");
Console.WriteLine();

foreach (var item in actspk)
{
    Console.Write($"{item}, ");
}
Console.WriteLine();

Console.WriteLine();
Console.WriteLine("*** TRAVEL TABLE ***");
Console.WriteLine();

foreach (var item in trav.Skip(1)) 
{
    Console.WriteLine($"\"\\x{item.Count.ToString("X2")}{item.Select(x => $"\\x{x.ToString("X2")}").Aggregate((x, y) => x + y)}\",");
}