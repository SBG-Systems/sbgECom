# Welcome to the sbgECom C library version 3.1
sbgECom is a C library provided under the MIT License and used to interface easily [SBG Systems](https://www.sbg-systems.com/) IMU, AHRS and INS to your application.

This library has been designed to be platform and OS independent and can safely be used on small micro-controller or larger multi-core CPUs.\
This package contains the following items:
 - sbgECom library full C source code
 - Doxygen library source code documentation
 - Examples C source code and binaries to quickly start using the sbgECom library
 - Several useful tools provided as binary package

The sbgECom library and examples can be compiled on any platform using CMake.\
The pre-compiled sbgECom examples and tools are 64 bits binaries available on Windows, Linux and Mac OS X platforms.

The library is written and maintained by SBG Systems SAS. You can contact the support@sbg-systems.com for if you need assistance.

# Platform support
The library has been designed to be easily ported to any platform by just providing/implementing a few low level methods and some configuration:
 - The platform configuration is set in `common/sbgConfig.h`
 - In the file `common/platform/sbgPlatform.c` please provide _sbgGetTime_, _sbgSleep_ and _sbgPlatformDebugLogMsg_
 - In the directory `common/interfaces/` you can provide custom open/read/write implementations for serial communications

# Building sbgECom Library
The sbgECom library and code examples are very easy to compile on any platform using CMake.\
The library has no third party library dependencies making it very easy to build.

SBG Systems doesn't provide the sbgECom as a pre-compiled library for obvious and good reasons.

## Dependencies
SBG Systems has validated the following toolchain:
- \>= CMake 3.0
- \>= GNU GCC 8 (any platform)
- \>= AppleClang 13 (Mac OS X)
- \>= Visual Studio 2015 or MSBuild equivalent (Windows)

## Building sbgECom
To build the sbgECom and C examples, go to the sbgECom library folder and type the following commands.

```sh
mkdir build
cd build
cmake ../cmake
cmake --build .
```

You should find the sbgECom static library and examples binaries in the `bin/Debug` folder.

# Code Examples
SBG Systems provides several and simple C code examples to quickly use the sbgECom library.
You can find both the source code as well as a binary for each example.

All examples source code are located in the `examples` directory. You can find pre-compiled 64 bits binaries in the `bin/examples` folder.

## Ellipse Minimal
Simple C example to illustrate how to connect and read data from an ELLIPSE using the sbgECom library.

You can test this example using the command below:

```sh
ellipseMinimal COM4 115200
```

## High Performance INS Minimal
Simple C example to illustrate how to read data from an High Performance INS over an UDP connection and using the sbgECom library.

You can test this example using the command below. The INS ip address is *192.168.1.1* and send logs on the UDP port *1234*:

```sh
hpInsMinimal COM4 192.168.1.1 5678 1234
```

## Pulse Minimal
Simple C example to illustrate how to connect and read data from a PULSE IMU using the sbgECom library.

You can test this example using the command below:

```sh
pulseMinimal COM4 921600
```

## Ellipse On Board Magnetic Calibration
Simple C example to illustrate how to use the ELLIPSE on board magnetic calibration algorithms.

You can test this example using the command below:

```sh
ellipseOnBoardMagCalib COM4 115200
```

## Air Date Input
Simple C example to illustrate how to send air date aiding measurements to an ELLIPSE using the sbgECom library.

You can test this example using the command below:

```sh
airDataInput COM4 115200
```

# Command Line Tools
SBG Systems offers two very useful tools to ease evaluation and integration. These tools are located in the `bin/tools` directory.

> Please read the dedicated README.md files provided with each tool.

## sbgBasicLogger
Simply parse sbgECom logs from a serial or ethernet interface and write log content to CSV like files.
This tool can also read sbgECom logs from a binary file making it very interesting to convert ELLIPSE datalog to easy to use text files.

## sbgEComApi 
Easily access sbgInsRest API configuration over a serial or UDP interface. You can execute GET and POST queries using simple to use command lines arguments.
This tool is perfect if you would like to setup a High Performance INS product over a serial or ethernet interface and using only bash scripts for example.

# CAN messages
SBG Systems provides a DBC and BusMaster CAN messages database definition to quickly interface your product with a CAN logger and application.
You can find these CAN database in the `can` directory

# Migrating from sbgECom v1.x to sbgECom v2.x or above
The sbgECom version 2.x change the C API even if the low level sbgECom protocol API remains backward compatible.
In otherwords, a C code written with sbgECom version 1.x will not compile directly with sbgECom versions 2.x and higher.
But your old C code using sbgECom versions 1.x will still be able to correctly setup and configure your ELLIPSE product.

## GNSS module
The _SbgEComGnssModelsStdIds_ enum has been simplified and now ELLIPSE-N and ELLIPSE-D should only use _SBG_ECOM_GNSS_MODEL_INTERNAL_ to select the internal GNSS receiver.
The only exception is for ELLIPSE-N hardware revision 1 & 2 as the ublox Max M8 has two modes of operations, either GPS+GLONASS or GPS+BeiDou. The default GPS+GLONASS mode is selected with the _SBG_ECOM_GNSS_MODEL_INTERNAL_ model and the GPS+BeiDou one with _SBG_ECOM_GNSS_MODEL_UBLOX_GPS_BEIDOU_.
The _SBG_ECOM_GNSS_MODEL_UBLOX_GPS_BEIDOU_ model couldn't be used with ELLIPSE-N with hardware revision 3.x or ELLIPSE-D as the GNSS receiver is tracking GPS+GLONASS+BeiDou+Galileo concurrently.

The table below helps you update your enums to sbgECom v2 correctly:
| sbgECom 1.x | sbgECom 2.x | Remarks |
| ------ | ------ | ------ |
| SBG_ECOM_GNSS_MODEL_UBLOX_GPS_GLONASS | SBG_ECOM_GNSS_MODEL_INTERNAL | To use for ELLIPSE-N and ELLIPSE-D |
| SBG_ECOM_GNSS_MODEL_NMEA | SBG_ECOM_GNSS_MODEL_NMEA | ELLIPSE-E, external NMEA receiver (listen only) |
| SBG_ECOM_GNSS_MODEL_UBLOX_GPS_BEIDOU | SBG_ECOM_GNSS_MODEL_UBLOX_GPS_BEIDOU | Can only be selected on ELLIPSE-N revision 1 & 2 |
| SBG_ECOM_GNSS_MODEL_UBLOX_EXTERNAL | SBG_ECOM_GNSS_MODEL_UBLOX_EXTERNAL | ELLIPSE-E, external ublox receiver (listen only) |
| SBG_ECOM_GNSS_MODEL_UBLOX_HIGH_DYNAMICS | SBG_ECOM_GNSS_MODEL_INTERNAL | For high dynamics applications, please selected the appropriate motion profile |
| SBG_ECOM_GNSS_MODEL_NOVATEL_EXTERNAL | SBG_ECOM_GNSS_MODEL_NOVATEL_EXTERNAL | ELLIPSE-E, external Novatel receiver (listen only) |
| SBG_ECOM_GNSS_MODEL_ELLIPSE_D_INTERNAL | SBG_ECOM_GNSS_MODEL_INTERNAL | Legacy ELLIPSE-D hardware 1 & 2, don't use it anymore |
| SBG_ECOM_GNSS_MODEL_UBLOX_HIGH_SPEED | SBG_ECOM_GNSS_MODEL_INTERNAL | For high speed applications, please selected the appropriate motion profile |
| SBG_ECOM_GNSS_MODEL_SEPTENTRIO_EXTERNAL | SBG_ECOM_GNSS_MODEL_SEPTENTRIO_EXTERNAL | ELLIPSE-E, external Septentrio receiver (listen only) |
| SBG_ECOM_GNSS_MODEL_UBLOX_LOW_SPEED | SBG_ECOM_GNSS_MODEL_INTERNAL | For low dynamics applications, please selected the appropriate motion profile |

Please also update your code according to the following recommendations and modifications:
- `sbgEComCmdGnss1GetModelInfo` method has been replaced by `sbgEComCmdGnss1GetModelId` with a simpler interface as the version field is no more used in ELLIPSE firmware.
- `sbgEComCmdGnss1GetLeverArmAlignment` and `sbgEComCmdGnss1SetLeverArmAlignment` methods have been suppressed by `sbgEComCmdGnss1InstallationGet` and `sbgEComCmdGnss1InstallationSet` methods.

## Magnetometers module

The method `sbgEComCmdMagGetModelInfo` has been replaced by `sbgEComCmdMagGetModelId` with a simpler interface as the version field is no more used in ELLIPSE firmware.

## Sensor module

The method `sbgEComCmdSensorGetMotionProfileInfo` has been replaced by `sbgEComCmdSensorGetMotionProfileId` with a simpler interface as the version field is no more used in ELLIPSE firmware.

## Legacy IG-500 sbgCom

sbgECom version 2.x drop the legacy IG-500 support so the methods `sbgEComCmdOutputGetLegacyConf` and `sbgEComCmdOutputSetLegacyConf` are removed. Please update your code to use sbgECom protocol instead.
