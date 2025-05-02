
int checkButton(int,String,int&); 
enum State {
  NUL, 
  PREV,  
};

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

#define buttonDebounce 30 

int values[pinNum] = {0,0,0,0,0,0,0,0,0,0,0,0}; 
int old_values[pinNum] = {0,0,0,0,0,0,0,0,0,0,0,0}; 
int pins[pinNum] = {
  button1,button2,button3,button4, 
  button5, button6,ipbutton, joinbutton, 
  joystickLeft, joystickUp, joystickRight, joystickDown};
int counter = 1; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  for (int i = 0; i < pinNum; i++){
    pinMode(pins[i],INPUT_PULLDOWN); 
  }
  pinMode(LED_BUILTIN,OUTPUT); 
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000); 
  digitalWrite(LED_BUILTIN,LOW);
} 
//nt debTimer = millis(); 
void loop() {
  // put your main code here, to run repeatedly:
  // int x = digitalRead(15); 
  // if (x == 1){
  //   Serial.println("a"); 
  //   delay(100);
  //   x = 0; 
  // }
  // int values[pinNum] = {0,0,0,0,0,0,0,0,0}; 
  
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

  delay(20); 
  sanityButton(values,old_values); 
  copyA(values,old_values,12); 
  // counter += 1; 
  // if (counter == 5) {
  //   printButton(values,1,button2); 
  //   counter = 0; 
  // }
  // printarr(values); 
}

void copyA(int* src, int* dst, int len) {
    memcpy(dst, src, sizeof(src[0])*len);
}

int checkButton(int pin, String str){
  int x = digitalRead(pin); 
  if (x == 1){ 
    delay(buttonDebounce);
    x = digitalRead(pin); 
    if (x == 1){
      Serial.print("hld ");
      Serial.println(str); 
      //Serial.print(" ");/////
      //Serial.println(millis());
      return 1; 
    }
  }
  return 0;

}

void printarr(int vals[pinNum]){
  for (int i = 0; i < pinNum; i++){
    Serial.print(vals[i]);
    Serial.print(" "); 
  }
  Serial.println(""); 
}

void printButton(int vals[pinNum],int offset, int pin){
  int k = digitalRead(pin); 
  Serial.print("B read: ");
  Serial.print(k); 
  Serial.print(" Array value: "); 
  Serial.println(vals[offset]); 
}

void sanityButton(int vals[pinNum], int old[pinNum]){
  int j; 
  String letters[pinNum] = {"a", "b", "c", "d","g","h","j","k","L", "U", "R", "D"};

  for (int i = 0; i < pinNum; i++){ 
    j = digitalRead(pins[i]);
   
    if (j != 1 && vals[i] == 1){
      Serial.print("rel "); 
      Serial.println(letters[i]);
      vals[i] = j; 

    }
    else if  (j != 1 && old[i] == 1) {
      Serial.print("rel "); 
      Serial.println(letters[i]);
      vals[i] = j; 
      old[i] = j; 
    }
  }
 
}