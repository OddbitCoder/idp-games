using System.Collections;
using System.Net.Http.Headers;

var lines = File.ReadAllLines(@"C:\Work\idp-games\src\adv0\glorkz");

var crlf = "\n";

var msg = "";
var msg_len = 0;
int msg_id = 1;
int offset = 0;

int sect = 1;

var travWriter = new BinaryWriter(File.Open("C:\\Work\\idp-games\\src\\adv0\\trav.bin", FileMode.Create));
var vocWriter = new BinaryWriter(File.Open("C:\\Work\\idp-games\\src\\adv0\\voc.bin", FileMode.Create));
var txtWriter = new BinaryWriter(File.Open("C:\\Work\\idp-games\\src\\adv0\\text.bin", FileMode.Create));

int[] cond = new int[141];
var fixd = new List<int>();

var trav = new List<byte>[141];
for (int i = 0; i < 141; i++) {
    trav[i] = new List<byte>();
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

    if (sect == 1 || sect == 2 || sect == 5 || sect == 6 || sect == 10 || sect == 12)
    {
        int msgId = Convert.ToInt32(split[0]);
        if (msg_id != msgId && msg_len > 0)
        {
            Console.WriteLine($"{{ {offset}, {msg_len} }}, /* {msg} */");
            txtWriter.Write(msg.Select(ch => (byte)ch).ToArray());
            offset += msg_len;
            msg = "";
            msg_id = msgId;
            msg_len = 0;
        }
        if (msgId != -1)
        {
            var msgPart = split[1].Trim();
            msg_len += (msg == "" ? 0 : crlf.Length) + msgPart.Length;
            msg += (msg == "" ? "" : crlf) + msgPart;
        }
        else
        {
            msg_id = 1;
        }
    }
    else if (sect == 4)
    {
        int wordId = Convert.ToInt32(split[0]);
        if (wordId != -1)
        {
            var word = split[1].Trim();
            //Console.WriteLine($"vocab(\"{HttpUtility.JavaScriptStringEncode(word)}\",-2,{wordId});");
            Console.WriteLine($"Writing entry for '{word}'...");
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
            Console.WriteLine($"Writing trav {id}; tverb list size = {tverb_count}");
            travWriter.Write((byte)tverb_count);
            travWriter.Write((byte)id);
            travWriter.Write((ushort)n);
            travWriter.Write((ushort)m);
            var tverbs = parts.Skip(1).Select(x => Convert.ToInt32(x));
            foreach (int tverb in tverbs)
            {
                travWriter.Write((byte)tverb);
            }
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
            Console.WriteLine($"{parts[0]},");
            fixd.Add(parts.Length == 2 ? Convert.ToInt32(parts[1]) : 0);
        }
    }
}

travWriter.Close();
vocWriter.Close();
txtWriter.Close();

Console.WriteLine("*** 9 ***");

foreach (var item in cond) 
{
    Console.WriteLine($"{item},");
}

Console.WriteLine("*** 7 fixd ***");

foreach (var item in fixd)
{
    Console.WriteLine($"{item}, ");
}

Console.WriteLine("*** TRAV TABLE ***");

foreach (var item in trav.Skip(1)) 
{
    Console.WriteLine($"\"\\x{item.Count.ToString("X2")}{item.Select(x => $"\\x{x.ToString("X2")}").Aggregate((x, y) => x + y)}\",");
}