#define UART_BAUD 115200

// UNCOMMENT FOR TEST A7672S ADITYA
#define PIN_TX 17
#define PIN_RX 16
#define PIN_PWR 2  // PWR pin for SIM Module

// UNCOMMENT FOR TEST K-TRON MODULE
// #define PIN_TX 16
// #define PIN_RX 17
// #define PIN_PWR 15  // PWR pin for SIM Module

String commandBuffer = "";   // Buffer for commands from Serial Monitor
String BuildINString2 = "";  // Buffer for SIM module response
byte inData;
char inChar;
int s_err = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("SETUP STARTED");

  // Init Serial1 for SIM communication
  Serial1.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
  Serial.println("INITIALIZATION DONE");

  // Toggle PWR pin to reset SIM
  pinMode(PIN_PWR, OUTPUT);
  digitalWrite(PIN_PWR, LOW);
  delay(2000);
  digitalWrite(PIN_PWR, HIGH);
  delay(30000);  // Wait for SIM module to boot
  Serial.println("Reset Done");
  Serial.println("READY FOR COMMANDS");
}

void loop() {
  // Handle Serial Monitor input
  if (Serial.available()) {
    char receivedChar = Serial.read();
    if (receivedChar == '\n' || receivedChar == '\r') {
      if (!commandBuffer.isEmpty()) {
        commandBuffer.trim();
        sendATCommand(commandBuffer);
        commandBuffer = "";
      }
    } else {
      commandBuffer += receivedChar;
    }
  }

  // Read async responses (URCs, etc.)
  ReadData2();
  if (!BuildINString2.isEmpty()) {
    Serial.println("Response: " + BuildINString2);
    BuildINString2 = "";
  }
}

void sendATCommand(const String &command) {
  BuildINString2 = "";
  Serial.println("Sending command: " + command);
  Serial1.println(command + "\r\n");

  delay(500);  // Allow time for response
  ReadData2();

  if (BuildINString2.isEmpty()) {
    Serial.println("No response received.");
    s_err++;
    if (s_err >= 3) {
      Serial.println("SIM TIMEOUT. RESETTING...");
      digitalWrite(PIN_PWR, LOW);
      delay(2000);
      digitalWrite(PIN_PWR, HIGH);
      delay(30000);
      s_err = 0;
      Serial.println("RESETTING SIM MODULE DONE");
    }
  } else {
    Serial.println("Response received: " + BuildINString2);

    // If AT+CNUM was sent, try to extract phone number
    if (command.equalsIgnoreCase("AT+CNUM")) {
      String number = ExtractPhoneNumber(BuildINString2);
      if (number.startsWith("+91")) {
        number = number.substring(3);
      } else if (number.startsWith("91")) {
        number = number.substring(2);
      }
      Serial.print("Extracted Number: ");
      Serial.println(number);

      unsigned long long numInt = strtoull(number.c_str(), NULL, 10);
      Serial.print("As Integer: ");
      Serial.println(numInt);
    } else {
      Serial.println("Phone number not found in response.");
    }
  }
}

// Function to read SIM response with timeout
void ReadData2() {
  unsigned long startTime = millis();
  BuildINString2 = "";

  while ((millis() - startTime) < 2000) {
    while (Serial1.available() > 0) {
      inData = Serial1.read();
      inChar = char(inData);
      BuildINString2 += inChar;
    }
  }
}

// Function to extract phone number from +CNUM response
String ExtractPhoneNumber(String input) {
  int firstQuote = input.indexOf('"', 6);  // Skip first pair
  int secondQuote = input.indexOf('"', firstQuote + 1);
  int thirdQuote = input.indexOf('"', secondQuote + 1);
  int fourthQuote = input.indexOf('"', thirdQuote + 1);

  if (thirdQuote != -1 && fourthQuote != -1) {
    return input.substring(thirdQuote + 1, fourthQuote);
  }
  return "";
}
