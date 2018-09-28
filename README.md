# A device provision library

This library provides the following functionality:


## Reset to factory defaults on button press

This functionality allows to reset the device to factory defaults by
performing the following steps:

- remove `conf9.json` which holds user-specific device configuraiton
- perform filesystem garbage collection
- reboot the device

Reset to factory defaults is done by configuring a reset button, attached to a
certain GPIO pin. Two modes are possible:

- Hold the reset button and reboot the device while holding a button.
  For this, set `provision.btn.pin` to a non-negative value, and set 
  `provision.btn.hold_ms` to `0`.
- Just hold the reset button pressed for a long time.
  For this, set `provision.btn.pin` to a non-negative value, and set 
  `provision.btn.hold_ms` to a positive value, e.g. `5000` - 5 seconds.


## Reset to factory defaults on bad WiFi credentials

This functionality resets the device to factory defaults if the WiFi
configuration is entered by user incorrectly
(wrong network name or wrong password).

If the device has at least once connected to the WiFi station successfully,
reset is never done afterwards.

This is done via configuration parameter `provision.wifi_configured`, which is
set to `false` on a fresh device. When the device first connects to the WiFi
station, `provision.wifi_configured` is set to `true`. When WiFi connection
fails, and `provision.wifi_configured` is `false`, factory reset is triggered.


## LED indication of the device state

- State 0: unprovisioned. This is factory defaults.
  Blink an LED with `provision.state0.led.blink_period` frequency.
- State 1: LAN (WiFi or Ethernet) configured,
  and device has never been joined to the LAN yet.
  If a device fails to connect to the configured LAN,
  it automatically resets to State 0.
  Blink an LED with `provision.state1.led.blink_period` frequency.
- State 2: A device at least once has successfully joined the LAN.
  From this state, a device does not automatically reset to State 0,
  even if it fails to connect to LAN.
  Blink an LED with configurable frequency2.
- State 3. A device has connected to the configured MQTT service or
  device management dashboard.
  Blink an LED with configurable frequency3.


## Configuration parameters reference

```javascript
"provision": {
  "configured": false,  // Set to true when first time connected to WiFi
  "button": {
    "pin": 17,      // Reset button GPIO number. Negative value disables reset
                    // button functionality. Default: arch-specific.
    "hold_ms": 0,   // Number of milliseconds to hold to trigger a factory reset.
                    // If negative, disable. If 0, reset on boot.
                    // If positive, reset when pressed for that many milliseconds.
  }
}
```
