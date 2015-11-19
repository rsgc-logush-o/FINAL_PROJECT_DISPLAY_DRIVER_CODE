

IntervalTimer displayInterrupt;

# define clockPin 2
# define latchPin 3
# define outputEnable 4
# define redData 5
# define blueData 9
# define greenData 7
# define redData1 6
# define greenData1 8
# define blueData1 10
# define rowSelectA 11
# define rowSelectB 12
# define rowSelectC 13
# define rowSelectD 14

int bamBit;
int bamCounter;
uint8_t red[32][32];
uint8_t green[32][32];
uint8_t blue[32][32];

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
  displayInterrupt.begin(displayToMatrix, 62500);
  

}

void loop() {




  

}





void displayToMatrix()
{
  if(bamCounter == 1 || bamCounter == 3 || bamCounter == 7) bamBit++;
  bamCounter++;
  for(int i = 0; i < 16; i++)
  {
    digitalWriteFast(outputEnable, HIGH);
  
    digitalWriteFast(latchPin, LOW);

    digitalWriteFast(rowSelectA, i & 1);
    digitalWriteFast(rowSelectB, (i>>1) & 1);
    digitalWriteFast(rowSelectC, (i>>2) & 1);
    digitalWriteFast(rowSelectD, (i>>3) & 1);

  
  for(int j = 0; j < 32; j++)
  {
    
    digitalWriteFast(clockPin, LOW);
    
    digitalWriteFast(redData, (red[j][i] >> bamBit) & 1);
    digitalWriteFast(greenData, (green[j][i] >> bamBit) & 1);
    digitalWriteFast(blueData, (blue[j][i] >> bamBit) & 1);
    digitalWriteFast(redData1, (red[j][i+16] >> bamBit) & 1);
    digitalWriteFast(greenData1, (green[j][i+16] >> bamBit) & 1);
    digitalWriteFast(blueData1, (blue[j][i+16] >> bamBit) & 1);


    
    digitalWriteFast(clockPin, HIGH);
    
   
    
  }
 
  digitalWriteFast(latchPin, HIGH);
 
 
  digitalWriteFast(outputEnable, LOW);
  }

  if(bamBit == 3 && bamCounter == 15)
  {
    bamBit = 0;
    bamCounter = 0;
  }
}

void writePixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{
 
    red[x][y] = r;
    green[x][y] = g;
    blue[x][y] = b;
  
}

