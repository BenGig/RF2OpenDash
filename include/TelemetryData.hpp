#pragma once

#include "DataTransfer.hpp"

class TelemetryData {

public:
	UnifiedRfData data;
	long rFactorVersion;

	~TelemetryData();

	bool Connect();
	bool Read();
	void Disconnect();
};

