# RF2OpenDash

VisualStudio 2015 Community Edition

Playing around with a telemetry and standings server for rFactor 2

The rFactor plugin writes a structure to shared memory.   

All common stuff is defined in include/DataTransfer.hpp

# What is Written: Transfer Structure

Currently a collection of items taken 1:1 from the rF plugin environment. To be refined.

# When is it written

Writes are triggered in the plugin callback functions: updateTelemetry, updateScoring, enter/exitRealtime.

# Where is it written: Memory File Mapping

The shared memory is accessed by the name defined in the common include file. 
See RF2OpenDashTester.cpp for an example how to read the data structure.
