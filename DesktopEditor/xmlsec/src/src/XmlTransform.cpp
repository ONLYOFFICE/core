#include "./XmlTransform.h"

IXmlTransform* IXmlTransform::GetFromType(const std::string& alg)
{
    if (true)
    {
        CXmlTransformRelationship* transform = new CXmlTransformRelationship();
        if (transform->m_algorithm == alg)
            return transform;
        RELEASEOBJECT(transform);
    }
    if (true)
    {
        CXmlTransformC14N* transform = new CXmlTransformC14N();
        if (transform->CheckC14NTransform(alg))
            return transform;
        RELEASEOBJECT(transform);
    }

    return NULL;
}
