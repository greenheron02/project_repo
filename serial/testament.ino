
int checkButton(int,String,int&); 
enum State {
  NUL, 
  PREV,  
};

/*
Enrico Addy - Testament.ino 

This program maps the arcade buttons on the control panel of the arcade shell to
the Raspberry Pi Pico so that button presses are communicated to the arcade computer

Buttons 1-6 are all of the general purpose buttons of the terminal

Ip button and Join button are for joining a game 

The Joystick directions are for mapping joystick inputs to the arcade computer


*/


#define pinNum 12 

#define button1 10
#define button2 11
#define button3 12
#define button4 13

#define button5 6
#define button6 7
#define ipbutton 8
#define joinbutton 9

#define joystickLeft 17
#define joystickUp 19
#define joystickRight 20
#define joystickDown 21

// Amount of debounce before a button press is detected
#define buttonDebounce 30 

// Sanitization arrays for recording button inputs
int values[pinNum] = {0,0,0,0,0,0,0,0,0,0,0,0}; 
int old_values[pinNum] = {0,0,0,0,0,0,0,0,0,0,0,0}; 

// Array of pins for each arcade input
int pins[pinNum] = {
  button1,button2,button3,button4, 
  button5, button6,ipbutton, joinbutton, 
  joystickLeft, joystickUp, joystickRight, joystickDown};


void setup() {
  // Begin serial with a baud rate of 9600
  Serial.begin(9600); 
  
  // Assign all pins as input with pulldown resisors 
  //
  // This will make the default values of each pin 0, so that 
  // noise does not cause a random button press
  for (int i = 0; i < pinNum; i++){
    pinMode(pins[i],INPUT_PULLDOWN); 
  }

  // Flash the onboard LED to signify start of the script
  pinMode(LED_BUILTIN,OUTPUT); 
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000); 
  digitalWrite(LED_BUILTIN,LOW);
} 

void loop() {
  
  // Check whether any button or joystick is pressed
  values[0] = checkButton(button1,"a"); 
  values[1] = checkButton(button2,"b"); 
  values[2] = checkButton(button3,"c");
  values[3] = checkButton(button4,"d"); 
  values[4] = checkButton(button5,"g"); 
  values[5] = checkButton(button6,"h"); 
  values[6] = checkButton(ipbutton,"j"); 
  values[7] = checkButton(joinbutton,"k"); 
  values[8] = checkButton(joystickLeft,"L");
  values[9] = checkButton(joystickUp,"U"); 
  values[10] = checkButton(joystickRight,"R"); 
  values[11] = checkButton(joystickDown,"D"); 

  // Delay to prevent serial buffer from overflow
  delay(20); 
  // Check whether a button is in a valid state
  sanityButton(values,old_values); 
  // Copy the first sanitization array into the old one
  copyA(values,old_values,12); 
 
}

// Copy the contents of an array into another array
void copyA(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

// Check whether a button or joystick input has been pressed
int checkButton(int pin, String str){
  // Read the button's pin
  int x = digitalRead(pin); 
  // If an input is detected
  if (x == 1){ 
    // wait the debounce period to reduce the chance that noise
    // affects the button input
    delay(buttonDebounce);

    // if an input is still detected
    x = digitalRead(pin); 
    if (x == 1){
      // start holding this button
      Serial.print("hld ");
      Serial.println(str); 
      return 1; 
    }
  }
  // otherwise do not start holding button
  return 0;

}

// Debug function, prints the contents of an array vals
void printarr(int vals[pinNum]){
  for (int i = 0; i < pinNum; i++){
    Serial.print(vals[i]);
    Serial.print(" "); 
  }
  Serial.println(""); 
}

//  Devbug function, reads sanitization array and button input, 
//  compares to check whether array is valid
void printButton(int vals[pinNum],int offset, int pin){
  int k = digitalRead(pin); 
  Serial.print("B read: ");
  Serial.print(k); 
  Serial.print(" Array value: "); 
  Serial.println(vals[offset]); 
}

// Checks whether a button output is in a valid state 
// Reset if in an invalid state 
// 
// An invalid function retains a 1 in the values array, causing an infinite button input
// or does not update when the input changes to zero
void sanityButton(int vals[pinNum], int old[pinNum]){
  int j; 
  // String of outputs corresponding to each button
  String letters[pinNum] = {"a", "b", "c", "d","g","h","j","k","L", "U", "R", "D"};

  // loop checks the status of a button and sanitization arrays
  for (int i = 0; i < pinNum; i++){ 
    j = digitalRead(pins[i]);
   
   // if the button is not pressed, but the sanitization array is pressed
    if (j != 1 && vals[i] == 1){
      // release the button and update the sanitization array
      Serial.print("rel "); 
      Serial.println(letters[i]);
      vals[i] = j; 

    }
    // if the button is not pressed, and the old sanitization array is pressed
    else if  (j != 1 && old[i] == 1) {
      // release the button
      Serial.print("rel "); 
      Serial.println(letters[i]);
      // set both sanitization arrays to the button's current value
      vals[i] = j; 
      old[i] = j; 
    }
  }
 
}
