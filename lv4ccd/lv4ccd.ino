String code = "";           //initialize the output string
char temp;

void setup() {
  Serial.begin(9600);       //initialize the Serial port
  Serial2.begin(9600);
}

void loop() {
  if (Serial2.available() > 0)     {
    temp = char( Serial2.read());    //read the input data
    code += temp;
  }
  if (temp == 0x0D){           // Or temp == '\r'
    Serial.println(code);
    code = "";
    temp = 0;
  }
}
