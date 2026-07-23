/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "Scene3d.h"

namespace PPTX
{
	namespace Logic
	{		
		Scene3d::Scene3d() {}
		OOX::EElementType Scene3d::getType() const
		{
			return OOX::et_a_scene3d;
		}
		void Scene3d::fromXML(XmlUtils::CXmlLiteReader& oReader)
		{
			if ( oReader.IsEmptyNode() )
				return;

			m_namespace = XmlUtils::GetNamespace(oReader.GetName());

			int nCurDepth = oReader.GetDepth();
			while( oReader.ReadNextSiblingNode( nCurDepth ) )
			{
				std::wstring strName = oReader.GetName();

				if (strName == L"a:camera")
					camera = oReader;
				if (strName == L"a:lightRig")
					lightRig = oReader;
				if (strName == L"a:backdrop")
					backdrop = oReader;
			}
			FillParentPointersForChilds();
		}
		void Scene3d::fromXML(XmlUtils::CXmlNode& node)
		{
			camera		= node.ReadNode(L"a:camera");
			lightRig	= node.ReadNode(L"a:lightRig");
			backdrop	= node.ReadNode(L"a:backdrop");

			FillParentPointersForChilds();
		}
		std::wstring Scene3d::toXML() const
		{
			XmlUtils::CNodeValue oValue;

			oValue.WriteNullable(camera);
			oValue.WriteNullable(lightRig);
			oValue.WriteNullable(backdrop);

			return XmlUtils::CreateNode(L"a:scene3d", oValue);
		}
		void Scene3d::toPPTY(NSBinPptxRW::CBinaryFileWriter* pWriter) const
		{
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeStart);
			pWriter->WriteBYTE(NSBinPptxRW::g_nodeAttributeEnd);

			pWriter->WriteRecord2(0, camera);
			pWriter->WriteRecord2(1, lightRig);
			pWriter->WriteRecord2(2, backdrop);
		}
		void Scene3d::fromPPTY(NSBinPptxRW::CBinaryFileReader* pReader)
		{
			LONG _end_rec = pReader->GetPos() + pReader->GetRecordSize() + 4;

			pReader->Skip(1); // start attributes

			while (true)
			{
				BYTE _at = pReader->GetUChar_TypeNode();
				if (_at == NSBinPptxRW::g_nodeAttributeEnd)
					break;

				break;
			}

			while (pReader->GetPos() < _end_rec)
			{
				BYTE _at = pReader->GetUChar();
				switch (_at)
				{
					case 0:
					{
						camera = new Logic::Camera();
						camera->fromPPTY(pReader);
					}break;
					case 1:
					{
						lightRig = new Logic::LightRig();
						lightRig->fromPPTY(pReader);
					}break;
					case 2:
					{
						backdrop = new Logic::Backdrop();
						backdrop->fromPPTY(pReader);
					}break;
					default:
						break;
				}
			}

			pReader->Seek(_end_rec);
		}
		void Scene3d::toXmlWriter(NSBinPptxRW::CXmlWriter* pWriter) const
		{
			std::wstring sNodeNamespace;
			std::wstring sAttrNamespace;
			if (XMLWRITER_DOC_TYPE_WORDART == pWriter->m_lDocType)
			{
				sNodeNamespace = L"w14:";
				sAttrNamespace = sNodeNamespace;
			}
			else
				sNodeNamespace = m_namespace + L":";

			pWriter->StartNode(sNodeNamespace + L"scene3d");

			pWriter->StartAttributes();
			pWriter->EndAttributes();

			pWriter->Write(camera);
			pWriter->Write(lightRig);
			pWriter->Write(backdrop);

			pWriter->EndNode(sNodeNamespace + L"scene3d");
		}
		void Scene3d::FillParentPointersForChilds()
		{
			if(camera.IsInit())
				camera->SetParentPointer(this);
			if(lightRig.IsInit())
				lightRig->SetParentPointer(this);
			if(backdrop.IsInit())
				backdrop->SetParentPointer(this);
		}
	} // namespace Logic
} // namespace PPTX
