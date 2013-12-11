using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace ASCOfficeEWSEditor.Editor.Format.NumberFormat
{
    internal class NumberFormatEntity : IdStyleEntity
    {
        public NumberFormatEntity(PropertyId property)
            : base(property)
        {
        }

        public override string GetEntityIdAttribName()
        {
            return entity_id_attrib_name;
        }

        private const string entity_id_attrib_name = "numFmtId";
    }
}
