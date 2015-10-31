#pragma once

//‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹‹
//›                                                                         ﬁ
//› Module: Data Transfer Header File                                       ﬁ
//›                                                                         ﬁ
//› Description: Exchange data written in shared memory                     ﬁ
//›                                                                         ﬁ
//ﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂﬂ

#include "InternalsPlugin.hpp"

// no more valid!
//struct TransferredData
//{
//	long rFactorVersion;
//	bool telemetryValid;
//	bool scoringValid;
//	bool driving;
//	bool inSession;
//	ScoringInfoV01 scoringInfoV01;
//	TelemInfoV01 telemInfoV01;
//	VehicleScoringInfoV01 vehicleScoringInfoV01[100];
//};

// One data set for each car
struct VehicleScoringInfo
{
	bool isPlayer;
	char driverName[32];
	char vehicleName[64];
	short totalLaps;
	signed char sector; // 0=sector3, 1=sector1, 2=sector2 (don't ask why)
	signed char finishStatus; // 0=none, 1=finished, 2=dnf, 3=dq
	double lapDist;
	double bestSector1;
	double bestSector2;
	double bestLapTime;
	double lastSector1;
	double lastSector2;
	double lastLapTime;
	double curSector1;
	double curSector2;
	short numPitstops;
	short numPenalties;
	bool inPits;
	unsigned char place;
	double timeBehindNext;
	long lapsBehindNext;
	double timeBehindLeader;
	long lapsBehindLeader;
	double lapStartTime;
	double speed;

	// Extension for rFactor 2
	unsigned char headlights;
	unsigned char pitState; // 0=none, 1=request, 2=entering, 3=stopped, 4=exiting
	long qualification;
	unsigned char primaryFlag; // currently only 0=green or 6=blue
};

// Part of player telemetry
struct WheelInfo
{
	double tireLoad;  // Newton
	double gripFract;              // an approximation of what fraction of the contact patch is sliding
	double pressure;  // kPa
	double temperature[3]; // Kelvin, left/center/right (not inside/center/outside!)
	double wear;
	bool flat;
	bool detached;
	double brakeTemp; // Celsius
};

struct TelemetryInfo
{
	long lapNumber;
	double lapStartTime;
	char vehicleName[64];
	char trackName[64];
	long gear;
	double engineRPM;
	double engineWaterTemp;
	double engineOilTemp;
	double clutchRPM;
	double unfilteredThrottle;
	double unfilteredBrake;
	double unfilteredSteering;
	double unfilteredClutch;
	WheelInfo wheel[4]; // front left, front right, rear left, rear right
	double fuel;
	double engineMaxRPM; // rev limit
	unsigned char scheduledPitstops;
	bool overheating;
	bool detached;               // whether any parts (besides wheels) have been detached
	unsigned char dentSeverity[8]; // dent severity at 8 locations around the car (0=none, 1=some, 2=more)

	// Additional data provided by rFactor 2
	double filteredThrottle;
	double filteredBrake;
	double filteredSteering;
	double filteredClutch;
	bool headlights;
	unsigned char speedLimiter;   // whether speed limiter is on
	unsigned char maxGears;       // maximum forward gears
	unsigned char frontTireCompoundIndex;   // index within brand
	unsigned char rearTireCompoundIndex;    // index within brand
	double fuelCapacity;          // capacity in liters
	unsigned char frontFlapActivated;       // whether front flap is activated
	unsigned char rearFlapActivated;        // whether rear flap is activated
	unsigned char rearFlapLegalStatus;      // 0=disallowed, 1=criteria detected but not allowed quite yet, 2=allowed
	unsigned char ignitionStarter;          // 0=off 1=ignition 2=ignition+starter
	char frontTireCompoundName[18];         // name of front tire compound
	char rearTireCompoundName[18];          // name of rear tire compound
	unsigned char speedLimiterAvailable;    // whether speed limiter is available
	unsigned char antiStallActivated;       // whether (hard) anti-stall is activated
	double rearBrakeBias;                   // fraction of brakes on rear
	double turboBoostPressure;              // current turbo boost pressure if available
};

struct EventInfo
{
	// Event info
	char trackName[64]; //char[64]
	long session; // 0=testday 1-4=practice 5-8=qual 9=warmup 10-13=race
	double currentTime;
	double endTime;
	long maxLaps;
	double lapDist;
	long numVehicles;
	unsigned char gamePhase;
	signed char yellowFlagState;
	signed char sectorFlag[3];
	bool inRealtime; // driving vs. monitor
	char playerName[32];

	double darkCloud;
	double raining;
	double ambientTemp;
	double trackTemp;
	double onPathWetness;
	double offPathWetness;
};

TCHAR transferBufferName[] = TEXT("\rf2transferMappingObject");
int transferBufferSize = 64000;


// Written to shared memory each time rFactor updates values
struct UnifiedRfData
{
	long rFactorVersion;		// 1 or 2
	bool telemetryValid;		// telemetry data received at least once
	bool scoringValid;			// scoring data received at least once
	bool driving;				// on track
	bool inSession;				// session active
	EventInfo event;			// event and environment
	TelemetryInfo telemetry;	// detailed player info
	VehicleScoringInfo scoring[100];	// all about opponents
};

