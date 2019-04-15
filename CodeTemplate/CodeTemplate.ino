// ----------------------------------------------------------------------------
// [Title of project] for use with the [Lesson Plan Title] lesson plan 
// available from Microsoft Education Workshop at http://aka.ms/hackingSTEM 
// 
// This project uses an Arduino UNO microcontroller board. More information can
// be found by visiting the Arduino website: 
// https://www.arduino.cc/en/main/arduinoBoardUno 
//  
// [Several sentences describing code function in accessible verbiage] 
// 
// Comments, contributions, suggestions, bug reports, and feature requests 
// are welcome! For source code and bug reports see: 
// http://github.com/[TODO: github path to Hacking STEM] 
// 
// Copyright 2019, Adi Azulay Microsoft EDU Workshop - HackingSTEM 
// MIT License terms detailed in LICENSE.txt 
// ----------------------------------------------------------------------------

// Program variables ----------------------------------------------------------
int exampleVariable = 0;
int sensorPin = A0;

// Excel variables ------------------------------------------------------------
float incomingExcelFloat = 0;
int incomingExcelInteger = 0;

// Serial data variables ------------------------------------------------------
const byte kNumberOfChannelsFromExcel = 3; //Number of channels coming in from Excel
// IMPORTANT: This must be equal to the number of channels set in Data Streamer

const char kDelimiter = ',';    // Data Streamer expects a comma delimeter

char* arr[kNumberOfChannelsFromExcel]; //Array of data coming in from Excel

// SETUP ----------------------------------------------------------------------
void setup() {
  // Initializations occur here
  Serial.begin(9600);
}

// START OF MAIN LOOP --------------------------------------------------------- 
void loop()
{
  // Process sensors
  processSensors();

  // Read Excel variables from serial port (Data Streamer)
  processIncomingSerial();

  // Check if specific string was sent from Excel
  if ( strcmp ("Foo", arr[0]) == 0){ // Compares STR1 to STR2 returns 0 if true.
      Serial.println("working");
  }

  // Convert incoming string to interger
  incomingExcelInteger = atoi(arr[1]);

  // Convert incoming string to float
  incomingExcelFloat = atof(arr[2]);

  // Process and send data to Excel via serial port (Data Streamer)
  sendDataToSerial();

}

// SENSOR INPUT CODE-----------------------------------------------------------
void processSensors() 
{
  // Add sensor processing code here
  exampleVariable = analogRead( sensorPin );
}

// Add any specialized methods and processing code here

// OUTGOING SERIAL DATA PROCESSING CODE----------------------------------------
void sendDataToSerial()
{
  const int kSerialInterval = 50;   // Interval between serial writes
  unsigned long serialPreviousTime; // Timestamp to track serial interval

  // Enter into this only when serial interval has elapsed
  if((millis() - serialPreviousTime) > kSerialInterval) 
  {
    serialPreviousTime = millis(); // Reset serial interval timestamp
    
    // Serial.print(); //This line is reserved for command variables
    Serial.print(kDelimiter);
    // Send data out separated by a comma (kDelimiter)
    // Repeat next 2 lines of code for each variable sent:
    Serial.print(exampleVariable);
    Serial.print(kDelimiter);
    
    // Example test for incoming Excel variables  
    Serial.print(incomingExcelInteger);
    Serial.print(kDelimiter);

    Serial.print(incomingExcelFloat, 4); // second parameter = decimal places
    
    Serial.println(); // Add final line ending character only once
  }
}

//-----------------------------------------------------------------------------
// DO NOT EDIT ANYTHING BELOW THIS LINE
//-----------------------------------------------------------------------------

// INCOMING SERIAL DATA PROCESSING CODE----------------------------------------
void processIncomingSerial()
{
  if(Serial.available()){
    parseData(GetSerialData());
  }
}

// Gathers bytes from serial port to build inputString
char* GetSerialData()
{
  char rawData[64];
  while (Serial.available()){
    Serial.readBytesUntil('\n', rawData, 64); //Read every byte in Serial buffer until line end or 64 bytes
  }

  static char inputString[64]; // Create a char array to store incoming data
  // Comment the three lines below if using comma verification
  memset(inputString, 0, sizeof(inputString)); // Clear the memory from a pervious reading
  strcpy (inputString, rawData);
  return inputString;

  //---Code Below this here can be used to verify that the string recieved is 
  // well formed and no data packets were lost. It counts the number of commas 
  // recived and compares it to the number of commas expceted. This code is 
  // commented out since packets rarely get lost in most applications. If you 
  // would like to use this to verification comment out lines above, and 
  // uncomment the code below.

  // int count = 0;
  // for (int i = 0; i < sizeof(rawData); i++){
  //   if (rawData[i] == ','){
  //     count++;
  //   }
  // }

  // if (count != kNumberOfChannelsFromExcel-1){
  //   return NULL;
  // } else {
  //   memset(inputString, 0, sizeof(inputString)); // Clear the memory from a pervious reading
  //   strcpy (inputString, rawData);
  //   return inputString;
  // }  
}

// Seperate the data at each delimeter
void parseData(char data[])
{
  if (data != NULL){
    Serial.println(data);
    char *token = strtok(data, ","); // Find the first delimeter and return the token before it
    int index = 0; // Index to track storage in the array
    while (token != NULL){ // Char* strings terminate w/ a Null character. We'll keep running the command until we hit it
      arr[index] = token; // Assign the token to an array
      token = strtok(NULL, ","); // Conintue to the next delimeter
      index++; // incremenet index to store next value
    }
  }
}