#include "imbox.h"


#define x0 (0.0)
#define y0 (0.0)
#define xl 0
#define xr 2
#define h (1.0*(xr-xl)/MAX)
#define f(x,y) (1.0-2.0*(x)*(y)/(1+(x)*(x)))

double IMx[MAX], IMy[MAX];







void imboxmain()
{

	
	for (int i = 0; i < MAX; i++)
	{
		IMx[i] = 4.0*i/MAX;
		IMy[i] = sin(IMx[i]);
	}


	imbox_paint2D(IMx, IMy);

}
