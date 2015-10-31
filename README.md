# RF2OpenDash

Playing around with a telemetry and standings server for rFactor 2 (rF1 planned).

All common stuff is defined in include/DataTransfer.hpp, most importantly the UnifiedRfData which is written to shared memory

# What is Written: Transfer Structure

See UnifiedRfData.

# When is it written

Writes are triggered in the plugin callback functions: updateTelemetry, updateScoring, enter/exitRealtime.

# Where is it written: Memory File Mapping

The shared memory is accessed by the name defined in the common include file. 
See RF2OpenDashTester.cpp for an example how to read the data structure.



VisualStudio 2015 Community Edition

