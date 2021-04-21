const int POT = A0;

byte current_digit;
int  adc_value;
bool converted;

void setup(){
  Serial.begin(9600);
  DDRD = 0b01111111;
  DDRB = 0b00001111;
  adc_value = analogRead(POT);
  disp_off();
  adc_setup();
  timer1_setup();
}

ISR(TIMER1_OVF_vect){
  disp_off();
  switch (current_digit){
    case 1:
      disp(adc_value / 1000); // prepare to display digit 1 (most left)
      PORTB &= 0b11111110; // Encender digito 1
      break;
    case 2:
      disp( (adc_value / 100) % 10); // prepare to display digit 2
      PORTB &= 0b11111101; // Encender digito 2
      break;
    case 3:
      disp( (adc_value / 10) % 10); // prepare to display digit 3
      PORTB &= 0b11111011; // Encender digito 3
      break;
    case 4:
      disp(adc_value % 10); // prepare to display digit 4 (most right)
      PORTB &= 0b11110111; // Encender digito 4
  }
  current_digit = (current_digit % 4) + 1;
}

void loop(){
  //adc_value = analogRead(POT);
  //delay(100);

  if (converted){
    bitSet(ADCSRA, ADSC); // C3:: start converting voltage on A0
    converted = false;
    }
  if (bit_is_clear(ADCSRA, ADSC)){
    int value = ADC;      // C4:: when ADSC = 0 read result
    converted = true;
    Serial.println (value);
    delay (500);         // C5:: give you time to see the ouput
    }
}

void disp(byte number){
  switch (number){
    case 0:
      PORTD = 0b00111111;
      break;
    case 1:
      PORTD = 0b00000110;
      break;
    case 2:
      PORTD = 0b01011011;
      break;
    case 3:
      PORTD = 0b01001111;
      break;
    case 4:
      PORTD = 0b01100110;
      break;
    case 5:
      PORTD = 0b01101101;
      break;
    case 6:
      PORTD = 0b01111101;
      break;
    case 7:
      PORTD = 0b00000111;
      break;
    case 8:
      PORTD = 0b01111111;
      break;
    case 9:
      PORTD = 0b01101111;
      break;
  }
}

void disp_off(){
  PORTB = B00001111;
}

void timer1_setup(){
  TCCR1A = 0;
  TCCR1B = 1;  // enable Timer1 with prescaler = 1 ( 16 ticks each 1 µs)
  TCNT1  = 0;  // set Timer1 preload value to 0 (reset)
  TIMSK1 = 1;  // enable Timer1 overflow interrupt
}

void adc_setup(){
  ADCSRA = 0b10000111;
  ADMUX = 0b01000000;
  converted = true;
}
