static byte glyphs[13] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6, 0x02, 0x62, 0x00};
                        // 0:0, 1:1,  2:2,  3:3,  4:4,  5:5,  6:6,  7:7,  8:8,  9:9,  10:-, 11:-1,12:(blank)

int latchPin = 2;             //connected to TPIC6C596 pin 10 [RCK]
int clockPin = 3;             //connected to TPIC6C596 pin 15 [SRCK]
int dataPin = 4;              //connected to TPIC6C596 pin 2  [SER IN]

int scR = 0;                  //score counter right (initialize to zero)
int scL = 0;                  //score counter left (initialize to zero)
int qNum = 1;                 //question number (initialize to #1)

byte dispR[3];                //holds display bytes for right score
byte dispL[3];                //holds display bytes for left score
byte dispQ[2];                //holds display bytes for question number

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

  dispL[0] = glyphs[12];      //digit 0 blank
  dispL[1] = glyphs[12];      //digit 1 blank
  dispL[2] = glyphs[0];       //digit 3 zero

  dispR[0] = glyphs[12];      //digit 0 blank
  dispR[1] = glyphs[12];      //digit 1 blank
  dispR[2] = glyphs[0];       //digit 3 zero

  dispQ[0] = glyphs[12];      //digit 0 blank
  dispQ[1] = glyphs[1];       //digit 1 one
}

void loop() {
  
  getInput();                 //check for button press and change scR, scL, and qNum values accordingly
  
  errChk();                   //sets error flags correctly

  setOutput();                //sets output bytes correctly
  
  updateDisplay();            //pushes updated values to display

}


void push(byte x){
  shiftOut(dataPin, clockPin, LSBFIRST, x);
}

void updateDisplay(){
  digitalWrite(latchPin, LOW);
  push(dispR[2]);
  push(dispR[1]);
  push(dispR[0]);
  push(dispL[2]);
  push(dispL[1]);
  push(dispL[0]);
  push(dispQ[1]);
  push(dispQ[0]);
  digitalWrite(latchPin, HIGH);
}

void errChk(){                //check to see if any values are out of range
  if((scR > 52) || (scR < -19)){
    errR = true;              //bounds of possible score are -360 to 520, but display only goes down to -190
  }
  if((scR < 53) && (scR > -20)){
    errR = false;             //reset error flag if score returns within bounds
  }

  if((scL > 52) || (scL < -36)){
    errL = true;              //bounds of possible score are -360 to 520
  }
  if((scL < 53) && (scL > -36)){
    errL = false;             //reset error flag if score returns within bounds
  }

  if((qNum > 30) || (qNum < 1)){
    errQ = true;              //bounds of possible question numbers
  }
  if((qNum < 31) && (qNum > 0)){
    errQ = false;             //reset error flag if qNum returns within bounds
  }
}

void errSet(){                //sets error-state values to display -- or ---
  if(errR == true){
    dispR[0]=glyphs[10];
    dispR[1]=glyphs[10];
    dispR[2]=glyphs[10];
  }

  if(errL == true){
    dispL[0]=glyphs[10];
    dispL[1]=glyphs[10];
    dispL[2]=glyphs[10];
  }

  if(errQ == true){
    dispQ[0]=glyphs[10];
    dispQ[1]=glyphs[10];
  }
}

void setR(){
  dispR[2] = glyphs[0];         //sets third digit to zero
  if (scR == 0){
    dispR[1] = glyphs[12];      //blanks second digit if score returns to zero
    dispR[0] = glyphs[12];      //blanks first digit if score returns to zero (prevents "-0")
  }

  if (scR != 0){
    dispR[1] = glyphs[abs(scR%10)];   //sets second digit to ones place of score
  
    int x = (abs(scR)/10)%10;         //holds tens place of scR as potential first digit
    switch (x){
      case 0:
        if (scR > 0)
          dispR[0] = glyphs[12];      //blanks first digit if score is below 100
        if (scR < 0)
          dispR[0] = glyphs[10];      //sets first digit to - if score is below zero
        break;
      default:
        if (scR > 0)
          dispR[0] = glyphs[x];       //sets first digit if score is 100 or higher
        if (scR < 0)
          dispR[0] = glyphs[11];      //sets first digit to -1 if score is -100 or below
        break;
    }
  }
}

void setQ(){
  dispQ[1] = glyphs[qNum%10];   //sets second digit to ones place of qNum
  
  int x = (qNum/10)%10;         //holds tens place of scR as potential first digit
  switch (x){
    case 0:
      dispQ[0] = glyphs[12];    //blanks first digit if qNum is below 10
      break;
    default:
      dispQ[0] = glyphs[x];     //sets first digit if qNum is 10 or higher
      break;
  }
}

void setOutput(){
  setR();
  //setL();                     //will implement after setR is complete
  setQ();
  errSet();
}

void getInput(){
  boolean buttonPress = false;  //sets to false every time before checking
  
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
    delay(200);                 //delay to avoid multi-trigger from bounce
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
