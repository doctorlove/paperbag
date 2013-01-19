using System;


//http://stackoverflow.com/questions/2725529/how-to-create-ascii-animation-in-windows-console-application-using-c

namespace paperbagcs
{
    class Program
    {
        static void Main(string[] args)
        {
            Expanding escape = new Expanding(30, 25);
            escape.Go();
        }

        private void Demo()
        {
            char[] chars = new char[] { '.', '-', '+', '^', '°', '*' };
            for (int i = 0; ; i++)
            {
                if (i != 0)
                {
                    // Delete the previous char by setting it to a space
                    Console.SetCursorPosition(6 - (i - 1) % 6 - 1, Console.CursorTop);
                    Console.Write(" ");
                }

                // Write the new char
                Console.SetCursorPosition(6 - i % 6 - 1, Console.CursorTop);
                Console.Write(chars[i % 6]);

                System.Threading.Thread.Sleep(100);
            }
        }
    }
}

