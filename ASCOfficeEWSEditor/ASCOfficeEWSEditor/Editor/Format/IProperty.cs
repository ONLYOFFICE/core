using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using ASCOfficeEWSEditor.XMLDataSpecific;

namespace ASCOfficeEWSEditor.Editor.Format
{
    public interface IProperty
    {
        bool IsNull();
        bool CollectFromEntity(XmlElement entity_tag);
        void SetToEntity(XmlElement entity_tag);
        bool CompareEntities(XmlElement ent1, XmlElement ent2);
    }

    internal abstract class Property<T> : IProperty
        where T : struct  
    {
        internal Property() {}
        internal Property(T init_value)
        {
            Value = init_value;
        }

        public bool CollectFromEntity(XmlElement entity_tag)
        {
            return CollectFromValue(ExtractValueFromEntity(entity_tag));
        }

        public bool CollectFromValue(T collected_value)
        {
            if (!Value.HasValue)
            {
                Value = collected_value;
                return true; // continue
            }
            else if (Value.Value.Equals(collected_value))
            {
                return true; // continue
            }
            else
            {
                Value = null;
                return false;
            }
        }

        public abstract void SetToEntity(XmlElement entity_tag);

        public bool IsNull()
        {
            return Value == null;
        }

        public bool CompareEntities(XmlElement ent1, XmlElement ent2)
        {
            return ExtractValueFromEntity(ent1).Equals(ExtractValueFromEntity(ent2));
        }

        public abstract T ExtractValueFromEntity(XmlElement entity_tag);

        public T? Value { get; private set; }
    }

    internal abstract class PropertyString : IProperty
    {
        internal PropertyString() { }
        internal PropertyString(string init_value)
        {
            Value = init_value;
        }

        public bool CollectFromEntity(XmlElement entity_tag)
        {
            string value_extracted = ExtractValueFromEntity(entity_tag);

            if (Value == null)
            {
                Value = value_extracted;
                return true; // continue
            }
            else if (Value == value_extracted)
            {
                return true; // continue
            }
            else
            {
                Value = null;
                return false;
            }
        }


        public void SetNull()
        {
            Value = null;
        }
        public bool IsNull()
        {
            return Value == null;
        }

        public bool CompareEntities(XmlElement ent1, XmlElement ent2)
        {
            return ExtractValueFromEntity(ent1) == ExtractValueFromEntity(ent2);
        }

        public abstract string ExtractValueFromEntity(XmlElement entity_tag);
        public abstract void SetToEntity(XmlElement entity_tag);

        public string Value { get; private set; }


    }

    internal abstract class PropertyBooleanTag : Property<bool>
    {
        public PropertyBooleanTag(bool init_value, Workbook workbook) : base(init_value)
        {
            this.workbook = workbook;
        }

        public PropertyBooleanTag(Workbook workbook)
        {
            this.workbook = workbook;
        }

        public override bool ExtractValueFromEntity(XmlElement font_tag)
        {
            XmlElement prop_tag = (XmlElement)font_tag.SelectSingleNode("main:" + TagName, workbook.NamespaceManager);
            if (prop_tag == null)
            {
                return false;
            }
            return XMLBoolean.ParseOrDefault(prop_tag.GetAttribute("val"), true);
        }

        public override void SetToEntity(XmlElement font_tag)
        {
            if (Value.HasValue)
            {
                XmlElement prop_tag = (XmlElement)font_tag.SelectSingleNode("main:" + TagName, workbook.NamespaceManager);

                if (Value.Value)
                {
                    if (prop_tag != null)
                    {
                        prop_tag.RemoveAttribute("val");
                    }
                    else
                    {
                        prop_tag = font_tag.OwnerDocument.CreateElement(TagName , workbook.NamespaceManager.LookupNamespace("main"));
                        font_tag.AppendChild(prop_tag);
                    }
                }
                else
                {
                    if (prop_tag != null)
                    {
                        font_tag.RemoveChild(prop_tag);
                    }
                }
            }
        }

        internal abstract string TagName { get; }

        private Workbook workbook;

    }

    internal abstract class PropertyBooleanAttrib : Property<bool>
    {
        public PropertyBooleanAttrib(bool init_value, bool default_value)
            : base(init_value)
        {
            this.default_value = default_value;
        }

        public PropertyBooleanAttrib(bool default_value)
        {
            this.default_value = default_value;
        }

        public override bool ExtractValueFromEntity(XmlElement entity_tag)
        {
            if (entity_tag == null)
            {
                return default_value;
            }
            return XMLBoolean.ParseOrDefault(entity_tag.GetAttribute(AttribName), default_value);
        }

        public override void SetToEntity(XmlElement entity_tag)
        {
            if (Value.HasValue)
            {
                if (Value.Value != default_value)
                {
                    entity_tag.SetAttribute(AttribName, XMLBoolean.ToString(Value.Value));
                }
                else
                {
                    entity_tag.RemoveAttribute(AttribName);
                }
            }
        }

        internal abstract string AttribName { get; }

        private bool default_value;

    }

    internal abstract class PropertyDoubleAttrib : Property<double>
    {
        public PropertyDoubleAttrib(double init_value, double default_value)
            : base(init_value)
        {
            this.default_value = default_value;
        }

        public PropertyDoubleAttrib(double default_value)
        {
            this.default_value = default_value;
        }

        public override double ExtractValueFromEntity(XmlElement entity_tag)
        {
            if (entity_tag == null)
            {
                return default_value;
            }
            return XMLDouble.ParseOrDefault(entity_tag.GetAttribute(AttribName), default_value);
        }

        public override void SetToEntity(XmlElement entity_tag)
        {
            if (Value.HasValue)
            {
                if (Value.Value != default_value)
                {
                    entity_tag.SetAttribute(AttribName, XMLDouble.ToString(Value.Value));
                }
                else
                {
                    entity_tag.RemoveAttribute(AttribName);
                }
            }
        }

        internal abstract string AttribName { get; }

        private double default_value;

    }

    internal abstract class PropertyIntAttrib : Property<int>
    {
        public PropertyIntAttrib(int init_value, int default_value)
            : base(init_value)
        {
            this.default_value = default_value;
        }

        public PropertyIntAttrib(int default_value)
        {
            this.default_value = default_value;
        }

        public override int ExtractValueFromEntity(XmlElement entity_tag)
        {
            if (entity_tag == null)
            {
                return default_value;
            }
            return XMLInt.ParseOrDefault(entity_tag.GetAttribute(AttribName), default_value);
        }

        public override void SetToEntity(XmlElement entity_tag)
        {
            if (Value.HasValue)
            {
                if (Value.Value != default_value)
                {
                    entity_tag.SetAttribute(AttribName, Value.Value.ToString());
                }
                else
                {
                    entity_tag.RemoveAttribute(AttribName);
                }
            }
        }

        internal abstract string AttribName { get; }

        private int default_value;

    }

    internal abstract class PropertyColor : IProperty
    {
        internal PropertyColor() { }
        internal PropertyColor(Colors.ColorValue init_value)
        {
            Value = init_value;
        }

        public bool CollectFromEntity(XmlElement entity_tag)
        {
            Colors.ColorValue value_extracted = ExtractValueFromEntity(entity_tag);

            if (Value == null)
            {
                Value = value_extracted;
                return true; // continue
            }
            else if (Value.Equals(value_extracted))
            {
                return true; // continue
            }
            else
            {
                Value = null;
                return false;
            }
        }


        public bool IsNull()
        {
            return Value == null;
        }

        public bool CompareEntities(XmlElement ent1, XmlElement ent2)
        {
            return ExtractValueFromEntity(ent1).Equals(ExtractValueFromEntity(ent2));
        }

        public abstract Colors.ColorValue ExtractValueFromEntity(XmlElement entity_tag);
        public abstract void SetToEntity(XmlElement entity_tag);

        public Colors.ColorValue Value { get; set; }
    }

    internal abstract class PropertyBridgeTag : IProperty 
    {
        public PropertyBridgeTag(IProperty included_property, Workbook workbook)
        {
            this.included_property = included_property;
            this.workbook = workbook;
        }

        public bool IsNull()
        {
            return included_property.IsNull() || tag_exists == null;
        }

        public bool CollectFromEntity(XmlElement entity_tag)
        {
            XmlElement bridge_tag = (XmlElement)entity_tag.SelectSingleNode("main:" + TagName, workbook.NamespaceManager);
            bool next_tag_exists = bridge_tag != null;

            if (!tag_exists.HasValue)
            {
                tag_exists = next_tag_exists;
            }
            else if (tag_exists.Value != next_tag_exists)
            {
                tag_exists = null;
                return false;
            }

            if (!next_tag_exists)
            {
                return true; // continue because all entities could have no bridge tag.
            }
            return included_property.CollectFromEntity(bridge_tag);
        }

        public virtual void SetToEntity(XmlElement entity_tag)
        {
            XmlElement border_part_tag = (XmlElement)entity_tag.SelectSingleNode("main:" + TagName, workbook.NamespaceManager);
            if(border_part_tag == null)
            {
                border_part_tag = entity_tag.OwnerDocument.CreateElement(TagName, workbook.NamespaceManager.LookupNamespace("main"));
                entity_tag.AppendChild(border_part_tag);
            }
            included_property.SetToEntity(border_part_tag);
        }

        public bool CompareEntities(XmlElement ent1, XmlElement ent2)
        {
            XmlElement bridge_tag1 = (XmlElement)ent1.SelectSingleNode("main:" + TagName, workbook.NamespaceManager);
            XmlElement bridge_tag2 = (XmlElement)ent2.SelectSingleNode("main:" + TagName, workbook.NamespaceManager);
            if (bridge_tag1 == null && bridge_tag1 == null)
            {
                return true;
            }
            if (bridge_tag1 == null || bridge_tag2 == null)
            {
                return false;
            }
            return included_property.CompareEntities(bridge_tag1, bridge_tag2);
        }

        internal abstract string TagName { get; }

        private IProperty included_property;
        private Workbook workbook;
        private bool? tag_exists;
    }

    internal class PropertyId : Property<int>
    {
        public PropertyId(int init_value)
            : base(init_value)
        {
        }

        public PropertyId()
        {
        }

        public override int ExtractValueFromEntity(XmlElement entity_tag)
        {
            throw new NotImplementedException();
        }

        public override void SetToEntity(XmlElement entity_tag)
        {
            throw new NotImplementedException();
        }

    }
}
