CONFIGURATION
=============

## Default conversion

You need to create an xml configuration file. It contains:

	# root of xml
	<settings> </settings>

	# report (.csv) path
	<reportPath> </reportPath>

	# directory with test documents
	<inputDirectory> </inputDirectory>

	# results directory
	<outputDirectory> </outputDirectory>

	# path to x2t
	<x2tPath> </x2tPath>

	# num cores to use
	<cores> </cores>

	# (non-required) reports only errors (default - 0)
	<errorsOnly> </errorsOnly>

	# (non-required) is delete successful conversions files (default - 0)
	<deleteOk> </deleteOk>

	# (non-required) trough conversion (format) -> (*t format) -> (output formats) (default - 0). Directory with *t files - outputDirectory/_t.
	<troughConversion> </troughConversion>

	# (non-required) save environment vars to x2t (for example "X2T_MEMORY_LIMIT") (default - 0). 
	<saveEnvironment> </saveEnvironment>

	# (non-required) timestamp in report file name (default - 1)
	<timestamp> </timestamp>

	# (non-required) is take params for csv/txt conversions from filename (default - 1).
	<filenameCsvTxtParams> </filenameCsvTxtParams>

		# params in any order, in any place, delimiter is unicode code in hex
		# encoding is case sensitive
		# usage csv:
		filename[cp(codepage)][del%(unicode code)]
		# example:
		some_csv_file[cpUTF-8][del%3b].csv

		# usage txt:
		filename[cp(codepage)]
		# example
		some_txt_file[cpUTF-8].txt

	# (non-required) is take password from filename (default - 1).
	<filenamePassword> </filenamePassword>

		# in any order, in any place
		# usage:
		filename[pass(password)]

		# example:
		some_file[pass123].txt

	# (non-required) default encoding for csv/txt files, if filenameCsvTxtParams is 0, or no param in filename (default - UTF-8)
	<defaultCsvTxtEncoding> </defaultCsvTxtEncoding>

	# (non-required) default unicode code delimiter for csv files, if filenameCsvTxtParams is 0, or no param in filename (default - 3b)
	<defaultCsvDelimiter> </defaultCsvDelimiter>

	# (non-required) timeout in seconds (default - 300). Set to 0 to disable timeouts
	<timeout> </timeout>

	# (non-required) path to xml file with a list of input files
	<inputFilesList> </inputFilesList>

		# struct of inputFilesList xml file
		<files>
			<file> </file>
		</files>

	# (non-required) fonts settings (default - "1")
	# you can set "system" param to "0" or "1",
	# if "0", set additional directory for fonts
	<fonts system="">
		<directory> </directory>
	</fonts>

	# (non-required) input extensions of files (default - all possible input extensions)
	<input> </input>

	# (non-required) output extensions of files (default - all possible output extensions)
	<output> </output>

	# input & output values example:
	<input> docx txt pptx xlsx<input>
	<output> txt doc pdf</output>
	
## Extraction
x2ttester can extract files with the required output extension instead of default x2t conversion. Set extraction mode:

	(non-required) sets extraction mode (default - "0")
	<extract> </extract>

When `extract` is "1", you can set the `output` parameter to determine which exts will be extracted. Default `output` is `emf wmf`.
Params `input`, `inputDirectory`, `outputDirectory`, `cores` works the same.

Extract mode has additional options:

	(non-required) converts non-zip office files into docx (e.g. pdf) (default - "0").
	<convertBeforeExtract> </convertBeforeExtract>

The conversion params in `convertBeforeExtract` are the same as the default conversion.

## Templates

	# main xml config

	<?xml version="1.0" encoding="utf-8"?>
	<settings>
		<reportPath> </reportPath>
		<inputDirectory> </inputDirectory>
		<outputDirectory> </outputDirectory>
		<x2tPath> </x2tPath>
		<cores> </cores>
		<errorsOnly> </errorsOnly>
		<deleteOk> </deleteOk>
		<timestamp> </timestamp>
		<timeout> </timeout>
		<fonts system="">
			<directory> </directory>
		</fonts>
		<input> </input>
		<output> </output>
	</settings>

	# inputFilesList xml
	
	<?xml version="1.0" encoding="utf-8"?>
	<files>
		<file> </file>
	</files>

USAGE
=====

Run a program with 1 argument - path to xml configuration file.