int button = 2;
int redLED = 3;
int greenLED = 4;
int button_int = 0;  // Interrupt 0 is on pin 2
int state = 0;
int down = 0;
int start = 0;
int finish = 0;
int win = 0;
int lose = 0;
float randomtime = 0;
float presstime = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(button, INPUT);
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  attachInterrupt(button_int, handle_click, RISING);
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 7812;  // target for counting
  TCCR1B |= (1 << WGM12); // turn on CTC
  TCCR1B |= (1<<CS12) | (1<<CS10); // 1024 prescaler
}

void loop()
{
  
}

ISR(TIMER1_COMPA_vect)
{
  static int count = 0;
  if(state==1)
  {
    count++;
    if(count==2)
    {
      count = 0;
      state = 2;
      digitalWrite(redLED, LOW);
      TIMSK1 &= ~(1<<OCIE1A);
    }
  }
  else if(state==3)
  {
    digitalWrite(greenLED, digitalRead(greenLED)^1);
    count++;
    if(count==3)
    {
      count = 0;
      state = 0;
      TIMSK1 &= ~(1<<OCIE1A);
    }
  }
  else if(state==4)
  {
    digitalWrite(redLED, digitalRead(redLED)^1);
    count++;
    if(count==9)
    {
      count = 0;
      state = 0;
      TIMSK1 &= ~(1<<OCIE1A);
    }
  }
}


void handle_click()
{
  static unsigned long last_int_time = 0;
  unsigned long int_time = millis(); // Read the clock

  if (int_time - last_int_time > 200)
  {  
      if(down==0)
      {
        down = 1;
        attachInterrupt(button_int, handle_click, FALLING);
        if(state==2)
        {
          start = millis();
        }
      }
      else
      {
        down = 0;
        attachInterrupt(button_int, handle_click, RISING);
        if(state==0)
        {
          state = 1;
          TCNT1 = 0;
          if (TIFR1 & (1<<OCF1A)) TIFR1 |= (1<<OCF1A);
          randomtime = random(1000, 5001);
          OCR1A = randomtime*7.812;  // target for counting
          TIMSK1 |= (1<<OCIE1A); // enable timer compare int.
          digitalWrite(redLED, HIGH);
        }
        else if(state==2)
        {
          finish = millis();
          presstime = finish - start;
          if(abs(presstime-randomtime)<500)
          {
            state = 3;
            TCNT1 = 0;
            if (TIFR1 & (1<<OCF1A)) TIFR1 |= (1<<OCF1A);
            OCR1A = 7812;
            digitalWrite(greenLED, HIGH);
            win++;
          }
          else
          {
            state = 4;
            TCNT1 = 0;
            if (TIFR1 & (1<<OCF1A)) TIFR1 |= (1<<OCF1A);
            OCR1A = 3125;
            digitalWrite(redLED, HIGH);
            lose++;
          }
          TIMSK1 |= (1<<OCIE1A); // enable timer compare int.
          Serial.println((String)win + "W" + lose + "L" + "  LED time:" + randomtime/1000 + "  Press time:" + presstime/1000);
        }
      }
  }

  last_int_time = int_time;
}

