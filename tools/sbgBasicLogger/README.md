# sbgBasicLogger

The sbgBasicLogger let you log and display sbgECom binary messages into CSV like text files.

## Output
The output is as close as possible as the sbgECom message definition.
You can both output incoming messages in text files and/or display the data in the terminal.

## Decimation
The tool can also decimate incoming IMU data using a simple moving average filter.
You can specify decimation to apply before displayed the data to the terminal and also one before writing the data to text files.

Only the following logs are decimated:
 - IMU short
 - IMU fast
 - IMU data

## Interfaces
The sbgBasicLogger can be used to parse incoming data from a serial or an Ethernet UDP interface.
You can also select an binary file containing raw sbgECom dump. This tool can thus be used to easily export sbgECom data to CVS like text files.

# Usage

The sbgBasicLogger implements a simple to use command line interface (CLI):

```sh
sbgBasicLogger [-hvwpH] [-a IP address] [-I UDP port in] [-O UDP port out] [-s SERIAL_DEVICE] [-r SERIAL_BAUDRATE] [-i INPUT-FILE] [--dir=DIRECTORY] [-d FILE DECIMATION] [-c CONSOLE DECIMATION]
```

## Serial example and only print on console

```sh
sbgBasicLogger -s <SERIAL-PORT> -r <BAUDRATE> -p
```

## Serial example, print on console and write files into <directory>

```sh
sbgBasicLogger -s <SERIAL-PORT> -r <BAUDRATE> -p -w --dir=directory/
```

## UDP example and only print on console

```sh
sbgBasicLogger -a <IP_ADDR> -I <UDP_PORT_IN> -O <UDP_PORT_OUT> -p
```

## Input file example and only print on console

```sh
sbgBasicLogger -i <BINARY_FILE> -p
```

## Options
```
  -h, --help                                         display this help and exit
  -v, --version                                      display sbgECom version and exit
  -a, --addr-ip=IP address                           open an UDP interface
  -I, --udp-port-in=UDP port in                      UDP port in
  -O, --udp-port-out=UDP port out                    UDP port out
  -s, --serial-device=SERIAL_DEVICE                  open a serial interface
  -r, --serial-baudrate=SERIAL_BAUDRATE              serial baudrate
  -i, --input-file=INPUT-FILE                        input file
  -w, --write-logs                                   write logs in different files
  --dir=DIRECTORY                                    directory to write logs into
  -d, --file-decimation=FILE DECIMATION              file decimation
  -c, --console-decimation=CONSOLE DECIMATION        output stream decimation
  -p, --print-logs                                   print the logs on the output stream
  -H, --disable-header                               disable header for files
```