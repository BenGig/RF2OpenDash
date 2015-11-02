#pragma once

#include "DataTransfer.hpp"
#include "json/json.h"

class TelemetryData {

public:
	UnifiedRfData data;
	long rFactorVersion;

	~TelemetryData();

	bool Connect();
	bool Read();
	void Disconnect();

	// JSON 
	char * JsonScoring();
};

