byte glyphs[13] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6, 0x02, 0x62, 0x00};
                 // 0:0, 1:1,  2:2,  3:3,  4:4,  5:5,  6:6,  7:7,  8:8,  9:9,  10:-, 11:-1,12:(blank)

int latchPin = 2;             //connected to TPIC6C596 pin 10 [RCK]
int clockPin = 3;             //connected to TPIC6C596 pin 15 [SRCK]
int dataPin = 4;              //connected to TPIC6C596 pin 2  [SER IN]

int scR = 5;                  //score counter right
int scL = 5;                  //score counter left
int qNum = 1;                 //question number

boolean errR = false;         //holds error state for right score
boolean errL = false;         //holds error state for left
boolean errQ = false;         //holds error state for question number

int iR = 7;                   //connected to right increment button
int dR = 8;                   //connected to right decrement button

int iQ = 9;                   //connected to question increment button
int dQ = 10;                  //connected to question decrement button

int iL = 11;                  //connected to left increment button
int dL = 12;                  //connected to left decrement button

 
void setup() {
  // put your setup code here, to run once:
  pinMode(latchPin, OUTPUT);  //latchPin
  pinMode(clockPin, OUTPUT);  //clockPin
  pinMode(dataPin, OUTPUT);   //dataPin

  pinMode(iR, INPUT);         //increment right score
  pinMode(dR, INPUT);         //decrement right score
  pinMode(iQ, INPUT);         //increment question number
  pinMode(dQ, INPUT);         //decrement question number
  pinMode(iL, INPUT);         //increment left score
  pinMode(dL, INPUT);         //decrement left score

}

void loop() {
  // put your main code here, to run repeatedly:

  getInput();                 //check for button press and change scR, scL, and qNum values accordingly

  digitalWrite(latchPin, LOW);
  push(glyphs[12]);
  push(glyphs[12]);
  push(glyphs[12]);
  push(glyphs[12]);
  push(glyphs[12]);
  push(glyphs[12]);
  push(glyphs[12]);
  push(glyphs[12]);
  digitalWrite(latchPin, HIGH);
  delay(75);

  digitalWrite(latchPin, LOW);
  push(glyphs[5]);
  push(B11101110);
  push(glyphs[1]);
  push(glyphs[12]);
  push(glyphs[12]);
  push(glyphs[12]);
  push(B00011100);
  push(B10011110);
  digitalWrite(latchPin, HIGH);
  delay(75);

}


void push(byte x){
  shiftOut(dataPin, clockPin, LSBFIRST, x);
}

void displayGlyph(int x){ 
  digitalWrite (latchPin, LOW);
  shiftOut (dataPin, clockPin, LSBFIRST, glyphs[x]);
  digitalWrite (latchPin, HIGH);
}


void displayByte(byte x){
  digitalWrite (latchPin, LOW);
  shiftOut (dataPin, clockPin, LSBFIRST, x);
  digitalWrite (latchPin, HIGH); 
  delay(75);
}


void errMsg(){                //this needs to be updated to accommodate expanded display
  for (int i = 1; i < 64; i=i*2){
    digitalWrite (latchPin, LOW);
    shiftOut (dataPin, clockPin, MSBFIRST, i);
    digitalWrite (latchPin, HIGH);
    delay(75);
  }
}

void errChk(){                //check to see if any values are out of range
  if((scR > 52) || (scR < -36)){
    errR = true;              //bounds of possible score are -360 to 520
  }
  if((scR < 53) && (scR > -36)){
    errR = false;             //reset error flag if score returns within bounds
  }

  if((scL > 52) || (scL < -36)){
    errL = true;              //bounds of possible score are -360 to 520
  }
  if((scL < 53) && (scL > -36)){
    errR = false;             //reset error flag if score returns within bounds
  }

  if((qNum > 30) || (qNum < 1)){
    errQ = true;              //bounds of possible question
  }
  if((qNum < 31) && (qNum > 0)){
    errQ = false;             //reset error flag if qNum returns within bounds
  }
}

void getInput(){
  boolean buttonPress = false;
  
  if(digitalRead(iR)==HIGH){
    scR++;
    buttonPress = true;
  }

  if(digitalRead(dR)==HIGH){
    scR--;
    buttonPress = true;
  }

  if(digitalRead(iQ)==HIGH){
    qNum++;
    buttonPress = true;
  }

  if(digitalRead(dQ)==HIGH){
    qNum--;
    buttonPress = true;
  }

  if(digitalRead(iL)==HIGH){
    scL++;
    buttonPress = true;
  }

  if(digitalRead(dL)==HIGH){
    scL--;
    buttonPress = true;
  }

  if(buttonPress == true){
    delay(200);               //delay to avoid multi-trigger from bounce
  }
}


  /* demo code that makes a segment chase around the outside of the digit:
  for (int i = 1; i < 64; i=i*2){
    digitalWrite (latchPin, LOW);
    shiftOut (dataPin, clockPin, MSBFIRST, i);
    digitalWrite (latchPin, HIGH);
    delay(75);
  }

  displayByte(B00001100);
  displayByte(B00011100);
  displayByte(B00111100);
  displayByte(B01111100);
  displayByte(B11111100);
  displayByte(B11111000);
  displayByte(B11110000);
  displayByte(B11100000);
  displayByte(B11000000);
  */ 
