/*
##########################################################################################################################################
#	Ultimo Robo e Desisto :)
#
#	Autor		Atualizado em:		Descricao
#	Time		27/04/2016			Primeira versão do código
#	<Insira>	<Insira>			<Insira>
#
#	Notas: HIGH = Ligado, LOW = Desligado
#
#	
#
#
##########################################################################################################################################
*/

//Variaveis para definir portas de entrada para o robo
const byte led=13;
const byte botao=12;
const byte motorDireito1=11;
const byte motorDireito2=10;
const byte motorEsquerdo1=9;
const byte motorEsquerdo2=8;
const byte sbfd=7;
const byte sbfe=6;
const byte sbtd=5;
const byte sbte=4;
const int trigpinfrente=3;
const int echopinfrente=2;
const int trigpinesq=A4;
const int echopinesq=A3;
const int trigpindir=A1;
const int echopindir=A5;
int distanciafrente=0;
int distanciaesquerda=0;
int distanciadireita=0;
unsigned long tempo=0;
unsigned long tempoanterior=0;
boolean estadobotao=0;
boolean estadosbfd;
boolean estadosbfe;
boolean estadosbtd;
boolean estadosbte;
int i;

void setup()
{
	pinMode(led,OUTPUT);
	pinMode(botao,INPUT);
	pinMode(motorEsquerdo1,OUTPUT);
	pinMode(motorEsquerdo2,OUTPUT);
	pinMode(motorDireito1,OUTPUT);
	pinMode(motorDireito2,OUTPUT);
	pinMode(sbfd,INPUT);
	pinMode(sbfe,INPUT);
	pinMode(sbtd,INPUT);
	pinMode(sbte,INPUT);
	pinMode(trigpinfrente,OUTPUT);
	pinMode(echopinfrente,INPUT);
	pinMode(trigpinesq,OUTPUT);
	pinMode(echopinesq,INPUT);
	pinMode(trigpindir,OUTPUT);
	pinMode(echopindir,INPUT);
}

//Funções de movimento do robo, usando portas digitas
void andarFrente()
{
	digitalWrite(motorDireito1,HIGH);
	digitalWrite(motorEsquerdo1,HIGH);
	
	digitalWrite(motorDireito2,HIGH);
	digitalWrite(motorEsquerdo2,HIGH);
	
}

void andarTras()
{
	digitalWrite(motorDireito1,LOW);
	digitalWrite(motorEsquerdo1,LOW);
	
	digitalWrite(motorDireito2,HIGH);
	digitalWrite(motorEsquerdo2,HIGH);
		
}

void pararMovimento()
{
	digitalWrite(motorDireito1,LOW);
	digitalWrite(motorEsquerdo1,LOW);
	
	digitalWrite(motorDireito2,LOW);
	digitalWrite(motorEsquerdo2,LOW);
	
}

void girarDireita()
{
	digitalWrite(motorDireito1,LOW);
	digitalWrite(motorEsquerdo1,HIGH);
	
	digitalWrite(motorDireito2,HIGH);
	digitalWrite(motorEsquerdo2,LOW);
	
}
void girarEsquerda()
{
	
	digitalWrite(motorDireito1,HIGH);
	digitalWrite(motorEsquerdo1,LOW);
	
	digitalWrite(motorDireito2,LOW);
	digitalWrite(motorEsquerdo2,HIGH);
		
}

int medicaoUltrasomFrontal()
{ //Função para retornar a distância aferida pelo sensor ultrasonico da frente
	digitalWrite(trigpinfrente,LOW);
	delayMicroseconds(2);
	digitalWrite(trigpinfrente,HIGH);
	delayMicroseconds(10);
	digitalWrite(trigpinfrente,LOW);
	unsigned long duracao=pulseIn(echopinfrente,HIGH);
	distanciafrente=duracao/58;
	return(distanciafrente);
}

int medicaoUltrasomEsquerda()
{ //Função para retornar a distância aferida pelo sensor ultrasonico da esquerda
	analogWrite(trigpinesq,0);
	delayMicroseconds(2);
	analogWrite(trigpinesq,255);
	delayMicroseconds(10);
	analogWrite(trigpinesq,0);
	unsigned long duracao=pulseIn(echopinesq,HIGH);
	distanciaesquerda=duracao/58;
	return(distanciaesquerda);
}

int medicaoUltrasomDireita()
{ //Função para retornar a distância aferida pelo sensor ultrasonico da direita
	analogWrite(trigpindir,0);
	delayMicroseconds(2);
	analogWrite(trigpindir,255);
	delayMicroseconds(10);
	analogWrite(trigpindir,0);
	unsigned long duracao=pulseIn(echopindir,255);
	distanciadireita=duracao/58;
	return(distanciadireita);
}

void piscarLed()
{
	// Função para piscar o led
	for(i = 1;i <= 5;i++)
	{
		digitalWrite(led,HIGH);
		delay(500);
		digitalWrite(led,LOW);
		delay(500);
	}
}

void loop(){
	estadobotao=digitalRead(botao);

	while(estadobotao==0)
	{
		// fica lendo o botao (pull-down) até se pressionado
		estadobotao=digitalRead(botao);
		piscarled();
		andarfrente();
		tempoanterior=millis();
		while(tempo<90000)
		{
			//luta enquanto durar tempo da partida
			estadosbfd=digitalRead(sbfd); // Bate um pano no cenário através dos sensores
			estadosbfe=digitalRead(sbfe); //
			estadosbtd=digitalRead(sbtd); //
			estadosbte=digitalRead(sbte); //
			ultrassonicofrente();
			ultrassonicoesquerda();
			ultrassonicodireita();
			if(estadosbfd==1 || estadosbfe==1)
			{ // """Vai para "trás" caso sensor da frente acusar o branco"""   NOTA: 0 - Branco,  1 - Preto
				andartras();
			}
			else if(estadosbtd==1 || estadosbte==1)
			{ // """Vai para "Frente" caso sensor de trás acusar o branco."""   NOTA: 0 - Branco,  1 - Preto
				andarfrente();
			}
			else if(distanciafrente<=60 && estadosbfd==0 && estadosbfe==0)
			{ // """Ir para "Frente" caso ache um oponente na "frente" e os sensore não estiver na borda"""
				andarfrente();
			}
			else if(distanciadireita<=60)
			{ // """Girar para Direita" caso ache um oponente a 7 centimetros"
				girardireita();
			}
			else if(distanciaesquerda<=60)
			{ // """Girar para Esquerda" caso ache um oponente a 7 centimetros"
				giraresquerda();
			}
			tempo=millis()-tempoanterior;
		}
		
		while(true)
		{// Após o tempo de 90 segs. o robo para
			pararMovimento();
		}
	}
}
