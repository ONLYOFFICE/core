CONFIGURATION
=============

You need to create an xml configuration file. It must contain:

	<settings>			# root of xml
	<reportPath>		# report.csv path
	<inputFolder>		# folder with test documents
	<outputFolder>		# results folder
	<x2tPath>			# path to x2t
	<*>					# input format with output formats
	

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