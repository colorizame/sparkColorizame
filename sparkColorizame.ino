// Escanner de Colores con SparkCore
// Hecho por el proyecto: Coloriza.me
// Programado por: Daniel Xutuc (dahngeek)

#define DELAY 250

//LED RGB
int blue = A6;
int green = A5;
int red = A4;
//FUNCIONES
int calibrar(String color);
int color(String rgb);
int escanearColor(String pizza);
int rMax;
int gMax;
int bMax;
int rMin;
int gMin;
int bMin;
int resultRed = 0;
int resultGreen = 0;
int resultBlue = 0;
//SENSOR
// --- Voltaje
int voltaje = A3;
// --- Fotoresistencia
int fotor = A2;

//Una estructura para no perdernos
struct Color 
{
  int r;
  int g;
  int b;
};

Color c;
Color read_color( void );
//declaramos todas las funciones
void setup(){ 
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  pinMode(voltaje, OUTPUT);
  digitalWrite(voltaje, HIGH);
  //Funciones al API del Particle cloud
  Spark.function("calibrate", calibrar);
  Spark.function("escanear", escanearColor);
  //Variables al API del Particle cloud
  Spark.variable("colorRed", &resultRed, INT);
  Spark.variable("colorGreen", &resultGreen, INT);
  Spark.variable("colorBlue", &resultBlue, INT);
}

void loop() {
    
}

//Para que servira esta funcion... hahaha .. escanear Color :D
int escanearColor(String pizza) {
    c = read_color();
    print_color();
    return 1;
}

//Mapear los colores
Color read_color( void )
{
  Color ret;
  digitalWrite( red, HIGH );
  delay( DELAY );
  ret.r = constrain( map( analogRead( fotor ), rMin, rMax, 0, 255 ), 0, 255 );
  delay( DELAY );
  digitalWrite( red, LOW );
  
  digitalWrite( green, HIGH );
  delay( DELAY );
  ret.g = constrain( map( analogRead( fotor ), gMin, gMax, 0, 255 ), 0, 255 );
  delay( DELAY );
  digitalWrite( green, LOW );
  
  digitalWrite( blue, HIGH );
  delay( DELAY );
  ret.b = constrain( map( analogRead( fotor ), bMin, bMax, 0, 255 ), 0, 255 );
  delay( DELAY );
  digitalWrite( blue, LOW );
  
  return ret;
}
//introducir los colores obtenidos a las variables globales que se mandan al Particle Cloud API
void print_color( void )
{
    resultRed = c.r;
    resultGreen = c.g;
    resultBlue = c.b;
}

int calibrar(String color) {
  //Calibrar el color blanco, definir los máximos.
    if  (color == "blanco"){
        digitalWrite(red, HIGH);
        delay(DELAY);
        rMax = analogRead(fotor);
        delay(DELAY);
        digitalWrite(red,LOW);
        digitalWrite(green, HIGH);
        delay(DELAY);
        gMax = analogRead(fotor);
        delay(DELAY);
        digitalWrite(green,LOW);
        digitalWrite(blue, HIGH);
        delay(DELAY);
        bMax = analogRead(fotor);
        delay(DELAY);
        digitalWrite(blue,LOW);
    } else {
      //Calibrar el color Negro, definir los minimos
        digitalWrite(red, HIGH);
        delay(DELAY);
        rMin = analogRead(fotor);
        delay(DELAY);
        digitalWrite(red,LOW);
        digitalWrite(green, HIGH);
        delay(DELAY);
        gMin = analogRead(fotor);
        delay(DELAY);
        digitalWrite(green,LOW);
        digitalWrite(blue, HIGH);
        delay(DELAY);
        bMin = analogRead(fotor);
        delay(DELAY);
        digitalWrite(blue,LOW);
    }
    return 0;
}

//Función para encender un led RGB... proximamen se usará para dar notificaciones
// cuando escaneen un color y cuando falle la conexión por ejemplo.
int color(String rgb) {
    int r = 0;
    int g = 0;
    int b = 0;
    char * params = new char[rgb.length() + 1];
    strcpy(params, rgb.c_str());
    char * p = strtok(params, "-");
    int commandStep = 0;
    while (p != NULL)
    {
        //get the values for 
        if (commandStep == 0)
        {
            r = atoi(p);
        }
        else if (commandStep == 1)
        {
            g = atoi(p);
        }
        else if (commandStep == 2)
        {
            b = atoi(p);
        }

        commandStep++;
        p = strtok(NULL, "-");
    }
    analogWrite(red, r);
    analogWrite(green, g);
    analogWrite(blue, b);
    return 1;
}
