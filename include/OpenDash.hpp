#pragma once

#include "TelemetryData.hpp"

class OpenDash
{
public:
	TelemetryData rf;

	int Connect();		// Returns 1 or 2 as rFactor version info, or 0 if failed
	bool Update() { return false; };
	void Disconnect();

	int RfVersion();	// 0 designates not connected

	OpenDash();
	~OpenDash();

private:

};

OpenDash::OpenDash()
{
}

OpenDash::~OpenDash()
{
}