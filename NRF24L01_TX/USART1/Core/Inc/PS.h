#ifndef _PS_H_
#define _PS_H_

#include "main.h"
   
#define Xleft    	     1
#define Xright           2
#define Ybefore    	   	 3
#define Yaffer    		 4
#define TOPleft          5
#define Topright         6
#define Lowleft          7
#define Lowright         8
#define Zdown            9

uint8_t Med_Ps2_Scan(void);
void show_ps2(void);

#endif