//========================================================================
//
// PDFDoc.h
//
// Copyright 1996-2003 Glyph & Cog, LLC
//
//========================================================================

#ifndef PDFDOC_H
#define PDFDOC_H

#include <aconf.h>

#ifdef USE_GCC_PRAGMAS
#pragma interface
#endif

#include <stdio.h>
#include "XRef.h"
#include "Catalog.h"
#include "Page.h"

class GString;
class BaseStream;
class OutputDev;
class Links;
class LinkAction;
class LinkDest;
class Outline;
class OutlineItem;
class OptionalContent;
class PDFCore;

//------------------------------------------------------------------------
// PDFDoc
//------------------------------------------------------------------------

class PDFDoc {
public:

  PDFDoc(GString *fileNameA, GString *ownerPassword = NULL,
	 GString *userPassword = NULL, PDFCore *coreA = NULL);

#ifdef _WIN32
  PDFDoc(wchar_t *fileNameA, int fileNameLen, GString *ownerPassword = NULL,
	 GString *userPassword = NULL, PDFCore *coreA = NULL);
#endif

  // This version takes a UTF-8 file name (which is only relevant on
  // Windows).
  PDFDoc(char *fileNameA, GString *ownerPassword = NULL,
	 GString *userPassword = NULL, PDFCore *coreA = NULL);

  PDFDoc(BaseStream *strA, GString *ownerPassword = NULL,
	 GString *userPassword = NULL, PDFCore *coreA = NULL);

  ~PDFDoc();

  // Was PDF document successfully opened?
  GBool isOk() { return ok; }

  // Get the error code (if isOk() returns false).
  int getErrorCode() { return errCode; }

  // Get file name.
  GString *getFileName() { return fileName; }
#ifdef _WIN32
  wchar_t *getFileNameU() { return fileNameU; }
#endif

  // Get the xref table.
  XRef *getXRef() { return xref; }

  // Get catalog.
  Catalog *getCatalog() { return catalog; }

  // Get base stream.
  BaseStream *getBaseStream() { return str; }

  // Get page parameters.
  double getPageMediaWidth(int page)
    { return catalog->getPage(page)->getMediaWidth(); }
  double getPageMediaHeight(int page)
    { return catalog->getPage(page)->getMediaHeight(); }
  double getPageCropWidth(int page)
    { return catalog->getPage(page)->getCropWidth(); }
  double getPageCropHeight(int page)
    { return catalog->getPage(page)->getCropHeight(); }
  int getPageRotate(int page)
    { return catalog->getPage(page)->getRotate(); }

  // Get number of pages.
  int getNumPages() { return catalog->getNumPages(); }

  // Return the contents of the metadata stream, or NULL if there is
  // no metadata.
  GString *readMetadata() { return catalog->readMetadata(); }

  // Return the structure tree root object.
  Object *getStructTreeRoot() { return catalog->getStructTreeRoot(); }

  // Display a page.
  void displayPage(OutputDev *out, int page,
		   double hDPI, double vDPI, int rotate,
		   GBool useMediaBox, GBool crop, GBool printing,
		   GBool (*abortCheckCbk)(void *data) = NULL,
		   void *abortCheckCbkData = NULL);

  // Display a range of pages.
  void displayPages(OutputDev *out, int firstPage, int lastPage,
		    double hDPI, double vDPI, int rotate,
		    GBool useMediaBox, GBool crop, GBool printing,
		    GBool (*abortCheckCbk)(void *data) = NULL,
		    void *abortCheckCbkData = NULL);

  // Display part of a page.
  void displayPageSlice(OutputDev *out, int page,
			double hDPI, double vDPI, int rotate,
			GBool useMediaBox, GBool crop, GBool printing,
			int sliceX, int sliceY, int sliceW, int sliceH,
			GBool (*abortCheckCbk)(void *data) = NULL,
			void *abortCheckCbkData = NULL);

  // Find a page, given its object ID.  Returns page number, or 0 if
  // not found.
  int findPage(int num, int gen) { return catalog->findPage(num, gen); }

  // Returns the links for the current page, transferring ownership to
  // the caller.
  Links *getLinks(int page);

  // Find a named destination.  Returns the link destination, or
  // NULL if <name> is not a destination.
  LinkDest *findDest(GString *name)
    { return catalog->findDest(name); }

  // Process the links for a page.
  void processLinks(OutputDev *out, int page);

#ifndef DISABLE_OUTLINE
  // Return the outline object.
  Outline *getOutline() { return outline; }

  // Return the target page number for an outline item.  Returns 0 if
  // the item doesn't target a page in this PDF file.
  int getOutlineTargetPage(OutlineItem *outlineItem);
#endif

  // Return the OptionalContent object.
  OptionalContent *getOptionalContent() { return optContent; }

  // Is the file encrypted?
  GBool isEncrypted() { return xref->isEncrypted(); }

  // Check various permissions.
  GBool okToPrint(GBool ignoreOwnerPW = gFalse)
    { return xref->okToPrint(ignoreOwnerPW); }
  GBool okToChange(GBool ignoreOwnerPW = gFalse)
    { return xref->okToChange(ignoreOwnerPW); }
  GBool okToCopy(GBool ignoreOwnerPW = gFalse)
    { return xref->okToCopy(ignoreOwnerPW); }
  GBool okToAddNotes(GBool ignoreOwnerPW = gFalse)
    { return xref->okToAddNotes(ignoreOwnerPW); }

  // Is this document linearized?
  GBool isLinearized();

  // Return the document's Info dictionary (if any).
  Object *getDocInfo(Object *obj) { return xref->getDocInfo(obj); }
  Object *getDocInfoNF(Object *obj) { return xref->getDocInfoNF(obj); }

  // Return the PDF version specified by the file.
  double getPDFVersion() { return pdfVersion; }

  // Save this file with another name.
  GBool saveAs(GString *name);

  // Return a pointer to the PDFCore object.
  PDFCore *getCore() { return core; }

  // Get the list of embedded files.
  int getNumEmbeddedFiles() { return catalog->getNumEmbeddedFiles(); }
  Unicode *getEmbeddedFileName(int idx)
    { return catalog->getEmbeddedFileName(idx); }
  int getEmbeddedFileNameLength(int idx)
    { return catalog->getEmbeddedFileNameLength(idx); }
  GBool saveEmbeddedFile(int idx, const char *path);
  GBool saveEmbeddedFileU(int idx, const char *path);
#ifdef _WIN32
  GBool saveEmbeddedFile(int idx, const wchar_t *path, int pathLen);
#endif
  char *getEmbeddedFileMem(int idx, int *size);
  GBool makeWritable(bool bWritable, GString *ownerPassword = NULL,
                     GString *userPassword = NULL);


private:

  void init(PDFCore *coreA);
  GBool setup(GString *ownerPassword, GString *userPassword);
  GBool setup2(GString *ownerPassword, GString *userPassword,
	       GBool repairXRef);
  void checkHeader();
  GBool checkEncryption(GString *ownerPassword, GString *userPassword);
  GBool saveEmbeddedFile2(int idx, FILE *f);

  GString *fileName;
#ifdef _WIN32
  wchar_t *fileNameU;
#endif
  FILE *file;
  BaseStream *str;
  PDFCore *core;
  double pdfVersion;
  XRef *xref;
  Catalog *catalog;
#ifndef DISABLE_OUTLINE
  Outline *outline;
#endif
  OptionalContent *optContent;

  GBool ok;
  int errCode;
};

#endif
