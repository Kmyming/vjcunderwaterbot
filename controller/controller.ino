// [For potentiometer]
int potpin = 34 ;
int potvalue;
int realpot;
int potfinal;

// [For Joystick]
int Vx = 35;
int xVal;
int centerx;
float xplusgrad;
float xplusconstant;
int xplus;
int xminus;
int x;
float xminusgrad;
int finalx;

int Vy = 32;
int yVal;
int centery;
float yplusgrad;
int yplus;
int yminus;
int y;
float yminusgrad;
float yplusconstant;
int finaly;

// [For button]
int buttpin = 13;
int buttstate;
int button = 0;

void setup() {
 Serial.begin(115200);

centerx = analogRead(Vx);
xplusgrad = 512.0/(4095.0 - centerx);
xplusconstant = 1023 - xplusgrad*4095;
xminusgrad = 512/centerx;

centery = analogRead(Vy);
yplusgrad = 512.0/(4095.0 - centery);
yminusgrad = 512/centery;
yplusconstant = 1023 - yplusgrad*4095;



}

void loop() {
 // For potentiometer
 potvalue = analogRead(potpin);
 realpot = map(potvalue, 0, 4095, 0, 1023);
 potfinal = 1023 - realpot;
 Serial.println(potfinal);
 
 // For Joystick
 int x = analogRead(Vx);
 int y = analogRead(Vy);


xplus = xplusgrad*x + xplusconstant;
xminus = xminusgrad*x;
if (x > 512) {
  Serial.print("x ");
  finalx = xplus;
  Serial.print(finalx);
} else {
  Serial.print("x ");
  finalx = xminus;
  Serial.print(finalx);
}

yplus = yplusgrad*y + yplusconstant;
yminus = yminusgrad*y;
if (y > 512) {
  Serial.print(" y ");
  finaly = yplus;
  Serial.print(finaly);
} else {
  Serial.print(" y ");
  finaly = yminus;
  Serial.print(finaly);
 
}
Serial.println();

 // For button
 buttstate = digitalRead(buttpin);
 if (buttstate == 1){
    button++;
    Serial.println(button);
    delay(300);
  } else{
    Serial.println(button);
    }
}
