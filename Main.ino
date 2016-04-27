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
const byte infraDireitoFrente=7;
const byte infraEsquerdoFrente=6;
const byte infraDireitoTraseiro=5;
const byte infraEsquerdoTraseiro=4;
const int trigPinFrontal=3;
const int echoPinFrontal=2;
const int trigPinEsquerdo=A4;
const int echoPinEsquerdo=A3;
const int trigPinDireito=A1;
const int echoPinDireito=A5;
int distanciaUltrasomFrontal=0;
int distanciaUltrasomEsquerdo=0;
int distanciaUltrasoDireito=0;
unsigned long tempoPartida=0;
unsigned long tempoAnterior=0;
boolean estadoBotao=0;
boolean estadoInfraDireitoFrente;
boolean estadoInfraEsquerdoFrente;
boolean estadoInfraDireitoTraseiro;
boolean estadoInfraEsquerdoTraseiro;
int i;

void setup()
{
	pinMode(led,OUTPUT);
	pinMode(botao,INPUT);
	pinMode(motorEsquerdo1,OUTPUT);
	pinMode(motorEsquerdo2,OUTPUT);
	pinMode(motorDireito1,OUTPUT);
	pinMode(motorDireito2,OUTPUT);
	pinMode(infraDireitoFrente,INPUT);
	pinMode(infraEsquerdoFrente,INPUT);
	pinMode(infraDireitoTraseiro,INPUT);
	pinMode(infraEsquerdoTraseiro,INPUT);
	pinMode(trigPinFrontal,OUTPUT);
	pinMode(echoPinFrontal,INPUT);
	pinMode(trigPinEsquerdo,OUTPUT);
	pinMode(echoPinEsquerdo,INPUT);
	pinMode(trigPinDireito,OUTPUT);
	pinMode(echoPinDireito,INPUT);
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

//Funções de medicao para o ultrasom. Ultrasom utilizado: Frontal, esquerdo e direito
int medicaoUltrasomFrontal()
{
	digitalWrite(trigPinFrontal,LOW);
	delayMicroseconds(2);
	digitalWrite(trigPinFrontal,HIGH);
	delayMicroseconds(10);
	
	digitalWrite(trigPinFrontal,LOW);
	unsigned long duracaoRespostaUltrasom = pulseIn(echoPinFrontal,HIGH);
	distanciaUltrasomFrontal = duracaoRespostaUltrasom/58;
	
	return(distanciaUltrasomFrontal);
}

int medicaoUltrasomEsquerda()
{
	analogWrite(trigPinEsquerdo,0);
	delayMicroseconds(2);
	analogWrite(trigPinEsquerdo,255);
	delayMicroseconds(10);
	
	analogWrite(trigPinEsquerdo,0);
	unsigned long duracaoRespostaUltrasom = pulseIn(echoPinEsquerdo,HIGH);
	distanciaUltrasomEsquerdo = duracaoRespostaUltrasom/58;
	
	return(distanciaUltrasomEsquerdo);
}

int medicaoUltrasomDireita()
{
	analogWrite(trigPinDireito,0);
	delayMicroseconds(2);
	analogWrite(trigPinDireito,255);
	delayMicroseconds(10);
	
	analogWrite(trigPinDireito,0);
	unsigned long duracaoRespostaUltrasom = pulseIn(echoPinDireito,255);
	distanciaUltrasoDireito = duracaoRespostaUltrasom/58;
	
	return(distanciaUltrasoDireito);
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

void loop()
{
	estadoBotao = digitalRead(botao);

	while(estadoBotao == 0)
	{
		estadoBotao=digitalRead(botao);
		piscarLed();
		andarFrente();
		tempoAnterior = millis();
		while(tempoPartida < 90000)
		{
			estadoInfraDireitoFrente = digitalRead(infraDireitoFrente);
			estadoInfraEsquerdoFrente = digitalRead(infraEsquerdoFrente);
			estadoInfraDireitoTraseiro = digitalRead(infraDireitoTraseiro);
			estadoInfraEsquerdoTraseiro = digitalRead(infraEsquerdoTraseiro);
			
			ultrassonicoFrente();
			ultrassonicoEsquerda();
			ultrassonicoDireita();
			
			//NOTA: 0 - Branco,  1 - Preto
			if(estadoInfraDireitoFrente == 1 || estadoInfraEsquerdoFrente == 1)
			{ 
				andarTras();
			}
			else if(estadoInfraDireitoTraseiro == 1 || estadoInfraEsquerdoTraseiro == 1)
			{ 
				andarFrente();
			}
			else if(distanciaUltrasomFrontal <= 60 && estadoInfraDireitoFrente == 0 && estadoInfraEsquerdoFrente == 0)
			{
				andarFrente();
			}
			else if(distanciaUltrasoDireito <= 60)
			{
				girarDireita();
			}
			else if(distanciaUltrasomEsquerdo <= 60)
			{
				girarEsquerda();
			}
			tempoPartida = millis() - tempoAnterior;
		}
		
		pararMovimento();
		
	}
}
