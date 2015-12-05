

IntervalTimer displayInterrupt;//SETTING THE TIMER INTERRUPT OBJECT THIS IS TEMPORARY I AM GOING TO USE THE ACTUAL TIMER REGISTERS ON THE PROCESSOR

//THESE ARE ALL OF THE PINS I AM USING THEY ARE SET TO THEIR REGION ON THEIR PORT
# define clockPin 0
# define latchPin 6
# define outputEnable 7
# define redData 0
# define blueData 4
# define greenData 2
# define redData1 1
# define greenData1 3
# define blueData1 5
# define rowSelectA 1
# define rowSelectB 2
# define rowSelectC 3
# define rowSelectD 4

//THESE ARE USED FOR KEEPING TRACK OF WHAT BAM BIT THE PROGRAM IS ON
uint8_t bamBit;
uint8_t bamCounter;

//THE COLOUR VALUES FOR EACH PIXEL
uint8_t red[32][32];
uint8_t green[32][32];
uint8_t blue[32][32];

uint8_t redBuf[32][32];
uint8_t greenBuf[32][32];
uint8_t blueBuf[32][32];

//THIS KEEPS TRACK OF WHICH ROW IT IS UPDATING
uint8_t row = 0;


//{|THIS IS A TABLE TO KEEP TRACK OF WHICH PIN IS WHERE ON EACH PORT|}

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



Serial.begin(9600);



  //SETTING EACH PIN THAT THE PROGRAM IS USING ON PORTC AS A GPIO PIN
  PORTC_PCR0 = (1<<8);
  PORTC_PCR1 = (1<<8);
  PORTC_PCR2 = (1<<8);
  PORTC_PCR3 = (1<<8);
  PORTC_PCR4 = (1<<8);
  
  //SETTING EACH PIN THAT THE PROGRAM IS USING ON PORTC AS A GPIO PIN
  PORTD_PCR0 = (1<<8);
  PORTD_PCR1 = (1<<8);
  PORTD_PCR2 = (1<<8);
  PORTD_PCR3 = (1<<8);
  PORTD_PCR4 = (1<<8);
  PORTD_PCR5 = (1<<8);
  PORTD_PCR6 = (1<<8);
  PORTD_PCR7 = (1<<8);

  //SETTING THE PINS THE PROGRAM IS USING ON PORT C AND D AS OUTPUTS
  GPIOC_PDDR = B00011111;
  GPIOD_PDDR = B11111111;

  
  //SETTING UP THE TIMER INTERRUPT
//  displayInterrupt.priority(0);
Serial.begin(9600);
contactProcessing();
  displayInterrupt.begin(displayToMatrix, 50);



  

}

void loop() {

  

if(Serial.available() > 0)
{
  for(int i = 0; i < 32; i++)
  {
   
    for(int j = 0; j < 32; j++)
    {
      int k = 0;
      while(k < 3)
      {
        if(Serial.available() > 0)
        {
          switch(k)
          {

            case 0:
              red[i][j] = Serial.read();

              break;

            case 1:
              green[i][j] = Serial.read();

              break;

            case 2:
              blue[i][j] = Serial.read();

              break;
          }
          k++;
        }
      }
    }
  }
}
  

}





void displayToMatrix()//THIS IS THE FUNCTION THAT UPDATES THE DISPLAY
{
 
  //UPDATING THE BAM BIT AND BAM COUNTER
  if(bamCounter == 16 || bamCounter == 48 || bamCounter == 112) bamBit++;
  bamCounter++;
  

    //SETTING THE OUPUT ENABLE PIN HIGH SO THERE IS NOTHING DISPLAYING WHEN SENDING THE DATA THIS IS TO PREVENT A GHOSTING EFFECT FROM HAPPENING
    GPIOD_PSOR = (1 << outputEnable);

    //SETTING THE LATCHPIN LOW TO ALLOW DATA TO BE SHIFTED IN
    GPIOD_PCOR = (1 << latchPin);


     //SELECTING WHICH ROW IS TURNED ON USING THE DEMUXER ON THE MATRIX
     GPIOC_PDOR = (row << 1);
  
        for(int i = 0; i < 32; i++)
        {
          //WRITING THE CLOCKPIN LOW
          GPIOC_PCOR = (1 << clockPin);
    

          //SETTING ALL OF THE DATA BITS IN PORTD LOW SO THAT SETTING THEM HIGH WORKS PROPERLY IF THIS DID NOT HAPPEN THEN I WOULD NEED TO SET THEM ALL AND THAT WOULD INTERFERE WITH THE LATCH AND OUTPUTENABLE PINS
          GPIOD_PCOR = B00111111;

          //SETTING ALL OF THE DATA PINS TO THEIR VALUE IN THE CURRENT BIT FOR BIT ANGLE MODULATION
          
          GPIOD_PSOR = (((red[i][row] >> bamBit) & 1) | (((red[i][row + 16] >> bamBit) & 1) << redData1) | (((green[i][row] >> bamBit) & 1) << greenData) | (((green[i][row + 16] >> bamBit) & 1) << greenData1) | (((blue[i][row] >> bamBit) & 1) << blueData) | (((blue[i][row + 16] >> bamBit) & 1) << blueData1));
          

    
          //SETTING THE CLOCKPIN HIGH BECAUSE THE DATA IS SET ON THE RISING EDGE OF THE CLOCKPIN
          GPIOC_PSOR = (1 << clockPin);
    
        }
    
  
 
 
      //SETTING THE LATCHPIN HIGH TO LATCH THE DATA IN
      GPIOD_PSOR = (1 << latchPin);
 
 
      //SETTING THE OUPUTENABLE PIN LOW TO DISPLAY THE DATA
      GPIOD_PCOR = (1 << outputEnable);
 
      //IF THE BAM HAS REACHED IT'S LIMIT RESET IT
      if(bamCounter == 240)
      {
        bamBit = 0;
        bamCounter = 0;
      }
      //INCREMENT THE ROW IN WHICH YOU ARE WRITING TO
      row++;
      //IF THE ROW HAS REACHED ITS LIMIT RESET IT
      if(row == 16) row = 0;

      
}

void writePixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)//THE FUNCTION TO WRITE TO THE PIXELS
{
    //WRITING THE VALUE OF RED GREEN AND BLUE TO THE SELECTED PIXELS
    red[x][y] = r;
    green[x][y] = g;
    blue[x][y] = b;
  
}

void contactProcessing()
{
  while(Serial.available() <= 0)
  {
    Serial.println("Y");
    delay(250);
  }
}



