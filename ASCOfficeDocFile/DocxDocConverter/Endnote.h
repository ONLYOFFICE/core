/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
#pragma once

#include "ITextItem.h"
#include "EndnoteRef.h"
#include "EndnoteReference.h"
#include "PLC.h"

namespace ASCDocFileFormat
{
  typedef PLC<EmptyPLCData> PlcfendTxt;
  typedef PLC<short> PlcfendRef;
	
  class Endnote: public ITextItem
  {
    struct EndnoteItemWithOffset
    {
      TextItemPtr endnoteItem;
      unsigned int endnoteItemOffset;

      EndnoteItemWithOffset():
      endnoteItem(), endnoteItemOffset(0)
      {
      }
 
      EndnoteItemWithOffset( const TextItemPtr& _endnoteItem, unsigned int _endnoteItemOffset ):
      endnoteItem(_endnoteItem), endnoteItemOffset(_endnoteItemOffset)
      {
      }
    };

    private:
	  list<EndnoteItemWithOffset> textItems;
	  unsigned int endnoteItemsOffset;
	  short aFtnIdx;

    public:
	  explicit Endnote( short _aFtnIdx = 0 );
	  Endnote( const Endnote& _endnote );
      void AddTextItem( const ITextItem& _textItem );
	  short GetIndex() const;
	  virtual ~Endnote();
	  virtual wstring GetAllText() const;
	  virtual operator wstring() const;
	  virtual vector<TextItemPtr> GetAllParagraphsCopy() const;
	  virtual vector<ITextItem*> GetAllParagraphs();
	  virtual vector<PapxInFkp> GetAllParagraphsProperties( vector<unsigned int>* allParagraphsOffsets ) const;
	  virtual vector<Chpx> GetAllRunProperties( vector<unsigned int>* allRunsOffsets ) const;
	  virtual vector<IParagraphItemPtr> GetAllRunsCopy( vector<unsigned int>* allRunsOffsets ) const;
      virtual vector<IParagraphItemPtr> GetAllParagraphItemsCopy( vector<unsigned int>* allParagraphItemsOffsets ) const;
  	  virtual IVirtualConstructor* New() const;
	  virtual IVirtualConstructor* Clone() const;
  };
}