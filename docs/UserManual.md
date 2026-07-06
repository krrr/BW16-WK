# Device Scan

Scan devices connected to the router, and record the number of packets in both directions: from devices to the router and from the router to devices.

This chip does not support WiFi 6 (and later versions), so WiFi 6 data frames may not be recorded. However, management frames are always transmitted using older protocol versions and can therefore be recorded.

After launching a deauthentication attack, immediately click "Scan Devices" to monitor whether the handshake process occurs (indicated by an increase in the handshake packet count), which allows you to determine whether the device has been successfully disconnected.