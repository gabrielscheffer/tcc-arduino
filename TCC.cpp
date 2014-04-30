// Do not remove the include below
#include "TCC.h"
#include "SoftwareSerial.h"

SoftwareSerial mySerial(2, 3);
const int leds[7] = { 4, 5, 6, 7, 8, 9, 10 }, branco = 10, buz = 11;
int meudelay = 500, dadoscomb[7][7] = {
		{ 0, 0, 0, 0, 0, 0, 1 }, // 1
		{ 0, 0, 1, 1, 0, 0, 0 }, // 2
		{ 0, 0, 1, 1, 0, 0, 1 }, // 3
		{ 1, 0, 1, 1, 0, 1, 0 }, // 4
		{ 1, 0, 1, 1, 0, 1, 1 }, // 5
		{ 1, 1, 1, 1, 1, 1, 0 }, // 6
		{ 0, 0, 0, 0, 0, 0, 0 } // branco
};

//Prototipos
void testaDigital(int porta) {
	digitalWrite(porta, HIGH);
	delay(meudelay);
	digitalWrite(porta, LOW);
}

void tocabuz(int potencia) {
	analogWrite(buz, potencia);
	delay(200);
	analogWrite(buz, LOW);
}

void mostraresultado(int resultado) {
	for (int i = 0; i < 7; i++) {
		digitalWrite(leds[i], dadoscomb[resultado][i]);
	}
}

void roleoDado() {
	int resultado = 0;
	int rolagensDados = random(15, 25);
	for (int i = 0; i < rolagensDados; i++) {
		resultado = random(0, 6); // result will be 0 to 5 not 1 to 6
		mostraresultado(resultado);
		delay(50 + i * 10);
	}
	for (int i = 0; i < 3; i++) {
		mostraresultado(branco);
		delay(500);
		mostraresultado(resultado);
		delay(500);
	}
	resultado++;
	Serial.println("Numero: " + String(resultado));
	for (int i = 0; i < resultado; i++) {
		tocabuz(200);
		delay(500);
	}
}
//-----------------------------------------------------------------------------
//Função chamada apenas uma vez ao ligar
void setup() {
	//Inicializa os Leds de saida e a porta serial de comunicação
	Serial.begin(9600);
	Serial.println("Testando leds 1...2...3");

	for (int i = 0; i < 7; i++) {
		pinMode(leds[i], OUTPUT);
		testaDigital(leds[i]);
	}
	pinMode(buz, OUTPUT);
	tocabuz(200);
	Serial.println("Abrindo comunicação com Xbee");
	mySerial.begin(9600);
	//mySerial.setTimeout(2000);
	Serial.println("INICIO >> Escreva 'rolar'.");
}
//Main
void loop() {
	String spalavra;

	while (mySerial.available() > 0){
		spalavra = mySerial.readStringUntil('\n'); // le proxima palavra do serial ate CR+LF
	}
	if (spalavra.length() > 0) {
		mySerial.write('\n');
		spalavra.toUpperCase();
		if (spalavra.compareTo("ROLAR") == 0) {
			randomSeed(analogRead(0));
			Serial.println("Ok, Vou rolar o dado para você");
			mySerial.println("Ok, Vou rolar o dado para você");
			roleoDado();
		} else if (spalavra.compareTo("OI") == 0) {
			Serial.println("Olá, escreva 'rolar' para continuar.");
			mySerial.println("Olá, escreva 'rolar' para continuar.");
		} else
			Serial.println(spalavra);
	}

//	if (mySerial.available())
//	Serial.write(mySerial.read());
//	if (Serial.available())
//	mySerial.write(Serial.read());

}
