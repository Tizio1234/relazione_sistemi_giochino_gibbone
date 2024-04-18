#define LED_N                 4

void input(){}

typedef int state_t;

#define LED_STATE_BASE        ((state_t)0)
#define LED_STATE_MAX         ((state_t)(LED_STATE_BASE + LED_N - 1))

#define DEFAULT_STATE LED_STATE_MAX

#define DELAY_DEFAULT_VAL     1000

state_t current_state = DEFAULT_STATE;
void stato(){
  if (current_state >= LED_STATE_BASE && current_state <= LED_STATE_MAX){
    if (current_state == LED_STATE_MAX){
      current_state = LED_STATE_BASE;
    } else {
      current_state++;
    }
    delay(DELAY_DEFAULT_VAL);
  }
}

int out_led_pins[LED_N] = {2, 3, 4, 5};

void output(){
  static state_t last_state = DEFAULT_STATE;

  if (current_state >= LED_STATE_BASE && current_state <= LED_STATE_MAX){
    digitalWrite(out_led_pins[last_state - LED_STATE_BASE], LOW);
    digitalWrite(out_led_pins[current_state - LED_STATE_BASE], HIGH);
  }

  last_state = current_state;
}

void setup() {
  for (uint8_t i = 0; i < LED_N; i++){
    pinMode(out_led_pins[i], OUTPUT);
  }
}

void loop() {
  input();
  stato();
  output();
}
