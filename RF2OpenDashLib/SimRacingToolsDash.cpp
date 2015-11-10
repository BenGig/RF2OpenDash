#include "..\include\SimRacingToolsDash.hpp"
#include "..\include\TelemetryData.hpp"
//#include "..\include\DataTransfer.hpp"

bool RfData::Update()
{
	VehicleScoringInfo* myVehicle;
	if (~rf.Read())
	{
		return false;
	}

	SessionRunning = rf.data.inSession;
	PlayerDriving = rf.data.driving;
	Track = rf.data.event.trackName;
	RaceLaps = rf.data.event.maxLaps;
	MaxLaps = rf.data.event.maxLaps;
	Cars = rf.data.event.numVehicles;
	IsRace = (rf.data.event.session > 9 && rf.data.event.session < 14);
	FlagGreen = (rf.data.event.gamePhase == 5) ;
	FlagFullCourseYellow = (rf.data.event.gamePhase == 6);;
	// TODO test if flags are interpreted correctly
	FlagsS1 = rf.data.event.sectorFlag[0];
	FlagsS2 = rf.data.event.sectorFlag[1];
	FlagsS3 = rf.data.event.sectorFlag[2];
	// TODO StartLightCount = number or current?
	StartLight = rf.data.event.startLight;
	StartLightCount = rf.data.event.numRedLights;
	// TODO Time vs. TimeClock ? Meaning?
	Time = rf.data.event.currentTime;
	TimeClock = rf.data.event.currentTime;
	TimeEnd = rf.data.event.endTime;
	TrackTemperature = rf.data.event.trackTemp;
	AmbientTemperature = rf.data.event.ambientTemp;
	Wetness_OnPath = rf.data.event.onPathWetness;
	Wetness_OffPath = rf.data.event.onPathWetness;
	Vehicle = rf.data.telemetry.vehicleName;
	LapStart = rf.data.telemetry.lapStartTime;
	Lap = rf.data.telemetry.lapNumber;
	Gear = rf.data.telemetry.gear;
	RPM = rf.data.telemetry.engineRPM;
	RPMMAX = rf.data.telemetry.engineMaxRPM;
	Water_Temp = rf.data.telemetry.engineWaterTemp;
	Oil_Temp = rf.data.telemetry.engineOilTemp;
	Acceleration = sqrt(pow(rf.data.telemetry.localAccel.x, 2) + pow(rf.data.telemetry.localAccel.y, 2) + pow(rf.data.telemetry.localAccel.z, 2));
	Brake = rf.data.telemetry.unfilteredBrake;
	Clutch = rf.data.telemetry.unfilteredClutch;
	Steering = rf.data.telemetry.unfilteredSteering;
	Fuel = rf.data.telemetry.fuel;
	Speed = sqrt(pow(rf.data.telemetry.localVel.x, 2) + pow(rf.data.telemetry.localVel.y, 2) + pow(rf.data.telemetry.localVel.z, 2));;
	LF_Speed = rf.data.telemetry.wheel[0].longitudinalGroundVel;
	LF_RideHeight = rf.data.telemetry.wheel[0].rideHeight;
	LF_BrakeTemp = rf.data.telemetry.wheel[0].brakeTemp;
	LF_TyreSlip = rf.data.telemetry.wheel[0].gripFract;
	LF_TyreLoad = rf.data.telemetry.wheel[0].tireLoad;
	LF_TyreTempIn = rf.data.telemetry.wheel[0].temperature[2];
	LF_TyreTempMid = rf.data.telemetry.wheel[0].temperature[1];
	LF_TyreTempOut = rf.data.telemetry.wheel[0].temperature[0];
	LF_TyrePressure = rf.data.telemetry.wheel[0].pressure;
	LF_TyreWear = rf.data.telemetry.wheel[0].wear;
	LF_TyreFlat = rf.data.telemetry.wheel[0].flat;
	LF_TyreMissing = rf.data.telemetry.wheel[0].detached;
	RF_Speed = rf.data.telemetry.wheel[1].longitudinalGroundVel;
	RF_RideHeight = rf.data.telemetry.wheel[1].rideHeight;;
	RF_BrakeTemp = rf.data.telemetry.wheel[1].brakeTemp;;
	RF_TyreSlip = rf.data.telemetry.wheel[1].gripFract;
	RF_TyreLoad = rf.data.telemetry.wheel[1].tireLoad;
	RF_TyreTempIn = rf.data.telemetry.wheel[1].temperature[0];
	RF_TyreTempMid = rf.data.telemetry.wheel[1].temperature[1];
	RF_TyreTempOut = rf.data.telemetry.wheel[1].temperature[2];
	RF_TyrePressure = rf.data.telemetry.wheel[1].pressure;
	RF_TyreWear = rf.data.telemetry.wheel[1].wear;
	RF_TyreFlat = rf.data.telemetry.wheel[1].flat;
	RF_TyreMissing = rf.data.telemetry.wheel[1].detached;
	RL_Speed = rf.data.telemetry.wheel[2].longitudinalGroundVel;
	RL_RideHeight = rf.data.telemetry.wheel[2].rideHeight;
	RL_BrakeTemp = rf.data.telemetry.wheel[2].brakeTemp;
	RL_TyreSlip = rf.data.telemetry.wheel[2].gripFract;
	RL_TyreLoad = rf.data.telemetry.wheel[2].tireLoad;
	RL_TyreTempIn = rf.data.telemetry.wheel[2].temperature[2];
	RL_TyreTempMid = rf.data.telemetry.wheel[2].temperature[1];
	RL_TyreTempOut = rf.data.telemetry.wheel[2].temperature[0];
	RL_TyrePressure = rf.data.telemetry.wheel[2].pressure;
	RL_TyreWear = rf.data.telemetry.wheel[2].wear;
	RL_TyreFlat = rf.data.telemetry.wheel[2].flat;
	RL_TyreMissing = rf.data.telemetry.wheel[2].detached;
	RF_Speed = rf.data.telemetry.wheel[3].longitudinalGroundVel;
	RR_RideHeight = rf.data.telemetry.wheel[3].rideHeight;
	RR_BrakeTemp = rf.data.telemetry.wheel[3].brakeTemp;
	RR_TyreSlip = rf.data.telemetry.wheel[3].gripFract;
	RR_TyreLoad = rf.data.telemetry.wheel[3].tireLoad;
	RR_TyreTempIn = rf.data.telemetry.wheel[3].temperature[0];
	RR_TyreTempMid = rf.data.telemetry.wheel[3].temperature[1];
	RR_TyreTempOut = rf.data.telemetry.wheel[3].temperature[2];
	RR_TyrePressure = rf.data.telemetry.wheel[3].pressure;
	RR_TyreWear = rf.data.telemetry.wheel[3].wear;
	RR_TyreFlat = rf.data.telemetry.wheel[3].flat;
	RR_TyreMissing = rf.data.telemetry.wheel[3].detached;
	EngineHot = rf.data.telemetry.overheating;

	for (int i = 0; i < rf.data.event.numVehicles; i++)
	{
		cars[i].Vehicle = rf.data.scoring[i].vehicleName;
		cars[i].IsPlayer = rf.data.scoring[i].isPlayer;
		cars[i].AIControl = (rf.data.scoring[i].control == 1);
		cars[i].InPits = rf.data.scoring[i].inPits;
		cars[i].Position = rf.data.scoring[i].place;
		cars[i].SplitBehind = rf.data.scoring[i].timeBehindNext;
		cars[i].LapsBehind = rf.data.scoring[i].lapsBehindNext;
		cars[i].SplitLeader = rf.data.scoring[i].timeBehindLeader;
		cars[i].LapsLeader = rf.data.scoring[i].lapsBehindLeader;
		cars[i].LapStartTime = rf.data.scoring[i].lapStartTime;
		cars[i].X = rf.data.scoring[i].pos.x;
		cars[i].Y = rf.data.scoring[i].pos.y;
		cars[i].Z = rf.data.scoring[i].pos.z;
	}
}

