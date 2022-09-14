using System.Web;

var lines = File.ReadAllLines(@"C:\Work\idp-games\src\adv0\glorkz");

var crlf = "\r\n";

var msg_1 = "";
var len_1 = 0;
int id_1 = 1;

int sect = 1;

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
        if (id_1 != msgId)
        {
            Console.WriteLine($"\t{{ \"{HttpUtility.JavaScriptStringEncode(msg_1)}\", {len_1} }},");
            //Console.WriteLine();
            msg_1 = "";
            id_1 = msgId;
            len_1 = 0;
        }
        if (msgId != -1)
        {
            var msgPart = split[1].Trim();
            len_1 += (msg_1 == "" ? 0 : 2) + msgPart.Length;
            msg_1 += (msg_1 == "" ? "" : crlf) + msgPart;
        }
    }
    else if (sect == 4)
    {
        var wordId = Convert.ToInt32(split[0]);
        if (wordId != -1)
        {
            var word = split[1].Trim();
            Console.WriteLine($"vocab(\"{HttpUtility.JavaScriptStringEncode(word)}\",-2,{wordId});");
        }
    }
}