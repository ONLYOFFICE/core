using System;
using System.IO;
using System.Collections;
using System.Text;
using System.Reflection;
using System.Xml;

namespace CleverAge.OdfConverter.OdfConverterLib
{
    /// <summary>
    /// An XmlUrlResolver for embedded resources.
    /// Looks for resources in the specified assemby first, then in the 'common' assembly.
    /// </summary>
    public class EmbeddedResourceResolver : XmlUrlResolver
    {
        private const string ODFToOOX_LOCATION = ".resources.odf2oox.";
        private const string OOXToODF_LOCATION = ".resources.oox2odf.";
        public const string ASSEMBLY_URI_SCHEME = "assembly";
        public const string ASSEMBLY_URI_HOST = "localhost";

        private Assembly appAssembly;
        private Assembly defaultAssembly;
        private string appNamespace;
        private string defaultNamespace;
        private bool isDirectTransform;
        private string resourceLocation;

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="assembly">The assembly embedding resources</param>
        /// <param name="prefix">The prefix used to locate resources within the assembly</param>
        /// <param name="isDirectTransform">Is the direction of conversion direct</param>
        public EmbeddedResourceResolver(Assembly appAssembly, string appNamespace, bool isDirectTransform)
            :
            this(appAssembly, appNamespace)
        {
            this.isDirectTransform = isDirectTransform;
        }

        public EmbeddedResourceResolver(Assembly appAssembly, string appNamespace, string resourceLocation, bool isDirectTransform)
            :
            this(appAssembly, appNamespace, isDirectTransform)
        {
            this.resourceLocation = resourceLocation;
        }

        public EmbeddedResourceResolver(Assembly appAssembly, string appNamespace)
        {
            this.appAssembly = appAssembly;
            //this.appNamespace = appNamespace;
            //this.defaultNamespace = this.GetType().Namespace;
            this.appNamespace = "ASCOfficeEWSEditor.Editor.ODSConverter.SpreadsheetConverter";
            this.defaultNamespace = "ASCOfficeEWSEditor.Editor.ODSConverter.OdfConverterLib";
            this.defaultAssembly = Assembly.GetExecutingAssembly();
        }

        public bool IsDirectTransform
        {
            set { this.isDirectTransform = value; }
        }

        public override Uri ResolveUri(Uri baseUri, String relativeUri)
        {
            // Console.WriteLine("ResolveUri: baseUri="+baseUri+" relativeUri="+relativeUri);
            if (baseUri == null && !relativeUri.Contains("://"))
            {
                return new Uri(ASSEMBLY_URI_SCHEME + "://" + ASSEMBLY_URI_HOST + "/" + relativeUri);
            }
            else if (relativeUri.Contains("DTD"))
            {
                return new Uri(ASSEMBLY_URI_SCHEME + "://" + ASSEMBLY_URI_HOST + "/dummy.dtd");
            }
            else
            {
                return base.ResolveUri(baseUri, relativeUri);
            }
        }

        public override object GetEntity(Uri absoluteUri, string role, Type ofObjectToReturn)
        {
            if (ASSEMBLY_URI_SCHEME.Equals(absoluteUri.Scheme))
            {
                string location = null;
                if (this.resourceLocation == null)
                {
                    location = this.isDirectTransform ? ODFToOOX_LOCATION : OOXToODF_LOCATION;
                }
                else
                {
                    location = this.resourceLocation;
                }
                string resource = location + absoluteUri.OriginalString.Remove(0, ASSEMBLY_URI_SCHEME.Length + ASSEMBLY_URI_HOST.Length + 4).Replace("/", ".");
                Stream stream = this.appAssembly.GetManifestResourceStream(this.appNamespace + resource);
                if (stream != null)
                {
                    return stream;
                }
                // fallback to the shared embedded resources
                else
                {
                    stream = this.defaultAssembly.GetManifestResourceStream(this.defaultNamespace + resource);
                    if (stream != null)
                    {
                        return stream;
                    }
                }
            }

            // use default behaviour instead
            return base.GetEntity(absoluteUri, role, ofObjectToReturn);
        }

        public Stream GetInnerStream(String fileName)
        {
            string location = null;
            if (this.resourceLocation == null)
            {
                location = this.isDirectTransform ? ODFToOOX_LOCATION : OOXToODF_LOCATION;
            }
            else
            {
                location = this.resourceLocation;
            }

            string path = this.appNamespace + location;
            return this.appAssembly.GetManifestResourceStream(path + fileName);
        }


        public static Stream GetSharedResource(string filename)
        {
            Assembly a = Assembly.GetExecutingAssembly();
            return a.GetManifestResourceStream(a.GetName() + ".resources." + filename);
        }
    }

}
