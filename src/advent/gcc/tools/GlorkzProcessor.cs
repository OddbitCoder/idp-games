var lines = File.ReadAllLines(@"C:\Work\idp-games-idp-udev\src\advent\glorkz");

var crlf = "\n\r";

var msg = "";
var msgLen = 0;
int prevMsgId = 0;
int offset = 0;

int sect = 1;

var vocWriter = new BinaryWriter(File.Open("C:\\Work\\idp-games-idp-udev\\src\\advent\\voc.bin", FileMode.Create));
var txtWriter = new BinaryWriter(File.Open("C:\\Work\\idp-games-idp-udev\\src\\advent\\text.bin", FileMode.Create));
var travWriter = new BinaryWriter(File.Open("C:\\Work\\idp-games-idp-udev\\src\\advent\\trav.bin", FileMode.Create));
var txtCodeWriter = new StreamWriter("C:\\Work\\idp-games-idp-udev\\src\\advent\\gcc\\txtCode.txt");

var vocDict = new Dictionary<ushort, List<(string, ushort)>>();

int[] cond = new int[141];

var trav = new List<byte>[141];
for (int i = 0; i < 141; i++) {
    trav[i] = new List<byte>();
}

int[] fixd = new int[101];
int[] plac = new int[101];

int[] actspk = new int[35];

LocInfo[] locInfo = new LocInfo[141];
for (int i = 0; i < locInfo.Length; i++) { locInfo[i] = new LocInfo(); }

Dictionary<int, List<string>> words = new Dictionary<int, List<string>>();

static ushort GetHashCode(string word)
{
    ushort hcode = 0;
    foreach (char ch in word)
    {
        hcode <<= 2;
        hcode += ch;
    }
    return hcode;
}

static string Encode(string str)
{
    return str.Replace("\r", "\\r").Replace("\n", "\\n").Replace("\"", "\\\"");
}

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
            if (sect == 1)
            {
                locInfo[prevMsgId].Desc = msg;
                locInfo[prevMsgId].Idx = prevMsgId;
            }
            txtCodeWriter.WriteLine(msgLen > 0 
                ? $"/* {prevMsgId} */ {{ \"{Encode(msg)}\", {msgLen} }},"
                : $"/* {prevMsgId} */ {{ 0, 0 }},"
            );
            if (msgId != -1)
            {
                for (int i = prevMsgId; i < msgId - 1; i++)
                {
                    txtCodeWriter.WriteLine($"/* {i + 1} */ {{ 0, 0 }},");
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
    else if (sect == 10)
    {
        int msgId = Convert.ToInt32(split[0]);
        if ((prevMsgId != msgId) && !(sect == 5 && (msgId == 0 || msgId >= 100)))
        {
            Console.WriteLine(msgLen > 0
                ? $"{{ {offset}, {msgLen} }}, /* {msg} */"
                : $"{{ 0, 0 }},"
            );
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
        string word = null;
        if (wordId != -1)
        {
            word = split[1].Trim();
            //vocWriter.Write((byte)word.Length);
            //vocWriter.Write((ushort)wordId);
            //vocWriter.Write(word.Select(ch => (byte)ch).ToArray());
            ushort hcode = (ushort)(GetHashCode(word) % 128);
            if (vocDict.TryGetValue(hcode, out var list))
            {
                list.Add((word, (ushort)wordId));
            }
            else
            {
                vocDict.Add(hcode, new List<(string, ushort)>(new[] { (word, (ushort)wordId) }));
            }
        }
        if (words.TryGetValue(wordId, out List<string> items))
        {
            items.Add(word);
        }
        else
        {
            words.Add(wordId, new List<string>(new[] { word }));
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
            var tmp = new List<(int, int)>();
            tmp.Add((tverbs.First(), m));
            foreach (byte tverb in tverbs.Skip(1))
            {
                trav[id].Add(tverb);
                tmp.Add((tverb, m)); 
            }
            if (n > 140)
            {
                locInfo[id].TravelInfo.Add((tmp, new LocInfo { Idx = n, Desc = "No desc" }));
            }
            else
            {
                locInfo[id].TravelInfo.Add((tmp, locInfo[n]));
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
                cond[idx] |= (int)Math.Pow(2, bit);
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

int pos = 0;
int maxLen = 0;

foreach (var item in trav.Skip(1)) 
{
    //Console.WriteLine($"\"\\x{item.Count.ToString("X2")}{item.Select(x => $"\\x{x.ToString("X2")}").Aggregate((x, y) => x + y)}\",");
    Console.WriteLine($"{{ {pos}, {item.Count} }},");
    txtCodeWriter.Write("{ \"");
    pos += item.Count;
    foreach (var val in item) {
        travWriter.Write(val);
        txtCodeWriter.Write($"\\x{val:X2}");
    }
    txtCodeWriter.WriteLine($"\", {item.Count} }},");
    if (item.Count > maxLen) { maxLen = item.Count; }
}

//Console.WriteLine(maxLen);

travWriter.Close();

Console.WriteLine();
Console.WriteLine("*** VOC HASHTABLE ***");
Console.WriteLine();

int wordLoc = 0;

for (ushort i = 0; i < 128; i++)
{
    if (vocDict.TryGetValue(i, out List<(string word, ushort val)> list))
    {
        int len = 0;
        txtCodeWriter.Write("{ \"");
        foreach (var (word, val) in list)
        {
            vocWriter.Write(word.ToCharArray());
            txtCodeWriter.Write(word);
            vocWriter.Write('\0');
            txtCodeWriter.Write("\\0");
            vocWriter.Write(val);
            txtCodeWriter.Write($"\\x{(val & 0x00FF):X2}");
            txtCodeWriter.Write($"\\x{(val >> 8):X2}\"\"");
            len += 3 + word.Length;
        }
        Console.WriteLine($"{{ {wordLoc}, {len} }}, /* {list.Select(x => x.word).Aggregate((a, b) => a + " " + b)} */");
        txtCodeWriter.WriteLine($"\", {len} }},");
        wordLoc += len;
    }
    else
    {
        Console.WriteLine($"{{ 0, 0 }},");
        txtCodeWriter.WriteLine("{ 0, 0 },");
    }
}

vocWriter.Close();

string GetWord(int id, int cond)
{
    if (words.TryGetValue(id, out List<string> wordList))
    {
        return $"[{id}:{cond}]({wordList.Aggregate((a, b) => a + ", " + b)})";
    }
    else
    {
        return $"({id}:{cond})";
    }
}

foreach (var item in locInfo.Skip(1))
{
    Console.WriteLine($"[{item.Idx}] {item.Desc.Replace('\n', ' ').Replace("\r", "")}");
    Console.WriteLine("TRAVEL:");
    foreach (var travItem in item.TravelInfo) 
    {
        Console.WriteLine(travItem.Item1.Select(x => GetWord(x.Item1, x.Item2)).Aggregate((a, b) => a + ", " + b));
        Console.WriteLine($"\t=> [{travItem.Item2.Idx}] {travItem.Item2.Desc.Replace('\n', ' ').Replace("\r", "")}");
    }
    Console.WriteLine();
}

txtCodeWriter.Close();

class LocInfo
{
    public string Desc { get; set; }
    public int Idx { get; set; }
    public List<(List<(int, int)>, LocInfo)> TravelInfo { get; set; }
        = new List<(List<(int verb, int conditions)>, LocInfo)>();
}

