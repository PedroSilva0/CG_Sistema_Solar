#include "luz.h"

void Luz::setPos(float x, float y, float z, float w)
{
	pos[0] = x;
	pos[1] = y;
	pos[2] = z;
	pos[3] = w;
}

void Luz::setAmb(float r, float g, float b)
{
	amb[0] = r;
	amb[1] = g;
	amb[2] = b;
}

void Luz::setDiff(float r, float g, float b)
{
	dif[0] = r;
	dif[1] = g;
	dif[2] = b;
}

void Luz::getPos(float res[4] )
{
	res[0] = pos[0];
	res[1] = pos[1];
	res[2] = pos[2];
	res[3] = pos[3];
}

void Luz::getAmb(float res[3])
{
	res[0] = amb[0];
	res[1] = amb[1];
	res[2] = amb[2];
}

void Luz::getDif(float res[3])
{
	res[0] = dif[0];
	res[1] = dif[1];
	res[2] = dif[2];
}
