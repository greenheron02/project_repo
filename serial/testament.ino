
int checkButton(int,String,int&); 
enum State {
  NUL, 
  PREV,  
};

#define pinNum 9 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  int pins[pinNum] = {15, 13, 12, 11, 10, 18, 19, 20, 21}; 
  for (int i = 0; i < pinNum; i++){
    pinMode(pins[i],INPUT); 
  }
  pinMode(LED_BUILTIN,OUTPUT); 
  digitalWrite(LED_BUILTIN,HIGH);
  delay(1000); 
  digitalWrite(LED_BUILTIN,LOW);
  // pinMode(15,INPUT); 
  // pinMode(13,INPUT); 
  // pinMode(12,INPUT); 
} 

void loop() {
  // put your main code here, to run repeatedly:
  // int x = digitalRead(15); 
  // if (x == 1){
  //   Serial.println("a"); 
  //   delay(100);
  //   x = 0; 
  // }
  int values[pinNum] = {0,0,0,0,0,0,0,0,0}; 
  
  values[0] = checkButton(15,"a"); 
  values[1] = checkButton(13,"b"); 
  values[2] = checkButton(12,"c");
  values[3] = checkButton(11,"d"); 
  // checkButton(10,"e");
  values[5] = checkButton(21,"L");
  values[6] = checkButton(20,"D"); 
  values[7] = checkButton(19,"R"); 
  values[8] = checkButton(18,"U"); 
  // delay(10); 
  // sanityButton(values); 
}

// check if a button has stopped being pressed
// send an input once
int checkButton(int pin, String str){
  int x = digitalRead(pin); 
  if (x == 1){ 
    Serial.print("hld ");
    Serial.println(str); 
    delay(50);
    x = digitalRead(pin); 

    if (x == 0){
      Serial.print("rel "); 
      Serial.println(str);  
      return 0; 

    }
  }
  return 1;

}

void sanityButton(int vals[pinNum]){
  int j; 
  int pins[pinNum] = {15, 13, 12, 11, 10, 18, 19, 20, 21};
  String letters[pinNum] = {"a", "b", "c", "d", "e", "L", "D", "R", "U"};

  for (int i = 0; i < pinNum; i++){ 
    j = digitalRead(pins[i]);

    if (j != vals[i]){
      Serial.print("rel "); 
      Serial.println(letters[i]);

    }
  }
 
}