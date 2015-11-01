#pragma once

class TelemetryData {

public:
	UnifiedRfData data;
	long rFactorVersion;

	~TelemetryData();

	bool Connect();
	bool Read();
	void Disconnect();
};

