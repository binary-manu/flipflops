#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include "..\\common\\gui.h"
#include "..\\common\\porte.h"

#define ElementiMenu1 4
#define ElementiMenu2 6
#define WAVE_X 50
#define MAX_PERIODS 12
#define TRUE 1
#define FALSE 0
#define WH 10

char BGI_PATH[256];

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned char BOOL;
typedef unsigned long dword;

/* Definisco i codici di scansione per i tasti funzione */
typedef enum { f1 = 59, f2 = 60, f3 = 61, f4 = 62, f12 = 134 } SCANCODE;

void FindBGI(void);

void FindBGI(void)
{
	FILE * f = fopen("dsimcfg.bgi", "r");
	if ( f == NULL)
	{
		strcpy(BGI_PATH, ".\\BGI\\");
		return;
	}
	fgets(BGI_PATH, 256, f);
	if (BGI_PATH[strlen(BGI_PATH)-1] == '\n')
		BGI_PATH[strlen(BGI_PATH)-1] = 0;
	fclose(f);
}

void SimulaFlipFlopD( void );
void DrawFlipFlopD( word paramX, word paramY );
void InitCommands( void );
void AggiornaStatoQ( void );
void AggiornaStatoD( void );
void AggiornaStatoCK( void );
void AggiornaStatoQn( void );
void OnScreenUpdate( void );
void Descrivi( void );
void Help( void );
void FlushSTDIN( void );
byte xlatScancode( SCANCODE scancode, SCANCODE * Codes, byte maxlen );
void AggiornaUscite(byte d, byte ck, byte * const q, byte * const qn);
void AggiornaUscite2( void );

char ch = 'P';
byte lastD;
byte lastQ;
byte lastQn;
byte d;
byte q;
byte qn;
byte ck = 0;
byte lastDstate;
byte clockcounter;
word qwck = 300, qwd  = 350, qwq  = 400, qwqn = 450, qwx = 0;
word y1;
word tyq;
word fyq;
word fyqn;
word tyqn;
char keys[] = { 'D', 'P' };
word DELAY = 50;
byte lastu1a=1, lastu1b=1, lastu2a=0, lastu2b=1, lastu3a=1, lastu3b=0;

void AggiornaOndaQuadra()
{
	word fyck = (ck == 1) ? qwck + WH : qwck - WH;
	word tyck = (ck == 1) ? qwck - WH : qwck + WH;
	word fyd = (d == 1) ? qwd + WH : qwd - WH;
	word tyd = (d == 1) ? qwd - WH : qwd + WH;

	word fy, ty;

	qwx+=WAVE_X;
	if ( qwx%40==0 )
	{
		++clockcounter;
		/* clock */
		ck ^= 1;
		line( qwx, tyck, qwx, fyck);
		AggiornaStatoCK();
		AggiornaUscite2();
		if (lastQ != q)
		{
			fyq = (q == 1) ? qwq + WH : qwq - WH;
			tyq = (q == 1) ? qwq - WH : qwq + WH;
			line( qwx, fyq, qwx, tyq);
			AggiornaStatoQ();
	       /*	}
		if (lastQn != qn)
		{ */
			fyqn = (qn == 1) ? qwqn + WH : qwqn - WH;
			tyqn = (qn == 1) ? qwqn - WH : qwqn + WH;
			line( qwx, fyqn, qwx, tyqn);
			AggiornaStatoQn();
		}
		lastQ = q;
		lastQn = qn;
	}
	putpixel(qwx, tyck, WHITE);
	putpixel(qwx+1, tyq, WHITE);
	putpixel(qwx+1, tyqn, WHITE);
	/* onda quadra di D */
	if (lastD != d)
	{
		line( qwx, fyd, qwx, tyd);
		AggiornaStatoD();
		AggiornaUscite2();
	}
	else
		putpixel(qwx, tyd , WHITE);
	lastD = d;
	qwx -= WAVE_X;
}

void OnScreenUpdate( void )
{

}

void AggiornaStatoQ( void )
{

	char str[2];
	itoa(lastQ, str, 10);
	setcolor(BLACK);
	outtextxy(600, qwq, str);
	outtextxy(590, 47, str);
	itoa(q, str, 10);
	setcolor(LIGHTBLUE);
	outtextxy(600, qwq, str);
	outtextxy(590, 47, str);
	setcolor(WHITE);
}

void AggiornaStatoQn( void )
{

	char str[2];
	itoa(lastQn, str, 10);
	setcolor(BLACK);
	outtextxy(600, qwqn, str);
	outtextxy(590, 148, str);
	itoa(qn, str, 10);
	setcolor(MAGENTA);
	outtextxy(600, qwqn, str);
	outtextxy(590, 148, str);
	setcolor(WHITE);
}

void AggiornaStatoD( void )
{

	char str[2];
	itoa(lastD, str, 10);
	setcolor(BLACK);
	outtextxy(600, qwd, str);
	outtextxy(50, 37, str);
	itoa(d, str, 10);
	setcolor(GREEN);
	outtextxy(600, qwd, str);
	outtextxy(50, 37, str);
	setcolor(WHITE);
}

void AggiornaStatoCK( void )
{

	char str[2];
	itoa(ck^1, str, 10);
	setcolor(BLACK);
	outtextxy(600, qwck, str);
	outtextxy(10, 87, str);
	itoa(ck, str, 10);
	setcolor(LIGHTRED);
	outtextxy(600, qwck, str);
	outtextxy(10, 87, str);
	setcolor(WHITE);
}

void InitCommands( void )
{

}

void AggiornaUscite(byte d, byte ck, byte * const q, byte * const qn)
{
	if (ck == 0)
		return;
	*q = d;
	*qn = d ^ 1;
}

void AggiornaUscite2()
{
	char str[2];
	byte u1a, u1b, u2a, u2b, u3a, u3b;
	/* Uscite delle 2 NAND che filtrano D in base al CK */
	u1a = (ck == 0) ? 1 : d ^ 1;
	u1b = (ck == 0) ? 1 : d;
	/* Uscite del SR Master */
	u2a = (ck == 0) ? lastu2a : u1a ^ 1;
	u2b = (ck == 0) ? lastu2b : u1b ^ 1;
	/* Uscite delle seconde porte di filtraggio */
	u3a = (ck == 1) ? lastu3a : u2a ^ 1;
	u3b = (ck == 1) ? lastu3b : u2b ^ 1;

	setcolor(BLACK);
	itoa(lastu1a, str, 10);
	outtextxy(150, 37, str);
	itoa(lastu1b, str, 10);
	outtextxy(150, 157, str);
	itoa(lastu2a, str, 10);
	outtextxy(280, 36, str);
	itoa(lastu2b, str, 10);
	outtextxy(280, 157, str);
	itoa(lastu3a, str, 10);
	outtextxy(415, 37, str);
	itoa(lastu3b, str, 10);
	outtextxy(415, 157, str);

	setcolor(LIGHTGRAY);
	itoa(u1a, str, 10);
	outtextxy(150, 37, str);
	itoa(u1b, str, 10);
	outtextxy(150, 157, str);
	itoa(u2a, str, 10);
	outtextxy(280, 36, str);
	itoa(u2b, str, 10);
	outtextxy(280, 157, str);
	itoa(u3a, str, 10);
	outtextxy(415, 37, str);
	itoa(u3b, str, 10);
	outtextxy(415, 157, str);

	lastu1a = u1a;
	lastu1b = u1b;
	lastu2a = u2a;
	lastu2b = u2b;
	lastu3a = u3a;
	lastu3b = u3b;
	setcolor(WHITE);
}

void SimulaFlipFlopD()
{
	d = 0;
	lastD = 0;
	lastQ = 0;
	q = 0;
	qn = 1;
	lastQn = 1;
	tyq = qwq + WH;
	fyq = qwq - WH;
	tyqn = qwqn - WH;
	fyqn = qwqn + WH;
	for(;;)
	{
	qwx = 0;
	cleardevice();
	AggiornaUscite2();
	AggiornaStatoQ();
	AggiornaStatoQn();
	AggiornaStatoD();
	AggiornaStatoCK();
	clockcounter = 0;
	DrawFrame(3, 265, 637, 477, "Onde quadre - Stati logici di CLOCK, D, Q, e Q negato");
	DrawFrame(3, 3, 637, 250, "Rappresentazione grafica del flip-flop D");
	DrawFlipFlopD(50, 30);
	setcolor(LIGHTRED);
	outtextxy(15, qwck, "CK");
	setcolor(GREEN);
	outtextxy(15, qwd, "D");
	setcolor(LIGHTBLUE);
	outtextxy(15, qwq, "Q");
	setcolor(MAGENTA);
	outtextxy(15, qwqn, "Q");
	outtextxy(15, qwqn-10, "_");
	setcolor(LIGHTGRAY);
	line(540, 280, 540, 470);
	setcolor(WHITE);
	for (;;)
	{
	ch = 'P';
	if ( kbhit() )
	{
		ch = getch();
	}
		if (ch == '-' && DELAY < 300)
			DELAY += 10;
		else if ( ch == '+' && DELAY > 10)
			DELAY -= 10;
		else
		{
		switch (ch & 0xDF)
		{
		case 'D':         /* Inverte D */
			d ^= 1;
		break;
		case 'W':                 /* Blocca il programma */
			while(!kbhit());
		break;
		case 'R':                 /* Resetta il delay di default */
			DELAY = 50;
		break;
		case 'X':                 /* Esce */
			return;
		/*break*/
		}
		}
	FlushSTDIN();
	AggiornaUscite(d, ck, &q, &qn);
	AggiornaOndaQuadra();
	delay(DELAY);
	qwx++;
	if ( clockcounter == MAX_PERIODS )
		break;
	}
	}
}

void Simula( void )
{
}

void DrawFlipFlopD( word paramX, word paramY )
{
	word X;
	word i;
	paramX += 30;
	X = paramX;
	for (i = 0; i <= 265; paramX += 265, i += 265)
	{
	DrawFFSR_NAND(paramX+130, paramY);
	DrawNAND(paramX, paramY, 0);
	line(paramX+81, paramY+20, paramX+81, paramY+10);
	line(paramX+81, paramY+10, paramX+130, paramY+10);
	DrawNAND(paramX, paramY+100, 0);
	line(paramX+81, paramY+120, paramX+81, paramY+130);
	line(paramX+81, paramY+130, paramX+130, paramY+130);
	}
	paramX = X;
	setcolor(MAGENTA);
	line(paramX+81, paramY+20, paramX+81, paramY+90);
	line(paramX+81, paramY+90, paramX, paramY+90);
	line(paramX, paramY+90, paramX, paramY+110);
	setcolor(WHITE);
	paramX -= 30;
	line(paramX, paramY+60, paramX+25, paramY+60);
	line(paramX+25, paramY+60, paramX+25, paramY+30);
	line(paramX+25, paramY+30, paramX+30, paramY+30);
	line(paramX+25, paramY+60, paramX+25, paramY+180);
	line(paramX+25, paramY+130, paramX+30, paramY+130);
	line(paramX+25,  paramY+180, paramX+80, paramY+180);
	DrawNOT(paramX+80, paramY+160, 0);
	line(paramX+200, paramY+180, paramX+280, paramY+180);
	line(paramX+280, paramY+180, paramX+280, paramY+130);
	line(paramX+280, paramY+130, paramX+310, paramY+130);
	line(paramX+280, paramY+130, paramX+280, paramY+30);
	line(paramX+280, paramY+30, paramX+310, paramY+30);
	line(paramX+242, paramY+20, paramX+242, paramY+10);
	line(paramX+242, paramY+10, paramX+300, paramY+10);
	line(paramX+242, paramY+120, paramX+242, paramY+110);
	arc(paramX+280, paramY+110, 0, 180, 6);
	line(paramX+242, paramY+110, paramX+273, paramY+110);
	line(paramX+286, paramY+110, paramX+300, paramY+110);
	setcolor(LIGHTRED);
	outtextxy(30, paramY+57, "CK");
	setcolor(GREEN);
	outtextxy(67, paramY+7, "D");
	setcolor(LIGHTBLUE);
	outtextxy(570, paramY+17, "Q");
	setcolor(MAGENTA);
	outtextxy(570, paramY+118, "Q");
	outtextxy(570, paramY+108, "_");
}

void FlushSTDIN( void )
{
	while ( kbhit() )
		getch();
}

void Descrivi( void )
{
	DrawFlipFlopD(50, 15);
	gotoxy(47,14);
	putchar('_');
	gotoxy(1, 15);
	printf("\t\t\t\t CK %c D %c Q %c Q\n", 179, 179, 179);
	printf("\t\t\t\t 0  %c X %c = %c =\n", 179, 179, 179);
	printf("\t\t\t\t 1  %c X %c = %c =\n", 179, 179, 179);
        printf("\t\t\t\t %c  %c X %c = %c =\n", 24, 179, 179, 179);
        printf("\t\t\t\t %c  %c 0 %c 0 %c 1\n", 25, 179, 179, 179);
        printf("\t\t\t\t %c  %c 1 %c 1 %c 0\n", 25, 179, 179, 179);
        gotoxy(1, 22);
        printf("Il simbolo = indica che l'uscita non cambia: si ha cioŠ memorizzazione.\n");
        printf("La %c indica una transizione verso l'alto del clock, che passa da 0 a 1.\n", 24);
        printf("La %c indica una transizione verso il basso del clock, che passa da 1 a 0.\n", 25);
        printf("La X indica che lo stato di quell'ingresso non e' importante per trovare\ngli stati delle uscite.");
        printf("\nCio' vale solo per l'ingresso D quando il clock disabilita il FF D.\n");
        printf("Infatti per ogni valore di D le uscite non variano se il CK non abilita il FF.");
	getch();
}

byte xlatScancode( SCANCODE scancode, SCANCODE * Codes, byte maxlen )
{
	byte i;
	for ( i = 0; i < maxlen; i++ )
	{
		if ( Codes[i] == scancode )
			return i;
	}

	return 0xFF;
}

void Help( void )
{
	clearviewport();
	gotoxy( 1, 1 );
	printf("Help di DSIM-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_");
	printf("\nDSIM e' un programma che permette di simulare un flip D Master-Slave\n\
Edge Triggered, ossia un FF D con abilitazione a transizione.\n\
Premendo F1 dal menu principale si apre l'ambiente di simulazione. La parte\n\
alta del video e' occupata da una rappresentazione grafica del FF D che\n\
illustra la sua composizione interna ed i suoi collegamenti. Ingressi e uscite\n\
sono indicati con i loro nomi e a fianco di ogni nome c'Š lo stato corrente\n\
di quello specifico ingresso o uscita. Sulla figura Š possibile vedere in\n\
grigio degli altri numeri, che rappresentano gli stati logici delle porte\n\
intermedie. La parte bassa del video e' invece occupata da un 'oscilloscopio\n\
virtuale' che traccia istante dopo istante le onde quadre corrispondenti ai 2\n\
ingressi e alle 2 uscite. I numeri colorati a destra delle onde indicano in\n\
forma numerica lo stato logico di quell'ingresso o di quell'uscita.\n\
La variazione di un'onda si riflette su tutti i numeri che portano lo stesso \n\
significato: quando il CK passa da 0 a 1, non solo l'onda cambia forma, ma si\n\
vede lo 0 rosso alla sua destra diventare 1, cos come lo 0 rosso vicino alla\n\
figura in alto. Questo vale per tutte le onde.\n\
Inoltre e' possibile velocizzare o rallentare la tracciatura delle onde, entro\n\
certi limiti, con i tasti + e - del tastierino numerico. Per riportare la\n\
velocita' al valore di default basta premere R.\n\
Con il tasto W invece e' possibile bloccare temporaneamente il programma, per\n\
poter analizzare nel dettaglio ogni singolo cambiamento degli stati delle porte\n\
del FF D. Per ripartire e' sufficiente premere un tasto qualsiasi.\n\
\nPremi un tasto qualsiasi . . .");
	getch();

}

int main()
{
	void ( *Exec[ElementiMenu1] )() =
	{
		SimulaFlipFlopD,
		Descrivi,
		Help
	};

	SCANCODE ScanCodes[ElementiMenu1-1] =
	{
		f1,
		f2,
		f3
	};

	byte * StringheMenu1[ElementiMenu1] =
	{
		"per simulare un flip flop D",
		"per vedere la mappa del flip flop D",
		"per consultare l'help",
		"per uscire da DSIM"
	};

	enum COLORS ColoriMenu1[ElementiMenu1] =
	{
		YELLOW,
		LIGHTBLUE,
		LIGHTRED,
		GREEN
	};

	byte * TastiMenu1[ElementiMenu1] =
	{
		"F1",
		"F2",
		"F3",
		"F12"
	};

	byte * TastiMenu2[ElementiMenu2] =
	{
		"D",
		"W",
		"+",
		"-",
		"R",
		"X"
	};

	enum COLORS ColoriMenu2[ElementiMenu2] =
	{
		MAGENTA,
		CYAN,
		LIGHTRED,
		GREEN,
		LIGHTCYAN,
		LIGHTMAGENTA
	};

	byte * StringheMenu2[ElementiMenu2] =
	{
		"per invertire lo stato dell'ingresso D",
        "per bloccare temporaneamente il programma; riparte con ogni tasto",
		"per aumentare la velocit… di tracciatura delle onde quadre",
		"per diminuire la velocit… di tracciatura delle onde quadre",
		"per ripristinare la velocit… di tracciatura di default",
		"per tornare a questo menu"
	};

	word i;
	word y;
	byte index;
	SCANCODE scancode;

/****************** Inizializzazione della modalit… grafica ****************/
	int gdriver = DETECT, gmode, errorcode;
	FindBGI();
	initgraph( &gdriver, &gmode, BGI_PATH );
	if ( ( errorcode = graphresult() ) != grOk )
	{
		clrscr();
		printf( "Errore nell\'apertura della grafica: %s\n", grapherrormsg( errorcode ) );
		printf( "Premere un tasto..." );
		getch();
		return -1;
	}
/***************************************************************************/
	for ( ; ; )
	{
	clearviewport();
	setcolor( WHITE );
	DrawFrame( 3, 3, 634, 43, "DSIM" );
	setcolor( RED );
	outtextxy( 100, 23, "Simulatore di flip flop D Master-Slave Edge Triggered" );
	setcolor( WHITE );
	DrawFrame( 3, 53, 634, 170 , "Menu principale" );

	y = 80;
	for ( i = 0; i < ElementiMenu1; i++ )
	{
		outtextxy( 20, y, "Premi" );
		setcolor( ColoriMenu1[i] );
		outtextxy( 70, y, TastiMenu1[i] );
		setcolor( WHITE );
		outtextxy( 100, y, StringheMenu1[i] );
		y += 20;
	}
	DrawFrame( 3, 180, 634, 315, "Informazioni sul programma" );
	outtextxy( 20, 207, "Questo programma Š stato scritto da " );
	setcolor( LIGHTGREEN );
	outtextxy( 35 * 8 + 20 + 3, 207, " Emanuele Giacomelli");
	setcolor( WHITE );
	outtextxy( 20, 227, "Classe 3D Informatica" );
	outtextxy( 20, 247, "Anno Scolastico 2003/2004" );
	outtextxy( 20, 267, "Laboratorio Sistemi di Trasmissione ed Elaborazione delle Informazioni" );
	outtextxy( 20, 287, "Istituto Tecnico Industriale Statale \"G. Peano\"");
	DrawFrame(3, 325, 634, 477, "Comandi (da usare nell'ambiente simulazione che compare premendo F1)");

	y = 350;
	for ( i = 0; i < ElementiMenu2; i++ )
	{
		outtextxy( 20, y, "Premi" );
		setcolor( ColoriMenu2[i] );
		outtextxy( 70, y, TastiMenu2[i] );
		setcolor( WHITE );
		outtextxy( 100, y, StringheMenu2[i] );
		y += 20;
	}

	for ( ; ; )
	{
	FlushSTDIN();
	if ( getch() != 0) continue; /* lettura del codice ASCII */
	scancode = getch(); /* lettura del codice di scansione */
	if ( scancode == f12 ) break;
	index = xlatScancode( scancode, ScanCodes, ElementiMenu1 );
	if ( index == 0xFF ) continue;
	setcolor( WHITE );
	clearviewport();
	( Exec[index] )();
	break;
	}

	if ( scancode != f12 )
		continue;
	break;
	}

/******************** Chiusura della modalit… grafica **********************/
	closegraph();
	exit(0);
/***************************************************************************/
}
