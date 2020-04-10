#define UP_ARROW 'H'
#define DOWN_ARROW 'P'
#define F1 59

void DrawFrame(short, short, short, short, char * const);
void DrawFrame(short x1, short y1, short x2, short y2, char * const msg)
{
	unsigned short l;
	line(x1, y1+3, x1, y2);
	line(x2, y1+3, x2, y2);
	line(x1, y2, x2, y2);
	outtextxy(x1+3+8, y1, msg);
	l = strlen(msg) * 8 + 16;
	line(l, y1+3, x2, y1+3);
	line(x1, y1+3, x1+8, y1+3);
}
