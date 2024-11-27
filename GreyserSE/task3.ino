#define RIGHT_SPEED 5
#define LEFT_SPEED 6
#define RIGHT_DIRECTION 4
#define LEFT_DIRECTION 7
#define LIGHT_RIGHT 1
#define LIGHT_LEFT 0
#define SOUND A3
#define BUTTON A2

int left_light = 0;
static bool is_on = 0;

void setup()
{
    Serial.begin(9600);
    pinMode(LIGHT_RIGHT, INPUT);
    pinMode(LIGHT_LEFT, INPUT);
    pinMode(SOUND, OUTPUT);
}

int binarize(int detect)
{
    if (detect >= 800) return 1;
    else return 0;
}

void forward(unsigned int speed)
{
    digitalWrite(RIGHT_DIRECTION, HIGH);
    digitalWrite(LEFT_DIRECTION, HIGH);
    
    analogWrite(RIGHT_SPEED, speed);
    analogWrite(LEFT_SPEED, speed);
}

void stop()
{
    analogWrite(RIGHT_SPEED, LOW);
    analogWrite(LEFT_SPEED, LOW);
    
    while (true) {
        digitalWrite(SOUND, HIGH);
        
        if ((binarize(analogRead(LIGHT_RIGHT))==1) || (binarize(analogRead(LIGHT_LEFT) == 1))) {
            digitalWrite(SOUND, LOW);
            break;
        }
    delay(300);
    };
}

void turn(int right, int left, int speed)
{
    if((right == 1) && (left == 0)) {
        digitalWrite(RIGHT_DIRECTION, LOW);
        digitalWrite(LEFT_DIRECTION, HIGH);
    } else if ((right == 0) && (left == 1)) {
        digitalWrite(RIGHT_DIRECTION,HIGH);
        digitalWrite(LEFT_DIRECTION, LOW);
    }
        analogWrite(RIGHT_SPEED, speed);
        analogWrite(LEFT_SPEED, speed);
}

void play_sound()
{
    const unsigned long sound_duration = 500; // Duration of the sound in milliseconds
    unsigned long start_time = millis();
    
    while (millis() - start_time < sound_duration) {
        digitalWrite(SOUND, HIGH);
    }
    
    digitalWrite(SOUND, LOW);
}

void search()
{
    unsigned long time = millis();
    int speed = 100;
    while ((binarize(analogRead(LIGHT_RIGHT)) == 0) && (binarize(analogRead(LIGHT_LEFT)) == 0)) {
        unsigned long timel = millis();
        while (millis() - timel < 50){         
            turn(!left_light, left_light, 150);
        } 

        unsigned long timef = millis();
        while (millis() - timef < 100){
            forward(speed);
        }

        if (speed > 150) {
            speed = 150;
        }

        speed += 1;

        if (millis() - time > 10000) {
            stop();
            break;
        }
        
     }
}

bool button_is_pressed()
{
    static bool old_state = 0;
    static bool is_pressed = 0;
    static unsigned long timeout_end = 0;
    
    bool state = digitalRead(BUTTON);
    
    if(!is_pressed) {
        if(state & !old_state) {
          is_pressed = 1;
          timeout_end = millis() + 5;
        }
          old_state = state;
    }
    else {
        if(millis() > timeout_end) {
        is_pressed = 0;
        if(state) return 1;
    }
    return 0;
    }
}

void loop()
{
    if (is_on) {
    if ((binarize(analogRead(LIGHT_LEFT)) == 1) && (binarize(analogRead(LIGHT_RIGHT)) == 1))
        forward(200);
    
    if ((binarize(analogRead(LIGHT_LEFT)) == 1) && (binarize(analogRead(LIGHT_RIGHT)) == 0))
        turn(0, 1, 200);
    
    if ((binarize(analogRead(LIGHT_LEFT)) == 0) && (binarize(analogRead(LIGHT_RIGHT)) == 1))
        turn(1, 0, 200);
    
    if ((binarize(analogRead(LIGHT_LEFT)) == 0) && (binarize(analogRead(LIGHT_RIGHT)) == 0))
        search();
    
    left_light = binarize(analogRead(LIGHT_LEFT));
    }
    if (button_is_pressed()) {
        is_on = !is_on;
    }
    delay(1);
}
