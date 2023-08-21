#define SERIAL_PORT_SPEED 9600
#define RC_NUM_CHANNELS 12

//movement define
#define dir1 4
#define step1 22

#define dir2 5
#define step2 24

#define dir3 6
#define step3 26

#define dir4 8 
#define step4 28

// controller
#define RC_CH6 6 


// Set up our channel pins - these are the pins that we connect to the receiver
#define RC_CH6_INPUT 19


// Set up some arrays to store our pulse starts and widths
uint16_t RC_VALUES[RC_NUM_CHANNELS];
uint32_t RC_START[RC_NUM_CHANNELS];
volatile uint16_t RC_SHARED[RC_NUM_CHANNELS];


void setup() {
  
  Serial.begin(9600);
  
  pinMode(RC_CH6_INPUT, INPUT);

  // Attach interrupts to our pins
  attachInterrupt(digitalPinToInterrupt(RC_CH6_INPUT), READ_RC6, CHANGE);
  
  //movement cod
  pinMode(dir1,OUTPUT);
  pinMode(step1,OUTPUT);
  
  pinMode(dir2,OUTPUT);
  pinMode(step2,OUTPUT);
  
  pinMode(dir3,OUTPUT);
  pinMode(step3,OUTPUT);

  pinMode(dir4,OUTPUT);
  pinMode(step4,OUTPUT);
 
}


void loop() {
  // put your main code here, to run repeatedly:
  rc_read_values();
  int channe6 = RC_VALUES[RC_CH6];
  
 
  //channele 6
  if(channe6<1190)
  {
    Serial.println("Aoutocenter");
  }

  if (Serial.available()) {
    String message = Serial.readString();

    if(message=="Right")
    {
      digitalWrite(dir1,HIGH);
      digitalWrite(dir2,HIGH);
      digitalWrite(dir3,LOW);
      digitalWrite(dir4,LOW);
      motor();
    }
    if(message=="Left")
    {
      digitalWrite(dir1,LOW);
      digitalWrite(dir2,LOW);
      digitalWrite(dir3,HIGH);
      digitalWrite(dir4,HIGH);
      motor();
    }
    if(message=="Stop")
    {
      digitalWrite(step1,LOW);
      digitalWrite(step2,LOW);
      digitalWrite(step3,LOW);
      digitalWrite(step4,LOW);
    }
  }
}

// Thee functions are called by the interrupts. We send them all to the same place to measure the pulse width
void READ_RC6() { 
   Read_Input(RC_CH6, RC_CH6_INPUT); 
}


// This function reads the pulse starts and uses the time between rise and fall to set the value for pulse width
void Read_Input(uint8_t channel, uint8_t input_pin) {
  if (digitalRead(input_pin) == HIGH) {
    RC_START[channel] = micros();
  } else {
    uint16_t rc_compare = (uint16_t)(micros() - RC_START[channel]);
    RC_SHARED[channel] = rc_compare;
  }
}

// this function pulls the current values from our pulse arrays for us to use. 
void rc_read_values() {
  noInterrupts();
  memcpy(RC_VALUES, (const void *)RC_SHARED, sizeof(RC_SHARED));
  interrupts();
}

//movement function
void motor()
{
  digitalWrite(step1,HIGH);
  digitalWrite(step2,HIGH);
  digitalWrite(step3,HIGH);
  digitalWrite(step4,HIGH);
  delayMicroseconds(2000);
  digitalWrite(step1,LOW);
  digitalWrite(step2,LOW);
  digitalWrite(step3,LOW);
  digitalWrite(step4,LOW);
  delayMicroseconds(2000);
}
