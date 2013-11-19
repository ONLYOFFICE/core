using System;
using System.Collections.Generic;
using System.Text;

namespace AVSOfficeEWSEditor.Editor.Charts
{
    public class Misc
    {
        public static bool StringToBool(string attrValue)
        {
            bool retVal = false;

            if (!string.IsNullOrEmpty(attrValue))
            {
                if (bool.TryParse(attrValue, out retVal))
                    return retVal;
                else
                {
                    int convertRes = 0;
                    if (int.TryParse(attrValue, out convertRes) && (convertRes != 0))
                        retVal = true;
                }
            }

            return retVal;
        }
    }
}
