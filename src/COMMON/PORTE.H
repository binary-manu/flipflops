/*****************************************************************************
* Funzioni per il disegno delle porte logiche e dei flip flop                *
* Scritto da Emanuele Giacomelli                                             *
* Classe 3D Informatica                                                      *
* 28/02/2004                                                                 *
* Ultimo aggiornamento:	03/04/2004                                           *
* Lab. Sistemi                                                               *
*****************************************************************************/

void DrawAND(short, short, int);
void DrawNOT(short, short, int);
void DrawOR(short, short, int);
void DrawNOR(short, short, int);
void DrawXNOR(short, short, int);
void DrawXOR(short, short, int);
void DrawNAND(short, short, int);
void DrawFFSR_NAND(short, short);
void DrawAND(short orgx, short orgy, int Lettere)
{
	if (Lettere != 0)
	{
	outtextxy(orgx+0, orgy+12, "A");
	outtextxy(orgx+0, orgy+32, "B");
	outtextxy(orgx+70, orgy+25, "Y");
	}
	line(orgx+0, orgy+10, orgx+20, orgy+10);
	line(orgx+0, orgy+30, orgx+20, orgy+30);
	line(orgx+20, orgy+0, orgx+20, orgy+40);
	line(orgx+20, orgy+0, orgx+40, orgy+0);
	line(orgx+20, orgy+40, orgx+40, orgy+40);
	arc(orgx+40, orgy+20, -90, 90, 20);
	line(orgx+60, orgy+20, orgx+80, orgy+20);
}

void DrawNOT(short orgx, short orgy, int Lettere)
{
	if (Lettere != 0)
	{
	outtextxy(orgx+0, orgy+25, "A");
	outtextxy(orgx+110, orgy+25, "Y");
	}
	line(orgx+0, orgy+20, orgx+40, orgy+20);
	line(orgx+40, orgy+0, orgx+70, orgy+20);
	line(orgx+40, orgy+40, orgx+70, orgy+20);
	line(orgx+40, orgy+40, orgx+40, orgy);
	circle(orgx+75, orgy+20, 5);
	line(orgx+80, orgy+20, orgx+120, orgy+20);
}

void DrawOR(short orgx, short orgy, int Lettere)
{
	if (Lettere != 0)
	{
	outtextxy(orgx+0, orgy+12, "A");
	outtextxy(orgx+0, orgy+32, "B");
	outtextxy(orgx+70, orgy+25, "Y");
	}
	ellipse(orgx+10, orgy+20, -70, 70, 10, 20);
	line(orgx+14, orgy+2, orgy+37, orgy+2);
	line(orgx+14, orgy+40, orgx+37, orgy+40);
	arc(orgx+38, orgy+21, -90, 90, 18);
	line(orgx+0, orgy+10, orgx+17, orgy+10);
	line(orgx+0, orgy+30, orgx+17, orgy+30);
	line(orgx+56, orgy+21, orgx+70, orgy+21);
}

void DrawNOR(short orgx, short orgy, int Lettere)
{
	if (Lettere != 0)
	{
	outtextxy(orgx+0, orgy+12, "A");
	outtextxy(orgx+0, orgy+32, "B");
	outtextxy(orgx+70, orgy+25, "Y");
	}
	ellipse(orgx+10, orgy+20, -70, 70, 10, 20);
	line(orgx+14, orgy+2, orgx+37, orgy+2);
	line(orgx+14, orgy+40, orgx+37, orgy+40);
	arc(orgx+38, orgy+21, -90, 90, 18);
	line(orgx+0, orgy+10, orgx+17, orgy+10);
	line(orgx+0, orgy+30, orgx+17, orgy+30);
	circle(orgx+60, orgy+21, 4);
	line(orgx+64, orgy+21, orgx+78, orgy+21);
}

void DrawXNOR(short orgx, short orgy, int Lettere)
{
	if (Lettere != 0)
	{
	outtextxy(orgx+0, orgy+12, "A");
	outtextxy(orgx+0, orgy+32, "B");
	outtextxy(orgx+70, orgy+25, "Y");
	}
	ellipse(orgx+10, orgy+20, -70, 70, 10, 20);
	ellipse(orgx+5, orgy+20, -70, 70, 10, 20);
	line(orgx+14, orgy+2, orgx+37, orgy+2);
	line(orgx+14, orgy+40, orgx+37, orgy+40);
	arc(orgx+38, orgy+21, -90, 90, 18);
	line(orgx+0, orgy+10, orgx+17, orgy+10);
	line(orgx+0, orgy+30, orgx+17, orgy+30);
	circle(orgx+60, orgy+21, 4);
	line(orgx+64, orgy+21, orgx+78, orgy+21);
}

void DrawXOR(short orgx, short orgy, int Lettere)
{
	if (Lettere != 0)
	{
	outtextxy(orgx+0, orgy+12, "A");
	outtextxy(orgx+0, orgy+32, "B");
	outtextxy(orgx+70, orgy+25, "Y");
	}
	ellipse(orgx+10, orgy+20, -70, 70, 10, 20);
	ellipse(orgx+5, orgy+20, -70, 70, 10, 20);
	line(orgx+14, orgy+2, orgx+37, orgy+2);
	line(orgx+14, orgy+40, orgx+37, orgy+40);
	arc(orgx+38, orgy+21, -90, 90, 18);
	line(orgx+0, orgy+10, orgx+17, orgy+10);
	line(orgx+0, orgy+30, orgx+17, orgy+30);
	line(orgx+56, orgy+21, orgx+70, orgy+21);
}

void DrawNAND(short orgx, short orgy, int Lettere)
{
	if (Lettere != 0)
	{
	outtextxy(orgx+0, orgy+12, "A");
	outtextxy(orgx+0, orgy+32, "B");
	outtextxy(orgx+70, orgy+25, "Y");
	}
	line(orgx+0, orgy+10, orgx+20, orgy+10);
	line(orgx+0, orgy+30, orgx+20, orgy+30);
	line(orgx+20, orgy+0, orgx+20, orgy+40);
	line(orgx+20, orgy+0, orgx+40, orgy+0);
	line(orgx+20, orgy+40, orgx+40, orgy+40);
	arc(orgx+40, orgy+20, -90, 90, 20);
	circle(orgx+64, orgy+20, 4);
	line(orgx+68, orgy+20, orgx+81, orgy+20);
}

void DrawFFSR_NAND(short paramX, short paramY)
{
	struct
	{
		short	x;
		short	y;
	} posFF;

	posFF.x = paramX;
	posFF.y = paramY;
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
	setcolor( WHITE );
	DrawNAND( posFF.x, posFF.y, 0 );
	DrawNAND( posFF.x, posFF.y + 100, 0 );
}