# Migrations
These pages help you easily migrate your code from previous sbgECom major versions.

## From sbgECom v1.x
The sbgECom version 2.x change the C API even if the low level sbgECom protocol API remains backward compatible.  
In otherwords, a C code written with sbgECom version 1.x will not compile directly with sbgECom versions 2.x and higher.  
But your old C code using sbgECom versions 1.x will still be able to correctly setup and configure your ELLIPSE product.  

### GNSS module
The _SbgEComGnssModelsStdIds_ enum has been simplified and now ELLIPSE-N and ELLIPSE-D should only use _SBG_ECOM_GNSS_MODEL_INTERNAL_ to select the internal GNSS receiver.  
The only exception is for ELLIPSE-N hardware revision 1 & 2 as the ublox Max M8 has two modes of operations, either GPS+GLONASS or GPS+BeiDou.  
The default GPS+GLONASS mode is selected with the _SBG_ECOM_GNSS_MODEL_INTERNAL_ model and the GPS+BeiDou one with _SBG_ECOM_GNSS_MODEL_UBLOX_GPS_BEIDOU_.  
The _SBG_ECOM_GNSS_MODEL_UBLOX_GPS_BEIDOU_ model couldn't be used with ELLIPSE-N with hardware revision 3.x or ELLIPSE-D as the GNSS receiver is tracking GPS+GLONASS+BeiDou+Galileo concurrently.  

The table below helps you update your enums to sbgECom v2 correctly:
| sbgECom 1.x                             | sbgECom 2.x                             | Remarks                                                                        |
| --------------------------------------- | --------------------------------------- | ------------------------------------------------------------------------------ |
| SBG_ECOM_GNSS_MODEL_UBLOX_GPS_GLONASS   | SBG_ECOM_GNSS_MODEL_INTERNAL            | To use for ELLIPSE-N and ELLIPSE-D                                             |
| SBG_ECOM_GNSS_MODEL_NMEA                | SBG_ECOM_GNSS_MODEL_NMEA                | ELLIPSE-E, external NMEA receiver (listen only)                                |
| SBG_ECOM_GNSS_MODEL_UBLOX_GPS_BEIDOU    | SBG_ECOM_GNSS_MODEL_UBLOX_GPS_BEIDOU    | Can only be selected on ELLIPSE-N revision 1 & 2                               |
| SBG_ECOM_GNSS_MODEL_UBLOX_EXTERNAL      | SBG_ECOM_GNSS_MODEL_UBLOX_EXTERNAL      | ELLIPSE-E, external ublox receiver (listen only)                               |
| SBG_ECOM_GNSS_MODEL_UBLOX_HIGH_DYNAMICS | SBG_ECOM_GNSS_MODEL_INTERNAL            | For high dynamics applications, please selected the appropriate motion profile |
| SBG_ECOM_GNSS_MODEL_NOVATEL_EXTERNAL    | SBG_ECOM_GNSS_MODEL_NOVATEL_EXTERNAL    | ELLIPSE-E, external Novatel receiver (listen only)                             |
| SBG_ECOM_GNSS_MODEL_ELLIPSE_D_INTERNAL  | SBG_ECOM_GNSS_MODEL_INTERNAL            | Legacy ELLIPSE-D hardware 1 & 2, don't use it anymore                          |
| SBG_ECOM_GNSS_MODEL_UBLOX_HIGH_SPEED    | SBG_ECOM_GNSS_MODEL_INTERNAL            | For high speed applications, please selected the appropriate motion profile    |
| SBG_ECOM_GNSS_MODEL_SEPTENTRIO_EXTERNAL | SBG_ECOM_GNSS_MODEL_SEPTENTRIO_EXTERNAL | ELLIPSE-E, external Septentrio receiver (listen only)                          |
| SBG_ECOM_GNSS_MODEL_UBLOX_LOW_SPEED     | SBG_ECOM_GNSS_MODEL_INTERNAL            | For low dynamics applications, please selected the appropriate motion profile  |

Please also update your code according to the following recommendations and modifications:
- `sbgEComCmdGnss1GetModelInfo` method has been replaced by `sbgEComCmdGnss1GetModelId` with a simpler interface as the version field is no more used in ELLIPSE firmware.
- `sbgEComCmdGnss1GetLeverArmAlignment` and `sbgEComCmdGnss1SetLeverArmAlignment` methods have been suppressed by `sbgEComCmdGnss1InstallationGet` and `sbgEComCmdGnss1InstallationSet` methods.

### Magnetometers module

The method `sbgEComCmdMagGetModelInfo` has been replaced by `sbgEComCmdMagGetModelId` with a simpler interface as the version field is no more used in ELLIPSE firmware.

### Sensor module

The method `sbgEComCmdSensorGetMotionProfileInfo` has been replaced by `sbgEComCmdSensorGetMotionProfileId` with a simpler interface as the version field is no more used in ELLIPSE firmware.

### Legacy IG-500 sbgCom

sbgECom version 2.x drop the legacy IG-500 support so the methods `sbgEComCmdOutputGetLegacyConf` and `sbgEComCmdOutputSetLegacyConf` are removed. Please update your code to use sbgECom protocol instead.
