//Serial Communication - C
/*  Le programme permet à la carte Arduino de recevoir des données depuis le port série
 *  et de les renvoyer par la suite à travers le port série.
 *  
 *  Reception de r : Eteindre la LED Rouge
 *  Reception de R : Allumer la LED Rouge
 *  
 *  Reception de g : Eteindre la LED Verte
 *  Reception de G : Allumer la LED Verte
 *  
 *  Si la donnée reçue est connue, on envoi : "Commande recue : ( commande reçue )"
 *  Sinon, "Commande inconnue..."
 *  
 */

#define r_led DDD2 //Direction du PIN2 du PORT D
#define g_led DDD3 //Direction du PIN3 du PORT D

#define r_port PORTD2 //PIN2 du PORT D (PD2)
#define g_port PORTD3 //PIN3 du PORT D (PD3)

#define F_CPU 16000000 //Fréquence du CPU
#define BAUD 9600 //Vitesse de transmission

#include <avr/io.h>


//Déclaration des fonctions
void USART_Init(); //Initialise la communication du port série
void USART_Transmit(unsigned char data); //Permet d'envoyer un caractère sur le port série
unsigned char USART_Received(); //Permet de récupérer le caractère reçu depuis le port série
void SendData(unsigned char data); //Permet d'envoyer une chaine de caractères sur le port série


//Buffer contenant la chaine de caractère à envoyer si le caractère est connu
char buffer[17] = "Commande recue : ";

//Contient le message d'erreur si la commande reçue est inconnue du programme
char Inconnue[20] = "Commande inconnue...";



//Fonction principale
int main(void)
{
  //Initialisation au lancement du programme
   DDRD |= _BV(r_led); //Défini PD2 comme une sortie
   DDRD |= _BV(g_led); //Défini PD3 comme une sortie
   USART_Init(); 

  //Boucle infinie
  while(1) {

  //Variable contenant le caractère reçu depuis le port série
  unsigned char r_data = USART_Received();


  //On allume/éteint telle ou telle LED selon le caractère reçu
  switch(r_data)
   {
    case 'r':
      PORTD &= ~_BV(r_port); //Eteint la LED Rouge
      SendData(r_data);
    break;
    case 'g':
      PORTD &= ~_BV(g_port); //Eteint la LED Verte
      SendData(r_data);   
    break;
    case 'R':
      PORTD |= _BV(r_port); //Allume la LED Rouge
      SendData(r_data);  
    break;
    case 'G':
      PORTD |= _BV(g_port); //Allume la LED verte
      SendData(r_data); 
    break;
    default:
      for(int rang = 0; rang < 20; rang++)
      {
        USART_Transmit(Inconnue[rang]);
        USART_Transmit('\n');
      }
    break;

    }
  
  }


}
////////////
//Fin main//
////////////


void USART_Init()
{
   UBRR0 = (F_CPU / 4 / BAUD - 1) / 2;
   UCSR0B = (1<<RXEN0) | (1<<TXEN0);
   UCSR0C = (3<<UCSZ00);
   UCSR0A = (1<< U2X0);
}

void USART_Transmit(unsigned char data)
{
    while(!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

unsigned char USART_Received()
{
  while ( !(UCSR0A & (1<<RXC0)) );
  return UDR0;
}


void SendData(unsigned char data)
{

  //Envoi du buffer en envoyant chaque caractère un par un
   for(int rang = 0; rang < 17; rang++)
    {
    USART_Transmit(buffer[rang]);
    }

  //On renvoi ensuite la donnée qui a été reçue ainsi qu'un saut à la ligne
    USART_Transmit(data);
    USART_Transmit('\n');
}
