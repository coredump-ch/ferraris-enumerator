# ferraris-enumerator

Counts people in the hacker space and publishes the result on a SpaceAPI server.

## Configuration

You need to create a file called `config.h` in the same directory as `ferraris-counter.ino` containing the follwing information:

| Field name     | Field type           | Meaning                                                  |
|----------------|----------------------|----------------------------------------------------------|
| `kSSID`        | `char const * const` | Your networks **SSID**                                   |
| `kPassword`    | `char const * const` | Your networks **Password**                               |
| `kHost`        | `char const * const` | The **hostname** of the target SpaceAPI server           |
| `kPort`        | `int const`          | The **port** your SpaceAPI server is listening on        |
| `kPath`        | `char const * const` | The **path** for the PUT request on your SpaceAPI server |
| `kField`       | `char const * const` | The **key** of the field transmitted in the PUT request  |

For the sake of simplicity, you can of course define these variables using `auto constexpr`. You should make sure to **_NEVER_** add this configuration file to your repository since this would compromise your network security.

## Platform

This code is designed to run on an ESP8266 sitting on a LoLin v3 development board using 8 switches and a two-wire seven-segment display. The code was written using the ESP8266 platform plugin for Arduino.

## Flashing

Follow [this guide](http://henrysbench.capnfatz.com/henrys-bench/arduino-projects-tips-and-more/arduino-esp8266-lolin-nodemcu-getting-started/) to set up Arduino Studio for your ESP8266. Then use it to flash the code.
