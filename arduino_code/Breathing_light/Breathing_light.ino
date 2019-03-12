int led;
int fade;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  led = 0;
  fade = 5;
}

void loop()
{
  analogWrite(LED_BUILTIN, led);
  led = led + fade;

  if (led <= 0) {
    fade = 5;
  } else if (led >= 255) {
    fade = -5;
  }
  delay(30);
}
