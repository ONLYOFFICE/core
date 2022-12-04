x2t converter
=============

Нou can convert files from one format to another.

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
	<m_sCsvDelimiterChar> </m_sCsvDelimiterChar>

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