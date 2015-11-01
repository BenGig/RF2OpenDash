#include "..\include\OpenDash.hpp"

int OpenDash::Connect()
{
	if (rf.Connect())
	{
		return rf.rFactorVersion;
	}
	return 0;
}

void OpenDash::Disconnect()
{
	rf.Disconnect();
}

int OpenDash::RfVersion()
{
	return rf.rFactorVersion;
}
