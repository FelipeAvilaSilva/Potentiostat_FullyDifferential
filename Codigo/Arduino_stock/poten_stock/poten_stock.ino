//Potentiostat sketch
// constants defined for library

#include <eRCaGuy_analogReadXXbit.h>
eRCaGuy_analogReadXXbit adc;

const float MAX_READING_10_bit = 1023.0;
const float MAX_READING_11_bit = 2046.0;
const float MAX_READING_12_bit = 4092.0;
const float MAX_READING_13_bit = 8184.0;
const float MAX_READING_14_bit = 16368.0;
const float MAX_READING_15_bit = 32736.0;
const float MAX_READING_16_bit = 65472.0;
const float MAX_READING_17_bit = 130944.0;
const float MAX_READING_18_bit = 261888.0;
const float MAX_READING_19_bit = 523776.0;
const float MAX_READING_20_bit = 1047552.0;
const float MAX_READING_21_bit = 2095104.0;

  const int motorPin = 3;     // Its used at Linear sweep voltammetry to controlled rotational speedconst
  int Pinread = A0;
  const int PinPWM = 9;
  int Time;                   // Its used at Chronoamperometry to defined sampled time
  int PWM;                    // Saved final value of applied potential in PWM values
  float StartPotential;       // These constants are used to store numerical values of potentials that user enters into serial monitor
  float EndPotential;
  int Startpot;
  int Endpot;
  long Intervals;             //Time delay to obtain expected scan rate
  int Scanrate;               // scan rate
  float Standtime;            // waiting time before start experiment
  float Vmax = 1.616;         //These constants are used to store numerical values resulting from Potential calibration. Signs are included in the respective equations.
  float Vmin = 1.358;
  float Imax = 637.61;        //These constants are used to store numerical values resulting from current calibration calibration. Signs are included in the respective equations.
  float Imin = 623.85;
  int AnalogReadingmax = 4081;
  int AnalogReadingmin = 229;

  String valString = "";
  char cmd;
  
  void Chronoamp() {
    Serial.println(" Chronoamperometry");
    delay(200);
    Serial.println ("Enter fixed Potential ");
    Serial.println ("ALLOWED RANGE: -1.36 a +1.41 volts");

    while (!Serial.available()) {;}

    StartPotential = Serial.parseFloat();
    Startpot = (StartPotential -Vmax) * (255 -0) / (-Vmin -Vmax) + 0; //Potential to PWM values
    delay(200);
    Serial.print("Start Potential:  ");
    Serial.println(StartPotential);
    delay(200);
    Serial.println ("Enter sampled time");
    
    while (!Serial.available()) {;}
    
    Serial.println ("Remember, to stop experiment enter 1");
    Time = Serial.parseFloat();
    delay(200);
    Serial.print("sampled time:  ");
    Serial.println(Time);
    delay(200);
    Serial.println("Experiment starts in 10 seconds");
    delay(10000);
    analogWrite(PinPWM, Startpot);
    Serial.println ("I (uA)");


    while (!Serial.available()) {
      int bits_of_precision = 12;
      int num_samples= 16;
      float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
      float tableC = (analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin; //Convert value of analog reading to CurrentSerial.println(tableC, 3);delay(Time);}
    }
    int Stop = Serial.parseInt();
    if (Stop == 1) {
      delay(1000);
      Serial.println("Please, copy data and after press CLEAR OUTPUT in serial monitor");
      delay(500);
      Serial.println(" Enter 5 to return main menu");
      while (!Serial.available()) {;}
      int finish = Serial.parseInt();
      delay(2000);
    }      
  }

  void cyclic() {
    int temp;
    Serial.println("Cyclic Voltammetry");
    Serial.println("ENTER SCAN RATE");
    Serial.println("ALLOWED RANGE: 1 -250 mV/s");
    delay(200);
    while (!Serial.available()) {;}     
    Scanrate = Serial.parseInt();  
    //Scanrate = 100;  
    delay(200);
    Serial.print("Scan rate:  ");
    Serial.print(Scanrate);
    Serial.println ("mV/s");
    delay(200);
    Serial.println("");
    Serial.println("");
    Serial.println ("Enter start potential");
    Serial.println ("Warning");
    Serial.println ("ALLOWED RANGE: -1.36 a +1.41 volts");

    //while (!Serial.available()) {;}

    StartPotential = -1; //Serial.parseFloat();
    Startpot = (StartPotential -Vmax) * (255 -0) / (-Vmin -Vmax) + 0; //Potential to PWM values
    delay(200);
    Serial.print("Start Potential:  ");
    Serial.println(StartPotential);
    delay(200);
    Serial.println("");
    Serial.println("");
    Serial.println ("Enter end potential");
    Serial.println ("Warning");
    Serial.println("ALLOWED RANGE: -1.36 a +1.41 volts");

    //while (!Serial.available()) {;}

    EndPotential = 1; //Serial.parseFloat();
    Endpot = (EndPotential -Vmax) * (255 -0) / (-Vmin -Vmax) + 0; //Potential to PWM values
    delay(200);
    Serial.print("End Potential:  ");
    Serial.println(EndPotential);
    delay(200);
    Serial.println("");
    Serial.println("");
    Serial.println (" Enter standby time");
    Serial.println ("Max 20 seconds");

    //while (!Serial.available()) {;}

    Standtime = Serial.parseInt();
    int st = Standtime * 1000;
    delay(200);
    Serial.print ("standby time:  ");
    Serial.println (st);

    Serial.println(" ");
    delay(st);
    Serial.println (" ");
    Serial.println (" ");
    Serial.print ("E (V) ");
    Serial.print(" ");
    Serial.println ("I (uA)");
    Serial.println (" ");
    if (Startpot > Endpot) {
      Intervals = (1000000L / ((Scanrate) * 128L));//based in scanrate is determinated time delays to obtained this rate
      for ( PWM = Startpot; PWM >= Endpot; PWM--) {
        int bits_of_precision = 12;
        int num_samples = 16;
        float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
        analogWrite(PinPWM, PWM); // apply current potential to pin 9
        float tableP = (PWM -0) * (-Vmin -Vmax) / (255 -0) + Vmax; //Convert current value of PWM to PotentialSerial.
        //Serial.print(tableP);
        delay(Intervals);
        Serial.print(" ");
        float tableC = -((analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin); //Convert value of analog reading to Current
        //Serial.println(tableC, 3);

        
        valString = "<";
        valString.concat(tableP);
        valString.concat(";");
        valString.concat(tableC);
        valString.concat(">");
        Serial.println(valString);
        

      }
      for ( PWM = Endpot ; PWM <= Startpot ; PWM++) {
        int bits_of_precision = 12;
        int num_samples = 16;
        float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
        analogWrite(PinPWM, PWM); // apply current potential to pin 9
        float tableP = (PWM -0) * (-Vmin -Vmax) / (255 -0) + Vmax; //Convert current value of PWM to Potential
        //Serial.print(tableP);
        delay(Intervals);
        Serial.print(" ");
        float tableC = -((analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin); //Convert value of analog reading to Current.
        //Serial.println(tableC, 3);    

        
        valString = "<";
        valString.concat(tableP);
        valString.concat(";");
        valString.concat(tableC);
        valString.concat(">");   
        Serial.println(valString);    
        

      }
      delay(1000);
      Serial.println("Please, copy data and after press CLEAR OUTPUT in serial monitor");
      delay(500);
      Serial.println(" Enter 5 to return main menu");
      
      while (!Serial.available()) {;}

      int finish = Serial.parseInt();
      delay(2000); 
    }else if (Startpot < Endpot) {
      Intervals = (1000000L / ((Scanrate) * 128L));//based in scanrate is determinated time intervals to obtained this rate
      for ( PWM = Startpot ; PWM <= Endpot ; PWM++) {
        int bits_of_precision = 12;
        int num_samples = 16;
        float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
        analogWrite(PinPWM, PWM); // apply current potential to pin 9
        float tableP = (PWM -0) * (-Vmin -Vmax) / (255 -0) + Vmax; //Convert current value of PWM to Potential
        Serial.print(tableP);
        delay(Intervals);
        Serial.print(" ");
        float tableC = (analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin; //Convert value of analog reading to Current.
        Serial.println(tableC, 3);
      }for ( PWM = Endpot; PWM >= Startpot; PWM--) {
        int bits_of_precision = 12;
        int num_samples = 16;
        float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
        analogWrite(PinPWM, PWM); // apply current potential to pin 9
        float tableP = (PWM -0) * (-Vmin -Vmax) / (255-0) + Vmax; //Convert current value of PWM to Potential
        Serial.print(tableP);
        delay(Intervals);
        Serial.print(" ");
        float tableC = (analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin; //Convert value of analog reading to Current.
        Serial.println(tableC, 3);
      }
      delay(1000);
      Serial.println("Please, copy data and after press CLEAR OUTPUT in serial monitor");
      delay(500);
      Serial.println(" Enter 5 to return main menu");
      
      while (!Serial.available()) {;}

      int finish = Serial.parseInt();
      delay(2000);

      } else {;}        
  }


  void lineal() {
    Serial.println("Lineal Sweep Voltammetry");
    Serial.println("Disc rotating voltammetry?");
    Serial.println("YES: Enter value (ALLOWED RANGE: 0 -255) NO: Enter 0 (zero)");
    delay(200);
    
    while (!Serial.available()) {}
    
    Serial.println("Electrode will start rotating in 5 seconds");
    int speedmotor = Serial.parseInt();
    delay(5000);
    analogWrite(motorPin, speedmotor);
    Serial.println(speedmotor);
    delay(200);
    Serial.println("ENTER SCAN RATE");
    Serial.println("ALLOWED RANGE: 1 -250 mV/s");
    delay(200);

    while (!Serial.available()) {;}

    Scanrate = Serial.parseInt();
    delay(200);
    Serial.print("Scan rate:  ");
    Serial.print(Scanrate);
    Serial.println ("mV/s");
    delay(200);
    Serial.println("");
    Serial.println("");
    Serial.println ("Enter start potential");
    Serial.println ("Warning");
    Serial.println ("ALLOWED RANGE: -1.36 a +1.41 volts");

    while (!Serial.available()) {;}

    StartPotential = Serial.parseFloat();
    Startpot = (StartPotential -Vmax) * (255 -0) / (-Vmin -Vmax) + 0; //Potential to PWM values
    delay(200);
    Serial.print("Start Potential:  ");
    Serial.println(StartPotential);
    delay(200);
    Serial.println("");
    Serial.println("");
    Serial.println ("Enter end potential");
    Serial.println ("Warning");
    Serial.println ("ALLOWED RANGE: -1.36 a +1.41 volts");

    while (!Serial.available()) {;}

    EndPotential = Serial.parseFloat();
    Endpot = (EndPotential -Vmax) * (255 -0)/ (-Vmin -Vmax) + 0; //Potential to PWM values
    delay(200);
    Serial.print("End Potential:  ");
    Serial.println(EndPotential);
    delay(200);
    Serial.println("");
    Serial.println("");
    Serial.println(" Enter standby time");
    Serial.println ("Max 20 seconds");

    while (!Serial.available()) {;}

    Standtime = Serial.parseInt();
    int st = Standtime * 1000;
    delay(200);
    Serial.print("standby time:  ");
    Serial.println (st);
    Serial.println(" ");
    delay(st);
    Serial.println (" ");
    Serial.println (" ");
    Serial.print ("E (V) ");
    Serial.print(" ");
    Serial.println ("I (uA)");
    Serial.println (" ");


    if (Startpot > Endpot) {
      Intervals = (1000000L / ((Scanrate) * 128L));//based in scanrate is determinated time delays to obtained this rate
      for ( PWM = Startpot; PWM >= Endpot; PWM--) {
        int bits_of_precision = 12;
        int num_samples = 16;
        float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
        analogWrite(PinPWM, PWM); // apply current potential to pin 9
        float tableP = (PWM -0) * (-Vmin -Vmax) / (255 -0) + Vmax; //Convert current value of PWM to Potential
        Serial.print(tableP);
        delay(Intervals);
        Serial.print(" ");
        float tableC = (analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin; //Convert value of analog reading to Current.
        Serial.println(tableC, 3);
      }

      delay(1000);
      Serial.println("Please, copy data and after press CLEAR OUTPUT in serial monitor");
      delay(500);
      Serial.println(" Enter 5 to return main menu");

      while (!Serial.available()) {;}

      int finish = Serial.parseInt();
      delay(2000);
      
    }else if (Startpot < Endpot) {
      Intervals = (1000000L / ((Scanrate) * 128L));//based in scanrate is determinatedtime delays to obtained this rate
      for ( PWM = Startpot ; PWM <= Endpot ; PWM++) {
        int bits_of_precision = 12;
        int num_samples = 16;
        float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
        analogWrite(PinPWM, PWM); // apply current potential to pin 9
        float tableP = (PWM -0) * (-Vmin -Vmax) / (255 -0) + Vmax; //Convert current value of PWM to Potential
        Serial.print(tableP);
        delay(Intervals);
        Serial.print(" ");
        float tableC = (analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin; //Convert value of analog reading to Current.
        Serial.println(tableC, 3);
      }
      
      delay(1000);
      Serial.println("Please, copy data and after press CLEAR OUTPUT in serial monitor");
      delay(500);
      Serial.println("Enter 5 to return main menu");

      while (!Serial.available()) {;}

      delay(2000);
      int finish = Serial.parseInt();
    } else {;}     
 }
  





void setup() {
  TCCR1B = TCCR1B & B11111000 | B00000001;
  Serial.begin(9600);
  analogReference(1);
  pinMode(PinPWM, OUTPUT);
  pinMode(Pinread, INPUT);
}

void loop() {
  /*for(;;){
    Serial.println("Select technique: ");
    Serial.println("1 -CYCLIC VOLTAMMETRY ");
    Serial.println("2 -LINEAR SWEEP VOLTAMMETRY ");
    Serial.println("3 -CHRONOAMPEROMETRY ");

    while (!Serial.available()) {;}

    switch (Serial.read()) {
      case'1':
        cyclic();
       break;
       
       case'2':
        lineal();
       break;
       
       case'3':
        Chronoamp();
       break;
       
       default: continue;}   
    }*/
    cyclic();
}
