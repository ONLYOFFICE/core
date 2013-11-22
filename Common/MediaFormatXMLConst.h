#pragma once

static LPCTSTR g_cpszXMLSettingParamName		= _T("XMLSetting");
static LPCTSTR g_cpszCLSIDParamName				= _T("CLSID");

static LPCTSTR g_cpszCLSIDNodeName				= _T("CLSID");

//Node в который сохраняются свойства Format-а
static LPCTSTR g_cpszSettingNodeName			= _T("Setting");
//SubNode в который сохраняются свойства, которые можно сохранить автоматически из IDispatch
static LPCTSTR g_cpszStandardSettingNodeName	= _T("StandardSetting");
//SubNode в который сохраняются свойства, для чтения и записи которых надо знать конкретный формат
static LPCTSTR g_cpszExtendedSettingNodeName	= _T("ExtendedSetting");

//AVSMPEG4VideoFormat
static LPCTSTR g_cpszZonesNodeName						= _T("Zones");
static LPCTSTR g_cpszZonesCountNodeName					= _T("ZonesCount");
static LPCTSTR g_cpszZoneTitleFormatNodeName			= _T("Zone%d");
static LPCTSTR g_cpszZoneFrameNodeName					= _T("ZoneFrame");
static LPCTSTR g_cpszZoneModeNodeName					= _T("ZoneMode");
static LPCTSTR g_cpszZoneWeightNodeName					= _T("ZoneWeight");
static LPCTSTR g_cpszZoneQuantNodeName					= _T("ZoneQuant");
static LPCTSTR g_cpszZoneFrameTypeNodeName				= _T("ZoneFrameType");
static LPCTSTR g_cpszZoneGrayscaleEnabledNodeName		= _T("ZoneGrayscaleEnabled");
static LPCTSTR g_cpszZoneChromaOptimizerEnabledNodeName	= _T("ZoneChromaOptimizerEnabled");
static LPCTSTR g_cpszZoneBVOPThresholdNodeName			= _T("ZoneBVOPThreshold");
static LPCTSTR g_cpszZoneCartoonModeEnabledNodeName		= _T("ZoneCartoonModeEnabled");

