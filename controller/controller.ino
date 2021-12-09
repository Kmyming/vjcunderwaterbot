// [For potentiometer]
int potpin = 34 ;
int potvalue;
int realpot;
int potfinal;

// [For Joystick]
int Vx = 35;
int centrex;
float xplusgrad;
float xplusconstant;
int xplus;
int xminus;
int x;
float xminusgrad;
int finalx;

int Vy = 32;
int centrey;
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
centrex = analogRead(Vx);
xplusgrad = 511.0/(4095.0 - centrex);
xplusconstant = 1023.0 - xplusgrad*4095.0;
xminusgrad = 512.0/centrex;


centrey = analogRead(Vy);
yplusgrad = 511.0/(4095.0 - centrey);
yminusgrad = 512.0/centrey;
yplusconstant = 1023.0 - yplusgrad*4095.0;


}

void loop() {
 // For potentiometer
 potvalue = analogRead(potpin);
 realpot = map(potvalue, 0, 4095, 0, 1023);
 potfinal = 1023 - realpot;
 Serial.println(potfinal);
 
 // For Joystick
 x = analogRead(Vx);
 y = analogRead(Vy);

xplus = xplusgrad*x + xplusconstant;
xminus = xminusgrad*x;
if (x >= centrex) {
  finalx = xplus;
} else {
  finalx = xminus;
}
Serial.println("x ");
Serial.print(finalx);

yplus = yplusgrad*y + yplusconstant;
yminus = yminusgrad*y;
if (y >= centrey) {
  finaly = yplus;
} else {
  finaly = yminus;
}
Serial.print(" y ");
Serial.print(finaly);

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
