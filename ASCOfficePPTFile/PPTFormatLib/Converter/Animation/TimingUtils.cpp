#include "TimingUtils.h"
#include "../../Reader/ExtXmlUtils.hpp"
#include "../../Enums/enums.h"


PPT::Intermediate::ShapeAnim PPT::Intermediate::ParseExisting5Level_CTn(const PPTX::Logic::CTn &oCTn)
{
    ShapeAnim spAn;
    spAn.presetID = oCTn.presetID.get_value_or(-1);
    std::wstring xml = oCTn.toXML();
    auto arrSpids = XmlUtils::FindAttrValuesInt(xml, L"spid");
    if (arrSpids.size())
        spAn.spid = arrSpids[0];

    return spAn;
}

PPT::Intermediate::ShapeAnim PPT::Intermediate::ParseExisting5Level_ETNC(const CRecordExtTimeNodeContainer *pETNC)
{
    if (pETNC == nullptr)
        return {};

    ShapeAnim spAn;
    for (const auto& child : pETNC->m_arrRgExtTimeNodeChildren)
    {
        if (child->m_haveSetBehavior &&
                child->m_pTimeSetBehavior &&
                child->m_pTimeSetBehavior->m_oBehavior.m_oClientVisualElement.m_bVisualShapeAtom &&
                child->m_pTimeSetBehavior->m_oBehavior.m_oClientVisualElement.m_oVisualShapeAtom.m_RefType == TL_ET_ShapeType)
        {
            spAn.spid = child->m_pTimeSetBehavior->m_oBehavior.m_oClientVisualElement.m_oVisualShapeAtom.m_nObjectIdRef;
            break;
        }
    }

    if (pETNC->m_haveTimePropertyList && pETNC->m_pTimePropertyList != nullptr)
    {
        for (const auto prop : pETNC->m_pTimePropertyList->m_arrElements)
        {
            if (prop->m_oHeader.RecInstance == TL_TPID_EffectID)
            {
                spAn.presetID = dynamic_cast<CRecordTimeEffectID*>(prop)->m_Value;
                break;
            }
        }
    }
    return spAn;
}

bool PPT::Intermediate::operator==(const ShapeAnim &s1, const ShapeAnim &s2)
{
    if (s1.IsValid() == false)
        return false;

    return s1.spid == s2.spid && (s1.presetID == s2.presetID || s1.presetID == 1 || s2.presetID == 1);  /// 1 - Appear
}
