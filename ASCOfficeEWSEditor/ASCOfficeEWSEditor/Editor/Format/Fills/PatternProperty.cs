using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using AVSOfficeEWSEditor.XMLDataSpecific;

namespace AVSOfficeEWSEditor.Editor.Format.Fills
{
    internal class PatternProperty : Property<PatternType>
    {
        public PatternProperty(PatternType init_value)
            : base(init_value)
        {
        }

        public PatternProperty()
        {
        }

        public override PatternType ExtractValueFromEntity(XmlElement pattern_fill_tag)
        {
            if (pattern_fill_tag == null)
            {
                return PatternType.none;
            }
            string pattern_val = pattern_fill_tag.GetAttribute("patternType");
            switch (pattern_val)
            {
                case "darkDown":
                    return PatternType.darkDown;
                case "darkGray":
                    return PatternType.darkGray;
                case "darkGrid":
                    return PatternType.darkGrid;
                case "darkHorizontal":
                    return PatternType.darkHorizontal;
                case "darkTrellis":
                    return PatternType.darkTrellis;
                case "darkUp":
                    return PatternType.darkUp;
                case "darkVertical":
                    return PatternType.darkVertical;
                case "gray0625":
                    return PatternType.gray0625;
                case "gray125":
                    return PatternType.gray125;
                case "lightDown":
                    return PatternType.lightDown;
                case "lightGray":
                    return PatternType.lightGray;
                case "lightGrid":
                    return PatternType.lightGrid;
                case "lightHorizontal":
                    return PatternType.lightHorizontal;
                case "lightTrellis":
                    return PatternType.lightTrellis;
                case "lightUp":
                    return PatternType.lightUp;
                case "lightVertical":
                    return PatternType.lightVertical;
                case "mediumGray":
                    return PatternType.mediumGray;
                case "solid":
                    return PatternType.solid;
                case "none":
                default:
                    return PatternType.none;
            }
        }

        public override void SetToEntity(XmlElement alignment_tag)
        {
            if (Value.HasValue)
            {
                if (Value.Value != PatternType.none)
                {
                    alignment_tag.SetAttribute("patternType", Value.Value.ToString());
                }
                else
                {
                    alignment_tag.RemoveAttribute("patternType");
                }
            }
        }
    }

    /// <summary>
    /// Pattern Type
    /// </summary>
    public enum PatternType
    {
        /// <summary>
        /// Dark down
        /// </summary>
        darkDown,
        /// <summary>
        /// Dark gray
        /// </summary>
        darkGray,
        /// <summary>
        /// Dark grid
        /// </summary>
        darkGrid,
        /// <summary>
        /// Dark horizontal
        /// </summary>
        darkHorizontal,
        /// <summary>
        /// Dark trellis
        /// </summary>
        darkTrellis,
        /// <summary>
        /// Dark up
        /// </summary>
        darkUp,
        /// <summary>
        /// Dark vertical
        /// </summary>
        darkVertical,
        /// <summary>
        /// Grayscale of 0.0625 (1/16) value
        /// </summary>
        gray0625,
        /// <summary>
        /// Grayscale of 0.125 (1/8) value
        /// </summary>
        gray125,
        /// <summary>
        /// Light down
        /// </summary>
        lightDown,
        /// <summary>
        /// Light gray
        /// </summary>
        lightGray,
        /// <summary>
        /// Light grid
        /// </summary>
        lightGrid,
        /// <summary>
        /// Light horizontal
        /// </summary>
        lightHorizontal,
        /// <summary>
        /// Light trellis
        /// </summary>
        lightTrellis,
        /// <summary>
        /// Light up
        /// </summary>
        lightUp,
        /// <summary>
        /// Light vertical
        /// </summary>
        lightVertical,
        /// <summary>
        /// Medium gray
        /// </summary>
        mediumGray,
        /// <summary>
        /// No fill
        /// </summary>
        none,
        /// <summary>
        /// Solid fill (only FgColor is rendered)
        /// </summary>
        solid
    }

}
