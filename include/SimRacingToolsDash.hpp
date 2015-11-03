#pragma once

#include "TelemetryData.hpp"
#include "OpenDash.hpp"

class RfData: public OpenDash
{
public:
	RfData() {};
	~RfData() {};

	bool Update();

	struct car
	{
		char * Vehicle;
		bool IsPlayer;
		bool AIControl;
		bool InPits;
		int Position;
		float SplitBehind;
		int LapsBehind;
		float SplitLeader;
		int LapsLeader;
		float LapStartTime;
		float X;
		float Y;
		float Z;
	};

	bool SessionRunning;
	bool PlayerDriving;
	char * Track;
	int RaceLaps;
	int MaxLaps;
	int Cars;
	bool IsRace;
	bool FlagGreen;
	bool FlagFullCourseYellow;
	bool FlagsS1;
	bool FlagsS2;
	bool FlagsS3;
	int StartLight;
	int StartLightCount;
	float Time;
	float TimeClock;
	float TimeEnd;
	float TrackTemperature;
	float AmbientTemperature;
	float Wetness_OnPath;
	float Wetness_OffPath;
	char * Vehicle;
	float LapStart;
	int Lap;
	int Gear;
	float RPM;
	float RPMMAX;
	float Water_Temp;
	float Oil_Temp;
	float Acceleration;
	float Brake;
	float Clutch;
	float Steering;
	float Fuel;
	float Speed;
	float LF_Speed;
	float LF_RideHeight;
	float LF_BrakeTemp;
	float LF_TyreSlip;
	float LF_TyreLoad;
	float LF_TyreTempIn;
	float LF_TyreTempMid;
	float LF_TyreTempOut;
	float LF_TyrePressure;
	float LF_TyreWear;
	bool LF_TyreFlat;
	bool LF_TyreMissing;
	float RF_Speed;
	float RF_RideHeight;
	float RF_BrakeTemp;
	float RF_TyreSlip;
	float RF_TyreLoad;
	float RF_TyreTempIn;
	float RF_TyreTempMid;
	float RF_TyreTempOut;
	float RF_TyrePressure;
	float RF_TyreWear;
	bool RF_TyreFlat;
	bool RF_TyreMissing;
	float RL_Speed;
	float RL_RideHeight;
	float RL_BrakeTemp;
	float RL_TyreSlip;
	float RL_TyreLoad;
	float RL_TyreTempIn;
	float RL_TyreTempMid;
	float RL_TyreTempOut;
	float RL_TyrePressure;
	float RL_TyreWear;
	bool RL_TyreFlat;
	bool RL_TyreMissing;
	float RR_Speed;
	float RR_RideHeight;
	float RR_BrakeTemp;
	float RR_TyreSlip;
	float RR_TyreLoad;
	float RR_TyreTempIn;
	float RR_TyreTempMid;
	float RR_TyreTempOut;
	float RR_TyrePressure;
	float RR_TyreWear;
	bool RR_TyreFlat;
	bool RR_TyreMissing;
	bool EngineHot;

	car cars[100];

private:
	TelemetryData rf;
};
