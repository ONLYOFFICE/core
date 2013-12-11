using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format.Borders
{
    internal class BorderProperty : PropertyBridgeTag 
    {
        public BorderProperty(BorderPosition border_position, IProperty included_property, Workbook workbook) : base(included_property, workbook)
        {
            switch (border_position)
            {
                case BorderPosition.bottom:
                    tag_name = "bottom";
                    break;
                case BorderPosition.diagonal:
                    tag_name = "diagonal";
                    break;
                case BorderPosition.left:
                    tag_name = "left";
                    break;
                case BorderPosition.right:
                    tag_name = "right";
                    break;
                case BorderPosition.top:
                    tag_name = "top";
                    break;
            }
        }

        internal override string TagName { get { return tag_name; } }
        private string tag_name;
    }

    /// <summary>
    /// This type identifies the place where the border is locates regarding the cell
    /// </summary>
    internal enum BorderPosition
    {
        /// <summary>
        /// Left border
        /// </summary>
        left,
        /// <summary>
        /// Right border
        /// </summary>
        right,
        /// <summary>
        /// Top border
        /// </summary>
        top,
        /// <summary>
        /// Bottom border
        /// </summary>
        bottom,
        /// <summary>
        /// Diagonal borders
        /// </summary>
        diagonal
    }
}
