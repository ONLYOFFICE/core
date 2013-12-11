using System;
using System.Collections.Generic;
using System.Text;
using ASCOfficeEWSEditor.Editor.Walkers;

namespace ASCOfficeEWSEditor.Editor.Format.Colors
{
    /// <summary>
    /// Object of this class encapsulates color properties
    /// </summary>
    public class Color
    {
        internal    Color(ColorValue color_value, RangeWalker range_walker, StyleSetter style_setter, PropertyColor property, Workbook workbook)
        {
            this.range_walker = range_walker;
            this.workbook = workbook;
            this.color_value = color_value;
            this.style_setter = style_setter;
            this.property = property;
        }
        internal Color(ColorValue color_value)
        {
            this.color_value = color_value;
        }
        /// <summary>
        /// Read: RGB value common for the range of cells or null if the value is automatic and can't be transformed to RGB.
        /// Write: Sets the passed RGB value for every cell in the range.
        /// </summary>
        public RGB? Rgb
        {
            get
            {
                try
                {
                    RGB out_val;
                    if (color_value.ToRGB(out out_val.R, out out_val.G, out out_val.B))
                    {
                        return out_val;
                    }
                }
                catch { }
                return null;
            }
            set
            {
                property.Value.SetRGB(value.Value.R, value.Value.G, value.Value.B);
                range_walker.SetStyles(style_setter); // style setter shall have reference to property, so it works. Until you break this.
            }
        }


        /// <summary>
        /// Forms a text hexadecimal representation of the color in RGB form.
        /// </summary>
        /// <returns>Textual form of RGB or empty string if the color is automatic.</returns>
        public override string ToString()
        {
            return color_value.ToString();
        }

        /// <summary>
        /// Whether the color is automatic and represents the foreground value
        /// </summary>
        /// <returns></returns>
        public bool IsForeground()
        {
            return color_value.IsForeground();
        }

        /// <summary>
        /// Whether the color is automatic and represents the background value
        /// </summary>
        /// <returns></returns>
        public bool IsBackground()
        {
            return color_value.IsBackground();
        }

        /// <summary>
        /// Tries to determine the color index that exactly matches the RGB value that the Color object stores.
        /// </summary>
        /// <returns>Exact index or null if no exact index exists</returns>
        public int? GetExactIndex()
        {
            return color_value.GetExactIndex();
        }

        /// <summary>
        /// Determine the closest color index for the RGB value that the Color object stores.
        /// </summary>
        /// <returns>Closest index</returns>
        public int GetClosestIndex()
        {
            return color_value.GetClosestIndex();
        }

        /// <summary>
        /// Whether the affected range has different color values for different cells.
        /// If "true" is returned, the Color object must not use any property for read. If you do this you'll get a null reference exception.
        /// </summary>
        /// <returns></returns>
        public bool HasDifferentValues()
        {
            return color_value == null;
        }





        private Workbook workbook;
        private RangeWalker range_walker;
        private StyleSetter style_setter;
        private PropertyColor property;
        private ColorValue color_value;
    }


    /// <summary>
    /// RGB value
    /// </summary>
    public struct RGB
    {
        /// <summary>
        /// Constructs an RGB struct from red-green-blue components
        /// </summary>
        /// <param name="r">Red</param>
        /// <param name="g">Green</param>
        /// <param name="b">Blue</param>
        public RGB(int r, int g, int b)
        {
            R = r;
            G = g;
            B = b;
        }
        /// <summary>
        /// Red
        /// </summary>
        public int R;
        /// <summary>
        /// Green
        /// </summary>
        public int G;
        /// <summary>
        /// Blue
        /// </summary>
        public int B;
    }
}
