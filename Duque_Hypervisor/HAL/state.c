#include "Headers/HyperVisor.h"

volatile static int state = OFF;

void set_state(int new_state)
{
	state = new_state;
}

int get_state()
{
	return state;
}