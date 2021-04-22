byte current_digit;
int  value;
bool converted;

void setup()
{
  DDRD = 0b01111111; // Configurar pines (0 al 6) puerto D como salidas
  DDRB = 0b00001111; // Configurar pines (8 al 11) puerto B como salidas
  disp_off();
  adc_setup();
  timer1_setup();
}

ISR(TIMER1_OVF_vect)
{
  disp_off();
  switch (current_digit) {
    case 1: // Calcular primer número y encender digito 1
      disp(value / 1000);
      PORTB &= ~(_BV(0));
      break;
    case 2: // Calcular segundo número y encender digito 4
      disp( (value / 100) % 10);
      PORTB &= ~(_BV(1));
      break;
    case 3: // Calcular tercer número y encender digito 3
      disp( (value / 10) % 10);
      PORTB &= ~(_BV(2));
      break;
    case 4: // Calcular cuarto número y encender digito 4
      disp(value % 10);
      PORTB &= ~(_BV(3));
  }
  current_digit = (current_digit % 4) + 1;
}

void loop()
{
  if (converted) {
    bitSet(ADCSRA, ADSC);  // Conversión analoga a digital
    converted = false;
  }
  if (bit_is_clear(ADCSRA, ADSC)) {
    value = ADC;          // Cuando ADSC = 0 leer resultado
    converted = true;
    delay (100);          // Tiempo para ver la salida
  }
}
// Configuración de números del 0 al 9
void disp(byte number)
{
  switch (number) {
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
// Limpiar pantalla
void disp_off()
{
  PORTB = B00001111;
}
// Configuración del Timer 1
void timer1_setup()
{
  TCCR1A = 0;          // Limpiar registro y trabajr en modo normal
  TCCR1B = _BV(WGM12); // Modo CTC
  TCCR1B = _BV(CS10);  // Prescaler en 1
  TCNT1  = 0;          // set Timer1 preload value to 0 (reset)
  TIMSK1 = _BV(TOIE1); // Habilitar  interrupción por desbordamiento del Timer
}
// Configuración del ADC
void adc_setup()
{
  ADMUX = _BV(REFS0);     // Referencia ADC = AVCC
  ADCSRA = _BV(ADEN)      // Activar ADC
           | _BV(ADPS2)   // Prescaler en 128
           | _BV(ADPS1)   // idem
           | _BV(ADPS0);  // idem
  converted = true;
}
