CONFIGURATION
=============

You need to create an xml configuration file. It must contain:

	# root of xml
	<settings> </settings>

	# report.csv path
	<reportPath> </reportPath>

	# folder with test documents
	<inputFolder> </inputFolder>

	# results folder
	<outputFolder> </outputFolder>

	# path to x2t
	<x2tPath> </x2tPath>

	# input format with output formats
	<*>
		<item> </item>
	</*>
	

You can use the following template:

	<?xml version="1.0" encoding="utf-8"?>
	<settings>
	<reportPath> </reportPath>
	<inputFolder> </inputFolder>
	<outputFolder> </outputFolder>
	<x2tPath> </x2tPath>
	<docx>
		<item>txt</item>
		<item>odt</item>
	</docx>
	</settings>

USAGE
=====

Run a program with 1 argument - path to xml configuration file.