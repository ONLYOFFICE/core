using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Globalization;
using System.Diagnostics;

namespace ASCOfficeEWSEditor.Editor.Format.Colors
{
    /// <summary>
    /// This class is responsible for indexed colors table.
    /// </summary>
    public class Palette
    {
        internal Palette(XmlNode style_sheet_xml, Workbook workbook)
        {
            this.workbook = workbook;
            this.style_sheet_xml = style_sheet_xml;
        }

        /// <summary>
        /// Returns actual RGB info for the given index
        /// </summary>
        /// <param name="index">Index. [0;63]</param>
        /// <returns>Packed RGB</returns>
        public int IndexToPackedRGB(int index)
        {
            XmlNode indexed_colors_xml = Get_IndexedColors();
            if(indexed_colors_xml != null)
            {
                XmlElement rgbColor_tag = (XmlElement)indexed_colors_xml.SelectSingleNode("main:rgbColor[position() = " + (index + 1 ).ToString() + "]", workbook.NamespaceManager);
                if (rgbColor_tag != null)
                {
                    string argb = rgbColor_tag.GetAttribute("rgb");
                    int packed_rgb;
                    if (argb.Length != 8 || !int.TryParse(argb.Substring(2), NumberStyles.HexNumber, null, out packed_rgb))
                    {
                        Trace.TraceWarning("Wrong format of rgbColor/@rgb. Value \"{0}\" substituted with 0", new object[] { argb });
                        return 0;
                    }
                    return packed_rgb;
                }
            }
            return default_palette[index];
        }

        /// <summary>
        /// Searches for exact match of the passed RGB value in the color table. Returns null if no match found
        /// </summary>
        /// <param name="packed_rgb">Packed RGB</param>
        /// <returns>Index. [0;63] or null</returns>
        public int? PackedRGBToExactIndex(int packed_rgb)
        {
            XmlNode indexed_colors_xml = Get_IndexedColors();
            if (indexed_colors_xml != null)
            {
                XmlElement rgbColor_tag = (XmlElement)indexed_colors_xml.SelectSingleNode("main:rgbColor[substring(@rgb, 2) = " +
                    packed_rgb.ToString("X06") + "]", workbook.NamespaceManager);
                if (rgbColor_tag != null)
                {
                    return rgbColor_tag.SelectNodes("preceding-sibling::main:rgbColor", workbook.NamespaceManager).Count;
                }
                return null;
            }
            foreach (KeyValuePair<int, int> pair in default_palette)
            {
                if(pair.Value == packed_rgb)
                {
                    return pair.Key;
                }
            }
            return null;
        }

        /// <summary>
        /// Determine the closest color index for the passed RGB value in the color table.
        /// </summary>
        /// <param name="packed_rgb">Packed RGB</param>
        /// <returns>Index. [0;63]</returns>
        public int PackedRGBToClosestIndex(int packed_rgb)
        {
            Dictionary<int, int> distance_map = new Dictionary<int, int>();
            int index_counter = 0;
            XmlNode indexed_colors_xml = Get_IndexedColors();
            if (indexed_colors_xml != null)
            {
                foreach(XmlNode rgb_val_tag in indexed_colors_xml.SelectNodes("main:rgbColor/@rgb", workbook.NamespaceManager))
                {
                    string rgb_val = rgb_val_tag.InnerText.Substring(2);
                    distance_map[CalculateDistance(int.Parse(rgb_val, NumberStyles.HexNumber), packed_rgb)] = index_counter;
                    ++index_counter;
                }
            }
            else
            {
                foreach (KeyValuePair<int, int> pair in default_palette)
                {
                    distance_map[CalculateDistance(pair.Value, packed_rgb)] = index_counter;
                    ++index_counter;
                }
            }
            Dictionary<int, int>.Enumerator enumerator = distance_map.GetEnumerator();
            enumerator.MoveNext();
            return enumerator.Current.Value;
        }

        private int CalculateDistance(int packed_rgb1, int packed_rgb2)
        {
            int R1 = ((packed_rgb1 & 0x00ff0000) >> 16);
            int G1 = ((packed_rgb1 & 0x0000ff00) >> 8);
            int B1 =  (packed_rgb1 & 0x000000ff);
            int R2 = ((packed_rgb2 & 0x00ff0000) >> 16);
            int G2 = ((packed_rgb2 & 0x0000ff00) >> 8);
            int B2 =  (packed_rgb2 & 0x000000ff);
            return 30 * (R1 - R2) * (R1 - R2) + 59 * (G1 - G2) * (G1 - G2) + 11 * (B1 - B2) * (B1 - B2);
        }

        private XmlNode Get_IndexedColors()
        {
            return style_sheet_xml.SelectSingleNode("main:colors/main:indexedColors", workbook.NamespaceManager);
        }


        private Workbook workbook;
        private XmlNode style_sheet_xml;
        private static DefaultPalette default_palette = new DefaultPalette();
    }

    internal class DefaultPalette : IEnumerable<KeyValuePair<int, int>>
    {
        public DefaultPalette()
        {
            values = new Dictionary<int, int>();
            values[0] =  0x000000;
            values[1] =  0xFFFFFF;
            values[2] =  0xFF0000;
            values[3] =  0x00FF00;
            values[4] =  0x0000FF;
            values[5] =  0xFFFF00;
            values[6] =  0xFF00FF;
            values[7] =  0x00FFFF;
            values[8] =  0x000000;
            values[9] =  0xFFFFFF;
            values[10] = 0xFF0000;
            values[11] = 0x00FF00;
            values[12] = 0x0000FF;
            values[13] = 0xFFFF00;
            values[14] = 0xFF00FF;
            values[15] = 0x00FFFF;
            values[16] = 0x800000;
            values[17] = 0x008000;
            values[18] = 0x000080;
            values[19] = 0x808000;
            values[20] = 0x800080;
            values[21] = 0x008080;
            values[22] = 0xC0C0C0;
            values[23] = 0x808080;
            values[24] = 0x9999FF;
            values[25] = 0x993366;
            values[26] = 0xFFFFCC;
            values[27] = 0xCCFFFF;
            values[28] = 0x660066;
            values[29] = 0xFF8080;
            values[30] = 0x0066CC;
            values[31] = 0xCCCCFF;
            values[32] = 0x000080;
            values[33] = 0xFF00FF;
            values[34] = 0xFFFF00;
            values[35] = 0x00FFFF;
            values[36] = 0x800080;
            values[37] = 0x800000;
            values[38] = 0x008080;
            values[39] = 0x0000FF;
            values[40] = 0x00CCFF;
            values[41] = 0xCCFFFF;
            values[42] = 0xCCFFCC;
            values[43] = 0xFFFF99;
            values[44] = 0x99CCFF;
            values[45] = 0xFF99CC;
            values[46] = 0xCC99FF;
            values[47] = 0xFFCC99;
            values[48] = 0x3366FF;
            values[49] = 0x33CCCC;
            values[50] = 0x99CC00;
            values[51] = 0xFFCC00;
            values[52] = 0xFF9900;
            values[53] = 0xFF6600;
            values[54] = 0x666699;
            values[55] = 0x969696;
            values[56] = 0x003366;
            values[57] = 0x339966;
            values[58] = 0x003300;
            values[59] = 0x333300;
            values[60] = 0x993300;
            values[61] = 0x993366;
            values[62] = 0x333399;
            values[63] = 0x333333;
        }

        public int this[int index]
        {
            get
            {
                return values[index];
            }
        }

        #region IEnumerable Members

        public IEnumerator<KeyValuePair<int, int>> GetEnumerator()
        {
            return values.GetEnumerator();
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return values.GetEnumerator();
        }

        #endregion

        private Dictionary<int, int> values;
    }
}
