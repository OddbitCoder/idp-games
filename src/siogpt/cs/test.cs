using System.ComponentModel.DataAnnotations;
using System.IO.Ports;

var serialPort = new SerialPort
{
    PortName = "COM6",
    BaudRate = 9600,
    DataBits = 8,
    Parity = Parity.None,
    StopBits = StopBits.One,
    Handshake = Handshake.None,
    // Set the read/write timeouts
    ReadTimeout = 500,
    WriteTimeout = 500
};

string str = "1234567890ABCDEF";
int i = 0;

bool xon = true;

serialPort.Open();
//serialPort.Close();
//serialPort.Open();

void DataReceivedHandler(
                        object sender,
                        SerialDataReceivedEventArgs e)
{
    SerialPort sp = (SerialPort)sender;
    string indata = sp.ReadExisting();
    foreach (var ch in indata) {
        if (ch == 17) { /*Console.Write("xon ");*/ xon = true; }
        else if (ch == 19) { /*Console.Write("xoff ");*/ xon = false; }
        else { Console.Write(ch); }
    }
}

serialPort.DataReceived += DataReceivedHandler;

while (true) {
    if (xon)
    {
        char ch = str[i];
        if (++i == str.Length) { i = 0; }
        serialPort.Write("" + ch);
        //Console.Write(".");
    }
}

