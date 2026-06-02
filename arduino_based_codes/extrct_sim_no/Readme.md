# A7672S AT Command Tester for ESP32

A lightweight ESP32 utility for testing **A7672S 4G LTE modules** through the Serial Monitor.

This tool allows you to:

* Send any AT command directly from the Serial Monitor.
* View modem responses in real time.
* Automatically detect communication failures.
* Automatically power-cycle the modem after repeated timeouts.
* Automatically extract and display the SIM phone number when `AT+CNUM` is executed.
* Debug network registration, SMS, calls, data connectivity, and modem status.

---

## Features

✅ Direct AT Command Interface

Send any command such as:

```text
AT
ATI
AT+CSQ
AT+COPS?
AT+CREG?
AT+CGATT?
AT+CNUM
```

and receive the modem response instantly.

---

✅ Automatic Phone Number Extraction

When the command:

```text
AT+CNUM
```

is executed, the firmware automatically:

1. Parses the modem response.
2. Extracts the SIM number.
3. Removes country code prefixes (`+91` or `91`).
4. Converts the number into an integer format.

Example:

```text
Sending command: AT+CNUM

Response received:
+CNUM: "","919876543210",129

Extracted Number: 9876543210
As Integer: 9876543210
```

---

✅ Automatic SIM Recovery

If the modem fails to respond 3 consecutive times:

```text
SIM TIMEOUT. RESETTING...
```

The ESP32 automatically:

* Toggles the modem PWR pin
* Waits for reboot
* Restores communication

No manual intervention required.

---

## Recommended Network

For best results use:

### Jio SIM

The A7672S generally performs best with Jio in India for:

* Network registration
* LTE connectivity
* SMS
* Voice services
* Data sessions

Other operators may require additional APN or network configuration.

---

# Hardware Connections

## Aditya A7672S Module

```text
ESP32              A7672S
--------------------------------
GPIO17   --------> RXD
GPIO16   <-------- TXD
GPIO2    --------> PWRKEY
GND      --------> GND
```

Current configuration:

```cpp
#define PIN_TX 17
#define PIN_RX 16
#define PIN_PWR 2
```

---

## K-TRON A7672S Module

Uncomment:

```cpp
// #define PIN_TX 16
// #define PIN_RX 17
// #define PIN_PWR 15
```

and comment the Aditya module definitions.

Connection:

```text
ESP32              K-TRON A7672S
--------------------------------
GPIO16   --------> RXD
GPIO17   <-------- TXD
GPIO15   --------> PWRKEY
GND      --------> GND
```

---

# Serial Settings

### ESP32 Debug Port

```text
Baud Rate : 115200
Data Bits : 8
Parity    : None
Stop Bits : 1
```

### Modem UART

```cpp
#define UART_BAUD 115200
```

---

# Uploading the Firmware

1. Open project in Arduino IDE or PlatformIO.
2. Select your ESP32 board.
3. Connect ESP32 through USB.
4. Upload firmware.
5. Open Serial Monitor.
6. Set baud rate to:

```text
115200
```

7. Wait until:

```text
SETUP STARTED
INITIALIZATION DONE
Reset Done
READY FOR COMMANDS
```

appears.

---

# Usage

Type any AT command into Serial Monitor:

```text
AT
```

Output:

```text
Sending command: AT
Response received: OK
```

Check signal quality:

```text
AT+CSQ
```

Check network operator:

```text
AT+COPS?
```

Check SIM number:

```text
AT+CNUM
```

The firmware will automatically extract and display the phone number.

---

# Troubleshooting

## No Response From Modem

Verify:

* TX and RX are not swapped incorrectly
* Common ground is connected
* Power supply is capable of delivering at least 2A peak current
* SIM module is powered on

---

## Garbage Characters

Verify both sides are configured for:

```text
115200 baud
```

---

## AT+CNUM Returns Empty

Many operators do not store the phone number inside the SIM card.

Try:

```text
AT+CNUM
```

directly from a terminal.

If no number is returned, the SIM does not provide MSISDN information.

---

## Module Keeps Resetting

Check:

* Power supply stability
* Antenna connection
* SIM card insertion
* Network availability

Repeated communication failures trigger the automatic recovery mechanism.

---

# Example Test Commands

```text
AT
ATI
AT+CPIN?
AT+CSQ
AT+CREG?
AT+COPS?
AT+CGATT?
AT+CGDCONT?
AT+CGSN
AT+CNUM
```

---

# License

Free to use for learning, testing, and development.

---

# Support

If you encounter issues while interfacing the A7672S with ESP32, feel free to open an issue or ask questions.

This repository is intended to help developers quickly validate hardware connections, UART communication, SIM functionality, and network registration before integrating the modem into larger IoT projects.
