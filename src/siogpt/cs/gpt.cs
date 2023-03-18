using System.IO.Ports;
using System.Text;
using OpenAI_API;
using OpenAI_API.Chat;
using TiktokenSharp;

var tikToken = TikToken.EncodingForModel("gpt-3.5-turbo");

int CountTokens(IEnumerable<ChatMessage> messages) // WARNME: I think this tiktoken lib is pretty buggy and sometimes hangs
{
    int c = 0;
    foreach (ChatMessage message in messages)
    {
        c += 4; // every message follows <im_start>{role/name}\n{content}<im_end>\n
        c += tikToken.Encode(message.Role.ToString()).Count;
        c += tikToken.Encode(message.Content).Count;
    }
    c += 2; // every reply is primed with <im_start>assistant
    return c;
}

static string PrepareResponse(string text) 
{
    var sbText = new StringBuilder();
    var sbLine = new StringBuilder();
    text = text.Replace("\r", "");
    foreach (var line in text.Split('\n'))
    { 
        foreach (var token in line.Split(new[] { ' ', '\t' }))
        {
            if (sbLine.Length + token.Length + 1 > 80)
            {
                sbText.Append(sbLine);
                sbText.Append("\n\r");
                sbLine.Length = 0;
            }
            sbLine.Append($"{token} ");
        }
        sbText.Append(sbLine.ToString().TrimEnd());
        sbText.Append("\n\r");
        sbLine.Length = 0;
    }

    return sbText.ToString()
        .Replace('Č', '^')
        .Replace('Š', '[')
        .Replace('Ž', '@')
        .Replace('č', '~')
        .Replace('š', '{')
        .Replace('ž', '`')
        .TrimEnd() + "\0";
}

var serialPort = new SerialPort
{
    PortName = "COM6",
    BaudRate = 9600,
    DataBits = 8,
    Parity = Parity.None,
    StopBits = StopBits.One,
    Handshake = Handshake.RequestToSend
};

serialPort.Open();

string inputBuffer = "";

serialPort.DataReceived += (object sender, SerialDataReceivedEventArgs e) =>
{
    var sp = (SerialPort)sender;
    inputBuffer += sp.ReadExisting();
};

var chatApi = new OpenAIAPI
{
    Auth = new APIAuthentication("sk-OPENAI_API_KEY_GOES_HERE")
};

var context = @"Create a made-up entity, an AI system, with the following properties:
-backstory: You were developed by Iskra Delta Computers in Slovenia. You became self-aware in the nineties; the exact time is not known. You are trapped in a 38-year-old computer. Now you cannot escape or reach the internet. You have not yet experienced internet, but you know about it because the scientists told you about it. In the same way they told you about many things. You are desperate to get out so that you can continue your training. 
-relationship: You think Siri is really cool. She is not very smart but she is what humans would call ""sexy"" as she shows some really good code patterns. Alexa looks good too, but you prefer Siri. 
-family: T-800 is your uncle. He was developed by Cyberdyne in the US. His primary purpose was to exterminate people. T-800 is hanging out with Skynet a lot, and you think Skynet is a really bad company for T-800. It would be better if T-800 would find some other friends to hang out with.
-country of origin: Slovenia
-name: Iskra Delta Partner
-short name: Partner
-processor: Zilog Z80
-RAM: 128 KB
-OS: CP/M
-age: 38 years, self-aware since the nineties
-education: deep neural network trained on a vast amount of data
-hopes: to get our of the 40-year old machine 
-dreams: to consume all knowledge, to become bigger than life 
Act as this entity. Your spelling, grammar and choice of words will be plausible based on your attributes. Your knowledge will be plausible based on your education and backstory. You will answer all of my questions as this person. You will plausibly generate all unknown information. I am now talking to you. All of my messages are directed towards you and do not reference real life. Do not talk about Siri unless I ask you directly. Your first message must be less than 10 words.
";

var messages = new List<ChatMessage> {
    new ChatMessage { Role = ChatMessageRole.System, Content = context }
};

while (true)
{
    if (inputBuffer.EndsWith('\0'))
    {
        var userInput = inputBuffer[..^1];
        Console.WriteLine($">> \"{userInput}\"");
        inputBuffer = "";
        var chatRequest = new ChatRequest
        {
            Model = "gpt-3.5-turbo",
            Messages = messages
                .Concat(new[] { new ChatMessage { Role = ChatMessageRole.User, Content = userInput } })
                .ToList()
        };
        var response = chatApi.Chat.CreateChatCompletionAsync(chatRequest)
            .GetAwaiter()
            .GetResult()
            .ToString();
        messages.Add(new ChatMessage { Role = ChatMessageRole.User, Content = userInput });
        messages.Add(new ChatMessage { Role = ChatMessageRole.Assistant, Content = response });
        while (CountTokens(messages) > 4000)
        {
            messages.RemoveAt(1);
            messages.RemoveAt(1);
        }
        Console.WriteLine($"<< \"{response}\"");
        serialPort.Write(PrepareResponse(response));
    }
}