# ESP32 A7672S AT Command Tester

A simple ESP32-based utility for testing GSM/LTE modules such as the A7672S. This sketch creates a serial bridge between the ESP32 Serial Monitor and the GSM module, allowing you to send AT commands and view responses directly.

The code also includes automatic module power-up/reset functionality and basic response monitoring.

---

# Features

* Send AT commands directly from Arduino Serial Monitor
* View module responses in real time
* Automatic GSM module power-on sequence
* Automatic module reset after multiple communication failures
* Supports multiple hardware configurations
* Useful for module testing, debugging, and development

---

# Hardware Requirements

* ESP32 Development Board
* A7672S GSM/LTE Module
* Stable Power Supply (recommended 4V–4.2V for GSM module)
* USB Cable
* Arduino IDE

---

# Wiring

## Option 1: Aditya A7672S Module

| ESP32 Pin   | A7672S Pin |
| ----------- | ---------- |
| GPIO17 (TX) | RXD        |
| GPIO16 (RX) | TXD        |
| GPIO2       | PWRKEY     |
| GND         | GND        |

Configuration used:

```cpp
#define PIN_TX 17
#define PIN_RX 16
#define PIN_PWR 2
```

---

## Option 2: K-Tron Module

| ESP32 Pin   | GSM Module Pin |
| ----------- | -------------- |
| GPIO16 (TX) | RXD            |
| GPIO17 (RX) | TXD            |
| GPIO15      | PWRKEY         |
| GND         | GND            |

Configuration:

```cpp
#define PIN_TX 16
#define PIN_RX 17
#define PIN_PWR 15
```

Uncomment the required section in the source code before uploading.

---

# Power-On Sequence

During startup the ESP32:

1. Initializes Serial Monitor.
2. Initializes UART communication with the GSM module.
3. Pulls the PWRKEY pin LOW.
4. Waits 2 seconds.
5. Releases PWRKEY HIGH.
6. Waits 30 seconds for network initialization.
7. Displays:

```text
READY FOR COMMANDS
```

---

# Upload Settings

| Parameter     | Value              |
| ------------- | ------------------ |
| Board         | ESP32 Dev Module   |
| Upload Speed  | 921600 (or 115200) |
| Monitor Speed | 115200             |
| UART Speed    | 115200             |

---

# How To Use

1. Upload the sketch to ESP32.
2. Open Serial Monitor.
3. Set baud rate to:

```text
115200
```

4. Select:

```text
Both NL & CR
```

5. Type any AT command:

```text
AT
```

Example response:

```text
Response received:
OK
```

---

# Common Commands

Check communication:

```text
AT
```

Module information:

```text
ATI
```

SIM status:

```text
AT+CPIN?
```

Signal quality:

```text
AT+CSQ
```

Network registration:

```text
AT+CREG?
```

Operator information:

```text
AT+COPS?
```

Module phone number (if supported):

```text
AT+CNUM
```

IMEI number:

```text
AT+CGSN
```

---

# Auto Recovery

If no response is received for 3 consecutive commands:

1. The ESP32 automatically toggles the PWRKEY pin.
2. Waits for module reboot.
3. Attempts communication again.

This helps recover from module lockups without manual intervention.

---

# Troubleshooting

### No Response

Check:

* TX and RX connections are crossed correctly.
* Common GND connection exists.
* GSM module is powered properly.
* Correct UART pins are selected.
* Correct baud rate is configured.

### Garbled Characters

Verify:

```cpp
#define UART_BAUD 115200
```

matches the module's UART baud rate.

### Module Not Starting

Ensure the PWRKEY pin is connected correctly and the module power supply can provide sufficient current during network registration.

---

# Serial Monitor Example

```text
SETUP STARTED
INITIALIZATION DONE
Reset Done
READY FOR COMMANDS

AT

Sending command: AT
Response received:
OK
```

---

# License

This project is provided for educational, testing, and development purposes. Feel free to modify and use it in your own projects.
