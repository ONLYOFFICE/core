#ifndef CELEMENT_H
#define CELEMENT_H

#include <vector>
#include <map>
#include <iostream>

class CElement
{   
    std::wstring m_sNameElement;
    std::wstring m_sNamespace;
    std::vector <std::pair<std::wstring, std::wstring>> m_arArguments;
    std::wstring m_sValue;
    std::vector <CElement*> m_arChildren;

    bool m_bEmpty;
    bool m_bNamespaceArguments;
public:
    CElement(std::wstring sNameElement, std::wstring sValue = L"", std::wstring sNamespace = L"", bool bNamespaceArguments = true);
    virtual ~CElement();

    void setValue(std::wstring sValue);
    virtual void SetDefoult();
    bool IsEmpty();

    int GetCountChildrens();
    int GetCountArguments();

    CElement* GetChildren(const int nIndex);
    std::pair<std::wstring, std::wstring>* GetArgument(const int nIndex);

    void AddArgument(const std::wstring sNameArgument,const std::wstring sValueArgument);
    void AddChildren(CElement* oChildren, int nIndex = -1);

    void DeleteChildren(const std::wstring sNameChildren);
    void DeleteChildren(CElement* oChildren);
    void DeleteArgument(const std::wstring sNameArgument);
    void Clear(const bool Children =      true,
               const bool NameElement =   true,
               const bool Value =         true,
               const bool Arguments =     true);
    void EditBoolNamespace(bool bNamespace);


    std::wstring GetArguments();
    std::wstring GetName();
    std::wstring GetValue();
    std::wstring GetElementXML();

private:
    std::map <std::wstring,bool> m_arClosingTag = {{ L"document",                           true },
                                                   { L"body",                               true },
                                                   { L"p",                                  true },
                                                   { L"sectPr",                             true },
                                                   { L"pPr",                                true },
                                                   { L"rPr",                                true },
                                                   { L"t",                                  true },
                                                   { L"r",                                  true },
                                                   { L"Types",                              true },
                                                   { L"font",                               true },
                                                   { L"fonts",                              true },
                                                   { L"settings",                           true },
                                                   { L"compat",                             true },
                                                   { L"rsids",                              true },
                                                   { L"mathPr",                             true },
                                                   { L"shapeDefaults",                      true },
                                                   { L"shapelayout",                        true },
                                                   { L"webSettings",                        true },
                                                   { L"styles",                             true },
                                                   { L"tblPr",                              true },
                                                   { L"tblCellMar",                         true },
                                                   { L"style",                              true },
                                                   { L"docDefaults",                        true },
                                                   { L"rPrDefault",                         true },
                                                   { L"rPr",                                true },
                                                   { L"pPrDefault",                         true },
                                                   { L"pPr",                                true },
                                                   { L"Relationships",                      true },
                                                   { L"Properties",                         true },
                                                   { L"Template",                           true },
                                                   { L"TotalTime",                          true },
                                                   { L"Pages",                              true },
                                                   { L"Words",                              true },
                                                   { L"Application",                        true },
                                                   { L"DocSecurity",                        true },
                                                   { L"Lines",                              true },
                                                   { L"Paragraphs",                         true },
                                                   { L"ScaleCrop",                          true },
                                                   { L"Company",                            true },
                                                   { L"LinksUpToDate",                      true },
                                                   { L"CharactersWithSpaces",               true },
                                                   { L"SharedDoc",                          true },
                                                   { L"HyperlinksChanged",                  true },
                                                   { L"AppVersion",                         true },
                                                   { L"coreProperties",                     true },
                                                   { L"title",                              true },
                                                   { L"subject",                            true },
                                                   { L"creator",                            true },
                                                   { L"keywords",                           true },
                                                   { L"description",                        true },
                                                   { L"lastModifiedBy",                     true },
                                                   { L"revision",                           true },
                                                   { L"latentStyles",                       true },
                                                   { L"Characters",                         true },
                                                   { L"created",                            true },
                                                   { L"modified",                           true },
                                                   { L"hyperlink",                          true },
                                                   { L"sdt",                                true },
                                                   { L"sdtPr",                              true },
                                                   { L"docPartObj",                         true },
                                                   { L"sdtEndPr",                           true },
                                                   { L"sdtContent",                         true },
                                                   { L"tabs",                               true },
                                                   { L"lang",                               false},
                                                   { L"bookmarkStart",                      false},
                                                   { L"bookmarkEnd",                        false},
                                                   { L"pgSz",                               false},
                                                   { L"pgMar",                              false},
                                                   { L"cols",                               false},
                                                   { L"docGrid",                            false},
                                                   { L"Default",                            false},
                                                   { L"Override",                           false},
                                                   { L"altName",                            false},
                                                   { L"panose1",                            false},
                                                   { L"charset",                            false},
                                                   { L"family",                             false},
                                                   { L"pitch",                              false},
                                                   { L"sig",                                false},
                                                   { L"embedRegular",                       false},
                                                   { L"zoom",                               false},
                                                   { L"defaultTabStop",                     false},
                                                   { L"drawingGridHorizontalSpacing",       false},
                                                   { L"drawingGridVerticalSpacing",         false},
                                                   { L"displayHorizontalDrawingGridEvery",  false},
                                                   { L"characterSpacingControl",            false},
                                                   { L"useFELayout",                        false},
                                                   { L"compatSetting",                      false},
                                                   { L"rsidRoot",                           false},
                                                   { L"rsid",                               false},
                                                   { L"mathFont",                           false},
                                                   { L"brkBin",                             false},
                                                   { L"brkBinSub",                          false},
                                                   { L"smallFrac",                          false},
                                                   { L"dispDef",                            false},
                                                   { L"lMargin",                            false},
                                                   { L"rMargin",                            false},
                                                   { L"defJc",                              false},
                                                   { L"wrapIndent",                         false},
                                                   { L"intLim",                             false},
                                                   { L"naryLim",                            false},
                                                   { L"themeFontLang",                      false},
                                                   { L"clrSchemeMapping",                   false},
                                                   { L"shapedefaults",                      false},
                                                   { L"idmap",                              false},
                                                   { L"decimalSymbol",                      false},
                                                   { L"listSeparator",                      false},
                                                   { L"w14:docId",                          false},
                                                   { L"w15:chartTrackingRefBased",          false},
                                                   { L"w15:docId",                          false},
                                                   { L"alignBordersAndEdges",               false},
                                                   { L"optimizeForBrowser",                 false},
                                                   { L"allowPNG",                           false},
                                                   { L"name",                               false},
                                                   { L"qFormat",                            false},
                                                   { L"uiPriority",                         false},
                                                   { L"semiHidden",                         false},
                                                   { L"unhideWhenUsed",                     false},
                                                   { L"top",                                false},
                                                   { L"left",                               false},
                                                   { L"bottom",                             false},
                                                   { L"right",                              false},
                                                   { L"rFonts",                             false},
                                                   { L"sz",                                 false},
                                                   { L"szCs",                               false},
                                                   { L"spacing",                            false},
                                                   { L"Relationship",                       false},
                                                   { L"lsdException",                       false},
                                                   { L"tblInd",                             false}};
    std::vector<std::wstring> m_arNamespaceBan = {L"cp", L"dcterms"};
};

#endif // CELEMENT_H
