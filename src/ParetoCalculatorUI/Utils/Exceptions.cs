using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ParetoCalculatorUI.Utils
{
    class ParetoCalculatorUIException : Exception
    {
        public ParetoCalculatorUIException()
        {
        }

        public ParetoCalculatorUIException(String message)
            : base(message)
        {
        }

        public ParetoCalculatorUIException(String message, Exception inner)
            : base(message, inner)
        {
        }
    }
}
