CONFIGURATION
=============

You need to create an xml configuration file. It must contain:

	# root of xml
	<settings> </settings>

	# report.csv path
	<reportPath> </reportPath>

	# directory with test documents
	<inputDirectory> </inputDirectory>

	# results directory
	<outputDirectory> </outputDirectory>

	# path to x2t
	<x2tPath> </x2tPath>

	# num cores to use
	<cores> </cores>

	# reports only errors (0 or 1)
	<errorsOnly> </errorsOnly>

	# fonts settings
	# if no node - using system fonts
	# you can set "system" param to "0" or "1",
	# if "1", set additional directory for fonts
	<fonts system="">
		<directory> </directory>
	</fonts>

	# (non-required) input extensions of files
	# if no this node, sets all input extensions
	<input> </input>

	# (non-required) output extensions of files, for example docx
	# if no this node, sets all possible output extensions
	<output> </output>

	# input & output values example:
	<input> docx txt pptx xlsx<input>
	<output> txt doc pdf</output>


	

You can use the following template:

	<?xml version="1.0" encoding="utf-8"?>
	<settings>
		<reportPath> </reportPath>
		<inputDirectory> </inputDirectory>
		<outputDirectory> </outputDirectory>
		<x2tPath> </x2tPath>
		<cores> </cores>
		<onlyErrors> </onlyErrors>
		<fonts system="">
			<directory> </directory>
		</fonts>
		<input> </input>
		<output> </output>
	</settings>

USAGE
=====

Run a program with 1 argument - path to xml configuration file.