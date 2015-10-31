//‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹
//›                                                                         ﬁ
//› Write rFactor2 data to memory mapped file                               ﬁ
//›                                                                         ﬁ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

#include "OpenDashPlugin.hpp"
#include "DataTransfer.hpp"         
#include <stdio.h>              // for sample output
#include <Windows.h>
//#include <conio.h>

const long OpenDashPlugin::rFactorVersion = 2;

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
	data.inSession = false;
	data.rFactorVersion = rFactorVersion;


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

void OpenDashPlugin::StartSession()
{
	data.inSession = true;
	WriteDataToMemory();
}

void OpenDashPlugin::EndSession()
{
	data.inSession = false;
	WriteDataToMemory();
}


void OpenDashPlugin::UpdateTelemetry(const TelemInfoV01 &info)
{
	// Log("Updating telemetry");
	data.telemetryValid = true;

	data.telemetry.lapNumber = info.mLapNumber;
	data.telemetry.lapStartTime = info.mLapStartET;
	strcpy(data.telemetry.vehicleName, info.mVehicleName);
	strcpy(data.telemetry.trackName, info.mTrackName);
	data.telemetry.gear = info.mGear;
	data.telemetry.engineRPM = info.mEngineRPM;
	data.telemetry.engineWaterTemp = info.mEngineWaterTemp;
	data.telemetry.engineOilTemp = info.mEngineOilTemp;
	data.telemetry.clutchRPM = info.mClutchRPM;
	data.telemetry.unfilteredThrottle = info.mUnfilteredThrottle;
	data.telemetry.unfilteredBrake = info.mUnfilteredBrake;
	data.telemetry.unfilteredSteering = info.mUnfilteredSteering;
	data.telemetry.unfilteredClutch = info.mUnfilteredClutch;
	memcpy(data.telemetry.wheel, info.mWheel, sizeof(info.mWheel)); 
	data.telemetry.fuel = info.mFuel;
	data.telemetry.engineMaxRPM = info.mEngineMaxRPM;
	data.telemetry.scheduledPitstops = info.mScheduledStops;
	data.telemetry.overheating = info.mOverheating;
	data.telemetry.detached = info.mDetached;  
	memcpy(data.telemetry.dentSeverity, info.mDentSeverity, sizeof(info.mDentSeverity));
	
	// Additional data provided by rFactor 2
	data.telemetry.filteredThrottle = info.mFilteredThrottle;
	data.telemetry.filteredBrake = info.mFilteredBrake;
	data.telemetry.filteredSteering = info.mFilteredSteering;
	data.telemetry.filteredClutch = info.mFilteredClutch;
	data.telemetry.headlights = info.mHeadlights;
	data.telemetry.speedLimiter = info.mSpeedLimiter;
	data.telemetry.maxGears = info.mMaxGears;
	data.telemetry.frontTireCompoundIndex = info.mFrontTireCompoundIndex;
	data.telemetry.rearTireCompoundIndex = info.mRearTireCompoundIndex;
	data.telemetry.fuelCapacity = info.mFuelCapacity;
	data.telemetry.frontFlapActivated = info.mFrontFlapActivated;
	data.telemetry.rearFlapActivated = info.mRearFlapActivated;
	data.telemetry.rearFlapLegalStatus = info.mRearFlapLegalStatus;
	data.telemetry.ignitionStarter = info.mIgnitionStarter;
	strcpy(data.telemetry.frontTireCompoundName, info.mFrontTireCompoundName);
	strcpy(data.telemetry.rearTireCompoundName, info.mRearTireCompoundName);
	data.telemetry.speedLimiterAvailable = info.mSpeedLimiterAvailable;
	data.telemetry.antiStallActivated = info.mAntiStallActivated;
	data.telemetry.rearBrakeBias = info.mRearBrakeBias; 
	data.telemetry.turboBoostPressure = info.mTurboBoostPressure;

	WriteDataToMemory();
}


void OpenDashPlugin::UpdateScoring(const ScoringInfoV01 &info)
{
	// Note: function is called twice per second now (instead of once per second in previous versions)

	data.scoringValid = true;

	strcpy(data.event.trackName, info.mTrackName);
	data.event.session = info.mSession;
	data.event.currentTime = info.mCurrentET;
	data.event.endTime = info.mEndET;
	data.event.maxLaps = info.mMaxLaps;
	data.event.lapDist = info.mLapDist;
	data.event.numVehicles = info.mNumVehicles;
	data.event.gamePhase = info.mGamePhase;
	data.event.yellowFlagState = info.mYellowFlagState;
	memcpy(data.event.sectorFlag, info.mSectorFlag, sizeof(info.mSectorFlag));
	data.event.inRealtime = info.mInRealtime;
	strcpy(data.event.playerName, info.mPlayerName);
	data.event.darkCloud = info.mDarkCloud;
	data.event.raining = info.mRaining;
	data.event.ambientTemp = info.mAmbientTemp;
	data.event.trackTemp = info.mTrackTemp;
	data.event.onPathWetness = info.mOnPathWetness;
	data.event.offPathWetness = info.mOffPathWetness;

	for (int i = 0; i++; i < info.mNumVehicles)
	{
		data.scoring[i].isPlayer = info.mVehicle[i].mIsPlayer;
		strcpy(data.scoring[i].driverName, info.mVehicle[i].mDriverName);
		strcpy(data.scoring[i].vehicleName, info.mVehicle[i].mVehicleName);
		data.scoring[i].totalLaps = info.mVehicle[i].mTotalLaps;
		data.scoring[i].sector = info.mVehicle[i].mSector; // 0=sector3, 1=sector1, 2=sector2 (don't ask why)
		data.scoring[i].finishStatus = info.mVehicle[i].mFinishStatus; // 0=none, 1=finished, 2=dnf, 3=dq
		data.scoring[i].lapDist = info.mVehicle[i].mLapDist;
		data.scoring[i].bestSector1 = info.mVehicle[i].mBestSector1;
		data.scoring[i].bestSector2 = info.mVehicle[i].mBestSector2;
		data.scoring[i].bestLapTime = info.mVehicle[i].mBestLapTime;
		data.scoring[i].lastSector1 = info.mVehicle[i].mLastSector1;
		data.scoring[i].lastSector2 = info.mVehicle[i].mLastSector2;
		data.scoring[i].lastLapTime = info.mVehicle[i].mLastLapTime;
		data.scoring[i].curSector1 = info.mVehicle[i].mCurSector1;
		data.scoring[i].curSector2 = info.mVehicle[i].mCurSector2;
		data.scoring[i].numPitstops = info.mVehicle[i].mNumPitstops;
		data.scoring[i].numPenalties = info.mVehicle[i].mNumPenalties;
		data.scoring[i].inPits = info.mVehicle[i].mInPits;
		data.scoring[i].place = info.mVehicle[i].mPlace;
		data.scoring[i].timeBehindNext = info.mVehicle[i].mTimeBehindNext;
		data.scoring[i].lapsBehindNext = info.mVehicle[i].mLapsBehindNext;
		data.scoring[i].timeBehindLeader = info.mVehicle[i].mTimeBehindLeader;
		data.scoring[i].lapsBehindLeader = info.mVehicle[i].mLapsBehindLeader;
		data.scoring[i].lapStartTime = info.mVehicle[i].mLapStartET;
		data.scoring[i].speed = sqrtl(powl(info.mVehicle[i].mLocalVel.x, 2) * powl(info.mVehicle[i].mLocalVel.y, 2) * powl(info.mVehicle[i].mLocalVel.z, 2));
		
		// Extension for rFactor 2
		data.scoring[i].headlights = info.mVehicle[i].mHeadlights;
		data.scoring[i].pitState = info.mVehicle[i].mPitState; // 0=none, 1=request, 2=entering, 3=stopped, 4=exiting
		data.scoring[i].qualification = info.mVehicle[i].mQualification;
		data.scoring[i].primaryFlag = info.mVehicle[i].mFlag; // currently only 0=green or 6=blue

	}

	WriteDataToMemory();
}

void OpenDashPlugin::WriteDataToMemory()
{
	CopyMemory((PVOID)pBuf, &data, sizeof(data));
}