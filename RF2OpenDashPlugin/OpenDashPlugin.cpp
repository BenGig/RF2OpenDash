//‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹
//›                                                                         ﬁ
//› Write rFactor2 data to memory mapped file                               ﬁ
//›                                                                         ﬁ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

#include "OpenDashPlugin.hpp"
#include "DataTransfer.hpp"          // corresponding header file
#include <math.h>               // for atan2, sqrt
#include <stdio.h>              // for sample output
#include <process.h>
#include <Windows.h>
#include <stdio.h>
//#include <conio.h>
#include <tchar.h>

// plugin information

extern "C" __declspec(dllexport)
const char * __cdecl GetPluginName() { return("OpenDashPlugin"); }

extern "C" __declspec(dllexport)
PluginObjectType __cdecl GetPluginType() { return(PO_INTERNALS); }

extern "C" __declspec(dllexport)
int __cdecl GetPluginVersion() { return(6); } // InternalsPluginV01 functionality

extern "C" __declspec(dllexport)
PluginObject * __cdecl CreatePluginObject() { return((PluginObject *) new OpenDashPlugin); }

extern "C" __declspec(dllexport)
void __cdecl DestroyPluginObject(PluginObject *obj) { delete((OpenDashPlugin *)obj); }


void OpenDashPlugin::Log(const char * const msg)
{
	FILE *fo;

	fo = fopen("c:\\tmp\\rf2transferOutput.txt", "a");
	if (fo != NULL)
	{
		fprintf(fo, "%s\n", msg);
		fclose(fo);
	}
}

void OpenDashPlugin::Startup(long version)
{
	memMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		transferBufferSize,                // maximum object size (low-order DWORD)
		transferBufferName);                 // name of mapping object

	if (memMapFile == NULL)
	{
		Log("Could not open memory mapping file");
		return;
	}
	// Log("Memory mapping file opened");

	pBuf = (LPTSTR)MapViewOfFile(memMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		transferBufferSize);

	if (pBuf == NULL)
	{
		Log(TEXT("Could not map view of file.\n"));

		CloseHandle(memMapFile);
		return;
	}

	data.scoringValid = false;
	data.telemetryValid = false;

	return;
}


void OpenDashPlugin::Shutdown()
{
	UnmapViewOfFile(pBuf);
	CloseHandle(memMapFile);
}


void OpenDashPlugin::EnterRealtime()
{
	// start up timer every time we enter realtime
	data.driving = true;
	data.telemetryValid = true;
	WriteDataToMemory();
}


void OpenDashPlugin::ExitRealtime()
{
	data.driving = false;
	WriteDataToMemory();
}


void OpenDashPlugin::UpdateTelemetry(const TelemInfoV01 &info)
{
	// Log("Updating telemetry");
	CopyMemory(&data.telemInfoV01, &info, sizeof(info));
	WriteDataToMemory();
}


void OpenDashPlugin::UpdateScoring(const ScoringInfoV01 &info)
{
	// Note: function is called twice per second now (instead of once per second in previous versions)

	data.scoringValid = true;

	CopyMemory(&data.scoringInfoV01, &info, sizeof(info));
	WriteDataToMemory();
}

void OpenDashPlugin::WriteDataToMemory()
{
	CopyMemory((PVOID)pBuf, &data, sizeof(data));
}