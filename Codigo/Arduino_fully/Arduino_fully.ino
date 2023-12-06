  //Potentiostato || Felipe Avila Silva || UFPel - Universidade Federal de Pelotas
  
  #include <stdio.h>
  #include <stdlib.h>
  #include <time.h>
  #include <eRCaGuy_analogReadXXbit.h>

  eRCaGuy_analogReadXXbit adc;

  const float MAX_READING_10_bit = 1023.0;  //resoluções
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

  int bits_of_precision = 12;
  int num_samples = 16;


  int Pinread = A3;                         //variaveis dos metodos
  const int PinPWM = 10;
  int Time;                   // Its used at Chronoamperometry to defined sampled time
  int PWM = 0;                    // Saved final value of applied potential in PWM values  
  float StartPotential;       // These constants are used to store numerical values of potentials that user enters into serial monitor
  float EndPotential;
  int Startpot;
  int Endpot;
  long Intervals;             //Time delay to obtain expected scan rate
  int Scanrate;               // scan rate  


  //calibração
  //calibração do prototipo e validado   SINGLE
  /*float Vmax = 1.721;                                 //These constants are used to store numerical values resulting from Potential calibration. Signs are included in the respective equations.
  float Vmin = 1.391;
  float Imax = 749.00;                               //These constants are used to store numerical values resulting from current calibration calibration. Signs are included in the respective equations.
  float Imin = 590.00;
  int AnalogReadingmax = 918;
  int AnalogReadingmin = 0;*/


  //calibração do prototipo e validado  FULLY 12 bits
  float Vmax = 1.525;                                 //These constants are used to store numerical values resulting from Potential calibration. Signs are included in the respective equations.
  float Vmin = 1.492;
  float Imax = 1543.00;                                //These constants are used to store numerical values resulting from current calibration calibration. Signs are included in the respective equations.
  float Imin = 1509.00;
  int AnalogReadingmax = 4078.44;
  int AnalogReadingmin = 0;

    //calibração do prototipo e validado  FULLY
  /*float Vmax = 1.512;                                 //These constants are used to store numerical values resulting from Potential calibration. Signs are included in the respective equations.
  float Vmin = 1.498;
  float Imax = 687.00;                                //These constants are used to store numerical values resulting from current calibration calibration. Signs are included in the respective equations.
  float Imin = 678.00;
  int AnalogReadingmax = 8161.09;
  int AnalogReadingmin = 0;*/


  /*//calibração do circuito simulado                              SINGLE
  float Vmax = 1.499;                                //These constants are used to store numerical values resulting from Potential calibration. Signs are included in the respective equations.
  float Vmin = 1.495;
  float Imax = 681.81;                               //These constants are used to store numerical values resulting from current calibration calibration. Signs are included in the respective equations.
  float Imin = 679.57;
  int AnalogReadingmax = 1023;
  int AnalogReadingmin = 7;*/


  /*//calibração do circuito simulado                                 FULLY
  float Vmax = 1.495;                                //These constants are used to store numerical values resulting from Potential calibration. Signs are included in the respective equations.
  float Vmin = 1.490;
  float Imax = 679.64;                               //These constants are used to store numerical values resulting from current calibration calibration. Signs are included in the respective equations.
  float Imin = 677.41;
  int AnalogReadingmax = 1023;
  int AnalogReadingmin = 8;*/


  //Variavel global
  String valString = "";
  unsigned long startTime; // Variável para armazenar o tempo de início
 

  void Chronoamp() {
   
    while (!Serial.available()) {;} 
    StartPotential = Serial.parseFloat(); 
    Startpot = (StartPotential -Vmax) * (255 -0) / (-Vmin -Vmax) + 0; //Potential to PWM values

    
    while (!Serial.available()) {;}   
    Time = (Serial.parseInt());
   
    //while (!Serial.available()) {    
      for (float x = 1; x <= Time; x = x + 0.1){       
        
        analogWrite(PinPWM, Startpot);        
        delay(100);  
        float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);      
        float tableP = x;
        float tableC = (analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin; //Convert value of analog reading to CurrentSerial.println(tableC, 3);delay(Time);}
      
        valString = "<";
        valString.concat(tableP);
        valString.concat(";");
        valString.concat(tableC);
        valString.concat(">");
        Serial.println(valString);             
      }    
      int finish = Serial.parseInt();
      valString = "<finish>";
      Serial.println(valString);           
}

  void cyclic() {          
    while (!Serial.available()) {;}     
    Scanrate = Serial.parseInt();   
    
    while (!Serial.available()) {;}
    StartPotential = Serial.parseFloat();
    Startpot = (StartPotential -Vmax) * (255 -0) / (-Vmin -Vmax) + 0; //Potential to PWM values    
     
    while (!Serial.available()) {;}
    EndPotential = Serial.parseFloat();
    Endpot = (EndPotential -Vmax) * (255 -0) / (-Vmin -Vmax) + 0; //Potential to PWM values 
        
    while (!Serial.available()) {;}     
    int loop = Serial.parseInt();
    delay(2000);

    float temp = millis();
    float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
    float temp2 = millis() - temp; 

    for (int i = 0; i < loop; i++){
      if (Startpot > Endpot) {        
        Intervals = ((1000000L / ((Scanrate) * 168)) - temp2);//based in scanrate is determinated time delays to obtained this rate
        for (PWM = Startpot; PWM >= Endpot; PWM--) {                 
          
          analogWrite(PinPWM, PWM); // apply current potential to pin 9        
          float tableP = -((PWM -0) * (-Vmin -Vmax) / (255 -0) + Vmax); //Convert current value of PWM to PotentialSerial.
          //Serial.print(tableP);
          delay(Intervals);    

          float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
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
        
          analogWrite(PinPWM, PWM); // apply current potential to pin 9
          float tableP = -((PWM -0) * (-Vmin -Vmax) / (255 -0) + Vmax); //Convert current value of PWM to Potential
          //Serial.print(tableP);
          delay(Intervals);
        
          float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
          float tableC = -((analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin); //Convert value of analog reading to Current.
          //Serial.println(tableC, 3);    
        
          valString = "<";
          valString.concat(tableP);
          valString.concat(";");
          valString.concat(tableC);
          valString.concat(">");
          Serial.println(valString);    
        }   
        //int finish = Serial.parseInt();        
         
      }else if (Startpot < Endpot) {
        Intervals = ((1000000L / ((Scanrate) * 168)) - temp2);//based in scanrate is determinated time intervals to obtained this rate
        for ( PWM = Startpot ; PWM <= Endpot ; PWM++) {          
        
          analogWrite(PinPWM, PWM); // apply current potential to pin 9        
          float tableP = -((PWM -0) * (-Vmin -Vmax) / (255 -0) + Vmax); //Convert current value of PWM to PotentialSerial.
          //Serial.print(tableP);
          delay(Intervals);
       
          float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
          float tableC = -((analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin); //Convert value of analog reading to Current
          //Serial.println(tableC, 3);
        
          valString = "<";
          valString.concat(tableP);
          valString.concat(";");
          valString.concat(tableC);
          valString.concat(">");
          Serial.println(valString);

        }for ( PWM = Endpot; PWM >= Startpot; PWM--) {
               
          analogWrite(PinPWM, PWM); // apply current potential to pin 9
          float tableP = -((PWM -0) * (-Vmin -Vmax) / (255 -0) + Vmax); //Convert current value of PWM to Potential
          //Serial.print(tableP);
          delay(Intervals);
        
          float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
          float tableC = -((analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin); //Convert value of analog reading to Current.
          //Serial.println(tableC, 3);    
        
          valString = "<";
          valString.concat(tableP);
          valString.concat(";");
          valString.concat(tableC);
          valString.concat(">");
          Serial.println(valString);
        }        
      } 
    } //loop
      int finish = Serial.parseInt();
      valString = "<finish>";
      Serial.println(valString);      
  } //função


  void lineal() {   

    while (!Serial.available()) {;}
    Scanrate = Serial.parseInt();         

    while (!Serial.available()) {;}
    StartPotential = Serial.parseFloat();
    Startpot = (StartPotential -Vmax) * (255 -0) / (-Vmin -Vmax) + 0; //Potential to PWM values
    
    while (!Serial.available()) {;}
    EndPotential = Serial.parseFloat();
    Endpot = (EndPotential -Vmax) * (255 -0)/ (-Vmin -Vmax) + 0; //Potential to PWM values

    while (!Serial.available()) {;}     
    int loop = Serial.parseInt();
    delay(2000);
    for (int i = 0; i < loop; i++){
      if (Startpot > Endpot) {
        Intervals = (1000000L / ((Scanrate) * 168)); //based in scanrate is determinated time delays to obtained this rate
        for ( PWM = Startpot; PWM >= Endpot; PWM--) {
                 
          analogWrite(PinPWM, PWM); // apply current potential to pin 9        
          float tableP = (PWM -0) * (-Vmin -Vmax) / (255 -0) + Vmax; //Convert current value of PWM to PotentialSerial.
          //Serial.print(tableP);
          delay(Intervals);
        
          float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
          float tableC = ((analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin); //Convert value of analog reading to Current
          //Serial.println(tableC, 3);
        
          valString = "<";
          valString.concat(tableP);
          valString.concat(";");
          valString.concat(tableC);
          valString.concat(">");
          Serial.println(valString);
        }       
      }else if (Startpot < Endpot) {
        Intervals = (1000000L / ((Scanrate) * 168));//based in scanrate is determinatedtime delays to obtained this rate
        for ( PWM = Startpot ; PWM <= Endpot ; PWM++) {        
        
          analogWrite(PinPWM, PWM); // apply current potential to pin 9        
          float tableP = (PWM -0) * (-Vmin -Vmax) / (255 -0) + Vmax; //Convert current value of PWM to PotentialSerial.
          //Serial.print(tableP);
          delay(Intervals);
        
          float analog_reading = adc.analogReadXXbit(Pinread, bits_of_precision, num_samples);
          float tableC = ((analog_reading -AnalogReadingmin) * (Imax + Imin) / (AnalogReadingmax -AnalogReadingmin) -Imin); //Convert value of analog reading to Current
          //Serial.println(tableC, 3);
        
          valString = "<";
          valString.concat(tableP);
          valString.concat(";");
          valString.concat(tableC);
          valString.concat(">");
          Serial.println(valString);
        }           
      }
    }
    int finish = Serial.parseInt();
      valString = "<finish>";
      Serial.println(valString);       
 }


void setup() {
  TCCR1B = TCCR1B & B11111000 | B00000001;
  Serial.begin(115200);  
  analogReference(DEFAULT);
  pinMode(PinPWM, OUTPUT);
  pinMode(Pinread, INPUT);  
  analogWrite(PinPWM, 128);
}


void loop() {
  for(;;){ 
    analogWrite(PinPWM, 128);
    while (!Serial.available()) {;}
    switch (Serial.read()) {
       case'0':      
        cyclic();        
       break;

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
  }    
}