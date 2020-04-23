//#define DEBUG                     //Debugger. Required this for searching through my CAM and ASCII string creator due to them not constructing the strings properly.
#define redMORSELED 5               //Sets pin 5(the red LED) as this
#define blueMORSELED 11             //Sets pin 11(the blue LED) as this
#define IRLIGHT A1                  //Sets pin A1(IR) as 'infraRed'

//Array to store CAM code
const String CAM_array [48] = {                                                         //Constants to stop the array being written over.
  //letters
  "=", "%%=", "%=@", "%=", "%", "%=%", "==", "%==", "%%", "=%%", "%@", "%%@", "=%",
  "=@", "@=", "@%", "==@", "%@=", "%@%", "@", "@@", "==%", "=%@", "=@%", "%@@", "===",

  //Numbers
  "@%%=", "@%%@", "@%=%", "@%==", "@%=@", "@%@%", "@%@=", "@%@@", "@=%%", "@%%%",

  //Punctuation/characters/maths
  "@%%", "@%=", "@=%", "@==", "@=@", "@@%", "@@=", "@@@", "@%@", "@===@", " ", ""       //Last entry is a clear string option.
};

//Array to store ASCII characters
const char ASCII_array [47] = {
  //letters
  'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
  'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',

  //Numbers
  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',

  //Punctuation/Characters/Maths
  '.', ',', '\'', '?', '!', '+', '-', '*', '/', '=', ' '                               //Didn't require a clear string option here due to ASCII not requiring
};

//Setup
//==================================================================================================
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(50);
  pinMode (redMORSELED, OUTPUT);                    //Sets pin as an output
  pinMode (blueMORSELED, OUTPUT);
  pinMode (IRLIGHT, OUTPUT);
}

//ASCII to CAM Translator (uses arrays to do so)
//===================================================================================================

String ASCIItoCAM (char c) {
  c = toUpperCase (c);                        //Changes all entries to upper case; the requested output.
  int indicator = 0;

  while (!(c == ASCII_array[indicator])) {    //While c is a char from the ASCII array, add to indicator.
    indicator++;
  }
  String CAMret = CAM_array[indicator];
  return CAMret;
}

//Adds pipes and spaces between letters that have been determined to be in CAM code. Adds these to the string that prints CAM code.
//===================================================================================================

String CAMtoString(String m) {
  int Length;
  String ASCIIString;                       //Sets the variable used to save the string
  m += '|';                                 //Adds a pipe between letters.
  Length = m.length();                      //Saves length of string
  String output;

  for (int i = 0; i < (Length); i++) {     //While the string is smaller than 'Length', add to string.
#ifdef DEBUG                               //Debug required due to problems with test harness. Needed to find what the program was skipping.
    Serial.print(1);
#endif
    if ((m[i] == '@') || (m[i] == '=') || (m[i] == '%')) {        //Determines if the input is CAM or ASCII.
      output += m[i];                                             //Adds the CAM code to a string.
    } else if (m[i] == '|') {                                     //Adds a pipe(|) if CAM code is not entered (has already been determined that code = CAM, so can't confuse with ASCII).
      ASCIIString += (CAMtoASCII(output));
      output = "";                                                //Clears string.
    } else if (m[i] == ' ') {                                     //Adds a space( ) into string.
#ifdef DEBUG
      Serial.print(2);
#endif
      ASCIIString += (CAMtoASCII(output));
      ASCIIString += (" ");
      output = "";
    } else {
      ASCIIString += ("#");        //Inserts # where an unidetified value is entered such as ^. Commented out during testing while using the test harness to successfully complete test. Test. More testing. For science.
    }

  }
  return (ASCIIString);            //Stores/saves first string, moves to next.
}

//CAM to ASCII code Translator. Also puts in #.
//===================================================================================================

char CAMtoASCII (String s) {
  int indicator = 0;

  while (!(s == CAM_array[indicator]) && indicator < 48) {       //While CAM is the input, it checks that the input is within the array.
    indicator++;
  }
  if (indicator < 48) {
    char ASCIIret = ASCII_array[indicator];                      //If the input is a string within the CAM array, add it to the output.
    return ASCIIret;
  } else {
    return '#';                                                  //If not, replace the unrecognised code with a #.
  }
}

//
//===================================================================================================

String printCAM(String s) {

  int Length;
  String CAMString;
  Length = s.length();                          //'Length' is equal to the length of the string being input.

  for (int i = 0 ; i < Length ; i++) {          //While the string is smaller than 'Length', add to string. Can use 'Length' here and above due to it being a local variable.
#ifdef DEBUG
    Serial.println("Running for loop");
#endif
    if ((ASCIItoCAM(s[i]) == CAM_array [46]) || (ASCIItoCAM(s[i - 1]) == CAM_array [46])) {     //Used to check for spaces. 46 is the space(' ') entry in the CAM array.
#ifdef DEBUG
      Serial.println("Running if statement");
#endif
    } else if (i == 0) {                           //Checks if the string is empty. If so, skip/do nothing.
#ifdef DEBUG
      Serial.println("Running else if");
#endif
    } else {
#ifdef DEBUG
      Serial.println("running else");
#endif
      CAMString += "|";                            //Adds a pipe between two characters if there is not a space on either side (determins if there is a space via the if statement).
    }
    CAMString += ASCIItoCAM(s[i]);
#ifdef DEBUG
    Serial.println("Adding to CAMString");
#endif
  }
  return (CAMString);
}

//LED print function
//=======================================================================================

void senddigital (int pin, String message) {
  int potent_delay = map(analogRead(A0), 0, 1023, 20, 500);    //Reads the potentiometer, only takes readings between 20 and 500.


  int percentSign = potent_delay;               // Originally read from pin(A0). Now reads from Potentiometer.
  int Equals = (potent_delay * 2);              // Variable 'percent_sign' replaced with 'potent_delay' so it reads from Potentiometer.
  int letterspace = (potent_delay * 3);         // Multiplies time value by 3.
  int AtSign = (potent_delay * 4);
  int nextletter = (potent_delay);
  int nextword = (potent_delay * 6);

  int Length = message.length();

  for (int i = 0; i < Length; i++) {
    if (message[i] == '%') {                //Determines which CAM code is entered. The rest of the if statement sets how long the LED flashes for.
      digitalWrite (pin, HIGH);             //HIGH turns the LED on.
      delay(percentSign);                   //'percentSign' is the length of time taken for the potentiometer that the LED flashes for.
      digitalWrite (pin, LOW);              //LOW turns LED off.
      delay(letterspace);                   //The length of time the LED is turned off for between flashes. Taken from Potentiometer.
    } else if (message[i] == '=') {
      digitalWrite (pin, HIGH);
      delay (Equals);
      digitalWrite (pin, LOW);
      delay(letterspace);
    } else if (message[i] == '@') {
      digitalWrite (pin, HIGH);
      delay(AtSign);
      digitalWrite (pin, LOW);
      delay(letterspace);
    } else if (message[i] == '|') {          //If there is a pipe...
      delay(nextletter);                     //...Wait for a 'nextLetter' amount of time.
    } else if (message[i] == ' ') {
      delay(nextword);
    }
  }
}


//Loop containing the print functions
//========================================================================================

void loop() {
  String ip;                 //User input
  String CAMIp;              //current message as CAM
  String ASCIIIp;            //Current message as text/ASCII
  String CAMunmodIp;         //Prints the input to LED(Blue)
  String userInput;        //The users input


  // put your main code here, to run repeatedly:
  while (!Serial.available()) {
    ;
  }
  userInput = Serial.readString();          //Reads serial monitor input and saves it as 'userInput'
  userInput.toLowerCase();                  //Stores all in userInput as lowercase. The conversion to uppercase is done later. This is here to create a base for everything to be converted to upper. 
  ip = userInput;                           //Stores lowercase input into 'ip' to be used later for printing.
  bool space = true;                        //If there is a space, assign it to variable 'firstNonSpace'.
  int firstNonSpace = 0;                //Sets 'firstNonSpace' as 0
  while (space) {
    if (ip[firstNonSpace] == ' ') {     //If there is a space...
      firstNonSpace++;                  //...Add 1 to 'firstNonSpace'.
    } else {
      space = false;                    //If there aer no spaces, do not add.
    }
  }
  if ((ip[firstNonSpace] == '%') || (ip[firstNonSpace] == '@') || (ip[firstNonSpace] == '=')) {    //Determines if the entry is ASCII or CAM or if there is a space before the entry.
                                                                                                   //'ip' uses 'firstNonSpace' to enter however many spaces were input, into the string. 
    CAMunmodIp = ip;
    CAMIp = CAMtoString(ip);
    Serial.println(CAMIp);                            //Print the translated ASCII in CAM
    senddigital(blueMORSELED, CAMunmodIp);            //Send the unmodified(non-translated) code to the LED.
  } else {

    ASCIIIp = printCAM(ip);
    Serial.println(ASCIIIp);                          //Prints translated code into ASCII.
    senddigital(redMORSELED, ASCIIIp);                //Prints to red LED
  }


  //IR print function
  //=====================================================================================

  String irIntensity(CAMIp);                          //Prints IR input
  if (CAMIp == "IR*") {
    tone(IRLIGHT, 38000);
    delay(2000);                                      //Flashes for 2000ms.
    noTone(IRLIGHT);
  }
}



