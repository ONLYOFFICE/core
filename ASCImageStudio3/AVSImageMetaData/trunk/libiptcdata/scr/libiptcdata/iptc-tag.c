/* iptc-tag.c
 *
 * Copyright © 2005 David Moore <dcm@acm.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details. 
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"
#include "iptc-tag.h"
#include "i18n.h"

#include <string.h>
#include <libiptcdata/_stdint.h>

static IptcTagInfo IptcTagTable[] = {
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_MODEL_VERSION,
		"ModelVersion",
		N_("Model Version"),
		N_("Version of IIM part 1."),
		IPTC_FORMAT_SHORT, IPTC_MANDATORY, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_DESTINATION,
		"Destination",
		N_("Destination"),
		N_("Routing information."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 0, 1024 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_FILE_FORMAT,
		"FileFormat",
		N_("File Format"),
		N_("File format of the data described by this metadata."),
		IPTC_FORMAT_SHORT, IPTC_MANDATORY, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_FILE_VERSION,
		"FileVersion",
		N_("File Version"),
		N_("Version of the file format."),
		IPTC_FORMAT_SHORT, IPTC_MANDATORY, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_SERVICE_ID,
		"ServiceID",
		N_("Service Identifier"),
		N_("Identifies the provider and product."),
		IPTC_FORMAT_STRING, IPTC_MANDATORY, IPTC_NOT_REPEATABLE, 0, 10 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_ENVELOPE_NUM,
		"EnvelopeNum",
		N_("Envelope Number"),
		N_("A number unique for the date in 1:70 and the service ID in 1:30."),
		IPTC_FORMAT_NUMERIC_STRING, IPTC_MANDATORY, IPTC_NOT_REPEATABLE, 8, 8 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_PRODUCT_ID,
		"ProductID",
		N_("Product I.D."),
		N_("Allows a provider to identify subsets of its overall service."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_ENVELOPE_PRIORITY,
		"EnvelopePriority",
		N_("Envelope Priority"),
		N_("Specifies the envelope handling priority and not the editorial urgency.  '1' for most urgent, '5' for normal, and '8' for least urgent.  '9' is user-defined."),
		IPTC_FORMAT_NUMERIC_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 1, 1 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_DATE_SENT,
		"DateSent",
		N_("Date Sent"),
		N_("The day the service sent the material."),
		IPTC_FORMAT_DATE, IPTC_MANDATORY, IPTC_NOT_REPEATABLE, 8, 8 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_TIME_SENT,
		"TimeSent",
		N_("Time Sent"),
		N_("The time the service sent the material."),
		IPTC_FORMAT_TIME, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 11, 11 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_CHARACTER_SET,
		"CharacterSet",
		N_("Coded Character Set"),
		N_("Control functions used for the announcement, invocation or designation of coded character sets."),
		IPTC_FORMAT_BINARY, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_UNO,
		"UNO",
		N_("Unique Name of Object"),
		N_("An eternal, globally unique identification for the object, independent of provider and for any media form."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 14, 80 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_ARM_ID,
		"ARMID",
		N_("ARM Identifier"),
		N_("Identifies the Abstract Relationship Method (ARM)."),
		IPTC_FORMAT_SHORT, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_OBJECT_ENV,	IPTC_TAG_ARM_VERSION,
		"ARMVersion",
		N_("ARM Version"),
		N_("Identifies the version of the Abstract Relationship Method (ARM)."),
		IPTC_FORMAT_SHORT, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_RECORD_VERSION,
		"RecordVersion",
		N_("Record Version"),
		N_("Identifies the version of the IIM, Part 2"),
		IPTC_FORMAT_SHORT, IPTC_MANDATORY, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_OBJECT_TYPE,
		"ObjectType",
		N_("Object Type Reference"),
		N_("Distinguishes between different types of objects within the IIM."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 3, 67 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_OBJECT_ATTRIBUTE,
		"ObjectAttribute",
		N_("Object Attribute Reference"),
		N_("Defines the nature of the object independent of the subject."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 4, 68 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_OBJECT_NAME,
		"ObjectName",
		N_("Object Name"),
		N_("A shorthand reference for the object."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 64 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_EDIT_STATUS,
		"EditStatus",
		N_("Edit Status"),
		N_("Status of the object, according to the practice of the provider."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 64 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_EDITORIAL_UPDATE,
		"EditorialUpdate",
		N_("Editorial Update"),
		N_("Indicates the type of update this object provides to a previous object.  The link to the previous object is made using the ARM.  '01' indicates an additional language."),
		IPTC_FORMAT_NUMERIC_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_URGENCY,
		"Urgency",
		N_("Urgency"),
		N_("Specifies the editorial urgency of content and not necessarily the envelope handling priority.  '1' is most urgent, '5' normal, and '8' least urgent."),
		IPTC_FORMAT_NUMERIC_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 1, 1 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_SUBJECT_REFERENCE,
		"SubjectRef",
		N_("Subject Reference"),
		N_("A structured definition of the subject matter.  It must contain an IPR, an 8 digit Subject Reference Number and an optional Subject Name, Subject Matter Name, and Subject Detail Name each separated by a colon (:)."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 13, 236 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_CATEGORY,
		"Category",
		N_("Category"),
		N_("Identifies the subject of the object in the opinion of the provider (Deprecated)."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 3 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_SUPPL_CATEGORY,
		"SupplCategory",
		N_("Supplemental Category"),
		N_("Further refines the subject of the object (Deprecated)."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_FIXTURE_ID,
		"FixtureID",
		N_("Fixture Identifier"),
		N_("Identifies objects that recur often and predictably, enabling users to immediately find or recall such an object."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_KEYWORDS,
		"Keywords",
		N_("Keywords"),
		N_("Used to indicate specific information retrieval words."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 0, 64 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_CONTENT_LOC_CODE,
		"ContentLocCode",
		N_("Content Location Code"),
		N_("Indicates the code of a country/geographical location referenced by the content of the object."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 3, 3 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_CONTENT_LOC_NAME,
		"ContentLocName",
		N_("Content Location Name"),
		N_("A full, publishable name of a country/geographical location referenced by the content of the object."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 0, 64 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_RELEASE_DATE,
		"ReleaseDate",
		N_("Release Date"),
		N_("Designates the earliest date the provider intends the object to be used."),
		IPTC_FORMAT_DATE, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 8, 8 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_RELEASE_TIME,
		"ReleaseTime",
		N_("Release Time"),
		N_("Designates the earliest time the provider intends the object to be used."),
		IPTC_FORMAT_TIME, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 11, 11 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_EXPIRATION_DATE,
		"ExpirationDate",
		N_("Expiration Date"),
		N_("Designates the latest date the provider intends the object to be used."),
		IPTC_FORMAT_DATE, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 8, 8 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_EXPIRATION_TIME,
		"ExpirationTime",
		N_("Expiration Time"),
		N_("Designates the latest time the provider intends the object to be used."),
		IPTC_FORMAT_TIME, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 11, 11 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_SPECIAL_INSTRUCTIONS,
		"SpecialInstructions",
		N_("Special Instructions"),
		N_("Other editorial instructions concerning the use of the object."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 256 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_ACTION_ADVISED,
		"ActionAdvised",
		N_("Action Advised"),
		N_("The type of action that this object provides to a previous object.  '01' Object Kill, '02' Object Replace, '03' Object Append, '04' Object Reference."),
		IPTC_FORMAT_NUMERIC_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_REFERENCE_SERVICE,
		"RefService",
		N_("Reference Service"),
		N_("The Service Identifier of a prior envelope to which the current object refers."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 0, 10 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_REFERENCE_DATE,
		"RefDate",
		N_("Reference Date"),
		N_("The date of a prior envelope to which the current object refers."),
		IPTC_FORMAT_DATE, IPTC_OPTIONAL, IPTC_REPEATABLE, 8, 8 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_REFERENCE_NUMBER,
		"RefNumber",
		N_("Reference Number"),
		N_("The Envelope Number of a prior envelope to which the current object refers."),
		IPTC_FORMAT_NUMERIC_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 8, 8 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_DATE_CREATED,
		"DateCreated",
		N_("Date Created"),
		N_("The date the intellectual content of the object was created rather than the date of the creation of the physical representation."),
		IPTC_FORMAT_DATE, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 8, 8 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_TIME_CREATED,
		"TimeCreated",
		N_("Time Created"),
		N_("The time the intellectual content of the object was created rather than the date of the creation of the physical representation."),
		IPTC_FORMAT_TIME, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 11, 11 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_DIGITAL_CREATION_DATE,
		"DigitalCreationDate",
		N_("Digital Creation Date"),
		N_("The date the digital representation of the object was created."),
		IPTC_FORMAT_DATE, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 8, 8 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_DIGITAL_CREATION_TIME,
		"DigitalCreationTime",
		N_("Digital Creation Time"),
		N_("The time the digital representation of the object was created."),
		IPTC_FORMAT_TIME, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 11, 11 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_ORIGINATING_PROGRAM,
		"OriginatingProgram",
		N_("Originating Program"),
		N_("The type of program used to originate the object."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_PROGRAM_VERSION,
		"ProgramVersion",
		N_("Program Version"),
		N_("The version of the originating program."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 10 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_OBJECT_CYCLE,
		"ObjectCycle",
		N_("Object Cycle"),
		N_("Where 'a' is morning, 'b' is evening, 'b' is both."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 1, 1 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_BYLINE,
		"Byline",
		N_("By-line"),
		N_("Name of the creator of the object, e.g. writer, photographer or graphic artist."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_BYLINE_TITLE,
		"BylineTitle",
		N_("By-line Title"),
		N_("Title of the creator or creators of the object."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_CITY,
		"City",
		N_("City"),
		N_("City of object origin."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_SUBLOCATION,
		"Sublocation",
		N_("Sub-location"),
		N_("The location within a city from which the object originates."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_STATE,
		"State",
		N_("Province/State"),
		N_("The Province/State where the object originates."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_COUNTRY_CODE,
		"CountryCode",
		N_("Country Code"),
		N_("The code of the country/primary location where the object was created."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 3, 3 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_COUNTRY_NAME,
		"CountryName",
		N_("Country Name"),
		N_("The name of the country/primary location where the object was created."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 64 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_ORIG_TRANS_REF,
		"OrigTransRef",
		N_("Original Transmission Reference"),
		N_("A code representing the location of original transmission."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_HEADLINE,
		"Headline",
		N_("Headline"),
		N_("A publishable entry providing a synopsis of the contents of the object."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 256 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_CREDIT,
		"Credit",
		N_("Credit"),
		N_("Identifies the provider of the object, not necessarily the owner/creator."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_SOURCE,
		"Source",
		N_("Source"),
		N_("The original owner of the intellectual content of the object."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_COPYRIGHT_NOTICE,
		"CopyrightNotice",
		N_("Copyright Notice"),
		N_("Any necessary copyright notice."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 128 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_CONTACT,
		"Contact",
		N_("Contact"),
		N_("The person or organization which can provide further background information on the object."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 0, 128 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_CAPTION,
		"Caption",
		N_("Caption/Abstract"),
		N_("A textual description of the data"),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 2000 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_WRITER_EDITOR,
		"WriterEditor",
		N_("Writer/Editor"),
		N_("The name of the person involved in the writing, editing or correcting the object or caption/abstract"),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_REPEATABLE, 0, 32 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_RASTERIZED_CAPTION,
		"RasterizedCaption",
		N_("Rasterized Caption"),
		N_("Contains rasterized object description and is used where characters that have not been coded are required for the caption."),
		IPTC_FORMAT_BINARY, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 7360, 7360 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_IMAGE_TYPE,
		"ImageType",
		N_("Image Type"),
		N_("Indicates the data format of the image object."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_IMAGE_ORIENTATION,
		"ImageOrientation",
		N_("Image Orientation"),
		N_("The layout of the image area: 'P' for portrait, 'L' for landscape, and 'S' for square."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 1, 1 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_LANGUAGE_ID,
		"LanguageID",
		N_("Language Identifier"),
		N_("The major national language of the object, according to the 2-letter codes of ISO 639:1988."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 2, 3 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_AUDIO_TYPE,
		"AudioType",
		N_("Audio Type"),
		N_("The number of channels and type of audio (music, text, etc.) in the object."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_AUDIO_SAMPLING_RATE,
		"AudioSamplingRate",
		N_("Audio Sampling Rate"),
		N_("The sampling rate in Hz of the audio data."),
		IPTC_FORMAT_NUMERIC_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 6, 6 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_AUDIO_SAMPLING_RES,
		"AudioSamplingRes",
		N_("Audio Sampling Resolution"),
		N_("The number of bits in each audio sample."),
		IPTC_FORMAT_NUMERIC_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_AUDIO_DURATION,
		"AudioDuration",
		N_("Audio Duration"),
		N_("The running time of the audio data in the form HHMMSS."),
		IPTC_FORMAT_NUMERIC_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 6, 6 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_AUDIO_OUTCUE,
		"AudioOutcue",
		N_("Audio Outcue"),
		N_("The content at the end of the audio data."),
		IPTC_FORMAT_STRING, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 64 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_PREVIEW_FORMAT,
		"PreviewFileFormat",
		N_("Preview File Format"),
		N_("Binary value indicating the file format of the object preview data in dataset 2:202."),
		IPTC_FORMAT_SHORT, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_PREVIEW_FORMAT_VER,
		"PreviewFileFormatVer",
		N_("Preview File Format Version"),
		N_("The version of the preview file format specified in 2:200."),
		IPTC_FORMAT_SHORT, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 2, 2 },
	{ IPTC_RECORD_APP_2,		IPTC_TAG_PREVIEW_DATA,
		"PreviewData",
		N_("Preview Data"),
		N_("The object preview data"),
		IPTC_FORMAT_BINARY, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 0, 256000 },
	{ IPTC_RECORD_PREOBJ_DATA,	IPTC_TAG_SIZE_MODE,
		"SizeMode",
		N_("Size Mode"),
		N_("Set to 0 if the size of the object is known and 1 if not known."),
		IPTC_FORMAT_BYTE, IPTC_MANDATORY, IPTC_NOT_REPEATABLE, 1, 1 },
	{ IPTC_RECORD_PREOBJ_DATA,	IPTC_TAG_MAX_SUBFILE_SIZE,
		"MaxSubfileSize",
		N_("Max Subfile Size"),
		N_("The maximum size for a subfile dataset (8:10) containing a portion of the object data."),
		IPTC_FORMAT_LONG, IPTC_MANDATORY, IPTC_NOT_REPEATABLE, 4, 4 },
	{ IPTC_RECORD_PREOBJ_DATA,	IPTC_TAG_SIZE_ANNOUNCED,
		"ObjectSizeAnnounced",
		N_("Object Size Announced"),
		N_("The total size of the object data if it is known."),
		IPTC_FORMAT_LONG, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 4, 4 },
	{ IPTC_RECORD_PREOBJ_DATA,	IPTC_TAG_MAX_OBJECT_SIZE,
		"MaxObjectSize",
		N_("Maximum Object Size"),
		N_("The largest possible size of the object if the size is not known."),
		IPTC_FORMAT_LONG, IPTC_OPTIONAL, IPTC_NOT_REPEATABLE, 4, 4 },
	{ IPTC_RECORD_OBJ_DATA,		IPTC_TAG_SUBFILE,
		"Subfile",
		N_("Subfile"),
		N_("The object data itself.  Subfiles must be sequential so that the subfiles may be reassembled."),
		IPTC_FORMAT_BINARY, IPTC_MANDATORY, IPTC_REPEATABLE, 0, UINT32_MAX },
	{ IPTC_RECORD_POSTOBJ_DATA,	IPTC_TAG_CONFIRMED_DATA_SIZE,
		"ConfirmedDataSize",
		N_("Confirmed Data Size"),
		N_("Total size of the object data."),
		IPTC_FORMAT_LONG, IPTC_MANDATORY, IPTC_NOT_REPEATABLE, 4, 4 },
	{ 0, 0, NULL, NULL, NULL }
};


/**
 * iptc_tag_get_name:
 * @record: record number of tag
 * @tag: dataset number of tag
 *
 * Retrieves the name of a tag.  The name is derived from the IPTC
 * IIM specification and gives an indication of the purpose of a
 * particular tag.  The name will consist of ASCII characters with
 * no spaces, for example "CopyrightNotice".
 *
 * Returns: a static string containing the tag name, NULL if none found
 */
const char *
iptc_tag_get_name (IptcRecord record, IptcTag tag)
{
	unsigned int i;

	for (i = 0; IptcTagTable[i].record; i++)
		if (IptcTagTable[i].record == record && 
				IptcTagTable[i].tag == tag)
			break;

	return (IptcTagTable[i].name);
}

/**
 * iptc_tag_get_title:
 * @record: record number of tag
 * @tag: dataset number of tag
 *
 * Retrieves the title of a tag.  The title is similar to
 * the name, except it will be translated to the current
 * locale (if available) and may contain spaces, for example
 * "Copyright Notice".  It is appropriate for the title to appear
 * in user interfaces.  The return value will be encoding using the UTF-8
 * character set.
 *
 * Returns: a static string containing the tag title, empty string
 * if none found
 */
char *
iptc_tag_get_title (IptcRecord record, IptcTag tag)
{
	unsigned int i;

	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	bindtextdomain (GETTEXT_PACKAGE, LIBIPTCDATA_LOCALEDIR);

	for (i = 0; IptcTagTable[i].record; i++)
		if (IptcTagTable[i].record == record && 
				IptcTagTable[i].tag == tag)
			break;

	if (!IptcTagTable[i].title)
		return "";
	else
		return (_(IptcTagTable[i].title));
}

/**
 * iptc_tag_get_description:
 * @record: record number of tag
 * @tag: dataset number of tag
 *
 * Retrieves the description of a tag, which explains the purpose
 * of the tag from the IPTC IIM specification.  It is translated
 * to the current locale if available.  The return value will
 * be encoded using the UTF-8 character set.
 *
 * Returns: a static string containing the tag desciption, empty
 * string if none found
 */
char *
iptc_tag_get_description (IptcRecord record, IptcTag tag)
{
	unsigned int i;

	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	bindtextdomain (GETTEXT_PACKAGE, LIBIPTCDATA_LOCALEDIR);

	for (i = 0; IptcTagTable[i].record; i++)
		if (IptcTagTable[i].record == record && 
				IptcTagTable[i].tag == tag)
			break;

	if (!IptcTagTable[i].description)
		return "";
	else
		return (_(IptcTagTable[i].description));
}

/**
 * iptc_tag_get_info:
 * @record: record number of tag
 * @tag: dataset number of tag
 *
 * Retrieves the #IptcTagInfo structure containing information about the
 * tag from the IPTC IIM specification such as purpose, data format,
 * and minimum and maximum lengths.
 *
 * Returns: a pointer to a static #IptcTagInfo structure or NULL if
 * the tag is not found.
 */
const IptcTagInfo *
iptc_tag_get_info (IptcRecord record, IptcTag tag)
{
	unsigned int i;

	for (i = 0; IptcTagTable[i].record; i++)
		if (IptcTagTable[i].record == record && 
				IptcTagTable[i].tag == tag)
			break;

	if (IptcTagTable[i].record)
		return IptcTagTable+i;
	else
		return NULL;
}

/**
 * iptc_tag_find_by_name:
 * @name: the name to search for
 * @record: output variable to store the record number
 * @tag: output variable to store the tag number
 *
 * Looks up the record and tag numbers for a tag specified by name.  For
 * example, searching for "Caption" would return 2:120 in the record and
 * tag variables.
 *
 * Returns: 0 on success, -1 on failure or if the tag name was not found
 */
int
iptc_tag_find_by_name (const char * name, IptcRecord * record, IptcTag * tag)
{
	unsigned int i;

	if (!name || !record || !tag)
		return -1;

	for (i = 0; IptcTagTable[i].record; i++) {
		if (!strcmp (IptcTagTable[i].name, name)) {
			*record = IptcTagTable[i].record;
			*tag = IptcTagTable[i].tag;
			return 0;
		}
	}

	return -1;
}

/**
 * iptc_format_get_name:
 * @format: the format to be retrieved
 *
 * Retrieves a string version of a data format.  For example,
 * %IPTC_FORMAT_STRING returns "String".
 *
 * Returns: a static string in the native locale
 */
char *
iptc_format_get_name (IptcFormat format)
{
	bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
	bindtextdomain (GETTEXT_PACKAGE, LIBIPTCDATA_LOCALEDIR);

	switch (format) {
		case IPTC_FORMAT_BINARY:
			return _("Binary");
		case IPTC_FORMAT_BYTE:
			return _("Byte");
		case IPTC_FORMAT_SHORT:
			return _("Short");
		case IPTC_FORMAT_LONG:
			return _("Long");
		case IPTC_FORMAT_STRING:
			return _("String");
		case IPTC_FORMAT_NUMERIC_STRING:
			return _("NumString");
		case IPTC_FORMAT_DATE:
			return _("Date");
		case IPTC_FORMAT_TIME:
			return _("Time");
		default:
			return _("Unknown");
	}
}
