#pragma once

//���������������������������������������������������������������������������
//�                                                                         �
//� Module: Data Transfer Header File                                       �
//�                                                                         �
//� Description: Exchange data written in shared memory                     �
//�                                                                         �
//���������������������������������������������������������������������������

#include "InternalsPlugin.hpp"

struct TransferredData
{
	bool telemetryValid;
	bool scoringValid;
	bool driving;
	ScoringInfoV01 scoringInfoV01;
	TelemInfoV01 telemInfoV01;
	VehicleScoringInfoV01 vehicleScoringInfoV01[100];
};

TCHAR transferBufferName[] = TEXT("\rf2transferMappingObject");
int transferBufferSize = 64000;
