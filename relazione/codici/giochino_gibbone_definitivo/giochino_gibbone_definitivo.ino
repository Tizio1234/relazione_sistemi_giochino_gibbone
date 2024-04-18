#define LED_N                 4

#define BUTTON_PIN            2
#define DEFEAT_OUT_PIN        3
const int out_led_pins[LED_N] = {4, 5, 6, 7};

bool pressed;
unsigned long current_time = 0;

void input(){
  current_time = millis();

  static int last_read = HIGH;
  int current_read = digitalRead(BUTTON_PIN);
  pressed = (last_read == HIGH && current_read == LOW);
  last_read = current_read;
}

typedef int state_t;

#define IDLE_STATE            ((state_t)0)
#define LED_STATE_BASE        ((state_t)1)
#define LED_STATE_MAX         ((state_t)(LED_STATE_BASE + LED_N - 1))
#define DEFEAT_STATE          ((state_t)LED_STATE_MAX + 1)

#define DEFAULT_STATE IDLE_STATE

#define DELAY_DEFAULT_VAL     1000
unsigned long delay_val = DELAY_DEFAULT_VAL;

#define DEFEAT_DURATION       1000

state_t current_state = DEFAULT_STATE;

void stato(){
  if (current_state == IDLE_STATE){
    if (pressed){
      current_state = LED_STATE_BASE;
    }
  } else if (current_state == DEFEAT_STATE){
    static bool started = false;
    static unsigned long start = 0;

    if(started){
      if(current_time >= (start + DEFEAT_DURATION)){
        current_state = IDLE_STATE;
        started = false;
        delay_val = DELAY_DEFAULT_VAL;
      }
    } else {
      start = current_time;
      started = true;
    }
  } else if (current_state >= LED_STATE_BASE && current_state <= LED_STATE_MAX){
    static bool started = false;
    static unsigned long start = 0;
    if (started){
      if (current_time >= (start + delay_val)){
        if (current_state == LED_STATE_MAX){
          if(pressed){
            delay_val /= 2;
            current_state = LED_STATE_BASE;
          } else {
            current_state = DEFEAT_STATE;
          }
        } else {
          if (pressed){
            current_state = DEFEAT_STATE;
          }
          current_state++;
        }
        started = false;
      } else {
        if (current_state == LED_STATE_MAX){
          if(pressed){
            delay_val /= 2;
            current_state = LED_STATE_BASE;
            started = false;
          }
        } else {
          if (pressed){
            current_state = DEFEAT_STATE;
            started = false;
          }
        }
      }
    } else {
      start = current_time;
      started = true;
    }
  }
}

void output(){
  static state_t last_state = DEFAULT_STATE;

  if (current_state != last_state){
    if (current_state == IDLE_STATE){
      for (uint8_t i = 0; i < LED_N; i++){
        digitalWrite(out_led_pins[i], LOW);
      }
      digitalWrite(DEFEAT_OUT_PIN, LOW);
    } else if (current_state == DEFEAT_STATE){
      for (uint8_t i = 0; i < LED_N; i++){
        digitalWrite(out_led_pins[i], LOW);
      }
      digitalWrite(DEFEAT_OUT_PIN, HIGH);
    } else if (current_state >= LED_STATE_BASE && current_state <= LED_STATE_MAX){
      digitalWrite(out_led_pins[last_state - LED_STATE_BASE], LOW);
      digitalWrite(out_led_pins[current_state - LED_STATE_BASE], HIGH);
    }
  }

  last_state = current_state;
}

void setup() {
  Serial.begin(115200);

  for (uint8_t i = 0; i < LED_N; i++){
    pinMode(out_led_pins[i], OUTPUT);
  }

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(DEFEAT_OUT_PIN, OUTPUT);
}

void loop() {
  input();
  stato();
  output();
}
