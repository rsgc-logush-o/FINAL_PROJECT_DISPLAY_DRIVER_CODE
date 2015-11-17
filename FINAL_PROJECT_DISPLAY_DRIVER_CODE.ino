uint8_t clockPin = B00000001;
uint8_t latchPin = B01000000;
uint8_t outputEnable = B1000000;
uint8_t redData = B00000001;
uint8_t blueData = B00010000;
uint8_t greenData = B0000100;
uint8_t redData1 = B00000010;
uint8_t greenData1 = B00001000;
uint8_t blueData1 = B00100000;
uint8_t rowSelectA = B00000010;
uint8_t rowSelectB = B00000100;
uint8_t rowSelectC = B00001000;
uint8_t rowSelectD = B00010000;

IntervalTimer displayInterrupt;

//RED DATA PD0
//RED DATA1 PD1
//GREEN DATA PD2
//GREEN DATA1 PD3
//BLUE DATA PD4
//BLUE DATA1 PD5
//LATCH PIN PD6
//OUTPUT ENABLE PD7

//CLOCK PIN PC0
//ROWSELECT A PC1
//ROWSELECT B PC2
//ROWSELECT C PC3
//ROWSELECT D PC4

void setup() {



DDRD |= 11111111;
DDRC |= 00011111;

  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(outputEnable, OUTPUT);
  pinMode(redData, OUTPUT);
  pinMode(blueData, OUTPUT);
  pinMode(greenData, OUTPUT);
  pinMode(redData1, OUTPUT);
  pinMode(greenData1, OUTPUT);
  pinMode(blueData1, OUTPUT);
  pinMode(rowSelectA, OUTPUT);
  pinMode(rowSelectB, OUTPUT);
  pinMode(rowSelectC, OUTPUT);
  pinMode(rowSelectD, OUTPUT);
  
  displayInterrupt.priority(0);
  displayInterrupt.begin(displayToMatrix, 100000);
  

}

void loop() {




  

}



void updateRows(int row, uint32_t red, uint32_t green, uint32_t blue, uint32_t red1, uint32_t green1, uint32_t blue1 )
{
  PORTC &= ~B00011110;
  //digitalWrite(outputEnable, HIGH);
  PORTD |= outputEnable;
  //digitalWrite(latchPin, LOW);
  PORTD &= ~latchPin;
//  digitalWrite(rowSelectA, row & B00000001);
//  digitalWrite(rowSelectB, row & B00000010);
//  digitalWrite(rowSelectC, row & B00000100);
//  digitalWrite(rowSelectD, row & B00001000);

  PORTC |= row<<1;
  for(int i = 0; i < 32; i++)
  {
    
    //digitalWrite(clockPin, LOW);
    PORTC &= ~clockPin;
//    digitalWrite(redData, (red >> i) & 1);
//    digitalWrite(greenData, (green >> i) & 1);
//    digitalWrite(blueData, (blue >> i) & 1);
//    digitalWrite(redData1, (red1 >> i) & 1);
//    digitalWrite(greenData1, (green1 >> i) & 1);
//    digitalWrite(blueData1, (blue1 >> i) & 1);

    PORTD &= 11000000;
    
    PORTD |= (redData | (red >> i) ) | (redData1 | (red1 >> i << 1)) | (greenData | (green >> i << 2)) | (greenData1 |(green1 >> i << 3)) | (blueData | (blue >> i << 4)) | (blueData1 | (blue1 >> i << 5));
    
    PORTC |= clockPin;
    
   
    
  }
 
 // digitalWrite(latchPin, HIGH);
 PORTD &= ~latchPin;
 
  //digitalWrite(outputEnable, LOW);

  PORTD &= ~outputEnable;
}

void displayToMatrix()
{
  for(int i = 0; i < 16; i++)
  {
    updateRows(i, 1023, 1023, 1023, 1023, 1023, 1023);
  }
}



