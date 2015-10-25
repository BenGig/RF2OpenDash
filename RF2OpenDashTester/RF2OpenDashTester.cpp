// RF2OpenDashTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "InternalsPlugin.hpp"
#include "DataTransfer.hpp"
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
// #pragma comment(lib, "user32.lib")

int main()
{
	TransferredData data;

	HANDLE hMapFile;
	LPCTSTR pBuf;

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		transferBufferName);               // name of mapping object

	if (hMapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());
		return 1;
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

		return 1;
	}

	memcpy(&data, pBuf, sizeof(data));

	printf(data.scoringInfoV01.mTrackName);
	//MessageBox(NULL, pBuf, info.mTrackName, MB_OK);

	UnmapViewOfFile(pBuf);

	CloseHandle(hMapFile);

	return 0;
}

