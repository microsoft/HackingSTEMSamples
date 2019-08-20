// ----------------------------------------------------------------------------
// This project uses an Arduino UNO microcontroller board. More information can
// be found by visiting the Arduino website: 
// https://www.arduino.cc/en/main/arduinoBoardUno 
//  
// Comments, contributions, suggestions, bug reports, and feature request
// are welcome! For source code and bug reports see:
// https://github.com/microsoft/HackingSTEMSamples
// 
// Copyright 2019, Adi Azulay Microsoft EDU Workshop - HackingSTEM 
// MIT License terms detailed in LICENSE.txt 
// ----------------------------------------------------------------------------

// Program variables ----------------------------------------------------------
int sensorValue = 0;
int sensorPin = A0;

// Incoming serial variables --------------------------------------------------
float incomingSerialFloat = 0;
int incomingSerialInt = 0;

// Serial data variables ------------------------------------------------------
const byte kNumberOfDataColumns = 3; //Number of columns from Data Streamer
// IMPORTANT: This must be equal to the number of channels set in Data Streamer

#define DELIMETER ',' // Data Streamer expects a comma delimeter
#define SERIALINTERVAL 50 // Interval between serial writes

char* incomingData[kNumberOfDataColumns]; // Array to store incoming data

// Uncomment the line below to use comma verification
//#define CheckCommaCount

// SETUP ----------------------------------------------------------------------
void setup()
{
  // Initializations occur here
  Serial.begin(9600);
}

// START OF MAIN LOOP --------------------------------------------------------- 
void loop()
{
  sensorValue = analogRead(sensorPin);

  // Read variables from serial port (Data Streamer)
  processIncomingSerial(); // This method is defined below

  // Check if specific string was recvied from serial port (Data Streamer)
  if (strcmp ("Foo", incomingData[0]) == 0)
  // Compares STR1 (Foo) to STR2 (incomingData[0]) returns 0 when true.
  {
    Serial.println("Recieved string: Foo");
  }

  // Convert incoming string to interger
  incomingSerialInt = atoi(incomingData[1]);

  // Convert incoming string to float
  incomingSerialFloat = atof(incomingData[2]);

  // Process and send data to serial port (Data Streamer)
  sendDataToSerial(); // This method is defined below

}

// OUTGOING SERIAL DATA PROCESSING CODE----------------------------------------
void sendDataToSerial()
{
  // Timestamp to track serial interval
  static unsigned long serialPreviousTime;

  // Enter into this only when serial interval has elapsed
  if((millis() - serialPreviousTime) > SERIALINTERVAL) 
  {
    serialPreviousTime = millis(); // Reset serial interval timestamp
    
    // Send data out separated by a comma (DELIMETER)
    // Repeat next 2 lines of code for each variable sent:
    Serial.print(sensorValue);
    Serial.print(DELIMETER);
    
    // Echos back the variable that was read from serial. You can use this to
    // validiate that the correct variables are being recived.
    Serial.print(incomingSerialInt);
    Serial.print(DELIMETER);

    Serial.print(incomingSerialFloat, 4); // Print float with 4 decimal places
    
    Serial.println(); // Add final line ending character only once
  }
}

//-----------------------------------------------------------------------------
// The code below here contains the methods used to communicate and parse data 
// from Data Streamer. Editing anything below this line may cause bugs when 
// sending or receiving data from Data Streamer.
//-----------------------------------------------------------------------------

// INCOMING SERIAL DATA PROCESSING CODE----------------------------------------
void processIncomingSerial()
{
  if(Serial.available()){
    parseData(getSerialData());
  }
}

// Gathers bytes from serial port to build inputString
char* getSerialData()
{
  char rawData[64];
  memset(rawData, 0, sizeof(rawData));

  while (Serial.available()){
    Serial.readBytesUntil('\n', rawData, 64); 
    //Read every byte in Serial buffer until line end or 64 bytes
  }

  static char inputString[64]; // Create a char array to store incoming data

  #ifndef CheckCommaCount
  // Clear the memory from a pervious reading
  memset(inputString, 0, sizeof(inputString));
  strcpy (inputString, rawData);
  return inputString;
  #endif

  #ifdef CheckCommaCount
  // Code Below here is used to verify that the string received is well-formed 
  // and no data packets were lost. It counts the number of commas received and
  // compares it to the number of commas expected. This code is not need for 
  // most applications since packets rarely get lost. If you would like to use 
  // this to verification define the CheckCommaCount variable at the start of 
  // the program. 

  int count = 0;
  for (int i = 0; i < sizeof(rawData); i++){
    if (rawData[i] == ','){
      count++;
    }
  }

  if (count != kNumberOfDataColumns-1){
    return NULL;
  } else {
    // Clear the memory from a pervious reading
    memset(inputString, 0, sizeof(inputString));
    strcpy (inputString, rawData);
    return inputString;
  }
  #endif
}

// Seperate the data at each delimeter
void parseData(char data[])
{
  if (data != NULL){
    // Find the first delimeter and return the token before it
    char *token = strtok(data, ",");
    int index = 0; // Index to track storage in the array
    while (token != NULL){
      incomingData[index] = token; // Assign the token to an array
      token = strtok(NULL, ","); // Conintue to the next delimeter
      index++; // incremenet index to store next value
    }
  }
}