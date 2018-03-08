byte glyphs[13] = {0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6, 0x02, 0x62, 0x00};
                 // 0:0, 1:1,  2:2,  3:3,  4:4,  5:5,  6:6,  7:7,  8:8,  9:9,  10:-, 11:-1,12:(blank)

int latchPin = 2;             //connected to TPIC6C596 pin 10 [RCK]
int clockPin = 3;             //connected to TPIC6C596 pin 15 [SRCK]
int dataPin = 4;              //connected to TPIC6C596 pin 2  [SER IN]

int scR = 5;                  //score counter right
int scL = 5;                  //score counter left
int qNum = 1;                 //question number

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

  getInput();                 //check for button press and change values accordingly

  updateDisplay(prepDisplay(scL, qNum, scR));

  if (scR > 52){              //theoretical max score for a quiz is 520
    errMsg();
    scR=0;
  }
  if (scR < 0){
    errMsg();
    scR=0;
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


void push(byte x){
  shiftOut(dataPin, clockPin, LSBFIRST, x);
}

long prepDisplay(long l, long q, long r){
  long output = (q*1000000)+(l*10000)+(r*10);
  return output;
}

void updateDisplay(long x){
  digitalWrite (latchPin, LOW);
  push(glyphs[x%10]);
  push(glyphs[(x/10)%10]);
  push(glyphs[(x/100)%10]);
  push(glyphs[(x/1000)%10]);
  push(glyphs[(x/10000)%10]);
  push(glyphs[(x/100000)%10]);
  push(glyphs[(x/1000000)%10]);
  push(glyphs[(x/10000000)%10]);
  digitalWrite (latchPin, HIGH);
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
