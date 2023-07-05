#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <list>
using namespace std;

//librerias necesarias para la ejecución del codigo, lamentablemente algunas son solamente de Windows
//aunque Windows.h tiene una librería semejante en Linux pero desconozco si son las mismas

void consola(int x, int y)  //manejo de consola
{
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;
	dwPos.X = x;
	dwPos.Y = y;
	SetConsoleCursorPosition(hCon, dwPos);
}

void ocultarCursor()  //esconde el cursor en la consola
{
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 50;
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(hCon, &cci);
}
void pintarLimite() //crea la interfaz de juego
{
	for (int i = 2; i < 98;i++)
	{
		consola(i, 3); printf("%c", 205);
		consola(i, 38); printf("%c", 205);
	}
	for (int i = 4; i < 38; i++)
	{
		consola(2, i); printf("%c", 186);
		consola(98, i); printf("%c", 186);
	}
	consola(2, 3); printf("%c", 201);
	consola(2, 38); printf("%c", 200);
	consola(98, 3); printf("%c", 187);
	consola(98, 38); printf("%c", 188);
}

class NAVE  //clase de la nave principal, nave del jugador
{
	int x, y;
	int vidas;
public: 
	NAVE();
	NAVE(int _x, int _y, int _vidas);
	void pintar();
	void borrar();
	void mover();
	void pintarCorazones();
	void perderVida();
	void destruir();
	int X(){ return x; }
	int Y(){ return y; }
	int vidasJugador(){ return vidas; }

};
NAVE::NAVE(int _x, int _y,int _vidas)  //se ajusta la posición de la nave y las vidas
{
	x = _x;
	y = _y;
	vidas = _vidas;
}
void NAVE::pintar()   //Diseño de la nave en las posiciones x e y
{
	consola(x, y); printf("  %c", 220);
	consola(x, y + 1); printf(" %c%c%c", 223, 223, 223);

}
void NAVE::borrar()  //Se encarga de borrar la nave
{
	consola(x, y);     printf("        ");
	consola(x, y + 1); printf("         ");
}

void NAVE::destruir()  //Animación de nave destruida
{
    borrar();
    consola(x, y); printf(" *** ");
    consola(x, y + 1); printf("**  *");
    Sleep(100);
    borrar();
    consola(x, y); printf(" ****");
    consola(x, y + 1); printf("*** *");
    Sleep(100);
    borrar();
    consola(x, y); printf("** **");
    consola(x, y + 1); printf(" *** ");
    Sleep(100);
    borrar();
}
void NAVE::mover() //movimiento de la nave con flechas (se destruye y se crea en una nueva posicion)
{
	if (_kbhit())
	{
		char tecla = _getch();
		borrar();
		if (tecla == 75 && x > 3)
		{
			x--;
		}
		else if (tecla == 77 && x + 6 < 95)
		{
			x++;
		}
		pintar();
	}
}
void NAVE::pintarCorazones() //Diseño de los corazones
{
	consola(50, 2);
	printf("Vida: %d",vidas);
	consola(64, 2);
	printf("      ");
}
void NAVE::perderVida()  //Disminuir vidas
{
	vidas--;
}



class Disparo   //Clase de disparo de la nave
{
	int x, y;
public:
	Disparo(int _x, int _y);
	void mover();
	bool fuera();
	int X(){ return x; }
	int Y(){ return y; }
};
Disparo::Disparo(int _x, int _y) // Posición del Disparo
{
	x = _x;
	y = _y;
}
void Disparo::mover() //Movimiento del Disparo
{
	consola(x, y); printf(" ");
	y--;
	consola(x, y); printf("%c",94);
	
}
bool Disparo::fuera()  //Se elimina el disparo si sobrepasa el limite superior
{
	if (y == 4) return true;
	return false;
	
}

 
class NaveAlien  //Clase nave Alien (Enemigas)
{
	int x,y;
public:
	NaveAlien(int _x, int _y);
	void pintar();
	void mover();
	void colsionNave(NAVE &nave);
	int X(){ return x; }
	int Y(){ return y; }
	void borrar();
	void destruir();
};

NaveAlien::NaveAlien(int _x, int _y)  // posiciones de la naves alien
{
	x = _x;
	y = _y;
}
void NaveAlien::pintar()  // diseño de la nave alien
{
	consola(x, y); printf(" %c %c", 220, 220);
	consola(x, y + 1); printf("  %c", 223);
	
}

void NaveAlien::colsionNave(NAVE &nave)   //codigo encargado de la colision de la nave alien con la nave principal
{
	if (x + 2 >= nave.X() && x <= nave.X() + 5 && y + 1 >= nave.Y() && y <= nave.Y() + 2)
	{
		nave.perderVida();
		nave.destruir();
		nave.pintar();
		nave.pintarCorazones();
		x = (rand() % 71) + 4;
		y = 4;
	}
}

void NaveAlien::borrar() //borra la nave alien
{
    consola(x, y); printf("   ");
    consola(x, y + 1); printf("   ");
}

void NaveAlien::destruir()   //animación de destrucción de la nave alien
{  
    borrar();
    consola(x, y); printf("  ** ");
    consola(x, y + 1); printf(" *  *");
    Sleep(100);
    borrar();
    consola(x, y); printf(" ****");
    consola(x, y + 1); printf("* ** *");
    Sleep(100);
    borrar();
    consola(x, y); printf("**** ");
    consola(x, y + 1); printf(" **  ");
    Sleep(100);
    borrar();
}

void NaveAlien::mover() // movimiento de la nave alien
{
    borrar();
    x++;

    if (x + 3 >= 95) {
		borrar();
        x = 4; 
        y += 3; 
    }

    pintar();
};







int main()
{
	system("cls");            //vacia la consola
	ocultarCursor();          // esconde el cursor 
	pintarLimite();        //limites del juego
	NAVE Personaje(49,34,3);   //se crea la nave principal con sus atributos
	Personaje.pintar();
	Personaje.pintarCorazones();
	list<NaveAlien*> enjambre;              //lista de naves alien     
	list<NaveAlien*>::iterator itAlien;
	int velocidad = 0;                      //variable para disminuir la velocidad
	int nae = 0;                            //variable para contar el numero de naves destruidas
	for (int i = 0; i < 7; i++) {          //se crean naves alien
		for (int j = 0; j < 3; j++) {
			NaveAlien* nave = new NaveAlien(4 + (i * 10), 4 + (j * 4));
			enjambre.push_back(nave);
			nae++;
		}
	}

	list<Disparo*> Bala;                     //se crean las balas
	list<Disparo*>::iterator itBala;
	bool gameOver = false;                 //condicion de derrota
	bool victoria = false;			//condicion de victoria

	while (!gameOver && !victoria)          //ciclo del juego
	{	
		velocidad++;

		if (velocidad >= 10)  // if para disminuir velocidad de las naves
			for (auto it = enjambre.begin(); it != enjambre.end(); ++it) {     //movimiento de las naves alien
				(*it)->mover();
				(*it)->colsionNave(Personaje);
			}
			velocidad = 0;
			if(_kbhit())                            //codigo para asignar tecla de disparo
			{
				char tecla = _getch();
				if (tecla == 'a' || tecla == 'A')
				{
					Bala.push_back(new Disparo(Personaje.X() + 2, Personaje.Y() - 1));
				}
			}
			//Disparo y sus colisiones
			for (itBala = Bala.begin() ; itBala != Bala.end() ;)
			{
				(*itBala)->mover();
				if ((*itBala)->fuera())
				{
					
					consola((*itBala)->X(), (*itBala)->Y()); printf(" ");
					delete(*itBala);
					itBala = Bala.erase(itBala);
				}
				else
				{
					itBala++;
				}
			}
			for (itAlien = enjambre.begin(); itAlien != enjambre.end();)
			{
				(*itAlien)->mover();
				(*itAlien)->colsionNave(Personaje);

				if ((*itAlien)->Y() >= 38)
				{
					consola((*itAlien)->X(), (*itAlien)->Y());
					printf(" ");
					delete (*itAlien);
					itAlien = enjambre.erase(itAlien);
				}
				else
				{
					itAlien++;
				}
			}
			for (itAlien = enjambre.begin(); itAlien != enjambre.end();)   //choque entre la nave alien y la nave principal
			{
				bool colision = false;
				for (itBala = Bala.begin(); itBala != Bala.end();)
				{
					if ((*itAlien)->X() == (*itBala)->X() && ((*itAlien)->Y() + 1 == (*itBala)->Y() || (*itAlien)->Y() == (*itBala)->Y()))
					{
						(*itAlien)->destruir();
						delete (*itAlien);
						itAlien = enjambre.erase(itAlien);

						consola((*itBala)->X(), (*itBala)->Y()); printf(" ");
						delete(*itBala);
						itBala = Bala.erase(itBala);
						colision = true;
						nae--;
						break;
					}
					else
					{
						itBala++;
					}
				}
				if (!colision)
				{
					itAlien++;
				}
			}

		if (Personaje.vidasJugador() <= 0)        //condicion de derrota
		{
			
			gameOver = true;
			consola(30, 15);
			printf("Game Over :(");
			

		}
		if ( nae <= 0)           //condicion de victoria
		{
			
			victoria = true;
			consola(30, 15);
			printf("Eliminaste a todos los Aliens :)");
			

		}

		Personaje.mover();
		Sleep(30);
	}
	_getch();
	
	return 0;
	
}
