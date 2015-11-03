#include "../include/TelemetryData.hpp"
#include "../include/DataTransfer.hpp"
#include <stdio.h>
#include <tchar.h>
#include <string>

HANDLE hMapFile;
LPCTSTR pBuf;
UnifiedRfData data;

bool TelemetryData::Connect()
{
	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		transferBufferName);               // name of mapping object

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());
		return false;
	}

	pBuf = (LPTSTR)MapViewOfFile(hMapFile, // handle to map object
		FILE_MAP_READ,  // read permission
		0,
		0,
		transferBufferSize);

	if (pBuf == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(hMapFile);

		return false;
	}
	memcpy(&rFactorVersion, pBuf, sizeof(pBuf));
	return true;
}

TelemetryData::~TelemetryData()
{
	Disconnect();
}


bool TelemetryData::Read()
{
	if (pBuf != NULL)
	{
		try
		{
			memcpy(&data, pBuf, sizeof(data));
		}
		catch(...) { 
			Disconnect();
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

void TelemetryData::Disconnect()
{
	UnmapViewOfFile(pBuf);
	CloseHandle(hMapFile);
	rFactorVersion = 0;
}

