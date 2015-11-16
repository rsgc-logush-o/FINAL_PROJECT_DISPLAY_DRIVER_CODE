int clockPin = 11;
int latchPin = 14;
int outputEnable = 15;
int redData = 5;
int blueData = 7;
int greenData = 9;
int redData1 = 6;
int greenData1 = 10;
int blueData1 = 8;
int rowSelectA = 16;
int rowSelectB = 17;
int rowSelectC = 18;
int rowSelectD = 19;

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
  

}

void loop() {


 for(int i = 0; i < 16; i++)
 {
    updateRows(i, j,j,j,j,j,j);
    
}


  

}



void updateRows(int row, uint32_t red, uint32_t green, uint32_t blue, uint32_t red1, uint32_t green1, uint32_t blue1 )
{
  digitalWrite(outputEnable, HIGH);
  digitalWrite(latchPin, LOW);
   digitalWrite(rowSelectA, row & B00000001);
  digitalWrite(rowSelectB, row & B00000010);
  digitalWrite(rowSelectC, row & B00000100);
  digitalWrite(rowSelectD, row & B00001000);
  for(int i = 0; i < 32; i++)
  {
    
    
    digitalWrite(redData, (red >> i) & 1);
    digitalWrite(greenData, (green >> i) & 1);
    digitalWrite(blueData, (blue >> i) & 1);
    digitalWrite(redData1, (red1 >> i) & 1);
    digitalWrite(greenData1, (green1 >> i) & 1);
    digitalWrite(blueData1, (blue1 >> i) & 1);
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);
   
    
  }
 
  digitalWrite(latchPin, HIGH);
  digitalWrite(outputEnable, LOW);
}

