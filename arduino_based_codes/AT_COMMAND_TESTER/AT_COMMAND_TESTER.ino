#define UART_BAUD 115200
// UNCOMMENT FOR TEST A7672S ADITYA
#define PIN_TX 17
#define PIN_RX 16
#define PIN_PWR 2  //PWR pin for SIM Module

// UNCOMMENT FOR TEST K-TRON MOLDULE
// #define PIN_TX 16
// #define PIN_RX 17
// #define PIN_PWR 15  //PWR pin for SIM Module

String commandBuffer = "";   // Buffer to store command from Serial Monitor
String BuildINString2 = "";  // Buffer to store module response
byte inData;
char inChar;
int s_err = 0;

void setup() {
  Serial.begin(115200);  // Initialize Serial Monitor
  Serial.println("SETUP STARTED");

  // Initialize Serial1 for AT commands
  Serial1.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
  Serial.println("INITIALIZATION DONE");

  delay(2000);

  // Blink LED on pin 2 for initialization indication
  pinMode(PIN_PWR, OUTPUT);
  digitalWrite(PIN_PWR, LOW);
  delay(2000);
  digitalWrite(PIN_PWR, HIGH);
  delay(30000);
  Serial.println("Reset Done");
  Serial.println("READY FOR COMMANDS");
}

void loop() {
  // Check for user input from Serial Monitor
  if (Serial.available()) {
    char receivedChar = Serial.read();

    // Check for newline (end of command)
    if (receivedChar == '\n' || receivedChar == '\r') {
      if (!commandBuffer.isEmpty()) {
        commandBuffer.trim();          //Trim whitespaces
        sendATCommand(commandBuffer);  // Send the complete command
        commandBuffer = "";            // Clear the buffer for the next command
      }
    } else {
      commandBuffer += receivedChar;  // Build the command string
    }
  }

  // Read and print responses from the module
  ReadData2();
  if (!BuildINString2.isEmpty()) {
    Serial.println("Response: " + BuildINString2);
    BuildINString2 = "";  // Clear response after printing
  }
}

// Function to send an AT command and read response
void sendATCommand(const String &command) {
  BuildINString2 = "";                            // Clear previous response
  Serial.println("Sending command: " + command);  // Debugging output
  Serial1.println(command + "\r\n");              // Send AT command with CRLF termination
  delay(500);                                     // Wait briefly to allow module to respond
  ReadData2();                                    // Read response from module

  if (BuildINString2.isEmpty()) {
    Serial.println("No response received.");
    s_err++;
    if (s_err == 3) {
      digitalWrite(PIN_PWR, LOW);
      delay(2000);
      digitalWrite(PIN_PWR, HIGH);
      delay(30000);
      s_err = 0;
      Serial.println("RESETTING SIM MODULE DONE");
    }
  } else {
    Serial.println("Response received: " + BuildINString2);
  //   // Send the AT+CNUM command
  // Serial1.println("AT+CNUM");
  ReadData2("CNUM");

  // Print full response
  Serial.println("Full SIM Response:");
  Serial.println(BuildINString2);

  // Extract phone number
  String number = ExtractPhoneNumber(BuildINString2);
  if (number.length() > 0) {
    Serial.print("Extracted Number: ");
    Serial.println(number);

    // Optionally convert to integer (if needed)
    unsigned long long numInt = strtoull(number.c_str(), NULL, 10);
    Serial.print("As Integer: ");
    Serial.println(numInt);
  } else {
    Serial.println("Phone number not found in response.");
  }
}
  }
}

// Function to read data from Serial1 with timeout
void ReadData2() {
  unsigned long startTime = millis();
  BuildINString2 = "";

  while ((millis() - startTime) < 2000) {  // Timeout after 2 seconds
    while (Serial1.available() > 0) {
      inData = Serial1.read();   // Read a byte
      inChar = char(inData);     // Convert to char
      BuildINString2 += inChar;  // Append to response string
      // Serial.print(inChar);      // Debugging: print each received character
    }
  }
}
