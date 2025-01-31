/*
 * (c) Copyright Ascensio System SIA 2010-2023
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "PdfEditor.h"

#include "../DesktopEditor/common/Path.h"

#include "SrcReader/Adaptors.h"
#include "lib/xpdf/PDFDoc.h"
#include "lib/xpdf/AcroForm.h"
#include "lib/xpdf/TextString.h"
#include "lib/xpdf/Lexer.h"
#include "lib/xpdf/Parser.h"

#include "SrcWriter/Catalog.h"
#include "SrcWriter/EncryptDictionary.h"
#include "SrcWriter/Info.h"
#include "SrcWriter/ResourcesDictionary.h"
#include "SrcWriter/Streams.h"

#define AddToObject(oVal)\
{\
	if (pObj->GetType() == PdfWriter::object_type_DICT)\
	((PdfWriter::CDictObject*)pObj)->Add(sKey, oVal);\
	else if (pObj->GetType() == PdfWriter::object_type_ARRAY)\
	((PdfWriter::CArrayObject*)pObj)->Add(oVal);\
}

void DictToCDictObject(Object* obj, PdfWriter::CObjectBase* pObj, bool bBinary, const std::string& sKey, bool bUnicode = false)
{
	Object oTemp;
	switch (obj->getType())
	{
	case objBool:
	{
		bool b = obj->getBool();
		AddToObject(b)
		break;
	}
	case objInt:
	{
		AddToObject(obj->getInt())
		break;
	}
	case objReal:
	{
		AddToObject(obj->getReal())
		break;
	}
	case objString:
	{
		if (bBinary)
		{
			GString* str = obj->getString();
			int nLength = str->getLength();
			BYTE* arrId = new BYTE[nLength];
			for (int nIndex = 0; nIndex < nLength; ++nIndex)
				arrId[nIndex] = str->getChar(nIndex);
			AddToObject(new PdfWriter::CBinaryObject(arrId, nLength));
			RELEASEARRAYOBJECTS(arrId);
		}
		else
		{
			TextString* s = new TextString(obj->getString());
			std::string sValue = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
			AddToObject(new PdfWriter::CStringObject(sValue.c_str(), bUnicode))
			delete s;
		}
		break;
	}
	case objName:
	{
		AddToObject(obj->getName())
		break;
	}
	case objNull:
	{
		AddToObject(new PdfWriter::CNullObject())
		break;
	}
	case objArray:
	{
		PdfWriter::CArrayObject* pArray = new PdfWriter::CArrayObject();
		AddToObject(pArray)
		for (int nIndex = 0; nIndex < obj->arrayGetLength(); ++nIndex)
		{
			obj->arrayGetNF(nIndex, &oTemp);
			DictToCDictObject(&oTemp, pArray, bBinary, "", bUnicode);
			oTemp.free();
		}
		break;
	}
	case objDict:
	{
		PdfWriter::CDictObject* pDict = new PdfWriter::CDictObject();
		AddToObject(pDict);
		for (int nIndex = 0; nIndex < obj->dictGetLength(); ++nIndex)
		{
			char* chKey = obj->dictGetKey(nIndex);
			obj->dictGetValNF(nIndex, &oTemp);
			DictToCDictObject(&oTemp, pDict, bBinary, chKey, bUnicode);
			oTemp.free();
		}
		break;
	}
	case objRef:
	{
		PdfWriter::CObjectBase* pBase = new PdfWriter::CObjectBase();
		pBase->SetRef(obj->getRefNum(), obj->getRefGen());
		AddToObject(new PdfWriter::CProxyObject(pBase, true))
		break;
	}
	case objNone:
	{
		AddToObject("None")
		break;
	}
	case objStream:
	case objCmd:
	case objError:
	case objEOF:
		break;
	}
}
PdfWriter::CDictObject* GetWidgetParent(PDFDoc* pdfDoc, PdfWriter::CDocument* pDoc, Object* pParentRef)
{
	if (!pParentRef || !pParentRef->isRef() || !pdfDoc)
		return NULL;
	PdfWriter::CDictObject* pParent = pDoc->GetParent(pParentRef->getRefNum());
	if (pParent)
		return pParent;

	Object oParent;
	if (!pParentRef->fetch(pdfDoc->getXRef(), &oParent)->isDict())
	{
		oParent.free();
		return pParent;
	}

	PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, pParentRef->getRefNum());
	pParent = new PdfWriter::CDictObject();
	pXref->Add(pParent, pParentRef->getRefGen());
	if (!pDoc->EditParent(pXref, pParent, pParentRef->getRefNum()))
	{
		RELEASEOBJECT(pXref);
		oParent.free();
		return NULL;
	}

	for (int i = 0; i < oParent.dictGetLength(); ++i)
	{
		char* chKey = oParent.dictGetKey(i);
		if (strcmp("Parent", chKey) == 0)
		{
			Object oParentRef;
			oParent.dictGetValNF(i, &oParentRef);
			PdfWriter::CDictObject* pParent2 = GetWidgetParent(pdfDoc, pDoc, &oParentRef);
			if (pParent2)
			{
				pParent->Add("Parent", pParent2);
				oParentRef.free();
				continue;
			}
			oParentRef.free();
		}
		Object oTemp;
		oParent.dictGetValNF(i, &oTemp);
		DictToCDictObject(&oTemp, pParent, false, chKey);
		oTemp.free();
	}

	oParent.free();
	return pParent;
}
HRESULT _ChangePassword(const std::wstring& wsPath, const std::wstring& wsPassword, CPdfReader* _pReader, CPdfWriter* _pWriter)
{
	if (!_pReader || !_pWriter)
		return S_FALSE;
	PDFDoc* pPDFDocument = _pReader->GetPDFDocument();
	if (!pPDFDocument)
		return S_FALSE;
	XRef* xref = pPDFDocument->getXRef();
	if (!xref)
		return S_FALSE;
	Object* trailerDict = xref->getTrailerDict();
	if (!trailerDict)
		return S_FALSE;

	PdfWriter::CDocument* pDoc = _pWriter->GetDocument();
	PdfWriter::CXref* pXref   = new PdfWriter::CXref(pDoc, 0);
	PdfWriter::CXref* m_pXref = new PdfWriter::CXref(pDoc, xref->getNumObjects()); // Для новых объектов
	if (!xref || !pDoc || !pXref || !m_pXref)
	{
		RELEASEOBJECT(pXref);
		RELEASEOBJECT(m_pXref);
		return S_FALSE;
	}
	pXref->SetPrev(m_pXref);

	for (int i = 0; i < xref->getSize(); ++i)
	{
		XRefEntry* pEntry = xref->getEntry(i);
		if (pEntry->type == xrefEntryFree)
			continue;

		if (i != pXref->GetSizeXRef())
		{
			PdfWriter::CXref* pXref2 = new PdfWriter::CXref(pDoc, i);
			pXref2->SetPrev(pXref);
			pXref = pXref2;
		}

		Object oTemp;
		xref->fetch(i, pEntry->gen, &oTemp);
		PdfWriter::CObjectBase* pObj = NULL;

		switch (oTemp.getType())
		{
		case objBool:
		{
			pObj = new PdfWriter::CBoolObject(oTemp.getBool());
			break;
		}
		case objInt:
		{
			pObj = new PdfWriter::CNumberObject(oTemp.getInt());
			break;
		}
		case objReal:
		{
			pObj = new PdfWriter::CRealObject(oTemp.getReal());
			break;
		}
		case objString:
		{
			TextString* s = new TextString(oTemp.getString());
			std::string sValue = NSStringExt::CConverter::GetUtf8FromUTF32(s->getUnicode(), s->getLength());
			pObj = new PdfWriter::CStringObject(sValue.c_str());
			delete s;
			break;
		}
		case objName:
		{
			pObj = new PdfWriter::CNameObject(oTemp.getName());
			break;
		}
		case objNull:
		{
			pObj = new PdfWriter::CNullObject();
			break;
		}
		case objArray:
		{
			pObj = new PdfWriter::CArrayObject();

			for (int nIndex = 0; nIndex < oTemp.arrayGetLength(); ++nIndex)
			{
				Object oT;
				oTemp.arrayGetNF(nIndex, &oT);
				DictToCDictObject(&oT, pObj, false, "");
				oT.free();
			}
			break;
		}
		case objDict:
		{
			pObj = new PdfWriter::CDictObject();

			for (int nIndex = 0; nIndex < oTemp.dictGetLength(); ++nIndex)
			{
				Object oT;
				char* chKey = oTemp.dictGetKey(nIndex);
				oTemp.dictGetValNF(nIndex, &oT);
				DictToCDictObject(&oT, pObj, false, chKey);
				oT.free();
			}
			break;
		}
		case objRef:
		{
			PdfWriter::CObjectBase* pBase = new PdfWriter::CObjectBase();
			pBase->SetRef(oTemp.getRefNum(), oTemp.getRefGen());
			pObj = new PdfWriter::CProxyObject(pBase, true);
			break;
		}
		case objStream:
		{
			Dict* pDict = oTemp.streamGetDict();
			Object oObjStm;
			if (pDict->lookup("Type", &oObjStm)->isName("ObjStm"))
			{
				oObjStm.free();
				break;
			}
			oObjStm.free();

			PdfWriter::CDictObject* pDObj = new PdfWriter::CDictObject();
			pObj = pDObj;

			int nLength = 0;
			for (int nIndex = 0; nIndex < pDict->getLength(); ++nIndex)
			{
				Object oT;
				char* chKey = pDict->getKey(nIndex);
				if (strcmp("Length", chKey) == 0)
				{
					Object oLength;
					nLength = pDict->getVal(nIndex, &oLength)->isInt() ? oLength.getInt() : 0;
					oLength.free();
					continue;
				}
				pDict->getValNF(nIndex, &oT);
				DictToCDictObject(&oT, pObj, false, chKey);
				oT.free();
			}

			PdfWriter::CStream* pStream = new PdfWriter::CMemoryStream();
			pDObj->SetStream(m_pXref, pStream, false);

			Stream* pImage = oTemp.getStream()->getUndecodedStream();
			pImage->reset();
			for (int nI = 0; nI < nLength; ++nI)
				pStream->WriteChar(pImage->getChar());
			break;
		}
		case objNone:
		case objCmd:
		case objError:
		case objEOF:
		default:
			break;
		}
		oTemp.free();

		if (pObj)
			pXref->Add(pObj);
	}

	PdfWriter::CDictObject* pTrailer = pXref->GetTrailer();
	for (int nIndex = 0; nIndex < trailerDict->dictGetLength(); ++nIndex)
	{
		Object oTemp;
		char* chKey = trailerDict->dictGetKey(nIndex);
		if (strcmp("Root", chKey) == 0 || strcmp("Info", chKey) == 0)
		{
			trailerDict->dictGetValNF(nIndex, &oTemp);
			DictToCDictObject(&oTemp, pTrailer, true, chKey);
		}
		oTemp.free();
	}

	bool bRes = pDoc->SaveNewWithPassword(pXref, m_pXref, wsPath, wsPassword, wsPassword, pTrailer);

	RELEASEOBJECT(pXref);

	return bRes ? S_OK : S_FALSE;
}
void GetCTM(XRef* pXref, Object* oPage, double* dCTM)
{
	if (!oPage || !oPage->isDict())
		return;

	Object oContents;
	if (!oPage->dictLookup("Contents", &oContents) || (!oContents.isArray() && !oContents.isStream()))
	{
		oContents.free();
		return;
	}

	Parser* parser = new Parser(pXref, new Lexer(pXref, &oContents), gFalse);

	int nNumArgs = 0;
	Object oObj;
	Object pArgs[maxArgs];

	parser->getObj(&oObj);
	while (!oObj.isEOF())
	{
		if (oObj.isCmd())
		{
			if (oObj.isCmd("q"))
			{
				Object obj;
				parser->getObj(&obj);
				while (!obj.isEOF() && !obj.isCmd("Q"))
				{
					obj.free();
					parser->getObj(&obj);
				}
				obj.free();
			}
			else if (oObj.isCmd("cm") && nNumArgs > 5)
			{
				double a1 = dCTM[0];
				double b1 = dCTM[1];
				double c1 = dCTM[2];
				double d1 = dCTM[3];

				dCTM[0] = pArgs[0].getNum() * a1 + pArgs[1].getNum() * c1;
				dCTM[1] = pArgs[0].getNum() * b1 + pArgs[1].getNum() * d1;
				dCTM[2] = pArgs[2].getNum() * a1 + pArgs[3].getNum() * c1;
				dCTM[3] = pArgs[2].getNum() * b1 + pArgs[3].getNum() * d1;
				dCTM[4] = pArgs[4].getNum() * a1 + pArgs[5].getNum() * c1 + dCTM[4];
				dCTM[5] = pArgs[4].getNum() * b1 + pArgs[5].getNum() * d1 + dCTM[5];
			}
			oObj.free();
			for (int i = 0; i < nNumArgs; ++i)
				pArgs[i].free();
			nNumArgs = 0;
		}
		else if (nNumArgs < maxArgs)
			pArgs[nNumArgs++] = oObj;

		parser->getObj(&oObj);
	}
	oObj.free();
	for (int i = 0; i < nNumArgs; ++i)
		pArgs[i].free();
	RELEASEOBJECT(parser);

	oContents.free();
}

CPdfEditor::CPdfEditor(const std::wstring& _wsSrcFile, const std::wstring& _wsPassword, CPdfReader* _pReader, const std::wstring& _wsDstFile, CPdfWriter* _pWriter)
{
	wsSrcFile  = _wsSrcFile;
	wsPassword = _wsPassword;
	pReader    = _pReader;
	pWriter    = _pWriter;
	m_nEditPage  = -1;
	nError     = 0;

	PDFDoc* pPDFDocument = pReader->GetPDFDocument();
	if (!pPDFDocument)
	{
		nError = 1;
		return;
	}

	// Если результат редактирования будет сохранен в тот же файл, что открыт для чтения, то файл необходимо сделать редактируемым
	std::string sPathUtf8New = U_TO_UTF8(_wsDstFile);
	std::string sPathUtf8Old = U_TO_UTF8(wsSrcFile);
	if (sPathUtf8Old == sPathUtf8New || NSSystemPath::NormalizePath(sPathUtf8Old) == NSSystemPath::NormalizePath(sPathUtf8New))
	{
		GString* owner_pswd = NSStrings::CreateString(wsPassword);
		GString* user_pswd  = NSStrings::CreateString(wsPassword);
		GBool bRes = pPDFDocument->makeWritable(true, owner_pswd, user_pswd);
		delete owner_pswd;
		delete user_pswd;
		if (!bRes)
		{
			nError = 2; // Не удалось проверить файл для записи
			return;
		}
	}
	else
	{
		if (!NSFile::CFileBinary::Copy(wsSrcFile, _wsDstFile))
		{
			nError = 2;
			return;
		}
		NSFile::CFileBinary oFile;
		if (!oFile.OpenFile(_wsDstFile, true))
		{
			nError = 2;
			return;
		}
		oFile.CloseFile();
	}

	XRef* xref = pPDFDocument->getXRef();
	PdfWriter::CDocument* pDoc = pWriter->GetDocument();
	if (!xref || !pDoc)
	{
		nError = 1;
		return;
	}

	// Получение каталога и дерева страниц из reader
	Object catDict, catRefObj, pagesRefObj;
	if (!xref->getCatalog(&catDict)->isDict() || !catDict.dictLookupNF("Pages", &pagesRefObj))
	{
		pagesRefObj.free(); catDict.free();
		nError = 3; // Не удалось получить каталог и дерево страниц
		return;
	}
	Object* trailer = xref->getTrailerDict();
	if (!trailer || !trailer->isDict() || !trailer->dictLookupNF("Root", &catRefObj)->isRef())
	{
		pagesRefObj.free(); catDict.free(); catRefObj.free();
		nError = 3; // Не удалось получить каталог и дерево страниц
		return;
	}
	Ref catRef = catRefObj.getRef();
	catRefObj.free();

	// Создание каталога для writer
	PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, catRef.num);
	if (!pXref)
	{
		pagesRefObj.free(); catDict.free();
		nError = 1;
		return;
	}
	PdfWriter::CCatalog* pCatalog = new PdfWriter::CCatalog();
	if (!pCatalog)
	{
		pagesRefObj.free(); catDict.free(); RELEASEOBJECT(pXref);
		nError = 1;
		return;
	}
	pXref->Add(pCatalog, catRef.gen);
	PdfWriter::CResourcesDict* pDR = NULL;
	PdfWriter::CXref* pDRXref = NULL;
	for (int nIndex = 0; nIndex < catDict.dictGetLength(); ++nIndex)
	{
		Object oAcroForm;
		char* chKey = catDict.dictGetKey(nIndex);
		if (strcmp("AcroForm", chKey) == 0)
		{
			catDict.dictGetVal(nIndex, &oAcroForm);
			PdfWriter::CDictObject* pAcroForm = new PdfWriter::CDictObject();

			for (int nIndex = 0; nIndex < oAcroForm.dictGetLength(); ++nIndex)
			{
				Object oTemp2;
				char* chKey = oAcroForm.dictGetKey(nIndex);
				if (strcmp("DR", chKey) == 0)
				{
					if (!oAcroForm.dictGetVal(nIndex, &oTemp2)->isDict())
					{
						oTemp2.free();
						continue;
					}

					Object oDR;
					oAcroForm.dictGetValNF(nIndex, &oDR);
					int nDRxrefNum = oDR.isRef() ? oDR.getRefNum() : xref->getNumObjects();
					int nDRxrefGen = oDR.isRef() ? oDR.getRefGen() : 0;
					oDR.free();
					pDRXref = new PdfWriter::CXref(pDoc, nDRxrefNum);

					pDR = new PdfWriter::CResourcesDict(NULL, true, false);
					pDRXref->Add(pDR, nDRxrefGen);

					pAcroForm->Add(chKey, pDR);
					for (int nIndex2 = 0; nIndex2 < oTemp2.dictGetLength(); ++nIndex2)
					{
						Object oTemp;
						char* chKey2 = oTemp2.dictGetKey(nIndex2);
						oTemp2.dictGetVal(nIndex2, &oTemp);
						DictToCDictObject(&oTemp, pDR, false, chKey2);
						oTemp.free();
					}
					oTemp2.free();

					pDR->Fix();
					continue;
				}
				else
					oAcroForm.dictGetValNF(nIndex, &oTemp2);
				DictToCDictObject(&oTemp2, pAcroForm, false, chKey);
				oTemp2.free();
			}

			if (!pAcroForm->Get("Fields"))
				pAcroForm->Add("Fields", new PdfWriter::CArrayObject());

			oAcroForm.free();
			pCatalog->Add(chKey, pAcroForm);
			continue;
		}
		else
			catDict.dictGetValNF(nIndex, &oAcroForm);
		DictToCDictObject(&oAcroForm, pCatalog, false, chKey);
		oAcroForm.free();
	}
	catDict.free();

	// Проверка уникальности имён текущих цифровых подписей pdf
	unsigned int nFormField = 0;
	AcroForm* form = pPDFDocument->getCatalog()->getForm();
	if (form)
	{
		nFormField = form->getNumFields() + 1;
		std::wstring sSig = L"Sig" + std::to_wstring(nFormField);
		int i = 0, nFormFields = form->getNumFields();
		while (i < nFormFields)
		{
			int nLength;
			Unicode* uName = form->getField(i)->getName(&nLength);
			std::wstring sName = NSStringExt::CConverter::GetUnicodeFromUTF32(uName, nLength);
			RELEASEMEM(uName);
			if (sName == sSig)
			{
				i = 0;
				nFormField++;
				sSig = L"Sig" + std::to_wstring(nFormField);
			}
			else
				i++;
		}
		nFormField--;
	}

	// Получение шифрования из reader и применения для writer
	int nCryptAlgorithm = -1;
	PdfWriter::CEncryptDict* pEncryptDict = NULL;
	if (xref->isEncrypted())
	{
		CryptAlgorithm encAlgorithm;
		GBool ownerPasswordOk;
		int permFlags, keyLength, encVersion;
		xref->getEncryption(&permFlags, &ownerPasswordOk, &keyLength, &encVersion, &encAlgorithm);
		nCryptAlgorithm = encAlgorithm;

		Object* pTrailerDict = xref->getTrailerDict();
		if (pTrailerDict)
		{
			pEncryptDict = new PdfWriter::CEncryptDict();

			// Нужно получить словарь Encrypt БЕЗ дешифровки, поэтому времено отключаем encrypted в xref
			xref->offEncrypted();

			Object encrypt, ID, ID1;
			if (pTrailerDict->dictLookup("Encrypt", &encrypt) && encrypt.isDict())
			{
				for (int nIndex = 0; nIndex < encrypt.dictGetLength(); ++nIndex)
				{
					Object oTemp;
					char* chKey = encrypt.dictGetKey(nIndex);
					encrypt.dictGetValNF(nIndex, &oTemp);
					DictToCDictObject(&oTemp, pEncryptDict, true, chKey);
					oTemp.free();
				}
			}

			if (!pEncryptDict->Get("Length"))
				pEncryptDict->Add("Length", 40);

			encrypt.free();

			if (pTrailerDict->dictLookup("ID", &ID) && ID.isArray() && ID.arrayGet(0, &ID1) && ID1.isString())
				DictToCDictObject(&ID1, pEncryptDict, true, "ID");
			ID.free(); ID1.free();

			xref->onEncrypted();

			pEncryptDict->SetRef(0, 0);
			pEncryptDict->Fix();

			pEncryptDict->SetPasswords(wsPassword, wsPassword);
			if (!pEncryptDict->UpdateKey(nCryptAlgorithm))
			{
				pagesRefObj.free();
				RELEASEOBJECT(pXref);
				RELEASEOBJECT(pDRXref);
				nError = 4; // Ошибка шифрования файла
				return;
			}
		}
	}

	// Применение редактирования для writer
	bool bRes = pDoc->EditPdf(_wsDstFile, xref->getLastXRefPos(), xref->getNumObjects() + 1, pXref, pCatalog, pEncryptDict, nFormField);
	if (bRes)
	{
		// Воспроизведение дерева страниц во writer
		GetPageTree(xref, &pagesRefObj);

		if (pDR && pDRXref)
			bRes = pDoc->EditResources(pDRXref, pDR);
	}
	pagesRefObj.free();
	if (!bRes)
		nError = 5; // Ошибка применения редактирования
}
void CPdfEditor::Close()
{
	PDFDoc* pPDFDocument = pReader->GetPDFDocument();
	PdfWriter::CDocument* pDoc = pWriter->GetDocument();
	if (!pPDFDocument || !pDoc)
		return;
	XRef* xref = pPDFDocument->getXRef();
	if (!xref)
		return;

	// Добавляем первый элемент в таблицу xref
	// он должен иметь вид 0000000000 65535 f
	PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, 0, 65535);
	if (!pXref)
		return;

	PdfWriter::CDictObject* pTrailer = NULL;
	Object* trailerDict = xref->getTrailerDict();
	if (trailerDict)
	{
		pTrailer = pXref->GetTrailer();

		for (int nIndex = 0; nIndex < trailerDict->dictGetLength(); ++nIndex)
		{
			Object oTemp;
			char* chKey = trailerDict->dictGetKey(nIndex);
			trailerDict->dictGetValNF(nIndex, &oTemp);
			DictToCDictObject(&oTemp, pTrailer, true, chKey);
			oTemp.free();
		}
	}

	Object info;
	pPDFDocument->getDocInfo(&info);
	PdfWriter::CXref* pInfoXref = NULL;
	PdfWriter::CInfoDict* pInfoDict = NULL;
	if (info.isDict())
	{
		// Обновление Info
		PdfWriter::CObjectBase* pInfo = pTrailer->Get("Info");
		pInfoXref = new PdfWriter::CXref(pDoc, pInfo ? pInfo->GetObjId() : 0);
		if (!pInfoXref)
		{
			RELEASEOBJECT(pXref);
			return;
		}
		pInfoDict = new PdfWriter::CInfoDict();
		if (!pInfoDict)
		{
			RELEASEOBJECT(pXref);
			RELEASEOBJECT(pInfoXref);
			return;
		}
		pInfoXref->Add(pInfoDict, pInfo ? pInfo->GetGenNo() : 0);

		for (int nIndex = 0; nIndex < info.dictGetLength(); ++nIndex)
		{
			Object oTemp;
			char* chKey = info.dictGetKey(nIndex);
			info.dictGetValNF(nIndex, &oTemp);
			DictToCDictObject(&oTemp, pInfoDict, true, chKey);
			oTemp.free();
		}
		pInfoDict->SetTime(PdfWriter::InfoModaDate);
	}
	info.free();

	if (!pWriter->EditClose() || !pDoc->AddToFile(pXref, pTrailer, pInfoXref, pInfoDict))
	{
		RELEASEOBJECT(pXref);
		return;
	}

	std::wstring wsPath = pDoc->GetEditPdfPath();
	std::string sPathUtf8New = U_TO_UTF8(wsPath);
	std::string sPathUtf8Old = U_TO_UTF8(wsSrcFile);
	if (sPathUtf8Old == sPathUtf8New || NSSystemPath::NormalizePath(sPathUtf8Old) == NSSystemPath::NormalizePath(sPathUtf8New))
	{
		GString* owner_pswd = NSStrings::CreateString(wsPassword);
		GString* user_pswd  = NSStrings::CreateString(wsPassword);
		pPDFDocument->makeWritable(false, owner_pswd, user_pswd);
		delete owner_pswd;
		delete user_pswd;

		NSFile::CFileBinary oFile;
		if (oFile.OpenFile(wsSrcFile))
		{
			pReader->ChangeLength(oFile.GetFileSize());
			oFile.CloseFile();
		}
	}

	pReader = NULL;
	pWriter = NULL;
	m_nEditPage = -1;
}
int CPdfEditor::GetError()
{
	return nError;
}
void CPdfEditor::GetPageTree(XRef* xref, Object* pPagesRefObj, PdfWriter::CPageTree* pPageParent)
{
	PdfWriter::CDocument* pDoc = pWriter->GetDocument();
	if (!pPagesRefObj || !xref || !pDoc)
		return;

	Object pagesObj;
	if (!pPagesRefObj->isRef() || !pPagesRefObj->fetch(xref, &pagesObj)->isDict("Pages"))
	{
		pagesObj.free();
		return;
	}

	Ref topPagesRef = pPagesRefObj->getRef();
	PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, topPagesRef.num);
	if (!pXref)
	{
		pagesObj.free();
		return;
	}

	PdfWriter::CPageTree* pPageT = new PdfWriter::CPageTree();
	if (!pPageT)
	{
		pagesObj.free();
		RELEASEOBJECT(pXref);
		return;
	}
	pXref->Add(pPageT, topPagesRef.gen);
	for (int nIndex = 0; nIndex < pagesObj.dictGetLength(); ++nIndex)
	{
		Object oTemp;
		char* chKey = pagesObj.dictGetKey(nIndex);
		if (strcmp("Resources", chKey) == 0)
		{
			if (pagesObj.dictGetVal(nIndex, &oTemp)->isDict())
			{
				PdfWriter::CResourcesDict* pDict = new PdfWriter::CResourcesDict(NULL, true, false);
				pPageT->Add("Resources", pDict);
				for (int nIndex = 0; nIndex < oTemp.dictGetLength(); ++nIndex)
				{
					Object oRes;
					char* chKey2 = oTemp.dictGetKey(nIndex);
					if (strcmp("Font", chKey2) == 0 || strcmp("ExtGState", chKey2) == 0 || strcmp("XObject", chKey2) == 0 || strcmp("Shading", chKey2) == 0 || strcmp("Pattern", chKey2) == 0)
						oTemp.dictGetVal(nIndex, &oRes);
					else
						oTemp.dictGetValNF(nIndex, &oRes);
					DictToCDictObject(&oRes, pDict, false, chKey2);
					oRes.free();
				}

				oTemp.free();
				continue;
			}
			else
			{
				oTemp.free();
				pagesObj.dictGetValNF(nIndex, &oTemp);
			}
		}
		else if (strcmp("Parent", chKey) == 0 && pPageParent)
		{
			pPageT->Add("Parent", pPageParent);
			continue;
		}
		else
			pagesObj.dictGetValNF(nIndex, &oTemp);
		DictToCDictObject(&oTemp, pPageT, false, chKey);
		oTemp.free();
	}
	pDoc->CreatePageTree(pXref, pPageT);
	pPageT->Fix();

	Object kidsArrObj;
	if (!pagesObj.dictLookup("Kids", &kidsArrObj)->isArray())
	{
		pagesObj.free();
		kidsArrObj.free();
		return;
	}
	pagesObj.free();

	for (int i = 0, count = kidsArrObj.arrayGetLength(); i < count; ++i)
	{
		Object kidRefObj;
		if (kidsArrObj.arrayGetNF(i, &kidRefObj))
			GetPageTree(xref, &kidRefObj, pPageT);
		kidRefObj.free();
	}
	kidsArrObj.free();
}
bool CPdfEditor::EditPage(int nPageIndex, bool bSet, int nNewPage)
{
	PDFDoc* pPDFDocument = pReader->GetPDFDocument();
	PdfWriter::CDocument* pDoc = pWriter->GetDocument();
	if (!pPDFDocument || !pDoc)
		return false;

	PdfWriter::CPage* pEditPage = pDoc->GetEditPage(nPageIndex);
	if (pEditPage)
	{
		if (bSet)
		{
			pDoc->SetCurPage(pEditPage);
			pWriter->EditPage(pEditPage);
			m_nEditPage = nPageIndex;
		}
		return true;
	}

	XRef* xref = pPDFDocument->getXRef();
	Catalog* pCatalog = pPDFDocument->getCatalog();
	if (!xref || !pCatalog)
		return false;
	std::pair<int, int> pPageRef = pDoc->GetPageRef(nPageIndex);
	if (pPageRef.first == 0)
		return false;

	// Получение объекта страницы
	Object pageRefObj, pageObj;
	pageRefObj.initRef(pPageRef.first, pPageRef.second);
	if (!pageRefObj.fetch(xref, &pageObj) || !pageObj.isDict())
	{
		pageObj.free();
		pageRefObj.free();
		return false;
	}
	pageRefObj.free();

	// Воспроизведение словаря страницы из reader для writer
	PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, pPageRef.first);
	if (!pXref)
	{
		pageObj.free();
		return false;
	}
	PdfWriter::CPage* pPage = new PdfWriter::CPage(pDoc);
	if (!pPage)
	{
		pageObj.free();
		RELEASEOBJECT(pXref);
		return false;
	}
	pXref->Add(pPage, pPageRef.second);
	for (int nIndex = 0; nIndex < pageObj.dictGetLength(); ++nIndex)
	{
		Object oTemp;
		char* chKey = pageObj.dictGetKey(nIndex);
		if (strcmp("Resources", chKey) == 0)
		{
			if (pageObj.dictGetVal(nIndex, &oTemp)->isDict())
			{
				PdfWriter::CResourcesDict* pDict = new PdfWriter::CResourcesDict(NULL, true, false);
				pPage->Add("Resources", pDict);
				for (int nIndex = 0; nIndex < oTemp.dictGetLength(); ++nIndex)
				{
					Object oRes;
					char* chKey2 = oTemp.dictGetKey(nIndex);
					if (strcmp("Font", chKey2) == 0 || strcmp("ExtGState", chKey2) == 0 || strcmp("XObject", chKey2) == 0 || strcmp("Shading", chKey2) == 0 || strcmp("Pattern", chKey2) == 0)
						oTemp.dictGetVal(nIndex, &oRes);
					else
						oTemp.dictGetValNF(nIndex, &oRes);
					DictToCDictObject(&oRes, pDict, false, chKey2);
					oRes.free();
				}

				oTemp.free();
				continue;
			}
			else
			{
				oTemp.free();
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
		}
		else if (strcmp("Annots", chKey) == 0)
		{
			if (pageObj.dictGetVal(nIndex, &oTemp)->isArray())
			{
				PdfWriter::CArrayObject* pArray = new PdfWriter::CArrayObject();
				pPage->Add("Annots", pArray);
				for (int nIndex = 0; nIndex < oTemp.arrayGetLength(); ++nIndex)
				{
					Object oAnnot;
					oTemp.arrayGetNF(nIndex, &oAnnot);
					DictToCDictObject(&oAnnot, pArray, false, "");
					oAnnot.free();
				}
				oTemp.free();
				continue;
			}
			else
			{
				oTemp.free();
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
		}
		else if (strcmp("Contents", chKey) == 0)
		{
			if (pageObj.dictGetVal(nIndex, &oTemp)->isArray())
			{
				DictToCDictObject(&oTemp, pPage, true, chKey);
				oTemp.free();
				continue;
			}
			else
			{
				oTemp.free();
				pageObj.dictGetValNF(nIndex, &oTemp);
			}
		}
		else if (strcmp("Parent", chKey) == 0)
		{
			pageObj.dictGetValNF(nIndex, &oTemp);
		}
		else
			pageObj.dictGetValNF(nIndex, &oTemp);
		DictToCDictObject(&oTemp, pPage, true, chKey);
		oTemp.free();
	}
	pPage->Fix();
	double dCTM[6] = { 1, 0, 0, 1, 0, 0 };
	GetCTM(xref, &pageObj, dCTM);
	pageObj.free();

	// Применение редактирования страницы для writer
	if (pDoc->EditPage(pXref, pPage, nPageIndex, nNewPage))
	{
		if (bSet)
		{
			pWriter->EditPage(pPage);
			m_nEditPage = nPageIndex;
		}
		pPage->StartTransform(dCTM[0], dCTM[1], dCTM[2], dCTM[3], dCTM[4], dCTM[5]);
		pPage->SetStrokeColor(0, 0, 0);
		pPage->SetFillColor(0, 0, 0);
		pPage->SetExtGrState(pDoc->GetExtGState(255, 255));
		pPage->BeginText();
		pPage->SetCharSpace(0);
		pPage->SetTextRenderingMode(PdfWriter::textrenderingmode_Fill);
		pPage->SetHorizontalScalling(100);
		pPage->EndText();
		return true;
	}

	RELEASEOBJECT(pXref);
	return false;
}
bool CPdfEditor::DeletePage(int nPageIndex)
{
	return pWriter->GetDocument()->DeletePage(nPageIndex);
}
bool CPdfEditor::AddPage(int nPageIndex)
{
	// Применение добавления страницы для writer
	if (!pWriter->AddPage(nPageIndex))
		return false;
	// По умолчанию выставляются размеры первой страницы, в дальнейшем размеры можно изменить
	double dPageDpiX, dPageDpiY;
	double dWidth, dHeight;
	pReader->GetPageInfo(0, &dWidth, &dHeight, &dPageDpiX, &dPageDpiY);

	dWidth  *= 25.4 / dPageDpiX;
	dHeight *= 25.4 / dPageDpiY;

	pWriter->put_Width(dWidth);
	pWriter->put_Height(dHeight);
	return true;
}
bool CPdfEditor::MovePage(int nPageIndex, int nPos)
{
	if (EditPage(nPageIndex, true, nPos))
	{
		m_nEditPage = nPos;
		return pWriter->GetDocument()->MovePage(nPageIndex, nPos);
	}
	return false;
}
bool CPdfEditor::EditAnnot(int nPageIndex, int nID)
{
	PDFDoc* pPDFDocument = pReader->GetPDFDocument();
	PdfWriter::CDocument* pDoc = pWriter->GetDocument();
	if (!pPDFDocument || !pDoc)
		return false;

	PdfWriter::CPage* pEditPage = pDoc->GetEditPage(nPageIndex);
	if (!pEditPage)
	{
		pEditPage = pDoc->GetCurPage();
		EditPage(nPageIndex);
		pDoc->SetCurPage(pEditPage);
		pWriter->EditPage(pEditPage);
	}

	XRef* xref = pPDFDocument->getXRef();
	std::pair<int, int> pPageRef = pDoc->GetPageRef(nPageIndex);
	if (!xref || pPageRef.first == 0)
		return false;

	// Получение объекта аннотации
	Object pageRefObj, pageObj, oAnnots;
	pageRefObj.initRef(pPageRef.first, pPageRef.second);
	if (!pageRefObj.fetch(xref, &pageObj)->isDict() || !pageObj.dictLookup("Annots", &oAnnots)->isArray())
	{
		pageRefObj.free(); pageObj.free(); oAnnots.free();
		return false;
	}
	pageRefObj.free(); pageObj.free();

	Object oAnnotRef, oAnnot, oType;
	for (int i = 0; i < oAnnots.arrayGetLength(); ++i)
	{
		if (oAnnots.arrayGetNF(i, &oAnnotRef)->isRef() && oAnnotRef.getRefNum() == nID)
			break;
		oAnnotRef.free();
	}
	oAnnots.free();
	if (!oAnnotRef.isRef() || !oAnnotRef.fetch(xref, &oAnnot)->isDict() || !oAnnot.dictLookup("Subtype", &oType)->isName())
	{
		oAnnotRef.free(); oAnnot.free(); oType.free();
		return false;
	}

	// Воспроизведение словаря аннотации из reader для writer
	PdfWriter::CXref* pXref = new PdfWriter::CXref(pDoc, oAnnotRef.getRefNum());
	if (!pXref)
	{
		oAnnotRef.free(); oAnnot.free(); oType.free();
		return false;
	}

	bool bIsWidget = false;
	PdfWriter::CAnnotation* pAnnot = NULL;
	if (oType.isName("Text"))
		pAnnot = new PdfWriter::CTextAnnotation(pXref);
	else if (oType.isName("Ink"))
		pAnnot = new PdfWriter::CInkAnnotation(pXref);
	else if (oType.isName("Line"))
		pAnnot = new PdfWriter::CLineAnnotation(pXref);
	else if (oType.isName("Highlight") || oType.isName("Underline") || oType.isName("Squiggly") || oType.isName("StrikeOut"))
		pAnnot = new PdfWriter::CTextMarkupAnnotation(pXref);
	else if (oType.isName("Square") || oType.isName("Circle"))
		pAnnot = new PdfWriter::CSquareCircleAnnotation(pXref);
	else if (oType.isName("Polygon") || oType.isName("PolyLine"))
		pAnnot = new PdfWriter::CPolygonLineAnnotation(pXref);
	else if (oType.isName("FreeText"))
	{
		std::map<std::wstring, std::wstring> mapFont = pReader->GetAnnotFonts(&oAnnotRef);
		m_mFonts.insert(mapFont.begin(), mapFont.end());
		pAnnot = new PdfWriter::CFreeTextAnnotation(pXref);
	}
	else if (oType.isName("Caret"))
		pAnnot = new PdfWriter::CCaretAnnotation(pXref);
	else if (oType.isName("Stamp"))
		pAnnot = new PdfWriter::CStampAnnotation(pXref);
	else if (oType.isName("Popup"))
		pAnnot = new PdfWriter::CPopupAnnotation(pXref);
	else if (oType.isName("Widget"))
	{
		bIsWidget = true;
		char* sName = NULL;
		Object oFT;
		if (oAnnot.dictLookup("FT", &oFT)->isName())
			sName = oFT.getName();

		if (!sName)
		{
			Object oParent, oParent2;
			oAnnot.dictLookup("Parent", &oParent);
			while (oParent.isDict())
			{
				if (oParent.dictLookup("FT", &oFT)->isName())
				{
					sName = oFT.getName();
					break;
				}
				oFT.free();
				oParent.dictLookup("Parent", &oParent2);
				oParent.free();
				oParent = oParent2;
			}
			oParent.free();
		}

		if (sName)
		{
			if (strcmp("Btn", sName) == 0)
			{
				bool bPushButton = false;
				oFT.free();
				int nFf = 0;
				if (oAnnot.dictLookup("Ff", &oFT)->isInt())
					nFf = oFT.getInt();
				if (!nFf)
				{
					Object oParent, oParent2;
					oAnnot.dictLookup("Parent", &oParent);
					while (oParent.isDict())
					{
						if (oParent.dictLookup("Ff", &oFT)->isInt())
						{
							nFf = oFT.getInt();
							break;
						}
						oFT.free();
						oParent.dictLookup("Parent", &oParent2);
						oParent.free();
						oParent = oParent2;
					}
					oParent.free();
				}

				bPushButton = (bool)((nFf >> 16) & 1);
				if (bPushButton)
					pAnnot = new PdfWriter::CPushButtonWidget(pXref);
				else
					pAnnot = new PdfWriter::CCheckBoxWidget(pXref);
			}
			else if (strcmp("Tx", sName) == 0)
				pAnnot = new PdfWriter::CTextWidget(pXref);
			else if (strcmp("Ch", sName) == 0)
				pAnnot = new PdfWriter::CChoiceWidget(pXref);
			else if (strcmp("Sig", sName) == 0)
				pAnnot = new PdfWriter::CSignatureWidget(pXref);
			else
				pAnnot = new PdfWriter::CWidgetAnnotation(pXref, PdfWriter::EAnnotType::AnnotWidget);
		}
		oFT.free();
	}

	if (!pAnnot)
	{
		oAnnotRef.free(); oAnnot.free(); oType.free();
		RELEASEOBJECT(pXref);
		return false;
	}
	pXref->Add(pAnnot, oAnnotRef.getRefGen());

	for (int nIndex = 0; nIndex < oAnnot.dictGetLength(); ++nIndex)
	{
		bool bUnicode = false;
		char* chKey = oAnnot.dictGetKey(nIndex);
		if (!strcmp("Popup", chKey))
		{
			Object oPopupRef;
			if (oAnnot.dictGetValNF(nIndex, &oPopupRef)->isRef() && EditAnnot(nPageIndex, oPopupRef.getRefNum()))
			{
				PdfWriter::CAnnotation* pPopup = pDoc->GetAnnot(oPopupRef.getRefNum());
				if (pPopup)
				{
					pAnnot->Add("Popup", pPopup);
					pPopup->Add("Parent", pAnnot);
				}
			}
			continue;
		}
		else if (!strcmp("Parent", chKey) && bIsWidget)
		{
			Object oParentRef;
			oAnnot.dictGetValNF(nIndex, &oParentRef);
			PdfWriter::CDictObject* pParent = GetWidgetParent(pPDFDocument, pDoc, &oParentRef);

			if (!pParent)
			{
				oParentRef.free();
				continue;
			}

			((PdfWriter::CWidgetAnnotation*)pAnnot)->SetParent(pParent);
			PdfWriter::CArrayObject* pKids = dynamic_cast<PdfWriter::CArrayObject*>(pParent->Get("Kids"));
			if (!pKids)
			{
				oParentRef.free();
				continue;
			}

			for (int i = 0; i < pKids->GetCount(); ++i)
			{
				PdfWriter::CObjectBase* pKid = pKids->Get(i);
				if (pKid->GetObjId() == oAnnotRef.getRefNum())
				{
					pKids->Insert(pKid, pAnnot, true);
					break;
				}
			}
			oParentRef.free();
		}
		else if (!strcmp("Opt", chKey))
			bUnicode = true;
		Object oTemp;
		oAnnot.dictGetValNF(nIndex, &oTemp);
		DictToCDictObject(&oTemp, pAnnot, false, chKey, bUnicode);
		oTemp.free();
	}

	if (oType.isName("Stamp"))
	{
		Object oAP, oAPN;
		if (oAnnot.dictLookup("AP", &oAP)->isDict() && oAP.dictLookup("N", &oAPN)->isStream())
		{
			Object oAPNRef;
			oAP.dictLookupNF("N", &oAPNRef);
			PdfWriter::CXref* pXRef = new PdfWriter::CXref(pDoc, oAPNRef.getRefNum());
			pDoc->EditXref(pXRef);

			PdfWriter::CDictObject* pAPN = new PdfWriter::CDictObject();
			pXRef->Add(pAPN, oAPNRef.getRefGen());
			((PdfWriter::CStampAnnotation*)pAnnot)->SetAPStream(pAPN);
			oAPNRef.free();

			Object oTemp;
			Dict* pODict = oAPN.streamGetDict();
			for (int nIndex = 0; nIndex < pODict->getLength(); ++nIndex)
			{
				char* chKey = pODict->getKey(nIndex);
				pODict->getValNF(nIndex, &oTemp);
				DictToCDictObject(&oTemp, pAPN, false, chKey);
				oTemp.free();
			}
			int nLength = 0;
			if (pODict->lookup("Length", &oTemp)->isInt())
				nLength = oTemp.getInt();
			PdfWriter::CStream* pStream = new PdfWriter::CMemoryStream(nLength);
			pAPN->SetStream(pStream);
			pAPN->Add("Length", nLength);
			Stream* pOStream = oAPN.getStream()->getUndecodedStream();
			pOStream->reset();
			for (int nI = 0; nI < nLength; ++nI)
				pStream->WriteChar(pOStream->getChar());
		}
		oAP.free(); oAPN.free();
	}
	oAnnotRef.free(); oAnnot.free(); oType.free();

	if (pDoc->EditAnnot(pXref, pAnnot, nID))
		return true;

	RELEASEOBJECT(pXref);
	return false;
}
bool CPdfEditor::DeleteAnnot(int nID, Object* oAnnots)
{
	PDFDoc* pPDFDocument = pReader->GetPDFDocument();
	PdfWriter::CDocument* pDoc = pWriter->GetDocument();
	if (!pPDFDocument || !pDoc)
		return false;

	XRef* xref = pPDFDocument->getXRef();
	bool bClear = false;
	if (!oAnnots)
	{
		std::pair<int, int> pPageRef = pDoc->GetPageRef(m_nEditPage);
		if (pPageRef.first == 0)
			return false;

		oAnnots = new Object();
		bClear = true;

		// Получение объекта аннотации
		Object pageRefObj, pageObj;
		pageRefObj.initRef(pPageRef.first, pPageRef.second);
		if (!pageRefObj.fetch(xref, &pageObj)->isDict() || !pageObj.dictLookup("Annots", oAnnots)->isArray())
		{
			pageRefObj.free(); pageObj.free(); oAnnots->free();
			RELEASEOBJECT(oAnnots);
			return false;
		}
		pageRefObj.free(); pageObj.free();
	}

	bool bRes = false;
	for (int i = 0; i < oAnnots->arrayGetLength(); ++i)
	{
		Object oAnnotRef, oAnnot;
		if (oAnnots->arrayGetNF(i, &oAnnotRef)->isRef() && oAnnotRef.getRefNum() == nID)
		{
			bRes = pDoc->DeleteAnnot(oAnnotRef.getRefNum(), oAnnotRef.getRefGen());
			if (oAnnotRef.fetch(xref, &oAnnot)->isDict())
			{
				Object oPopupRef;
				if (oAnnot.dictLookupNF("Popup", &oPopupRef)->isRef())
					pDoc->DeleteAnnot(oPopupRef.getRefNum(), oPopupRef.getRefGen());
				oPopupRef.free();
			}
		}
		else if (oAnnots->arrayGet(i, &oAnnot)->isDict())
		{
			Object oIRTRef;
			if (oAnnot.dictLookupNF("IRT", &oIRTRef)->isRef() && oIRTRef.getRefNum() == nID)
				DeleteAnnot(oAnnotRef.getRefNum(), oAnnots);
			oIRTRef.free();
		}
		oAnnotRef.free(); oAnnot.free();
	}

	if (bClear)
	{
		oAnnots->free();
		RELEASEOBJECT(oAnnots);
	}

	return bRes;
}
bool CPdfEditor::EditWidgets(IAdvancedCommand* pCommand)
{
	CWidgetsInfo* pFieldInfo = (CWidgetsInfo*)pCommand;
	PDFDoc* pPDFDocument = pReader->GetPDFDocument();
	PdfWriter::CDocument* pDoc = pWriter->GetDocument();

	std::vector<CWidgetsInfo::CParent*> arrParents = pFieldInfo->GetParents();
	for (CWidgetsInfo::CParent* pParent : arrParents)
	{
		PdfWriter::CDictObject* pDParent = pDoc->GetParent(pParent->nID);
		if (pDParent)
			continue;

		Object oParentRef;
		// TODO узнать gen родителя
		oParentRef.initRef(pParent->nID, 0);
		GetWidgetParent(pPDFDocument, pDoc, &oParentRef);
		// TODO перевыставить детей
		oParentRef.free();
	}
	return true;
}
int CPdfEditor::GetPagesCount()
{
	return pWriter->GetDocument()->GetPagesCount();
}
void CPdfEditor::GetPageInfo(int nPageIndex, double* pdWidth, double* pdHeight, double* pdDpiX, double* pdDpiY)
{
	PdfWriter::CPage* pPage = pWriter->GetDocument()->GetPage(nPageIndex);
	if (!pPage)
		return;

	int nRotate = pPage->GetRotate();
	if (nRotate % 180 == 0)
	{
		*pdWidth  = pPage->GetWidth();
		*pdHeight = pPage->GetHeight();
	}
	else
	{
		*pdWidth  = pPage->GetHeight();
		*pdHeight = pPage->GetWidth();
	}

	*pdDpiX = 72.0;
	*pdDpiY = 72.0;
}
int CPdfEditor::GetRotate(int nPageIndex)
{
	PdfWriter::CPage* pPage = pWriter->GetDocument()->GetPage(nPageIndex);
	if (!pPage)
		return 0;
	return pPage->GetRotate();
}
bool CPdfEditor::IsEditPage()
{
	return m_nEditPage >= 0;
}
void CPdfEditor::ClearPage()
{
	PDFDoc* pPDFDocument = pReader->GetPDFDocument();
	XRef* xref = pPDFDocument->getXRef();
	PdfWriter::CDocument* pDoc = pWriter->GetDocument();
	std::pair<int, int> pPageRef = pDoc->GetPageRef(m_nEditPage);

	// Получение объекта страницы
	Object pageRefObj, pageObj;
	pageRefObj.initRef(pPageRef.first, pPageRef.second);
	if (!pageRefObj.fetch(xref, &pageObj)->isDict())
	{
		pageObj.free(); pageRefObj.free();
		return;
	}
	pageRefObj.free();

	Object oAnnots;
	// ВРЕМЕННО удаление Link аннотаций при редактировании
	if (pageObj.dictLookup("Annots", &oAnnots)->isArray())
	{
		for (int nIndex = 0; nIndex < oAnnots.arrayGetLength(); ++nIndex)
		{
			Object oAnnot, oSubtype, oAnnotRef;
			if (oAnnots.arrayGet(nIndex, &oAnnot)->isDict("Annot") && oAnnot.dictLookup("Subtype", &oSubtype)->isName("Link"))
			{
				oAnnots.arrayGetNF(nIndex, &oAnnotRef);
				DeleteAnnot(oAnnotRef.getRefNum(), &oAnnots);
			}
			oAnnot.free(); oSubtype.free(); oAnnotRef.free();
		}
	}
	pageObj.free();

	pDoc->ClearPage();
}
void CPdfEditor::AddShapeXML(const std::string& sXML)
{
	return pWriter->GetDocument()->AddShapeXML(sXML);
}
void CPdfEditor::EndMarkedContent()
{
	pWriter->GetDocument()->EndShapeXML();
}
bool CPdfEditor::IsBase14(const std::wstring& wsFontName, bool& bBold, bool& bItalic, std::wstring& wsFontPath)
{
	std::map<std::wstring, std::wstring>::iterator it = m_mFonts.find(wsFontName);
	if (it != m_mFonts.end())
		wsFontPath = it->second;
	std::map<std::wstring, std::wstring> mFonts = pReader->GetFonts();
	std::map<std::wstring, std::wstring>::iterator it2 = mFonts.find(wsFontName);
	if (it2 != mFonts.end())
		wsFontPath = it2->second;
	if (wsFontPath.empty())
		return false;
	if (wsFontName == L"Helvetica")
		return true;
	if (wsFontName == L"Helvetica-Bold")
	{
		bBold = true;
		return true;
	}
	if (wsFontName == L"Helvetica-Oblique")
	{
		bItalic = true;
		return true;
	}
	if (wsFontName == L"Helvetice-BoldOblique")
	{
		bBold = true;
		bItalic = true;
		return true;
	}
	if (wsFontName == L"Courier")
		return true;
	if (wsFontName == L"Courier-Bold")
	{
		bBold = true;
		return true;
	}
	if (wsFontName == L"Courier-Oblique")
	{
		bItalic = true;
		return true;
	}
	if (wsFontName == L"Courier-BoldOblique")
	{
		bBold = true;
		bItalic = true;
		return true;
	}
	if (wsFontName == L"Times" || wsFontName == L"Times-Roman")
		return true;
	if (wsFontName == L"Times-Bold")
	{
		bBold = true;
		return true;
	}
	if (wsFontName == L"Times-Oblique")
	{
		bItalic = true;
		return true;
	}
	if (wsFontName == L"Times-BoldOblique")
	{
		bBold = true;
		bItalic = true;
		return true;
	}
	if (wsFontName == L"Symbol")
		return true;
	if (wsFontName == L"ZapfDingbats")
		return true;
	return false;
}
