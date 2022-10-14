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

	# (non-required) input extensions of files
	# if no value here, sets all input extensions
	<input> </input>

	# (non-required) output extensions of files, for example docx
	# if no value here, sets all possible output extensions
	<output> </output>

	# input & output values example:
	<input> docx txt pptx xlsx <input>
	<output> txt doc pdf </output>


	

You can use the following template:

	<?xml version="1.0" encoding="utf-8"?>
	<settings>
		<reportPath> </reportPath>
		<inputDirectory> </inputDirectory>
		<outputDirectory> </outputDirectory>
		<x2tPath> </x2tPath>
		<cores> </cores>
		<input> </input>
		<output> </output>
	</settings>

USAGE
=====

Run a program with 1 argument - path to xml configuration file.