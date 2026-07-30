#pragma once
#include <Arduino.h>
#include "settings.h"

// Scans surrounding Beacons in promiscuous mode across channels,
// picks up to 4 strongest APs, calculates time offsets using current RTC time,
// updates g_appSettings.beacon_records and saves to Flash.
// Returns the number of records saved.
int captureAndSaveBeaconRecords();

// Spawns a background FreeRTOS task to run captureAndSaveBeaconRecords asynchronously.
void startAsyncBeaconCapture();

// Attempts to restore system RTC time using saved Beacon records in Flash.
// Called during setup() when enable_beacon_time_sync is true.
// Returns true if RTC was restored successfully, false otherwise.
bool tryRestoreRtcFromBeacons();
