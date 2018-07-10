# A device provision library

This library provides the following functionality:

- Provide visual indication of the device state:
    * State 0: unprovisioned. This is factory defaults.
      Blink an LED with `provision.state0.led.blink_period` frequency.
    * State 1: LAN (WiFi or Ethernet) configured,
      and device has never been joined to the LAN yet.
      If a device fails to connect to the configured LAN,
      it automatically resets to State 0.
      Blink an LED with `provision.state1.led.blink_period` frequency.
    * State 2: A device at least once has successfully joined the LAN.
      From this state, a device does not automatically reset to State 0,
      even if it fails to connect to LAN.
      Blink an LED with configurable frequency2.
    * State 3. A device has connected to the configured MQTT service or
      device management dashboard.
      Blink an LED with configurable frequency3.
- Reset to factory defaults on a long button press. Configurable parameters:
   * `provision.reset.button.pin` - reset button GPIO number
   * `provision.reset.button.hold_ms` - number of milliseconds to hold to trigger reset
- Reset to factory defaults on N frequent reboots. Configurable parameters:
     * `provision.reset.reboots.count` - minimum number of reboots
     * `provision.reset.reboots.interval_ms` - max number of milliseconds between reboots