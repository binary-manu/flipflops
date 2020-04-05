#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <time.h>
#include "..\\common\\gui.h"
#include "..\\common\\porte.h"

char BGI_PATH[256];
#define ElementiMenu1 4
#define MIN_DELAY 10	/*Tempo minimo tra un'operazione automatica e l'altra*/
#define MAX_DELAY 90    /*Tempo massimo tra un'operaz. e l'altra*/
#define TRUE 1
#define FALSE 0

typedef unsigned char byte;
typedef unsigned short word;
typedef unsigned char BOOL;
typedef unsigned long dword;

/* Definisco i codici di scansione per i tasti funzione */
typedef enum { f1 = 59, f2 = 60, f3 = 61, f4 = 62, f12 = 134 } SCANCODE;
/* Tipi di flip flop SetReset, a porte NAND o NOR */
enum { ffNAND_SR, ffNOR_SR } FlipFlopType;

void SimulaFlipFlopNAND( void );
void SimulaFlipFlopNOR( void );
void DrawFlipFlop( void );
void InitCommands( void );
void UpdateState( void );
void OnScreenUpdate( void );
void Simula( void );
void Descrivi( void );
void Help( void );
void FlushSTDIN( void );
byte xlatScancode( SCANCODE scancode, SCANCODE * Codes, byte maxlen );

struct
{
	unsigned S  : 1;
	unsigned R  : 1;
	unsigned S1 : 1;
	unsigned R1 : 1;
} RaceCond;

struct
{
	unsigned Q  : 1;
	unsigned Qn : 1;
} State[4];

char map[16];

byte curstate;
byte keys[] = { 'A', 'S', 'R' };
byte STORESTATE;
byte lastRstate;
byte lastSstate;
byte lastQstate;
byte lastQnstate;
byte q;
byte qn;
byte r;
byte s;
byte both;
byte timetodelay = 30;  /* Per default il delay e' di 30 secondi */
BOOL AutoMode = FALSE;  /* Parte con la modalita' automatica disattiva */

struct
{
	word x;
	word y;
}
  posFF = {280, 50 }
, posS  = {180, 260}
, posR  = {180, 280}
, posQ  = {180, 300}
, posQn = {180, 320}
, posSf = {255, 50}
, posRf = {255, 185}
, posQnf= {395, 165}
, posQf = {395, 65};

void OnScreenUpdate( void )
{
	byte strstate[2];
	strstate[1] = '\0';
	setcolor(BLACK);
	strstate[0] = lastQstate + 0x30;
	outtextxy(posQ.x, posQ.y, strstate);
	outtextxy(posQf.x, posQf.y, strstate);
	strstate[0] = lastRstate + 0x30;
	outtextxy(posR.x, posR.y, strstate);
	outtextxy(posRf.x, posRf.y, strstate);
	strstate[0] = lastSstate + 0x30;
	outtextxy(posS.x, posS.y, strstate);
	outtextxy(posSf.x, posSf.y, strstate);
	strstate[0] = lastQnstate + 0x30;
	outtextxy(posQn.x, posQn.y, strstate);
	outtextxy(posQnf.x, posQnf.y, strstate);
	setcolor( LIGHTMAGENTA );
	strstate[0] = q + 0x30;
	outtextxy(posQ.x, posQ.y, strstate);
	outtextxy(posQf.x, posQf.y, strstate);
	strstate[0] = qn + 0x30;
	outtextxy(posQn.x, posQn.y, strstate);
	outtextxy(posQnf.x, posQnf.y, strstate);
	strstate[0] = r + 0x30;
	outtextxy(posR.x, posR.y, strstate);
	outtextxy(posRf.x, posRf.y, strstate);
	strstate[0] = s + 0x30;
	outtextxy(posS.x, posS.y, strstate);
	outtextxy(posSf.x, posSf.y, strstate);
}

void UpdateState( void )
{
	time_t tm, newt, rnd;
	s = (curstate & 0x02) >> 1;
	r = (curstate & 0x01);
	if ( lastSstate == RaceCond.S && lastRstate == RaceCond.R &&
		r == RaceCond.R1 && s == RaceCond.S1 && both == 1)
	{
		/* Simula la race condition */
		word i;
		for ( i = 0; i < 20; i++ )
		{
			q ^= 1;
			qn ^= 1;
			OnScreenUpdate();
			lastQstate = q;
			lastQnstate = qn;
			delay(500);
		}
		time( &rnd );
		q = ( *( &(word)rnd ) ) % 2;
		qn = q ^ 1;
		OnScreenUpdate();
		lastQstate = q;
		lastQnstate = qn;
	}
	else if ( curstate == STORESTATE )
		OnScreenUpdate();
	else
	{
		q = State[curstate].Q ;
		qn = State[curstate].Qn;
		OnScreenUpdate();
		lastQstate = q;
		lastQnstate = qn;
	}
	/* da tempo all'utente per vedere il risultato,
	prima dell'operazione successiva */
	time(&tm);
	if ( AutoMode )
	{
		byte i = 0;
		byte strtm[3];
		while ( !kbhit() )
		{
			if ( i == timetodelay )
			{
				/* Mette un carattere "garbage" nello stream */
				ungetch( 'X' );
				break;
			}
			setcolor( BLACK );
			itoa( i - 1, strtm, 10 );
			outtextxy( 610, 470, strtm);
			itoa( i, strtm, 10 );
			setcolor( WHITE );
			outtextxy( 610, 470, strtm );
			i++;
			delay( 1000 );
		}
		if ( ( getch() & 0xDF ) == 'D' )
		{
			AutoMode = FALSE;
			setcolor( BLACK );
			outtextxy( 165, 470, "attiva" );
			setcolor( LIGHTRED );
			outtextxy( 165, 470, "disattiva" );
		}
		setcolor( BLACK );
		itoa( i-1, strtm, 10 );
		outtextxy( 610, 470, strtm );
	}


}
void InitCommands( void )
{
	char strn[3];
	setcolor( WHITE );
	outtextxy( 300, posS.y, "Premi   per invertire lo stato di S" );
	outtextxy( 300, posR.y, "Premi   per invertire lo stato di R" );
	outtextxy( 300, posQ.y, "Premi   per invertire lo stato di S e R" );
	outtextxy( 300, posQn.y, "Premi   tornare al menu");
	setcolor( GREEN );
	outtextxy( 345, posS.y, "S" );
	setcolor( LIGHTBLUE );
	outtextxy( 345, posR.y, "R" );
	setcolor( LIGHTRED );
	outtextxy( 345, posQ.y, "A" );
	setcolor( YELLOW );
	outtextxy( 345, posQn.y, "X" );
	setcolor( LIGHTGRAY );
	outtextxy( 5, 450, "Premi   per attivare/disattivare la modalit… automatica" );
	outtextxy( 500, 450, "Intervallo:    s" );
	setcolor ( MAGENTA );
	outtextxy( 50, 450, "D" );
	setcolor( LIGHTGRAY );
	outtextxy( 500, 430, "+: +10s  -: -10s" );
	outtextxy( 5, 470, "Stato corrente:" );
	setcolor( LIGHTRED );
	outtextxy ( 165, 470, "disattiva" );
	setcolor( WHITE );
	DrawFrame( 3, 3, 634, 210, "Rappresentazione grafica del flip flop" );
	DrawFrame( 3, 220, 634, 350, "Stati logici di ingressi e uscite" );
	outtextxy( 15, posS.y, "Stato logico di S" );
	outtextxy( 15, posR.y, "Stato logico di R" );
	outtextxy( 15, posQ.y, "Stato logico di Q" );
	outtextxy( 15, posQn.y, "Stato logico di Q" );
	outtextxy( 15 + 128, posQn.y-10, "_" );
	setcolor( WHITE );
	itoa( timetodelay, strn, 10 );
	outtextxy( 600, 450, strn );
}

void Simula( void )
{
	byte strstate[2];
	byte ch;
	byte code;
	InitCommands();
	curstate = 0x02;
	UpdateState();
	for( ; ; )
	{
	FlushSTDIN();
	if ( !AutoMode )
	{
		code = getch();
		ch = code & 0xDF;
	}
	else
	{
		time_t rnd;
		time( &rnd );
		ch = keys[ ( * ( & (word) rnd ) ) % 3];
		code = 0;
	}

	lastRstate = curstate & 0x01;
	lastSstate = ( curstate & 0x02 ) >> 1;
	both = 0;
	switch ( code )
	{
	case 43:
		if ( timetodelay < MAX_DELAY )
		{
			char strn[3];
			itoa( timetodelay, strn, 10 );
			setcolor( BLACK );
			outtextxy( 600, 450, strn );
			setcolor( WHITE );
			timetodelay += 10;
			itoa( timetodelay, strn, 10 );
			outtextxy( 600, 450, strn );
		}
		break;
	case 45:
		if ( timetodelay > MIN_DELAY )
		{
			char strn[3];
			itoa( timetodelay, strn, 10 );
			setcolor( BLACK );
			outtextxy( 600, 450, strn );
			setcolor( WHITE );
			timetodelay -= 10;
			itoa( timetodelay, strn, 10 );
			outtextxy( 600, 450, strn );
		}
		break;
	}
	switch ( ch )
	{
	case 'S':
		curstate ^= 0x02;
		break;
	case 'R':
		curstate ^= 0x01;
		break;
	case 'A':
		curstate ^= 0x03;
		both = 1;
		break;
	case 'X':
		setcolor( WHITE );
		return;
		/*break;*/
	case 'D':
		AutoMode = TRUE;
		setcolor( BLACK );
		outtextxy( 165, 470, "disattiva" );
		setcolor( LIGHTGREEN );
		outtextxy( 165, 470, "attiva" );
		setcolor( WHITE );
		continue;
	}
	UpdateState();
	}
}

void DrawFlipFlop( void )
{
	setcolor( LIGHTRED );
	line( posFF.x + 72, posFF.y + 120, posFF.x + 72, posFF.y + 80 );
	line( posFF.x - 20, posFF.y + 80, posFF.x + 72, posFF.y + 80 );
	line( posFF.x - 20, posFF.y + 30, posFF.x - 20, posFF.y + 80 );
	line( posFF.x - 20, posFF.y + 30, posFF.x - 20, posFF.y + 30 );
	line( posFF.x - 20, posFF.y + 30, posFF.x, posFF.y + 30 );
	setcolor( LIGHTGREEN );
	line( posFF.x + 72, posFF.y + 21, posFF.x + 72, posFF.y + 50 );
	line( posFF.x - 15, posFF.y + 50, posFF.x + 72, posFF.y + 50 );
	arc( posFF.x - 20, posFF.y + 50, 0, 180, 5 );
	line( posFF.x - 25, posFF.y + 50, posFF.x - 40, posFF.y + 50 );
	line( posFF.x - 40, posFF.y + 50, posFF.x - 40, posFF.y + 110 );
	line( posFF.x - 40, posFF.y + 110, posFF.x, posFF.y + 110 );
	setcolor( YELLOW );
	outtextxy( posFF.x, posFF.y, "S" );
	outtextxy( posFF.x + 90, posFF.y + 15, "Q" );
	outtextxy( posFF.x, posFF.y + 135, "R" );
	outtextxy( posFF.x + 90, posFF.y + 115, "Q" );
	outtextxy( posFF.x + 90, posFF.y + 105, "_" );
	setcolor( WHITE );
	if ( FlipFlopType == ffNAND_SR )
	{
		DrawNAND( posFF.x, posFF.y, 0 );
		DrawNAND( posFF.x, posFF.y + 100, 0 );
	}
	else if ( FlipFlopType == ffNOR_SR )
	{
		DrawNOR( posFF.x, posFF.y, 0 );
		DrawNOR( posFF.x, posFF.y + 100, 0 );
	}
}

void SimulaFlipFlopNAND( void )
{
	/* Condizioni per la race condition nel set reset a porte NAND */
	RaceCond.S  = 0;
	RaceCond.R  = 0;
	RaceCond.S1 = 1;
	RaceCond.R1 = 1;
	/* S = 0 / R = 0 */
	State[0].Q  = 1;
	State[0].Qn = 1;
	/* S = 0 / R = 1 */
	State[1].Q  = 1;
	State[1].Qn = 0;
	/* S = 1 / R = 0 */
	State[2].Q  = 0;
	State[2].Qn = 1;
	/* Quando S = 1 e R = 1 vi Š la condizione di memorizzazione */
	STORESTATE = 3;
	FlipFlopType = ffNAND_SR;
	DrawFlipFlop();
	Simula();
}

void SimulaFlipFlopNOR( void )
{
	RaceCond.S  = 1;
	RaceCond.R  = 1;
	RaceCond.S1 = 0;
	RaceCond.R1 = 0;
	State[1].Q  = 1;
	State[1].Qn = 0;
	State[2].Q  = 0;
	State[2].Qn = 1;
	State[3].Q  = 0;
	State[3].Qn = 0;
	STORESTATE  = 0;
	FlipFlopType = ffNOR_SR;
	DrawFlipFlop();
	Simula();
}

void FlushSTDIN( void )
{
	while ( kbhit() )
		getch();
}

void Descrivi( void )
{

	word i, k, n = 0;
	word x = posFF.x;
	word y = posFF.y;
	char str[2];
	str[1] = '\0';
	clearviewport();
	posFF.x = 50;
	posFF.y = 10;
	FlipFlopType = ffNAND_SR;
	DrawFlipFlop();
	FlipFlopType = ffNOR_SR;
	posFF.y = 250;
	DrawFlipFlop();
	posFF.x = x;
	posFF.y = y;
	for ( i = 10; i <= 250; i += 240 )
	{
	outtextxy( 400, i, "S" );
	outtextxy( 420, i, "R" );
	outtextxy( 440, i, "Q" );
	outtextxy( 460, i, "Q" );
	outtextxy( 460, i-10, "_" );

	outtextxy( 400, i+20, "0" );
	outtextxy( 420, i+20, "0" );
	outtextxy( 400, i+40, "0" );
	outtextxy( 420, i+40, "1" );
	outtextxy( 400, i+60, "1" );
	outtextxy( 420, i+60, "0" );
	outtextxy( 400, i+80, "1" );
	outtextxy( 420, i+80, "1" );
	}

	setcolor( GREEN );
	for ( x = 440; x <= 460; x += 20 )
	{
		for ( y = 30; y <= 270; y += 240 )
		{
			for ( i = 0, k = y; i < 4; i++, k += 20 )
			{
				str[0] = map[n*4+i];
				outtextxy( x, k, str );
			}
			n++;
		}
	}
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
	printf(
"Help di SRSIM -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\
\nDal menu principale puoi accedere alla schermata di simulazione nella quale\n\
ti  e\' possibile osservare una rappresentazione grafica del flip flop che hai\n\
( a porte NAND o NOR ).\n\
Premendo i tasti indicati a video puoi invertire lo stato di uno dei due\n\
ingressi oppure di entrambi contemporaneamente; in questo modo puoi far\n\
avvenire una race condition e vederne gli effetti in simulazione.\n\
\nIl programma permette all\'utente di modificare manualmente gli ingressi del\n\
flip-flop tramite la pressione di certi tasti, tuttavia il programma puo\'\n\
essere fatto passare in \'modalita\' automatica\' nella quale gli ingressi\n\
vengono portati ad uno stato logico puramente casuale e tale stato cambia\n\
automaticamente al trascorrere di un certo intervallo di tempo. Questo tempo\n\
e' indicato in basso a destra nella schermata di simulazione e puo' essere\n\
incrementato o decrementato di 10 secondi premendo i tasti + e - del tastierino\n\
numerico. Il tempo minimo e' di 10 secondi, il massimo di 90 secondi.\n\
Al raggiungimento dl limite il timer si resetta e gli ingressi vengoo impostati\n\
ad un valore casuale. La pressione di un tasto qualsiasi (esclusa la D)\n\
durante il conteggio del tempo rimanente prima dell'operazione successiva\n\
resetta il timer e fa\' scattare l'operazione casuale seguente.\n\
In modalita\' automatica non funziona perci• nessun comando, tranne il tasto D\n\
che disattiva l'automatismo (va\' premuto mentre il timer e\' in funzione)\n\
Mentre si e\' in modalita\' automatica, non e' possibile modificare il tempo\n\
limite del timer.\n\n\n\
Premi un tasto qualsiasi..." );

	getch();

}

void FindBGI(void);

void FindBGI(void)
{
	FILE * f = fopen("srsimcfg.bgi", "r");
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

int main()
{
	void ( *Exec[ElementiMenu1] )() =
	{
		SimulaFlipFlopNAND,
		SimulaFlipFlopNOR,
		Descrivi,
		Help
	};

	SCANCODE ScanCodes[ElementiMenu1] =
	{
		f1,
		f2,
		f3,
		f4
	};

	byte * StringheMenu1[ElementiMenu1] =
	{
		"per simulare un flip flop set reset a porte NAND",
		"per simulare un flip flop set reset a porte NOR",
		"per leggere una descrizione del flip flop set reset",
		"per uscire da SRSIM"
	};

	enum COLORS ColoriMenu1[ElementiMenu1] =
	{
		YELLOW,
		LIGHTBLUE,
		GREEN,
		LIGHTRED
	};

	byte * TastiMenu1[ElementiMenu1] =
	{
		"F1",
		"F2",
		"F3",
		"F12"
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

	map[0] = '1';
	map[1] = '1';
	map[2] = '0';
	map[3] = '=';
	map[4] = '=';
	map[5] = '1';
	map[6] = '0';
	map[7] = '0';
	map[8] = '1';
	map[9] = '0';
	map[0xA] = '1';
	map[0xB] = '=';
	map[0xC] = '=';
	map[0xD] = '0';
	map[0xE] = '1';
	map[0xF] = '0';
	map[0x10] = 'k';


	for ( ; ; )
	{
	clearviewport();
	setcolor( WHITE );
	DrawFrame( 3, 3, 634, 43, "SRSIM" );
	setcolor( RED );
	outtextxy( 170, 23, "Simulatore di flip - flop set - reset" );
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
	DrawFrame( 3, 180, 634, 477, "Informazioni sul programma" );
	outtextxy( 20, 207, "Questo programma Š stato scritto da " );
	setcolor( LIGHTGREEN );
	outtextxy( 35 * 8 + 20 + 3, 207, " Emanuele Giacomelli");
	setcolor( WHITE );
	outtextxy( 20, 227, "Classe 3D Informatica" );
	outtextxy( 20, 247, "Anno Scolastico 2003/2004" );
	outtextxy( 20, 267, "Laboratorio Sistemi di Trasmissione ed Elaborazione delle Informazioni" );
	outtextxy( 20, 287, "Istituto Tecnico Industriale Statale \"G. Peano\"");
	outtextxy( 20, 400, "Se devi consultare l'help, premi" );
	setcolor( CYAN );
	outtextxy( 32*8+25, 400, "F4" );

	for ( ; ; )
	{
	fflush( stdin );
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
	return 0;
/***************************************************************************/
}
