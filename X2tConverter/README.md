x2t converter
=============

You can convert files from one format to another.

# CONFIGURATION
You need to create the xml configuration file.

	# first line contains information about xml, for exmample:
	"<?xml version=\"1.0\" encoding=\"utf-8\"?>

	# next you need to create some root tag and close this in the end, for example:
	<TaskQueueDataConvert>
		...
	</TaskQueueDataConvert>

## Required fields:

	# (Required) path to the file to convert from
	<m_sFileFrom> </m_sFileFrom>

	# (Required) path to the file to convert to
	<m_sFileTo> </m_sFileTo>

	# (Required, if no extension in filename) the to convert from
	<m_nFormatFrom> </m_nFormatFrom>

	# (Required, if no extension in filename) the to convert to
	<m_nFormatTo> </m_nFormatTo>

Information about formats you can find [here](https://github.com/ONLYOFFICE/core/blob/master/Common/OfficeFileFormats.h).

## Non-required fields:

	# configuration for .csv files
	<m_nCsvTxtEncoding> </m_nCsvTxtEncoding>
	<m_nCsvDelimiter> </m_nCsvDelimiter>
	<m_nCsvDelimiterChar> </m_nCsvDelimiterChar>

	# spreadsheet to .csv/.tsv: every sheet, not only the active one (true/false, default false).
	# The result at m_sFileTo is then a zip, as for the images of all pages: one file per sheet,
	# sheet<N>.<name>.csv (.tsv when tab-separated), e.g. sheet1.Names.csv, sheet2.Q1_2025.csv:
	# N is the sheet's place in the workbook, counting hidden sheets; <name> is the sheet's name
	# with white space turned into _ and anything a file name may not hold dropped (of ASCII only
	# letters, digits, _ and - are kept), at most 64 characters; with nothing left it is sheet<N>.csv.
	# Give m_nFormatTo explicitly - the format is not taken from the .zip extension.
	<m_bCsvAllSheets> </m_bCsvAllSheets>

	# save additional information about conversion or not
	<m_bDontSaveAdditional> </m_bDontSaveAdditional>

	# path to AllFonts.js file
	<m_sAllFontsPath> </m_sAllFontsPath>

	# path to fonts (usually uses with CApplicationFontsWorker)
	<m_sFontDir> </m_sFontDir>

	# config for image formats
	<m_oThumbnail>

		# format of image
		<format> </format>

		# Aspect of image
		<aspect> </aspect>

		# only first page or not
		<first> </first>

		# zip images or not
		<zip> </zip>

		# width of image
		<width> </width>

		# height of image
		<height> </height>

	</m_oThumbnail>

	# additional JSON params
	<m_sJsonParams> </m_sJsonParams>

# USAGE
Just run x2t with 1 arg - path to created xml configuration file.