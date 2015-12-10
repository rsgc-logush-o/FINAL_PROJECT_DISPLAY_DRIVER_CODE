
//////////////////////////////////////////////////
//               OLIVER LOGUSH                  //
//                   2015                       //
//  DISPLAY DRIVER FOR A 32x32 RGB LED DISPLAY  //
//                                              //
//                Version 1.0                   //
//                                              //
//                                              //
//////////////////////////////////////////////////


//This is the interval timer that calls an iterrupt
IntervalTimer displayInterrupt;



//Defining all of the pins locations on each port register
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

//These are the variables that keep track of where the bit angle modulation is at
uint8_t bamBit;
uint8_t bamCounter;

//These are the arrays for each colour value of each pixel
uint8_t red[32][32];
uint8_t green[32][32];
uint8_t blue[32][32];

//This keeps track of the row that the program is updating
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


  //Begins the serial communication
  Serial.begin(9600);



  //Setting all of the pins used in PORTC as GPIO pins
  PORTC_PCR0 = (1 << 8);
  PORTC_PCR1 = (1 << 8);
  PORTC_PCR2 = (1 << 8);
  PORTC_PCR3 = (1 << 8);
  PORTC_PCR4 = (1 << 8);

  //Setting all of the pins used in PORTD as GPIO pins
  PORTD_PCR0 = (1 << 8);
  PORTD_PCR1 = (1 << 8);
  PORTD_PCR2 = (1 << 8);
  PORTD_PCR3 = (1 << 8);
  PORTD_PCR4 = (1 << 8);
  PORTD_PCR5 = (1 << 8);
  PORTD_PCR6 = (1 << 8);
  PORTD_PCR7 = (1 << 8);

  //Setting the appropriate pins as outputs
  GPIOC_PDDR = B00011111;
  GPIOD_PDDR = B11111111;



  //call contactProcessing which does not allow the program to start until it has recieved communication from processing
  contactProcessing();
  //Initialize the displayInterrupt calling it every 50 microseconds to call the displayToMatrix function
  displayInterrupt.begin(displayToMatrix, 50);





}

void loop() {


  //If it recieves something over the serial port it will begin these loops to recieve the data and store it
  if (Serial.available() > 0)
  {
    for (int i = 0; i < 32; i++)
    {

      for (int j = 0; j < 32; j++)
      {
        int k = 0;
        while (k < 3)
        {
          if (Serial.available() > 0)
          {
            switch (k)
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





void displayToMatrix()
{

  //This increments the bamBit once the bamCounter reaches the appropriate value to move on
  if (bamCounter == 16 || bamCounter == 48 || bamCounter == 112) bamBit++;
  //Increments the bam counter once per cycle
  bamCounter++;


  //This sets the latchPin low and outputEnable high to allow new data to be shifted in
  GPIOD_PSOR = (1 << outputEnable);
  GPIOD_PCOR = (1 << latchPin);


  //Setting the row demux to the row number it is on
  GPIOC_PDOR = (row << 1);

  //This loop shifts out all of the data in the right bam bit to the right row
  for (int i = 0; i < 32; i++)
  {
    //This writes the clockPin low
    GPIOC_PCOR = (1 << clockPin);


    //This clears the data pins
    GPIOD_PCOR = B00111111;

    
    //This snippet of code sets the data pins to the values they need to be based on the row count and the bamBit
    GPIOD_PSOR = (((red[i][row] >> bamBit) & 1) | (((red[i][row + 16] >> bamBit) & 1) << redData1) | (((green[i][row] >> bamBit) & 1) << greenData) | (((green[i][row + 16] >> bamBit) & 1) << greenData1) | (((blue[i][row] >> bamBit) & 1) << blueData) | (((blue[i][row + 16] >> bamBit) & 1) << blueData1));



    //This sets the clockPin high
    GPIOC_PSOR = (1 << clockPin);

  }




  //Set the latchPin high and outputEnable low to allow the pixels to be displayed
  GPIOD_PSOR = (1 << latchPin);
  GPIOD_PCOR = (1 << outputEnable);

  //If the bamCounter has reached the max value reset it and the bamBit to 0
  if (bamCounter == 240)
  {
    bamBit = 0;
    bamCounter = 0;
  }

  //Increment the row counter
  row++;

  //If the row has reached 16 reset it
  if (row == 16) row = 0;


}

//The function to write individual pixels (used mainly for debugging)
void writePixel(uint8_t x, uint8_t y, uint8_t r, uint8_t g, uint8_t b)
{
  
  red[x][y] = r;
  green[x][y] = g;
  blue[x][y] = b;

}

//This is the function that halts the program until processing has sent something to confirm the connection
void contactProcessing()
{
  while (Serial.available() <= 0)
  {
    Serial.println("Y");
    delay(250);
  }
}



