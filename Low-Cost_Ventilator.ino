#define BRAKE 0
#define CW    1
#define CCW   2
#define CS_THRESHOLD 15   // Definition of safety current (Check: "1.3 Monster Shield Example").

//MOTOR 1
#define MOTOR_A1_PIN 7
#define MOTOR_B1_PIN 8
#define PWM_MOTOR_1 5
#define CURRENT_SEN_1 A2
#define EN_PIN_1 A0
#define MOTOR_1 0

const float ADC_mV = 4.8828125;       // convesion multiplier from Arduino ADC value to voltage in mV
const float SensorOffset = 200.0;     // in mV taken from datasheet
const float sensitivity = 4.413;      // in mV/mmH2O taken from datasheet
const float mmh2O_cmH2O = 10;         // divide by this figure to convert mmH2O to cmH2O
const float mmh2O_kpa = 0.00981;      // convesion multiplier from mmH2O to kPa
short usSpeed = 150;  //default motor speed
unsigned short usMotor_Status = BRAKE;
 
void setup()                         
{
  pinMode(MOTOR_A1_PIN, OUTPUT);
  pinMode(MOTOR_B1_PIN, OUTPUT);
  pinMode(PWM_MOTOR_1, OUTPUT);
  pinMode(CURRENT_SEN_1, OUTPUT);
  pinMode(EN_PIN_1, OUTPUT);
  Serial.begin(9600);              // Initiates the serial to do the monitoring 
  Serial.println("Begin motor control");
  Serial.println(); //Print function list for user selection
  Serial.println("Enter number for control option:");
  Serial.println("1. Adult");
  Serial.println("2. Preschooler");
  Serial.println("3. Infant");
  Serial.println("4. STOP");
  Serial.println("5. READ CURRENT");
  Serial.println("+. INCREASE SPEED");
  Serial.println("-. DECREASE SPEED");
  Serial.println();
}

void loop() 
{
  char user_input;   
  float sensorValue = (analogRead(A5) * ADC_mV - SensorOffset) / sensitivity / mmh2O_cmH2O;       // result in cmH2O
  Serial.print(millis());
  Serial.print(" , ");
  Serial.println(sensorValue, 2);
  delay(3000); 
  while(Serial.available())
  {
    user_input = Serial.read(); //Read user input and trigger appropriate function
    digitalWrite(EN_PIN_1, HIGH);
      if (user_input=='1')
          {
            Adult();
          } 
      else if (user_input=='2')
          {
            Preschooler();
          }  
      else if (user_input=='3')
          {
            Infant();
          } 
      else if (user_input=='4')
          {
            Stop();
          } 
      else if(user_input=='5')
          {
            IncreaseSpeed();
          }
      else if(user_input=='6')
          {
            DecreaseSpeed();    
          }
      else 
          {
         Serial.println("Invalid option entered.");
          }     
    }
}

void Stop()
{
  Serial.println("Stop");
  usMotor_Status = BRAKE;
  motorGo(MOTOR_1, usMotor_Status, 0);
 
}

void Forward()
{
  Serial.println("Forward");
  usMotor_Status = CW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
 
}

void Reverse()
{
  Serial.println("Reverse");
  usMotor_Status = CCW;
  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  
}

void IncreaseSpeed()
{
  usSpeed = usSpeed + 10;
  if(usSpeed > 255)
  {
    usSpeed = 255;  
  }
  
  Serial.print("Speed +: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed);
  
}

void DecreaseSpeed()
{
  usSpeed = usSpeed - 10;
  if(usSpeed < 0)
  {
    usSpeed = 0;  
  }
  
  Serial.print("Speed -: ");
  Serial.println(usSpeed);

  motorGo(MOTOR_1, usMotor_Status, usSpeed); 
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         //Function that controls the variables: motor(0 ou 1), direction (cw ou ccw) e pwm (entra 0 e 255);
{
  if(motor == MOTOR_1)
  {
    if(direct == CW)
    {
      digitalWrite(MOTOR_A1_PIN, LOW); 
      digitalWrite(MOTOR_B1_PIN, HIGH);
    }
    else if(direct == CCW)
    {
      digitalWrite(MOTOR_A1_PIN, HIGH);
      digitalWrite(MOTOR_B1_PIN, LOW);      
    }
    else
    {
      digitalWrite(MOTOR_A1_PIN, LOW);
      digitalWrite(MOTOR_B1_PIN, LOW);            
    }
    analogWrite(PWM_MOTOR_1, pwm); 
  }
}


void Adult ()
{
  
  Serial.println("Adult mode");
  while(1)
  {
      usMotor_Status = CW;
      motorGo(MOTOR_1, usMotor_Status, usSpeed - 115);
      delay (2000);
      break;
  }    
}


void Preschooler ()
{
  Serial.println("Pre-schooler mode");
  while(1)
  {
      usMotor_Status = CW;
      motorGo(MOTOR_1, usMotor_Status, usSpeed-100);
      delay (2000);
      break;
  }
}


void Infant ()
{
  Serial.println("Infant mode");
  while(1)
  {
      usMotor_Status = CW;
      motorGo(MOTOR_1, usMotor_Status, usSpeed);
      delay (2000);
      break;
  }
    
}
