
using System;
using System.Diagnostics;
using System.Linq;
using System.Threading;

namespace paperbagcs
{
    public class Expanding
    {
        private int _width;
        private int _bagWidth;
        private int _edge;
        private int _stars;

        private char[] _buffer;

        private void FillEmptyRow(int row)
        {
          for(int i = 0; i<_width; ++i)
	        {
		        _buffer[row*_width + i] = ' ';
	        }
        }

        private void FillBagRow(int row)
        {
	        char c = ' ';
	        for(int i = 0; i<_width; ++i)
	        {
		        if(i==_edge || i==_edge + _bagWidth)
			        c = '|';
		        _buffer[row*_width + i] = c;
		        c = ' ';
	        }
        }

        private void FillBagBase(int row)
        {
	        for(int i = 0; i<_width; ++i)
	        {
		        if(i>=_edge && i<=_edge+_bagWidth)
			        _buffer[row*_width + i] = '-';
		        else
		            _buffer[row*_width + i] = ' ';
	        }
        }

        private void FormGrid()
        {
            _buffer = new char[_width*_width];

            for (int row = 0; row < _width; ++row)
            {
                if (row <= _edge || row > _edge + _bagWidth)
                    FillEmptyRow(row);
                else if (row == _edge + _bagWidth)
                    FillBagBase(row);
                else
                    FillBagRow(row);
            }
        }

        private void DrawGrid()
        {
            Debug.Write(_buffer);

            int line = 0;
            Console.SetCursorPosition(0, line++);
            for (int i = 0; i < _buffer.Length; ++i)
            {
                Console.Write(_buffer[i]);
                if(i%_width == 0)
                    Console.SetCursorPosition(0, line++);
            }
        }

        private char Above(int pos)
        {
            if (pos < _width)//top row
                return ' ';
            return _buffer[pos - _width];
        }

        private char Below(int pos)
        {
            if (pos >= (_width - 1) * _width)//bottom row
                return ' ';
            return _buffer[pos + _width];
        }

        private char Left(int pos)
        {
            if (pos % _width == 0)//left column
                return ' ';
            return _buffer[pos - 1];
        }

        private char Right(int pos)
        {
            if (pos % _width == _width-1)//right column
                return ' ';
            return _buffer[pos + 1];
        }

        private bool MoveCreatures()
        {
            //Do *not* update the actual buffer here... one move will affect the next step 
            if (_stars == 0)
            {
                /*
                 *   | |
                 *   | |
                 *   ---
                 */

                int pos = (_edge + _bagWidth / 2) * _width + _edge + _bagWidth / 2; ;
                _buffer[pos] = '*';
                ++_stars;
                return true;
            }
            if(_stars < _width*_width)
            {
                bool breached = false;
                char[] newBuffer = _buffer.ToArray();
                for (int i = 0; i < _buffer.Length; ++i)
                {
                    if (Above(i) == '*' || Below(i) == '*' || Left(i) == '*' || Right(i) == '*')
                    {
                        if(_buffer[i] == '|' || _buffer[i] == '-')
                            breached = true;
                        newBuffer[i] = '*';
                        ++_stars;
                    }
                }
                _buffer = newBuffer;
                return !breached;
            }
            return false;
        }

        public Expanding(int width, int bagWidth)
        {
            if (bagWidth > width)
                throw new ArgumentException("Bag too big for frame");

            _width = width;
            _bagWidth = bagWidth;
            _edge = (_width - _bagWidth) / 2;
        }

        public void Go()
        {
            FormGrid();
            while (MoveCreatures())
            {
                DrawGrid();
                Thread.Sleep(1000);
            }
            DrawGrid();

            Console.WriteLine("\nDone");
        }
    }
}

