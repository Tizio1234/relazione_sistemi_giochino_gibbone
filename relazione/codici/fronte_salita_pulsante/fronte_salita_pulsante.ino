#define BUTTON_PIN 2

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.println("Start");
}

void loop() {
  static int last_read = LOW;
  int current_read = digitalRead(BUTTON_PIN);

  if (current_read == LOW && last_read == HIGH)
  Serial.println("Button has been pressed");

  last_read = current_read;
}
