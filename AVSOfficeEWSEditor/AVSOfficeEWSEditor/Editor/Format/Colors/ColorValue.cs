using System;
using System.Collections.Generic;
using System.Text;
using System.Globalization;
using System.Xml;

namespace AVSOfficeEWSEditor.Editor.Format.Colors
{
    /// <summary>
    /// Object of this class stores color information
    /// </summary>
    internal class ColorValue  // : IComparable
    {
        // RGB
        internal ColorValue(string hex_argb_value, bool fore_or_back, Workbook workbook)
        {
            this.workbook = workbook;
            this.source = SourceType.rgb;
            this.fore_or_back = fore_or_back;
            FromARGB(hex_argb_value);
        }

        // Index
        internal ColorValue(int index_init, bool fore_or_back, Workbook workbook)
        {
            this.workbook = workbook;
            this.fore_or_back = fore_or_back;

            if (index_init >= 64)
            {
                this.source = SourceType.auto;
                R = G = B = fore_or_back ? 0 : 255;
                return;
            }
            this.source = SourceType.indexed;

            index = index_init;
            FromIndex(index_init);
        }

        // Theme
        internal ColorValue(int theme_init, double tint_init, bool fore_or_back, Workbook workbook)
        {
            this.workbook = workbook;
            this.source = SourceType.theme;
            this.fore_or_back = fore_or_back;

            theme_num = theme_init;
            tint = tint_init;
            FromTheme(theme_init);
            ApplyTint(tint_init);
        }

        // Auto
        internal ColorValue(bool fore_or_back, Workbook workbook)
        {
            this.workbook = workbook;
            this.source = SourceType.auto;
            this.fore_or_back = fore_or_back;

            R = G = B = fore_or_back ? 0 : 255;
        }

        private void FromARGB(string argb)
        {
            int packed_rgb;
            if(argb.Length != 8 || !int.TryParse(argb.Substring(2), NumberStyles.HexNumber, null, out packed_rgb))
            {
                R = G = B = 0;
                return;
            }
            FromPackedRGB(packed_rgb);
        }

        private void FromPackedRGB(int packed_rgb)
        {
            R = ((packed_rgb & 0x00ff0000) >> 16);
            G = ((packed_rgb & 0x0000ff00) >> 8);
            B =  (packed_rgb & 0x000000ff);
        }

        private int ToPackedRGB()
        {
            return R << 16 | G << 8 | B;
        }

        private void FromIndex(int index)
        {
            int packed_rgb = workbook.StyleSheet.Palette.IndexToPackedRGB(index);
            FromPackedRGB(packed_rgb);
        }

        private void FromTheme(int theme_num)
        {
            string color_str = workbook.Theme.GetRGBByColor(WrongThemeIndexToCorrectThemeIndex(theme_num));
            switch(color_str)
            {
                case "":
                    R = G = B = fore_or_back ? 0 : 255;
                    break;
                case "window":
                    R = G = B = 255;
                    break;
                case "windowText":
                    R = G = B = 0;
                    break;
                default:
                    FromARGB("FF" + color_str);
                    break;
            }
        }

        private static int WrongThemeIndexToCorrectThemeIndex(int wrong_index)
        {
            switch(wrong_index)
            {
                case 0:
                    return 1;
                case 1:
                    return 0;
                case 2:
                    return 3;
                case 3:
                    return 2;
                default:
                    return wrong_index;
            }
        }

        private static int CorrectThemeIndexToWrongThemeIndex(int correct_index)
        {
            return WrongThemeIndexToCorrectThemeIndex(correct_index);
        }

        private void ApplyTint(double tint)
        {
            if(tint == 0)
            {
                return;
            }
            double h;
            double s;
            double l;

            ToHSL(out h, out s, out l);

            if (tint < 0)
            {
                l = l * (1 + tint);
            }
            else if (tint > 0)
            {
                l = l * (1 - tint) + tint;
            }
            //else left not changed

            FromHSL(h, s, l);
        }

        private void ToHSL(out double h, out double s, out double l)
        {
            int maxRGB = Math.Max(R, Math.Max(G, B));
            int minRGB = Math.Min(R, Math.Min(G, B));

            // Hue ************
            if (maxRGB == minRGB)
            {
                h = 0;
            }
            else if (maxRGB == R && G >= B)
            {
                h = (double)(G - B) / (6 * (maxRGB - minRGB));
            }
            else if ((maxRGB == R && G < B))
            {
                h = (double)(G - B) / (6 * (maxRGB - minRGB)) + 1;
            }
            else if (maxRGB == G)
            {
                h = (double)(B - R) / (6 * (maxRGB - minRGB)) + (double)1 / 3;
            }
            else// if (maxRGB == B)
            {
                h = (double)(R - G) / (6 * (maxRGB - minRGB)) + (double)2 / 3;
            }

            // Luminosity ************

            l = (double)(maxRGB + minRGB) / 2 / 255;

            // Saturation ************

            if (l == 0)
            {
                s = 0;
            }
            else if (l > 0 && l <= 0.5)
            {
                s = (double)(maxRGB - minRGB) / (maxRGB + minRGB);
            }
            else if (l > 0.5 && l < 1)
            {
                s = (double)(maxRGB - minRGB) / (2 * 255 - (maxRGB + minRGB));
            }
            else// if (l == 1)
            {
                s = 0;
            }
        }

        private void FromHSL(double h, double s, double l)
        {
            double q = l < 0.5 ? l * (1 + s) : l + s - (l * s);
            double p = 2 * l - q;

            double[] t3 = new double[] { h + 1.0 / 3.0, h, h - 1.0 / 3.0 };
            double[] clr = new double[] { 0, 0, 0 };

            for (int i = 0; i < 3; ++i)
            {
                double t = t3[i];
                if (t < 0)
                {
                    t = t + 1;
                }
                else if (t > 1)
                {
                    t = t - 1;
                }

                if (t < (double)1 / 6)
                {
                    clr[i] = (int)((p + (q - p) * 6 * t) * 255);
                }
                else if (t >= (double)1 / 6 && t < (double)1 / 2)
                {
                    clr[i] = (int)(q * 255);
                }
                else if (t >= (double)1 / 2 && t < (double)2 / 3)
                {
                    clr[i] = (int)((p + (q - p) * 6 * ((double)2 / 3 - t)) * 255);
                }
                else
                {
                    clr[i] = (int)(p * 255);
                }
            }
            R = (int)Math.Round(clr[0]);
            G = (int)Math.Round(clr[1]);
            B = (int)Math.Round(clr[2]);
        }

        /// <summary>
        /// Obtain RGB value in decimal form
        /// </summary>
        /// <param name="r">Red</param>
        /// <param name="g">Green</param>
        /// <param name="b">Blue</param>
        /// <returns>Returns false if the color is automatic and can't be transformed to RGB</returns>
        public bool ToRGB(out int r, out int g, out int b)
        {
            r = R;
            g = G;
            b = B;
            if (source == SourceType.auto)
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// Set RGB value
        /// </summary>
        /// <param name="r">Red</param>
        /// <param name="g">Green</param>
        /// <param name="b">Blue</param>
        public void SetRGB(int r, int g, int b)
        {
            R = r;
            G = g;
            B = b;
            source = SourceType.rgb;
        }

        /// <summary>
        /// Whether the color is automatic and represents the foreground value
        /// </summary>
        /// <returns></returns>
        public bool IsForeground()
        {
            return source == SourceType.auto && fore_or_back;
        }

        /// <summary>
        /// Whether the color is automatic and represents the background value
        /// </summary>
        /// <returns></returns>
        public bool IsBackground()
        {
            return source == SourceType.auto && !fore_or_back;
        }


        /// <summary>
        /// Tries to determine the color index that exactly matches the RGB value that the Color object stores.
        /// </summary>
        /// <returns>Exact index or null if no exact index exists</returns>
        public int? GetExactIndex()
        {
            if (source == SourceType.auto)
            {
                return fore_or_back ? 64 : 65;
            }
            if (source == SourceType.indexed && index != null)
            {
                return index;
            }
            return workbook.StyleSheet.Palette.PackedRGBToExactIndex(ToPackedRGB());
        }

        /// <summary>
        /// Determine the closest color index for the RGB value that the Color object stores.
        /// </summary>
        /// <returns>Closest index</returns>
        public int GetClosestIndex()
        {
            if (source == SourceType.auto)
            {
                return fore_or_back ? 64 : 65;
            }
            if (source == SourceType.indexed && index != null)
            {
                return index.Value;
            }

            return workbook.StyleSheet.Palette.PackedRGBToClosestIndex(ToPackedRGB());
        }




        /// <summary>
        /// Set value as an index in saved or default palette
        /// </summary>
        /// <param name="index">New index</param>
        public void SetIndex(int index)
        {
            this.index = index;
            source = SourceType.indexed;
        }

        /// <summary>
        /// Red [0, 255]
        /// </summary>
        public int R { get; private set; }

        /// <summary>
        /// Green [0, 255]
        /// </summary>
        public int G { get; private set; }

        /// <summary>
        /// Blue [0, 255]
        /// </summary>
        public int B { get; private set; }

        #region Value type equality implementation
        /// <summary>
        /// Whether two Color objects are equal
        /// </summary>
        /// <param name="obj">Comparable object</param>
        /// <returns></returns>
        public override bool Equals(object obj)
        {
            ColorValue color = obj as ColorValue;
            if ((System.Object)color == null)
            {
                return false;
            }

            return Equals(color);
        }

        /// <summary>
        /// Whether two Color objects are equal
        /// </summary>
        /// <param name="color">Comparable object</param>
        /// <returns></returns>
        public bool Equals(ColorValue color)
        {
            // If parameter is null return false.
            if (color == null)
            {
                return false;
            }

            if (source != color.source || R != color.R || G != color.G || B != color.B)
            {
                return false;
            }
            return true;
        }

        /// <summary>
        /// Returns hash code
        /// </summary>
        /// <returns>Hash code</returns>
        public override int GetHashCode()
        {
            return R << 16 | G << 8 | B | ((int)source) << 24;
        }
        #endregion

        // Do not implement operator==. It would cause IProperty.IsNull to crash

        /// <summary>
        /// Forms a text hexadecimal representation of the color in RGB form. Recommended for debug purposes only.
        /// </summary>
        /// <returns>Textual form of RGB or empty string if the color is automatic</returns>
        public override string ToString()
        {
            if(source == SourceType.auto)
            {
                return "";
            }
            return ToPackedRGB().ToString("X06");
        }

        internal void SetAttribute(XmlElement color_tag)
        {
            switch (source)
            {
                case SourceType.auto:
                    color_tag.SetAttribute("auto", "true");
                    break;
                case SourceType.indexed:
                    color_tag.SetAttribute("indexed", index.Value.ToString());
                    break;
                case SourceType.rgb:
                    color_tag.SetAttribute("rgb", "FF" + ToString());
                    break;
                case SourceType.theme:
                    color_tag.SetAttribute("theme", theme_num.ToString());
                    color_tag.SetAttribute("tint", tint.ToString());
                    break;
            }
        }


        internal enum SourceType
        {
            rgb,
            theme,
            indexed,
            auto
        }

        private SourceType source;
        private int? index;
        private int theme_num;
        private double tint;
        private readonly bool fore_or_back;

        private Workbook workbook;
    }

    /// <summary>
    /// Automatic foreground Color
    /// </summary>
    internal class FgColor : ColorValue
    {
        internal FgColor(Workbook workbook)
            : base(true, workbook)
        {}
    }

    /// <summary>
    /// Automatic background Color
    /// </summary>
    internal class BgColor : ColorValue
    {
        internal BgColor(Workbook workbook)
            : base(false, workbook)
        {}
    }

}
