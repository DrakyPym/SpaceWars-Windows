// Los Iluminati   4CV1
// #pragma comment(linker, "/manifestdependency:\"type='win32' name='SpaceWars.exe' version='1.0.0.0' processorArchitecture='*' publicKeyToken='0000000000000000' language='*'\"")

#include <iostream>
#include <stdio.h>
#include <windows.h> //PERMITE POSCIONAR LOS CARACTERES, CAMBIARLES EL COLOR
#include <conio.h>
#include <stdlib.h>
#include <list>
#include <vector>
#include <semaphore.h>
#include <pthread.h>
#include <mutex>
#include <ctime>
#include <mmsystem.h> //multimedia
#include <Windows.h>
#include <ShellAPI.h>
// MEDIDAS DE LA CONSOLA
#define color SetConsoleTextAttribute

// FLECHAS DEL TECLADO
#define ARRIBA 72
#define ABAJO 80
#define IZQUIERDA 75
#define DERECHA 77
#define ESPACIO 32

// COLORES

#define verde 10
#define blanco 15
#define azul 11
#define amarillo 6
#define rojo 4
using namespace std;

// Funciones
void regenerarBalas();
void Maximizar_Ventana();
int Obtener_Ancho_Ventana();
int Obtener_Espacios(int ancho, int a);
int Obtener_Altura_Ventana();
void Posicionar1(int x, int y);
void OcultarCursor();
void Colorear1(int a);
void Pintar_Limites();
void juego_N();
void juego_O();
void Colorear(int a);
void Posicionar(int x, int y);
void Logo();
void Win();
void Lose();
void Boton_Nave(char color);
void Boton_Ovni(char color);
void Boton_Exit(char color);
void Boton_Volver(char color);
void Mover_Menu();
void Menu();
void Menu_Volver();
// void Menu_volverWin();
void Maximizar_Ventana();
void Mover_Volver();
void Musica(int estadoDelJuego);
void sonidoDanio();
void sonidoDisparo();
void sonidosDetener();
// FIN FUNCIONES
//----audio
void sonidosDetener()
{
	mciSendString(TEXT("close all"), NULL, 0, NULL);
}

void Musica(int estadoDelJuego)
{
	sonidosDetener();
	switch (estadoDelJuego)
	{
	case 0:
		mciSendString(TEXT("play ingame.mp3"), NULL, 0, NULL);
		break;
	case 1:
		mciSendString(TEXT("play menu.mp3"), NULL, 0, NULL);
		break;
	default:
		mciSendString(TEXT("play endgame.mp3"), NULL, 0, NULL);
		break;
	}
}

void sonidoDisparo()
{
	// mciSendString(TEXT("stop disparo.mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play disparo.mp3"), NULL, 0, NULL);
}

void sonidoDanio()
{
	mciSendString(TEXT("pause ingame.mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play danio.mp3"), NULL, 0, NULL);
	mciSendString(TEXT("play ingame.mp3"), NULL, 0, NULL);
}
//-----audio
int corazones = 3;
int inicialX = 10, inicialY = 10;
void Maximizar_Ventana()
{
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, SW_MAXIMIZE);
}

int Obtener_Ancho_Ventana()
{
	int ancho = 0;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	ancho = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	return ancho;
}

int Obtener_Espacios(int ancho, int a)
{
	int espacios = 0;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	espacios = (ancho - a) / 2;

	return espacios;
}

int Obtener_Altura_Ventana()
{
	int alto = 0;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

	alto = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	return alto;
}
int Dimensionar_Coordenadas_ENVIAR(int w, int AL) // ESTO VA A CONVERTIR LA COORDENADA NATIVA EN PROCENTAJE QUE VA A ESTAR LISTA PARA ENVIARSE
{
	int Co_Enviar = 0;

	Co_Enviar = (w * 100) / AL;

	return Co_Enviar;
}

int Dimensionar_Coordenadas_Recibir(int w, int AL) // ESTE VA A CONVERTIR LA COORDENADA EN PORCENTAJE QUE RECIBE DE LA OTRA PC Y LA VA A CONVERTIR EN COORDENADA NATIVA
{
	int Co_Recibir = 0;

	Co_Recibir = (w * AL);

	return Co_Recibir;
}

void Posicionar1(int x, int y)
{
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;

	dwPos.X = x;
	dwPos.Y = y;

	SetConsoleCursorPosition(hCon, dwPos);
}

void OcultarCursor()
{
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = 2;
	cci.bVisible = FALSE;

	SetConsoleCursorInfo(hCon, &cci);
}

class NAVE
{

private:
	int x;
	int y;
	int corazones;
	int vida;

public:
	NAVE(int x, int y, int corazones);
	void Cor()
	{
		corazones--;
		sonidoDanio();
	}
	void Pintar();
	void Borrar();
	void Mover();
	void Colorear(int a);
	void Pintar_Corazones();
	void morir();
	void setVidas(int vidas);

	int getVidas();
	void vivir();
	int X()
	{
		return x;
	}
	int Y()
	{
		return y;
	}
	void Posicionar(int inicialX, int inicialY)
	{
		x = inicialX;
		y = inicialY;
	}
};

// Constructor
NAVE::NAVE(int _x, int _y, int _corazones)
{
	x = _x;
	y = _y;
	corazones = _corazones;
}

void NAVE::Pintar()
{
	int a = amarillo;
	Posicionar1(x, y);
	Colorear(a);
	printf("%c%c   ", 175, 187);
	Posicionar1(x, y + 1);
	Colorear(a);
	printf("%c%c%c  ", 175, 187, 187);
	Posicionar1(x, y + 2);
	Colorear(a);
	printf(" %c%c%c%c", 204, 205, 205, 175);
	Posicionar1(x, y + 3);
	Colorear(a);
	printf("%c%c%c  ", 175, 188, 188);
	Posicionar1(x, y + 4);
	Colorear(a);
	printf("%c%c   ", 175, 188);
}

void NAVE::Borrar()
{
	Posicionar1(x, y);
	cout << "     ";
	Posicionar1(x, y + 1);
	cout << "     ";
	Posicionar1(x, y + 2);
	cout << "     ";
	Posicionar1(x, y + 3);
	cout << "     ";
	Posicionar1(x, y + 4);
	cout << "     ";
}

void NAVE::Mover()
{
	int ancho = Obtener_Ancho_Ventana();
	int alto = Obtener_Altura_Ventana();

	if (kbhit())
	{
		Borrar();
		if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && x > 4)
			x -= 2;
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && x < ancho - 7)
			x += 2;
		if ((GetAsyncKeyState(VK_UP) & 0x8000) && y > 4)
			y--;
		if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && y < alto - 5)
			y++;
		Pintar();
	}
}

void NAVE::Colorear(int a) // FUNCION PARA CAMBIAR DE COLOR LOS CARACTERES
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	color(hConsole, a);
}

void NAVE::Pintar_Corazones()
{

	Posicionar1(64, 2);
	printf("Vida");
	Posicionar1(70, 2);
	printf("          ");

	for (int i = 0; i < corazones; i++)
	{
		Posicionar1(70 + i, 2);
		printf("%c", 3);
	}
}

void NAVE::morir()
{
	int a = amarillo;
	if (corazones == 0)
	{
		Borrar();
		Posicionar1(x, y);
		Colorear(a);
		printf("     ");
		Posicionar1(x, y + 1);
		Colorear(a);
		printf(" **  ");
		Posicionar1(x, y + 2);
		Colorear(a);
		printf("  ** ");
		Posicionar1(x, y + 3);
		Colorear(a);
		printf(" **  ");
		Posicionar1(x, y + 4);
		Colorear(a);
		printf("     ");
		Sleep(200);

		Borrar();
		Posicionar1(x, y);
		Colorear(a);
		printf("     ");
		Posicionar1(x, y + 1);
		Colorear(a);
		printf(" * * ");
		Posicionar1(x, y + 2);
		Colorear(a);
		printf("* * *");
		Posicionar1(x, y + 3);
		Colorear(a);
		printf("  *  ");
		Posicionar1(x, y + 4);
		Colorear(a);
		printf("     ");
		Sleep(200);

		Borrar();
		Posicionar1(x, y);
		Colorear(a);
		printf("*   *");
		Posicionar1(x, y + 1);
		Colorear(a);
		printf(" *** ");
		Posicionar1(x, y + 2);
		Colorear(a);
		printf("* * *");
		Posicionar1(x, y + 3);
		Colorear(a);
		printf(" *** ");
		Posicionar1(x, y + 4);
		Colorear(a);
		printf("*   *");
		Sleep(200);

		Sleep(500);
		system("cls");
		Menu_Volver();
	}
}

int NAVE::getVidas()
{
	return corazones;
}

void NAVE::setVidas(int vidas)
{
	corazones = vidas;
}

void NAVE::vivir()
{
	int a = amarillo;
	if (corazones > 0 && corazones <= 3)
	{
		Borrar();
		Posicionar1(x, y);
		Colorear(a);
		printf(" +   + ");
		Posicionar1(x, y + 1);
		Colorear(a);
		printf("+  *  +");
		Posicionar1(x, y + 2);
		Colorear(a);
		printf("+ *** +");
		Posicionar1(x, y + 3);
		Colorear(a);
		printf(" + * + ");
		Posicionar1(x, y + 4);
		Colorear(a);
		printf("+*****+");
		Sleep(200);

		Borrar();
		Posicionar1(x, y);
		Colorear(a);
		printf("+     +");
		Posicionar1(x, y + 1);
		Colorear(a);
		printf(" * + * ");
		Posicionar1(x, y + 2);
		Colorear(a);
		printf("+* * *+");
		Posicionar1(x, y + 3);
		Colorear(a);
		printf(" + * + ");
		Posicionar1(x, y + 4);
		Colorear(a);
		printf("*+ + +*");
		Sleep(200);

		Borrar();
		Posicionar1(x, y);
		Colorear(a);
		printf("+++ +++");
		Posicionar1(x, y + 1);
		Colorear(a);
		printf("+**+**+");
		Posicionar1(x, y + 2);
		Colorear(a);
		printf(" +***+ ");
		Posicionar1(x, y + 3);
		Colorear(a);
		printf("  +*+  ");
		Posicionar1(x, y + 4);
		Colorear(a);
		printf("*+ + +*");
		Sleep(200);

		Sleep(500);
		system("cls");
		Menu_Volver();
	}
}

int ancho = Obtener_Ancho_Ventana();
int alto = Obtener_Altura_Ventana();
NAVE nave(10, alto / 2, corazones);
//...............................................................................
class OVNI
{

private:
	int x;
	int y;
	int corazones;
	int vida;

public:
	OVNI(int x, int y, int corazones);
	void Cor()
	{
		corazones--;
		sonidoDanio();
	}
	void Pintar();
	void Borrar();
	void Mover();
	void Colorear(int a);
	void Pintar_Corazones();
	void morir();
	void setVidas(int vidas);

	int getVidas();
	void vivir();
	int X()
	{
		return x;
	}
	int Y()
	{
		return y;
	}
	void Posicionar(int inicialX, int inicialY)
	{
		x = inicialX;
		y = inicialY;
	}
};

// Constructor
OVNI::OVNI(int _x, int _y, int _corazones)
{
	x = _x;
	y = _y;
	corazones = _corazones;
}

void OVNI::Pintar()
{
	int a = rojo;
	Posicionar1(x, y);
	Colorear(a);
	printf("   %c ", 201);
	Posicionar1(x, y + 1);
	Colorear(a);
	printf(" %c%c%c ", 201, 254, 187);
	Posicionar1(x, y + 2);
	Colorear(a);
	printf("%c%c%c%c%c", 40, 254, 254, 254, 41);
	Posicionar1(x, y + 3);
	Colorear(a);
	printf(" %c%c%c ", 200, 254, 188);
	Posicionar1(x, y + 4);
	Colorear(a);
	printf("   %c ", 200);
}

void OVNI::Borrar()
{
	Posicionar1(x, y);
	cout << "     ";
	Posicionar1(x, y + 1);
	cout << "     ";
	Posicionar1(x, y + 2);
	cout << "     ";
	Posicionar1(x, y + 3);
	cout << "     ";
	Posicionar1(x, y + 4);
	cout << "     ";
}

void OVNI::Mover()
{
	int ancho = Obtener_Ancho_Ventana();
	int alto = Obtener_Altura_Ventana();

	if (kbhit())
	{
		Borrar();
		if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && x > 4)
			x -= 2;
		if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && x < ancho - 7)
			x += 2;
		if ((GetAsyncKeyState(VK_UP) & 0x8000) && y > 4)
			y--;
		if ((GetAsyncKeyState(VK_DOWN) & 0x8000) && y < alto - 5)
			y++;
		Pintar();
	}
}

void OVNI::Colorear(int a) // FUNCION PARA CAMBIAR DE COLOR LOS CARACTERES
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	color(hConsole, a);
}

void OVNI::Pintar_Corazones()
{

	Posicionar1(64, 2);
	printf("Vida");
	Posicionar1(70, 2);
	printf("          ");

	for (int i = 0; i < corazones; i++)
	{
		Posicionar1(70 + i, 2);
		printf("%c", 3);
	}
}

void OVNI::morir()
{
	int a = rojo;
	if (corazones == 0)
	{
		Borrar();
		Posicionar1(x, y);
		Colorear(a);
		printf("     ");
		Posicionar1(x, y + 1);
		Colorear(a);
		printf(" **  ");
		Posicionar1(x, y + 2);
		Colorear(a);
		printf("  ** ");
		Posicionar1(x, y + 3);
		Colorear(a);
		printf(" **  ");
		Posicionar1(x, y + 4);
		Colorear(a);
		printf("     ");
		Sleep(200);

		Borrar();
		Posicionar1(x, y);
		Colorear(a);
		printf("     ");
		Posicionar1(x, y + 1);
		Colorear(a);
		printf(" * * ");
		Posicionar1(x, y + 2);
		Colorear(a);
		printf("* * *");
		Posicionar1(x, y + 3);
		Colorear(a);
		printf("  *  ");
		Posicionar1(x, y + 4);
		Colorear(a);
		printf("     ");
		Sleep(200);

		Borrar();
		Posicionar1(x, y);
		Colorear(a);
		printf("*   *");
		Posicionar1(x, y + 1);
		Colorear(a);
		printf(" *** ");
		Posicionar1(x, y + 2);
		Colorear(a);
		printf("* * *");
		Posicionar1(x, y + 3);
		Colorear(a);
		printf(" *** ");
		Posicionar1(x, y + 4);
		Colorear(a);
		printf("*   *");
		Sleep(200);

		Sleep(500);
		system("cls");
		Menu_Volver();
	}
}

int OVNI::getVidas()
{
	return corazones;
}

void OVNI::setVidas(int vidas)
{
	corazones = vidas;
}

void OVNI::vivir()
{
	int a = rojo;
	if (corazones > 0 && corazones <= 3)
	{
		Borrar();
		Posicionar1(x, y);
		Colorear(a);
		printf(" +   + ");
		Posicionar1(x, y + 1);
		Colorear(a);
		printf("+  *  +");
		Posicionar1(x, y + 2);
		Colorear(a);
		printf("+ *** +");
		Posicionar1(x, y + 3);
		Colorear(a);
		printf(" + * + ");
		Posicionar1(x, y + 4);
		Colorear(a);
		printf("+*****+");
		Sleep(200);

		Borrar();
		Posicionar1(x, y);
		Colorear(a);
		printf("+     +");
		Posicionar1(x, y + 1);
		Colorear(a);
		printf(" * + * ");
		Posicionar1(x, y + 2);
		Colorear(a);
		printf("+* * *+");
		Posicionar1(x, y + 3);
		Colorear(a);
		printf(" + * + ");
		Posicionar1(x, y + 4);
		Colorear(a);
		printf("*+ + +*");
		Sleep(200);

		Borrar();
		Posicionar1(x, y);
		Colorear(a);
		printf("+++ +++");
		Posicionar1(x, y + 1);
		Colorear(a);
		printf("+**+**+");
		Posicionar1(x, y + 2);
		Colorear(a);
		printf(" +***+ ");
		Posicionar1(x, y + 3);
		Colorear(a);
		printf("  +*+  ");
		Posicionar1(x, y + 4);
		Colorear(a);
		printf("*+ + +*");
		Sleep(200);

		Sleep(500);
		system("cls");
		Menu_Volver();
	}
}

OVNI ovni(Obtener_Ancho_Ventana() - 10, alto / 2, corazones);
//..............................................................

class ASTEROIDE
{
private:
	int x;
	int y;

public:
	ASTEROIDE(int _x, int _y);
	void Pintar();
	void Mover();
	void Mover2();
	void Mover3();
	void Mover4();
	void choque(struct NAVE &N);
	void choqueB(struct NAVE &N);
	void choqueO(struct OVNI &N);
};

ASTEROIDE::ASTEROIDE(int _x, int _y)
{
	x = _x;
	y = _y;
}

void ASTEROIDE::Pintar()
{

	Posicionar(x, y);
	Colorear1(blanco);
	printf("%c", 184);
}

void ASTEROIDE::Mover()
{
	int ancho = Obtener_Ancho_Ventana();
	int alto = Obtener_Altura_Ventana();

	int limiteIzquierdo = 7;
	int limiteDerecho = ancho - 4; // Posici�n del l�mite derecho donde terminan los ladrillos

	int nuevaX = x; // Nueva posici�n horizontal del asteroide

	// Borrar el asteroide de su posici�n actual
	Posicionar(x, y);
	Colorear1(blanco);
	printf(" ");

	y++; // Mover el asteroide hacia abajo

	if (y > alto - 1)
	{
		nuevaX = limiteIzquierdo + rand() % (limiteDerecho - limiteIzquierdo + 1);
		y = 4; // Reiniciar posici�n vertical
	}

	x = nuevaX;

	Pintar();
}

void ASTEROIDE::choque(struct NAVE &N)
{
	if (x >= N.X() && x < N.X() + 5 && y >= N.Y() && y <= N.Y() + 2)
	{
		N.Cor();
		N.Pintar();
		N.Pintar_Corazones();
		x = rand() % 71 + 4;
		y = 4;
	}
}

void ASTEROIDE::choqueO(struct OVNI &N)
{
	if (x >= N.X() && x < N.X() + 5 && y >= N.Y() && y <= N.Y() + 2)
	{
		N.Cor();
		N.Pintar();
		N.Pintar_Corazones();
		x = rand() % 71 + 4;
		y = 4;
	}
}
class BALA
{
private:
	int x;
	int y;

public:
	BALA(int xInicial, int yInicial)
	{
		x = xInicial;
		y = yInicial;
		sonidoDisparo();
	}
	int X()
	{
		return x;
	}
	int Y()
	{
		return y;
	}
	void mover();
	bool fuera();
	void Pintar();
	void MoverOvni();
	bool fueraOvni();
};
void BALA::mover()
{
	int ancho = Obtener_Ancho_Ventana();
	Posicionar(x, y);
	printf(" "); // Limpiamos la posici�n actual de la bala
	if (x < ancho - 4)
		x++;
	Pintar(); // Volvemos a pintar la bala en la nueva posici�n
}

void BALA::MoverOvni()
{
	int ancho = Obtener_Ancho_Ventana();
	Posicionar(x, y);
	printf(" "); // Limpiamos la posici�n actual de la bala
	if (x > 5)
		x--;

	//   Posicionar(x, y);
	Pintar(); // Volvemos a pintar la bala en la nueva posici�n
}
void BALA::Pintar()
{
	Posicionar(x, y);

	Colorear1(azul);
	printf("%c", 64);
}

bool BALA::fuera()
{
	int ancho = Obtener_Ancho_Ventana();
	if (x >= ancho - 4)
		return true; // La bala est� fuera si su posici�n x es mayor o igual al ancho de la ventana - 4
	return false;
}

bool BALA::fueraOvni()
{
	int ancho = Obtener_Ancho_Ventana();
	if (x <= 5)
		return true; // La bala est� fuera si su posici�n x es mayor o igual al ancho de la ventana - 4
	return false;
}

void Colorear1(int a)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	color(hConsole, a);
}

void Pintar_Limites()
{

	int ancho = Obtener_Ancho_Ventana();
	int alto = Obtener_Altura_Ventana();

	for (int i = 2; i < ancho; i++)
	{
		Posicionar1(i, 3);
		printf("%c", 205);
		Posicionar1(i, alto);
		printf("%c", 205);
	}

	for (int i = 4; i < alto; i++)
	{
		Posicionar1(2, i);
		printf("%c", 186);
		Posicionar1(ancho - 1, i);
		printf("%c", 186);
	}

	Posicionar1(2, 3);
	printf("%c", 201);
	Posicionar1(2, alto);
	printf("%c", 200);
	Posicionar1(ancho - 1, 3);
	printf("%c", 187);
	Posicionar1(ancho - 1, alto);
	printf("%c", 188);
}
//--------------------------------------------------------------------------

bool disparoPresionado = false;
bool disparoPermitido = true;
int numdisparos = 0;
vector<BALA *> balas;
vector<ASTEROIDE> asteroidesV;
sem_t semaforoBalas;
time_t ultimoTiempoRegeneracion = time(nullptr);
pthread_t tBala;
bool juegoActivo = true;

void moverBala()
{
	if (!juegoActivo)
	{
		return;
	}

	if (disparoPresionado && disparoPermitido && balas.size() < 30 && numdisparos < 30)
	{
		balas.push_back(new BALA(nave.X() + 5, nave.Y() + 2));

		disparoPermitido = false;
	}

	for (int i = 0; i < balas.size(); i++)
	{
		balas[i]->mover();
		if (balas[i]->fuera())
		{
			Posicionar1(balas[i]->X(), balas[i]->Y());
			printf(" ");
			delete balas[i];
			balas.erase(balas.begin() + i);
			i--;
		}
	}
}

void moverBalaO()
{
	if (!juegoActivo)
	{
		return;
	}

	if (disparoPresionado && disparoPermitido && balas.size() < 30 && numdisparos < 30)
	{
		balas.push_back(new BALA(ovni.X() - 1, ovni.Y() + 2));

		disparoPermitido = false;
	}

	for (int i = 0; i < balas.size(); i++)
	{
		balas[i]->MoverOvni();
		if (balas[i]->fueraOvni())
		{
			Posicionar1(balas[i]->X(), balas[i]->Y());
			printf(" ");
			delete balas[i];
			balas.erase(balas.begin() + i);
			i--;
		}
	}
}

void *moverBalaHilo(void *data)
{
	moverBala();
	return nullptr;
}

void *moverBalaHiloO(void *data)
{
	moverBalaO();
	return nullptr;
}

void regenerarBalas()
{
	time_t tiempoActual = time(nullptr);
	double tiempoTranscurrido = difftime(tiempoActual, ultimoTiempoRegeneracion);
	if (!juegoActivo)
	{
		return;
	}
	if (tiempoTranscurrido >= 5)
	{
		ultimoTiempoRegeneracion = tiempoActual;
		disparoPermitido = true;
		disparoPresionado = false;
		numdisparos = 0;
	}

	for (int i = 0; i < balas.size(); i++)
	{
		balas[i]->mover();
	}

	if (balas.size() < 30 && numdisparos >= 30)
	{
		if (tiempoTranscurrido >= 5)
		{
			sem_wait(&semaforoBalas); // Bloquear el sem�foro

			balas.clear();

			sem_post(&semaforoBalas); // Desbloquear el sem�foro
		}
		return;
	}

	if (balas.size() < 30 && numdisparos < 30)
	{
		if (disparoPresionado && disparoPermitido)
		{
			balas.push_back(new BALA(nave.X() + 5, nave.Y() + 2));
			disparoPermitido = false;
			numdisparos++;
		}
	}
}

void regenerarBalasO()
{
	time_t tiempoActual = time(nullptr);
	double tiempoTranscurrido = difftime(tiempoActual, ultimoTiempoRegeneracion);
	if (!juegoActivo)
	{
		return;
	}
	if (tiempoTranscurrido >= 5)
	{
		ultimoTiempoRegeneracion = tiempoActual;
		disparoPermitido = true;
		disparoPresionado = false;
		numdisparos = 0;
	}

	for (int i = 0; i < balas.size(); i++)
	{
		balas[i]->MoverOvni();
	}

	if (balas.size() < 30 && numdisparos >= 30)
	{
		if (tiempoTranscurrido >= 5)
		{
			sem_wait(&semaforoBalas); // Bloquear el sem�foro

			balas.clear();

			sem_post(&semaforoBalas); // Desbloquear el sem�foro
		}
		return;
	}

	if (balas.size() < 30 && numdisparos < 30)
	{
		if (disparoPresionado && disparoPermitido)
		{
			balas.push_back(new BALA(ovni.X() - 1, ovni.Y() + 2));
			disparoPermitido = false;
			numdisparos++;
		}
	}
}

void reiniciarJuego()
{
	disparoPermitido = true;
	juegoActivo = true;
	Musica(0);
	balas.clear();
	nave.setVidas(3);
	nave.Pintar_Corazones();
	ovni.setVidas(3);
	nave.Pintar_Corazones();
}

void juego_N()
{
	nave.Posicionar(10, Obtener_Altura_Ventana() / 2);
	sem_init(&semaforoBalas, 0, 1);
	bool Fin_Del_Juego = false;
	int x1 = 0, y1 = 0, x2;
	Colorear1(verde);
	Pintar_Limites();
	OcultarCursor();
	nave.Pintar();

	nave.Pintar_Corazones();

	int cantidadAsteroides = 4;
	pthread_create(&tBala, nullptr, moverBalaHilo, nullptr);
	reiniciarJuego();
	for (int i = 0; i < cantidadAsteroides; i++)
	{
		x1 = 2 + rand() % (ancho / 2 - 6);
		y1 = 4 + rand() % (alto - 10);
		ASTEROIDE ast(x1, y1);
		asteroidesV.push_back(ast);
		ast.choque(nave);
	}
	Musica(0);
	while (!Fin_Del_Juego)
	{
		for (int i = 0; i < asteroidesV.size(); i++)
		{
			asteroidesV[i].Mover();
			asteroidesV[i].choque(nave);
		}

		if (_kbhit())
		{
			char tecla = _getch();
			if (tecla == ' ')
			{
				sonidoDisparo();
				disparoPresionado = true;
				numdisparos++;
			}
		}
		else
		{
			disparoPresionado = false;
			disparoPermitido = true;
		}

		regenerarBalas();
		if (nave.getVidas() == 0)
		{
			system("cls");
			Fin_Del_Juego = true;
			nave.morir();

			juegoActivo = false;
			Fin_Del_Juego = false;
		}

		sem_wait(&semaforoBalas);

		moverBala();
		nave.Mover();
		sem_post(&semaforoBalas);
		Sleep(8);
	}
	Musica(2);
	Mover_Volver();
	pthread_join(tBala, nullptr);
	sem_destroy(&semaforoBalas);
}
void juego_O()
{
	ovni.Posicionar(Obtener_Ancho_Ventana() - 10, Obtener_Altura_Ventana() / 2);
	sem_init(&semaforoBalas, 0, 1);
	bool Fin_Del_Juego = false;
	int x1 = 0, y1 = 0, x2;
	Colorear1(verde);
	Pintar_Limites();
	OcultarCursor();
	ovni.Pintar();

	ovni.Pintar_Corazones();

	int cantidadAsteroides = 6;
	pthread_create(&tBala, nullptr, moverBalaHiloO, nullptr);
	reiniciarJuego();
	for (int i = 0; i < cantidadAsteroides; i++)
	{
		x1 = 2 + rand() % (ancho / 2 - 6);
		y1 = 4 + rand() % (alto - 10);
		ASTEROIDE ast(x1, y1);
		asteroidesV.push_back(ast);
		ast.choqueO(ovni);
	}
	Musica(0);
	while (!Fin_Del_Juego)
	{
		for (int i = 0; i < asteroidesV.size(); i++)
		{
			asteroidesV[i].Mover();
			asteroidesV[i].choqueO(ovni);
		}

		if (_kbhit())
		{
			char tecla = _getch();
			if (tecla == ' ')
			{
				sonidoDisparo();
				disparoPresionado = true;
				numdisparos++;
			}
		}
		else
		{
			disparoPresionado = false;
			disparoPermitido = true;
		}

		regenerarBalasO();
		if (ovni.getVidas() == 0)
		{
			system("cls");
			Fin_Del_Juego = true;
			ovni.morir();

			juegoActivo = false;
			Fin_Del_Juego = false;
		}

		sem_wait(&semaforoBalas);

		moverBalaO();
		ovni.Mover();
		sem_post(&semaforoBalas);
		Sleep(8);
	}
	Musica(2);
	Mover_Volver();
	pthread_join(tBala, nullptr);
	sem_destroy(&semaforoBalas);
}

struct Coordenadas_Botones
{ // Estructura del boton
	int x;
	int y;
	char color;
};

void Colorear(int a)
{ // FUNCION PARA CAMBIAR DE COLOR LOS CARACTERES
	int cl = a;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	color(hConsole, cl);
}

void Posicionar(int x, int y)
{
	HANDLE hCon;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD dwPos;

	dwPos.X = x;
	dwPos.Y = y;

	SetConsoleCursorPosition(hCon, dwPos);
}

void Win()
{
	int Largo_Logo = 70, CoordenadaY_Logo = 10, Altura_Logo = 10;
	;
	int ancho = Obtener_Ancho_Ventana();
	int espacios = Obtener_Espacios(ancho, Largo_Logo);
	int CoordenadaX_Logo = espacios;

	char Logo[Altura_Logo][Largo_Logo + 1] =
		{
			"   *    ________     *        *          *     *    _______  *        ",
			"     __(___)____)___     *                      ___(_______)___       ",
			"    <_________)_>_>_)           *     *        <  <  < | >  >  > *    ",
			"  *   <___>__>____>           *            *     <_____|_____>        ",
			"                                                                      ",
			"        O    O  OOOO    O    O          O    O  OOOOOO  O    O        ",
			"         O  O  O    O   O    O          O    O    OO    OO   O        ",
			"          OO   O    O   O    O          O OO O    OO    O O  O        ",
			"          OO   O    O   O    O          O OO O    OO    O  O O        ",
			"          OO    OOOO     OOOO            O  O   OOOOOO  O   OO        ",
		};

	for (int i = 0; i < Altura_Logo - 5; i++)
	{
		for (int j = 0; j < Largo_Logo; j++)
		{
			Posicionar(CoordenadaX_Logo + j, CoordenadaY_Logo + i);
			Colorear(blanco);
			printf("%c", Logo[i][j]);
		}
		cout << endl;
	}

	for (int i = 5; i < Altura_Logo; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			Posicionar(CoordenadaX_Logo + j, CoordenadaY_Logo + i);
			Colorear(azul);
			printf("%c", Logo[i][j]);
		}
	}

	for (int i = 5; i < Altura_Logo; i++)
	{
		for (int j = 40; j < Largo_Logo; j++)
		{
			Posicionar(CoordenadaX_Logo + j, CoordenadaY_Logo + i);
			Colorear(azul);
			printf("%c", Logo[i][j]);
		}
	}
}

void Lose()
{
	int Largo_Logo = 70, CoordenadaY_Logo = 10, Altura_Logo = 10;
	;
	int ancho = Obtener_Ancho_Ventana();
	int espacios = Obtener_Espacios(ancho, Largo_Logo);
	int CoordenadaX_Logo = espacios;

	char Logo[Altura_Logo][Largo_Logo + 1] =
		{
			"   *    ________     *        *          *     *    _______  *        ",
			"     __(___)____)___     *                      ___(_______)___       ",
			"    <_________)_>_>_)           *     *        <  <  < | >  >  > *    ",
			"  *   <___>__>____>           *            *     <_____|_____>        ",
			"                                                                      ",
			"    O    O  OOOO    O    O          O        OOOO    OOOOO  OOOOOO    ",
			"     O  O  O    O   O    O          O       O    O  O       O         ",
			"      OO   O    O   O    O          O       O    O   OOOO   OOOO      ",
			"      OO   O    O   O    O          O       O    O       O  O         ",
			"      OO    OOOO     OOOO           OOOOOO   OOOO   OOOOO   OOOOOO    ",
		};

	for (int i = 0; i < Altura_Logo - 5; i++)
	{
		for (int j = 0; j < Largo_Logo; j++)
		{
			Posicionar(CoordenadaX_Logo + j, CoordenadaY_Logo + i);
			Colorear(blanco);
			printf("%c", Logo[i][j]);
		}
		cout << endl;
	}

	for (int i = 5; i < Altura_Logo; i++)
	{
		for (int j = 0; j < 40; j++)
		{
			Posicionar(CoordenadaX_Logo + j, CoordenadaY_Logo + i);
			Colorear(rojo);
			printf("%c", Logo[i][j]);
		}
	}

	for (int i = 5; i < Altura_Logo; i++)
	{
		for (int j = 40; j < Largo_Logo; j++)
		{
			Posicionar(CoordenadaX_Logo + j, CoordenadaY_Logo + i);
			Colorear(rojo);
			printf("%c", Logo[i][j]);
		}
	}
}

void Logo()
{ // LOGO
	int Largo_Logo = 70, CoordenadaY_Logo = 10, Altura_Logo = 10;
	;
	int ancho = Obtener_Ancho_Ventana();
	int espacios = Obtener_Espacios(ancho, Largo_Logo);
	int CoordenadaX_Logo = espacios;

	char Logo[Altura_Logo][Largo_Logo + 1] =
		{
			"   *    ________     *        *          *     *    _______  *        ",
			"     __(___)____)___     *                      ___(_______)___       ",
			"    <_________)_>_>_)           *     *        <  <  < | >  >  > *    ",
			"  *   <___>__>____>           *            *     <_____|_____>        ",
			"                                                                      ",
			" OOOOO  OOOOO     OO     OOOOO  OOOOOO  O    O    OO    OOOOO    OOOOO",
			"O       O    O   O  O   O       O       O    O   O  O   O    O  O     ",
			" OOOO   OOOOO   OOOOOO  O       OOOO    O OO O  OOOOOO  OOOOO    OOOO ",
			"     O  O       O    O  O       O       O OO O  O    O  O   O        O",
			"OOOOO   O       O    O   OOOOO  OOOOOO   O  O   O    O  O    O  OOOOO ",
		};

	for (int i = 0; i < Altura_Logo - 5; i++)
	{ // Espacio
		for (int j = 0; j < Largo_Logo; j++)
		{
			Posicionar(CoordenadaX_Logo + j, CoordenadaY_Logo + i);
			Colorear(blanco);
			printf("%c", Logo[i][j]);
		}
		cout << endl;
	}

	for (int i = 5; i < Altura_Logo; i++)
	{ // SPACE
		for (int j = 0; j < 40; j++)
		{
			Posicionar(CoordenadaX_Logo + j, CoordenadaY_Logo + i);
			Colorear(amarillo);
			printf("%c", Logo[i][j]);
		}
	}

	for (int i = 5; i < Altura_Logo; i++)
	{ // WARS
		for (int j = 40; j < Largo_Logo; j++)
		{
			Posicionar(CoordenadaX_Logo + j, CoordenadaY_Logo + i);
			Colorear(rojo);
			printf("%c", Logo[i][j]);
		}
	}
}

void Boton_Nave(char color)
{

	int CoordenadaY_Boton = 10, Altura_Boton = 3, Largo_Boton = 16;
	int ancho = Obtener_Ancho_Ventana();
	int espacios = Obtener_Espacios(ancho, Largo_Boton);

	Coordenadas_Botones Boton_Clasic;
	Boton_Clasic.x = espacios;
	Boton_Clasic.y = 10;
	Boton_Clasic.color = color;

	char nave[Altura_Boton][Largo_Boton + 1] = {
		" oooooooooooooo ",
		"o     Nave     o",
		" oooooooooooooo ",
	};

	for (int i = 0; i < Altura_Boton; i++)
	{
		for (int j = 0; j < Largo_Boton; j++)
		{
			Posicionar(Boton_Clasic.x + j, Boton_Clasic.y + 15 + i);
			Colorear(Boton_Clasic.color);
			printf("%c", nave[i][j]);
		}
		cout << endl;
	}
}

void Boton_Ovni(char color)
{

	int Altura_Boton = 3, Largo_Boton = 16;
	int ancho = Obtener_Ancho_Ventana();
	int espacios = Obtener_Espacios(ancho, Largo_Boton);

	Coordenadas_Botones Boton_Alone;
	Boton_Alone.x = espacios;
	Boton_Alone.y = 10;
	Boton_Alone.color = color;

	char ovni[Altura_Boton][Largo_Boton + 1] = {
		" oooooooooooooo ",
		"o     Ovni     o",
		" oooooooooooooo ",
	};

	for (int i = 0; i < Altura_Boton; i++)
	{
		for (int j = 0; j < Largo_Boton; j++)
		{
			Posicionar(Boton_Alone.x + j, Boton_Alone.y + 19 + i);
			Colorear(Boton_Alone.color);
			printf("%c", ovni[i][j]);
		}
		cout << endl;
	}
}

void Boton_Exit(char color)
{

	int Altura_Boton = 3, Largo_Boton = 16;
	int ancho = Obtener_Ancho_Ventana();
	int espacios = Obtener_Espacios(ancho, Largo_Boton);

	Coordenadas_Botones Boton_Ex;
	Boton_Ex.x = espacios;
	Boton_Ex.y = 10;
	Boton_Ex.color = color;

	char exit[Altura_Boton][Largo_Boton + 1] = {
		" oooooooooooooo ",
		"o     EXIT     o",
		" oooooooooooooo ",
	};

	for (int i = 0; i < Altura_Boton; i++)
	{
		for (int j = 0; j < Largo_Boton; j++)
		{
			Posicionar(Boton_Ex.x + j, Boton_Ex.y + 23 + i);
			Colorear(Boton_Ex.color);
			printf("%c", exit[i][j]);
		}
		cout << endl;
	}
}

void Boton_Volver(char color)
{

	int Altura_Boton = 3, Largo_Boton = 16;
	int ancho = Obtener_Ancho_Ventana();
	int espacios = Obtener_Espacios(ancho, Largo_Boton);

	Coordenadas_Botones Boton_Alone;
	Boton_Alone.x = espacios;
	Boton_Alone.y = 10;
	Boton_Alone.color = color;

	char volver[Altura_Boton][Largo_Boton + 1] = {
		" oooooooooooooo ",
		"o    Volver    o",
		" oooooooooooooo ",
	};

	for (int i = 0; i < Altura_Boton; i++)
	{
		for (int j = 0; j < Largo_Boton; j++)
		{
			Posicionar(Boton_Alone.x + j, Boton_Alone.y + 19 + i);
			Colorear(Boton_Alone.color);
			printf("%c", volver[i][j]);
		}
		cout << endl;
	}
}
//---------------------------------------------------------------------------------------
void Mover_Volver()
{
	 std::cin.clear();        // Limpiar el estado del búfer de entrada
	int selec = 0;
	char activo = verde, inactivo = blanco;
	OcultarCursor();
	bool espacioPresionado = false;
	Musica(2);
	do
	{
		if (kbhit())
		{
			char tecla = _getch();

			if (tecla == ABAJO)
			{
				Boton_Volver(inactivo);
				Boton_Exit(activo);
				selec = 1;
			}

			if (tecla == ARRIBA)
			{
				Boton_Volver(activo);
				Boton_Exit(inactivo);
				selec = 0;
			}

			if (tecla == ESPACIO)
			{ // Verificar si se presion� Espacio
				espacioPresionado = true;
			}
		}

		if (espacioPresionado)
		{
			switch (selec)
			{
			case 0:
				system("cls");
				Menu();
				break;
			case 1:
				exit(1);
				break;
			}
		}

		Sleep(30);
	} while (!espacioPresionado);
}

void Mover_Menu()
{
	std::cin.clear();        // Limpiar el estado del búfer de entrada
	int selec = 0, choice = 0;
	char activo = verde, inactivo = blanco;
	OcultarCursor();
	bool espacioPresionado = false;

	do
	{
		if (kbhit())
		{
			char tecla = _getch();

			if (tecla == ARRIBA)
			{
				choice--;
				if (choice == 3)
					choice = 0;
				if (choice == -1)
					choice = 2;
			}
			if (tecla == ABAJO)
			{
				choice++;
				if (choice == 3)
					choice = 0;
				if (choice == -1)
					choice = 2;
			}

			if (choice == 0)
			{
				Boton_Nave(activo);
				Boton_Ovni(inactivo);
				Boton_Exit(inactivo);
				selec = 0;
			}

			if (choice == 1)
			{
				Boton_Nave(inactivo);
				Boton_Ovni(activo);
				Boton_Exit(inactivo);
				selec = 1;
			}

			if (choice == 2)
			{
				Boton_Nave(inactivo);
				Boton_Ovni(inactivo);
				Boton_Exit(activo);
				selec = 2;
			}

			if (tecla == ESPACIO)
			{ // Verificar si se presion� Espacio
				espacioPresionado = true;
			}
		}

		if (espacioPresionado)
		{
			switch (selec)
			{
			case 0:
				system("cls");

				juego_N();
				break;
			case 1:
				system("cls");
				juego_O();
				break;
			case 2:
				exit(1);
				break;
			}
		}

		Sleep(30);
	} while (!espacioPresionado);
}

void Menu_Volver()
{
	std::cin.clear();        // Limpiar el estado del búfer de entrada
	bool game_over = false;

	Maximizar_Ventana();
	Lose();
	Boton_Volver(verde);
	Boton_Exit(blanco);

	while (!game_over)
	{
		Mover_Volver();
	}
}

void Menu_VolverWin()
{
	std::cin.clear();        // Limpiar el estado del búfer de entrada
	bool game_over = false;

	Maximizar_Ventana();
	Win();
	Boton_Volver(verde);
	Boton_Exit(blanco);

	while (!game_over)
	{
		Mover_Volver();
	}
}

void Menu()
{
	bool game_over = false;
	Musica(1);
	Maximizar_Ventana();
	Logo();
	Boton_Nave(verde);
	Boton_Ovni(blanco);
	Boton_Exit(blanco);

	while (!game_over)
	{
		Mover_Menu();
	}
}

int main()
{
	Menu();
	return 0;
}
