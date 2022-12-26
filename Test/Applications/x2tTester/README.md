CONFIGURATION
=============

You need to create an xml configuration file. It must contain:

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

	# (non-required) timestamp in report file name (default - 1)
	<timestamp> </timestamp>

	# (non-required) is take params for csv/txt conversions from filename (default - 1).
	<filenameCsvTxtParams> </filenameCsvTxtParams>

		# params in any order, in any place, delimiter is unicode code in hex
		# encoding is case sensitive
		# input file with csv:
		filename[cp(codepage)][del%(unicode code)]
		# example:
		some_csv_file[cpUTF-8][del%3b].csv

		# same with txt file, but only encoding:
		filename[cp(codepage)]
		# example
		some_txt_file[cpUTF-8].txt

	# (non-required) default encoding for csv/txt files, if filenameCsvTxtParams is 0, or no param in filename (default - UTF-8)
	<defaultCsvTxtEncoding> </defaultCsvTxtEncoding>

	# (non-required) default unicode code delimiter for csv files, if filenameCsvTxtParams is 0, or no param in filename (default - 3b)
	<defaultCsvDelimiter> </defaultCsvDelimiter>

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
	

You can use the following templates:

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