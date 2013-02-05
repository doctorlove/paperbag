using System;


//http://stackoverflow.com/questions/2725529/how-to-create-ascii-animation-in-windows-console-application-using-c

namespace paperbagcs
{
    class Program
    {
        static void Main(string[] args)
        {
            Expanding escape = new Expanding(30, 26);
            escape.Go();
        }
    }
}

