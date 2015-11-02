#include "TelemetryData.hpp"
#include "InternalsPlugin.hpp"
#include "DataTransfer.hpp"
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

char* JsonVehicleScoring(VehicleScoringInfo* v)
{
	std::string buf;

	buf = "{ \"vehicle\": { ";
	buf += "\"driverName\": \"";
	buf += v->driverName; buf += "\",";
	buf += "\"vehicleName\": \"";
	buf += v->vehicleName; buf += "\",";
	buf += "\"bestLapTime\": \"";
	buf += v->bestLapTime; buf += "\",";
	buf += "\"bestSector1\": \"";
	buf += v->bestSector1; buf += "\",";
	buf += "\"bestSector2\": \"";
	buf += v->bestSector2; buf += "\",";
	buf += "\"curSector1\": \"";
	buf += v->curSector1; buf += "\",";
	buf += "\"curSector2\": \"";
	buf += v->curSector2; buf += "\",";
	buf += "\"estimatedLapTime\": \"";
	buf += v->estimatedLapTime; buf += "\",";
	buf += "\"finishStatus\": \"";
	buf += v->finishStatus; buf += "\",";
	buf += "\"inPits\": \"";
	buf += v->inPits; buf += "\",";
	buf += "\"isPlayer\": \"";
	buf += v->isPlayer; buf += "\",";
	buf += "\"lapDist\": \"";
	buf += v->lapDist; buf += "\",";
	buf += "\"lapsBehindLeader\": \"";
	buf += v->lapsBehindLeader; buf += "\",";
	buf += "\"lapsBehindNext\": \"";
	buf += v->lapsBehindNext; buf += "\",";
	buf += "\"lapStartTime\": \"";
	buf += v->lapStartTime; buf += "\",";
	buf += "\"lastLapTime\": \"";
	buf += v->lastLapTime; buf += "\",";
	buf += "\"lastSector1\": \"";
	buf += v->lastSector1; buf += "\",";
	buf += "\"lastSector2\": \"";
	buf += v->lastSector2; buf += "\",";

	buf += "}}";

	return buf.c_str;
}

char * TelemetryData::JsonScoring()
{
	VehicleScoringInfo* vehicles[100];
	for (int vh; vh < data.event.numVehicles; vh++)
	{
		for (int i; i < data.event.numVehicles; i++)
		{
			if (data.scoring[i].place == vh)
			{
				vehicles[vh] = &data.scoring[i];
			}
		}
	}



	using namespace Json;
	Value value = ...;
	StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	std::unique_ptr<Json::StreamWriter> writer(
		builder.newStreamWriter());
	return nullptr;
}
