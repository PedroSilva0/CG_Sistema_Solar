#ifndef ROTACAO_H
#define ROTACAO_H	

class Rotacao {
private:
	float x = 0, y = 0, z = 0, angulo = 0;
public:
	void set_values(float, float, float, float);
	float getX();
	float getY();
	float getZ();
	float getAng();
};
#endif

