#include <stdbool.h> 
#include <string.h>

#define MIN(x,y) ((x<y)?x:y)
#define MAX(x,y) ((x>y)?x:y)
#define MULT9(x) ((x << 3) + x)


void smoothNoFilter(unsigned int pdim, unsigned char *src, char *dst) {
	printf("pdim: %d\n", pdim);
	int dim = pdim * 3;
	// copy the first row of pixels to dst
	memcpy(dst, src, dim);
	// copy the last row of pixels to dst
	memcpy(dst + ((pdim - 1)*dim), src + ((pdim - 1)*dim), dim);
	
	register unsigned char* a = src;
	register unsigned char* b = src + dim;
	register unsigned char* c = src + dim + dim;
	register unsigned char* x = src + 3;
	register unsigned char* y = src + dim + 3;
	register unsigned char* z = src + dim + dim + 3;
	register unsigned char* u = src + 6;
	register unsigned char* v = src + dim + 6;
	register unsigned char* w = src + dim + dim + 6;
	char* destr = dst + dim + 3;
	char* destg = dst + dim + 4;
	char* destb = dst + dim + 5;
	register int redSum1, greenSum1, blueSum1, redSum2, greenSum2, blueSum2;
	unsigned int i, j;
	// for the even case:
	if (pdim % 2  == 0) {
		for (i = 1; i < pdim - 1; i++) {
			destr = dst + dim*i + 3*sizeof(unsigned char);
			destg = dst + dim*i + 4*sizeof(unsigned char);
			destb = dst + dim*i + 5*sizeof(unsigned char);

			a = src + dim*(i-1);
			b = src + dim*(i);
			c = src + dim*(i+1);
			x = src + dim*(i-1) + 3*sizeof(unsigned char);
			y = src + dim*(i) + 3*sizeof(unsigned char);
			z = src + dim*(i+1) + 3*sizeof(unsigned char);
			u = src + dim*(i-1) + 6*sizeof(unsigned char);
			v = src + dim*(i) + 6*sizeof(unsigned char);
			w = src + dim*(i+1) + 6*sizeof(unsigned char);

			// copy the first pixel to dst
			*(destr - 3) = *b;
			*(destg - 3) = *(b + 1);
			*(destb - 3) = *(b + 2);

			for ( j = 1; j < pdim - 1; j+=2) {
				redSum1 = 0;
				greenSum1 = 0;
				blueSum1 = 0;
				redSum2 = 0;
				greenSum2 = 0;
				blueSum2 = 0;

				redSum1 += (int)*(x) + (int)*(u);
				x++; u++;
				greenSum1 += (int)*(x) + (int)*(u);
				x++; u++;
				blueSum1 += (int)*(x) + (int)*(u);
				x++; u++;
				

				redSum1 += (int)*(y) + (int)*(v);
				y++; v++;
				greenSum1 += (int)*(y) + (int)*(v);
				y++; v++;
				blueSum1 += (int)*(y) + (int)*(v);
				y++, v++;

				redSum1 += (int)*(z) + (int)*(w);
				z++; w++;
				greenSum1 += (int)*(z) + (int)*(w);
				z++; w++;
				blueSum1 += (int)*(z) + (int)*(w);
				z++; w++;
				
				redSum2 += redSum1 + (int)*u + (int)*v + (int)*w;
				u++; v++; w++;
				greenSum2 += greenSum1 + (int)*u + (int)*v + (int)*w;
				u++; v++; w++;
				blueSum2 += blueSum1 + (int)*u + (int)*v + (int)*w;
				u++; v++; w++;
				
				redSum1 += (int)*a + (int)*b + (int)*c;
				a++; b++; c++;
				greenSum1 += (int)*a + (int)*b + (int)*c;
				a++; b++; c++;
				blueSum1 += (int)*a + (int)*b + (int)*c;
				

				a+=4; x+=3; 
				b+=4; y+=3;
				c+=4; z+=3;


				// put the values in the target 'pixel'
				*destr = (unsigned char)(redSum1 / 9);
				*destg = (unsigned char)(greenSum1 / 9);
				*destb = (unsigned char)(blueSum1 / 9);
				// moving on to the next 'pixel'
				destr += 3;
				destg += 3;
				destb += 3;
				// put the values in the target 'pixel'
				*destr = (unsigned char)(redSum2 / 9);
				*destg = (unsigned char)(greenSum2 / 9);
				*destb = (unsigned char)(blueSum2 / 9);
				// moving on to the next 'pixel'
				destr += 3;
				destg += 3;
				destb += 3;
			}
			// copy the last pixel to dst
			*(destr) = *(v - 3);
			*(destg) = *(v - 2);
			*(destb) = *(v - 1);
		}
	} else {
		for (i = 1; i < pdim - 1; i++) {

			destr = dst + dim*i + 3*sizeof(unsigned char);
			destg = dst + dim*i + 4*sizeof(unsigned char);
			destb = dst + dim*i + 5*sizeof(unsigned char);

			a = src + dim*(i-1);
			b = src + dim*(i);
			c = src + dim*(i+1);
			x = src + dim*(i-1) + 3*sizeof(unsigned char);
			y = src + dim*(i) + 3*sizeof(unsigned char);
			z = src + dim*(i+1) + 3*sizeof(unsigned char);
			u = src + dim*(i-1) + 6*sizeof(unsigned char);
			v = src + dim*(i) + 6*sizeof(unsigned char);
			w = src + dim*(i+1) + 6*sizeof(unsigned char);

			// copy the first pixel to dst
			*(destr - 3) = *b;
			*(destg - 3) = *(b + 1);
			*(destb - 3) = *(b + 2);

			for ( j = 1; j < pdim - 1 ; j+=2) {
				redSum1 = 0;
				greenSum1 = 0;
				blueSum1 = 0;
				redSum2 = 0;
				greenSum2 = 0;
				blueSum2 = 0;

				redSum1 += (int)*(x) + (int)*(u);
				x++; u++;
				greenSum1 += (int)*(x) + (int)*(u);
				x++; u++;
				blueSum1 += (int)*(x) + (int)*(u);
				x++; u++;
				

				redSum1 += (int)*(y) + (int)*(v);
				y++; v++;
				greenSum1 += (int)*(y) + (int)*(v);
				y++; v++;
				blueSum1 += (int)*(y) + (int)*(v);
				y++, v++;

				redSum1 += (int)*(z) + (int)*(w);
				z++; w++;
				greenSum1 += (int)*(z) + (int)*(w);
				z++; w++;
				blueSum1 += (int)*(z) + (int)*(w);
				z++; w++;
				
				redSum2 += redSum1 + (int)*u + (int)*v + (int)*w;
				u++; v++; w++;
				greenSum2 += greenSum1 + (int)*u + (int)*v + (int)*w;
				u++; v++; w++;
				blueSum2 += blueSum1 + (int)*u + (int)*v + (int)*w;
				u++; v++; w++;
				
				redSum1 += (int)*a + (int)*b + (int)*c;
				a++; b++; c++;
				greenSum1 += (int)*a + (int)*b + (int)*c;
				a++; b++; c++;
				blueSum1 += (int)*a + (int)*b + (int)*c;
				

				a+=4; x+=3; 
				b+=4; y+=3;
				c+=4; z+=3;


				// put the values in the target 'pixel'
				*destr = (unsigned char)(redSum1 / 9);
				*destg = (unsigned char)(greenSum1 / 9);
				*destb = (unsigned char)(blueSum1 / 9);
				// moving on to the next 'pixel'
				destr += 3*sizeof(unsigned char);
				destg += 3*sizeof(unsigned char);
				destb += 3*sizeof(unsigned char);
				// put the values in the target 'pixel'
				*destr = (unsigned char)(redSum2 / 9);
				*destg = (unsigned char)(greenSum2 / 9);
				*destb = (unsigned char)(blueSum2 / 9);
				// moving on to the next 'pixel'
				destr += 3*sizeof(unsigned char);
				destg += 3*sizeof(unsigned char);
				destb += 3*sizeof(unsigned char);
			}
			/*since wer'e in the odd case, the square {{a,x,u}
													   {b,y,v}
													   {c,z,w}}
			is layed out exactly around the pixel which we want to update (the equivalent of y)
			*/
			// redSum2   -= (int)*(a - 3) + (int)*(b - 3) + (int)*(c - 3);
			// greenSum2 -= (int)*(a - 2) + (int)*(b - 2) + (int)*(c - 2);
			// blueSum2  -= (int)*(a - 1) + (int)*(b - 1) + (int)*(c - 1);
			// redSum2   += (int)*u       + (int)*v       + (int)*w;
			// greenSum2 += (int)*(u + 1) + (int)*(v + 1) + (int)*(w + 1);
			// blueSum2  += (int)*(u + 2) + (int)*(v + 2) + (int)*(w + 2);
			// *(destr - 3) = redSum2;
			// *(destg - 3) = greenSum2;
			// *(destb - 3) = blueSum2;
			// copy the last pixel to dst
			*(destr - 3) = *(v - 6);
			*(destg - 3) = *(v - 5);
			*(destb - 3) = *(v - 4);
		}
	}
}

void sharp(int pdim, unsigned char* src, char* dst) {
	int dim = pdim * 3;
	// copy the first row of pixels to dst
	memcpy(dst, src, dim);
	// compy the last row of pixels to dst
	memcpy(dst + ((pdim - 1)*dim), src + ((pdim - 1)*dim), dim);
	unsigned char* ar = src;
	unsigned char* ag = src + 1;
	unsigned char* ab = src + 2;
	unsigned char* br = src + dim;
	unsigned char* bg = src + dim + 1;
	unsigned char* bb = src + dim + 2;
	unsigned char* cr = src + dim + dim;
	unsigned char* cg = src + dim + dim + 1;
	unsigned char* cb = src + dim + dim + 2;
	unsigned char* xr = src + 3;
	unsigned char* xg = src + 4;
	unsigned char* xb = src + 5;
	unsigned char* yr = src + dim + 3;
	unsigned char* yg = src + dim + 4;
	unsigned char* yb = src + dim + 5;
	unsigned char* zr = src + dim + dim + 3;
	unsigned char* zg = src + dim + dim + 4;
	unsigned char* zb = src + dim + dim + 5;
	
	unsigned char* ur = src + 6;
	unsigned char* ug = src + 7;
	unsigned char* ub = src + 8;
	unsigned char* vr = src + dim + 6;
	unsigned char* vg = src + dim + 7;
	unsigned char* vb = src + dim + 8;
	unsigned char* wr = src + dim + dim + 6;
	unsigned char* wg = src + dim + dim + 7;
	unsigned char* wb = src + dim + dim + 8;
	char* destr = dst + dim + 3;
	char* destg = dst + dim + 4;
	char* destb = dst + dim + 5;
	
	register int redSum, greenSum, blueSum;
	unsigned int i, j;
	for (i = 1; i < pdim - 1; i++) {
		destr = dst + dim*i + 3*sizeof(unsigned char);
		destg = dst + dim*i + 4*sizeof(unsigned char);
		destb = dst + dim*i + 5*sizeof(unsigned char);
		ar = src + dim*(i-1);
		ag = src + dim*(i-1) + 1*sizeof(unsigned char);
		ab = src + dim*(i-1) + 2*sizeof(unsigned char);
		br = src + dim*(i);
		bg = src + dim*(i) + 1*sizeof(unsigned char);
		bb = src + dim*(i) + 2*sizeof(unsigned char);
		cr = src + dim*(i+1);
		cg = src + dim*(i+1) + 1*sizeof(unsigned char);
		cb = src + dim*(i+1) + 2*sizeof(unsigned char);
		xr = src + dim*(i-1) + 3*sizeof(unsigned char);
		xg = src + dim*(i-1) + 4*sizeof(unsigned char);
		xb = src + dim*(i-1) + 5*sizeof(unsigned char);
		yr = src + dim*(i) + 3*sizeof(unsigned char);
		yg = src + dim*(i) + 4*sizeof(unsigned char);
		yb = src + dim*(i) + 5*sizeof(unsigned char);
		zr = src + dim*(i+1) + 3*sizeof(unsigned char);
		zg = src + dim*(i+1) + 4*sizeof(unsigned char);
		zb = src + dim*(i+1) + 5*sizeof(unsigned char);
		ur = src + dim*(i-1) + 6*sizeof(unsigned char);
		ug = src + dim*(i-1) + 7*sizeof(unsigned char);
		ub = src + dim*(i-1) + 8*sizeof(unsigned char);
		vr = src + dim*(i) + 6*sizeof(unsigned char);
		vg = src + dim*(i) + 7*sizeof(unsigned char);
		vb = src + dim*(i) + 8*sizeof(unsigned char);
		wr = src + dim*(i+1) + 6*sizeof(unsigned char);
		wg = src + dim*(i+1) + 7*sizeof(unsigned char);
		wb = src + dim*(i+1) + 8*sizeof(unsigned char);
		// copy the first pixel to dst
		*(destr - 3) = *br;
		*(destg - 3) = *bg;
		*(destb - 3) = *bb;
		for ( j = 1; j < pdim - 1; j++) {
			redSum = 0;
			greenSum = 0;
			blueSum = 0;
			// the equivalent of applykernel multiplication
			redSum   -= (int)*ar + (int)*xr + (int)*ur;
			greenSum -= (int)*ag + (int)*xg + (int)*ug;
			blueSum  -= (int)*ab + (int)*xb + (int)*ub;
			redSum   -= (int)*br - MULT9((int)*yr) + (int)*vr;
			greenSum -= (int)*bg - MULT9((int)*yg) + (int)*vg;
			blueSum  -= (int)*bb - MULT9((int)*yb) + (int)*vb;
			redSum   -= (int)*cr + (int)*zr + (int)*wr;
			greenSum -= (int)*cg + (int)*zg + (int)*wg;
			blueSum  -= (int)*cb + (int)*zb + (int)*wb;
			
			// put the values in the target 'pixel'
			*destr = (unsigned char)(MIN(MAX(redSum, 0), 255));
			*destg = (unsigned char)(MIN(MAX(greenSum, 0), 255));
			*destb = (unsigned char)(MIN(MAX(blueSum, 0), 255));
			// moving on to the next 'pixel'
			destr += 3*sizeof(unsigned char);
			destg += 3*sizeof(unsigned char);
			destb += 3*sizeof(unsigned char);
			ar += 3*sizeof(unsigned char);
			ag += 3*sizeof(unsigned char);
			ab += 3*sizeof(unsigned char);
			br += 3*sizeof(unsigned char);
			bg += 3*sizeof(unsigned char);
			bb += 3*sizeof(unsigned char);
			cr += 3*sizeof(unsigned char);
			cg += 3*sizeof(unsigned char);
			cb += 3*sizeof(unsigned char);
			xr += 3*sizeof(unsigned char);
			xg += 3*sizeof(unsigned char);
			xb += 3*sizeof(unsigned char);
			yr += 3*sizeof(unsigned char);
			yg += 3*sizeof(unsigned char);
			yb += 3*sizeof(unsigned char);
			zr += 3*sizeof(unsigned char);
			zg += 3*sizeof(unsigned char);
			zb += 3*sizeof(unsigned char);
			ur += 3*sizeof(unsigned char);
			ug += 3*sizeof(unsigned char);
			ub += 3*sizeof(unsigned char);
			vr += 3*sizeof(unsigned char);
			vg += 3*sizeof(unsigned char);
			vb += 3*sizeof(unsigned char);
			wr += 3*sizeof(unsigned char);
			wg += 3*sizeof(unsigned char);
			wb += 3*sizeof(unsigned char);
		}
		// copy the last pixel to dst
		*(destr) = *(vr - 3);
		*(destg) = *(vg - 3);
		*(destb) = *(vb - 3);
	}
}


void sharp_less_vars(int pdim, unsigned char* src, char* dst) {
	int dim = pdim * 3;
	// copy the first row of pixels to dst
	memcpy(dst, src, dim);
	// copy the last row of pixels to dst
	memcpy(dst + ((pdim - 1)*dim), src + ((pdim - 1)*dim), dim);
	register unsigned char* a = src;
	register unsigned char* b = src + dim;
	register unsigned char* c = src + dim + dim;
	register unsigned char* x = src + 3;
	register unsigned char* y = src + dim + 3;
	register unsigned char* z = src + dim + dim + 3;
	register unsigned char* u = src + 6;
	register unsigned char* v = src + dim + 6;
	register unsigned char* w = src + dim + dim + 6;
	char* destr = dst + dim + 3;
	char* destg = dst + dim + 4;
	char* destb = dst + dim + 5;
	register int redSum, greenSum, blueSum;
	unsigned int i, j;
	for (i = 1; i < pdim - 1; i++) {
		// copy the first pixel to dst
		*(destr - 3) = *b;
		*(destg - 3) = *(b + 1);
		*(destb - 3) = *(b + 2);
		
		for ( j = 1; j < pdim - 1; j++) {
			redSum = 0;
			greenSum = 0;
			blueSum = 0;

			redSum -=   (int)*(a) + (int)*(x) + (int)*(u) + (int)*(b) 
			          + (int)*(v) + (int)*(c) + (int)*(z) + (int)*(w);
			
			redSum += MULT9((int)*(y));
			a++; x++; u++;
			b++; y++; v++;
			c++; z++; w++;

			greenSum -=   (int)*(a) + (int)*(x) + (int)*(u) + (int)*(b) 
			            + (int)*(v) + (int)*(c) + (int)*(z) + (int)*(w);
			
			greenSum += MULT9((int)*(y));
			a++; x++; u++;
			b++; y++; v++;
			c++; z++; w++;

			blueSum -=   (int)*(a) + (int)*(x) + (int)*(u) + (int)*(b) 
			           + (int)*(v) + (int)*(c) + (int)*(z) + (int)*(w);
			
			blueSum += MULT9((int)*(y));
			a++; x++; u++;
			b++; y++; v++;
			c++; z++; w++;
		
			// put the values in the target 'pixel'
			// truncate the pixel values [0,255]
			if (0 != (redSum & 0b10000000000000000000000000000000)) {
				*destr = (unsigned char) 0;
			} 
			else if (0 != (redSum & 0b11111111111111111111111100000000)) {
				*destr = (unsigned char)255;
			} else {
				*destr = redSum;
			}
			if (0 != (greenSum & 0b10000000000000000000000000000000)) {
				*destg = (unsigned char)0;
			} 
			else if (0 != (greenSum & 0b11111111111111111111111100000000)) {
				*destg = (unsigned char)255;
			} else {
				*destg = greenSum;
			}
			if (0 != (blueSum & 0b10000000000000000000000000000000)) {
				*destb = (unsigned char)0;
			} 
			else if (0 != (blueSum & 0b11111111111111111111111100000000)) {
				*destb = (unsigned char)255;
			} else {
				*destb = blueSum;
			}
			
			// moving on to the next 'pixel'
			destr += 3;
			destg += 3;
			destb += 3;
		}
		// copy the last pixel to dst

		*(destr) = *(y);
		*(destg) = *(y + 1);
		*(destb) = *(y + 2);

		a+=6; x+=6; u+=6;
		b+=6; y+=6; v+=6;
		c+=6; z+=6; w+=6;
		destr +=6; destg+=6; destb+=6;
	}
}

void sharp_unrolled(int pdim, unsigned char* src, char* dst) {
	int dim = pdim * 3;
	// copy the first row of pixels to dst
	memcpy(dst, src, dim);
	// copy the last row of pixels to dst
	memcpy(dst + ((pdim - 1)*dim), src + ((pdim - 1)*dim), dim);
	register unsigned char* a = src;
	register unsigned char* b = src + dim;
	register unsigned char* c = src + dim + dim;
	register unsigned char* x = src + 3;
	register unsigned char* y = src + dim + 3;
	register unsigned char* z = src + dim + dim + 3;
	register unsigned char* u = src + 6;
	register unsigned char* v = src + dim + 6;
	register unsigned char* w = src + dim + dim + 6;
	char* destr = dst + dim + 3;
	char* destg = dst + dim + 4;
	char* destb = dst + dim + 5;
	register int redSum1, greenSum1, blueSum1, redSum2, greenSum2, blueSum2;
	unsigned int i, j;
	for (i = 1; i < pdim - 1; i++) {

		destr = dst + dim*i + 3*sizeof(unsigned char);
		destg = dst + dim*i + 4*sizeof(unsigned char);
		destb = dst + dim*i + 5*sizeof(unsigned char);

		a = src + dim*(i-1);
		b = src + dim*(i);
		c = src + dim*(i+1);
		x = src + dim*(i-1) + 3*sizeof(unsigned char);
		y = src + dim*(i) + 3*sizeof(unsigned char);
		z = src + dim*(i+1) + 3*sizeof(unsigned char);
		u = src + dim*(i-1) + 6*sizeof(unsigned char);
		v = src + dim*(i) + 6*sizeof(unsigned char);
		w = src + dim*(i+1) + 6*sizeof(unsigned char);
		// copy the first pixel to dst
		*(destr - 3) = *b;
		*(destg - 3) = *(b + 1);
		*(destb - 3) = *(b + 2);
		
		for ( j = 1; j < pdim - 1; j++) {
			redSum1 = 0;
			greenSum1 = 0;
			blueSum1 = 0;
			redSum2 = 0;
			greenSum2 = 0;
			blueSum2 = 0;

			redSum2   -= (int)*(x) + (int)*(u);
			x++; u++;
			greenSum2 -= (int)*(x) + (int)*(u);
			x++; u++;
			blueSum2  -= (int)*(x) + (int)*(u);
			x++; u++;

			redSum2   += -1*(int)*(y) + 9*(int)*(v);
			y++; v++;
			greenSum2 += -1*(int)*(y) + 9*(int)*(v);
			y++; v++;
			blueSum2  += -1*(int)*(y) + 9*(int)*(v);
			y++, v++;

			redSum2   -= (int)*(z) + (int)*(w);
			z++; w++;
			greenSum2 -= (int)*(z) + (int)*(w);
			z++; w++;
			blueSum2  -= (int)*(z) + (int)*(w);
			z++; w++;

			redSum1 -= (int)*(a) + (int)*(b) + (int)*(c);
			a++; b++; c++;
			greenSum1 -= (int)*(a) + (int)*(b) + (int)*(c);
			a++; b++; c++;
			blueSum1 -= (int)*(a) + (int)*(b) + (int)*(c);
 

			redSum1 += redSum2 + 10*(*(b+1)) -10*(*(y));
			greenSum1 += greenSum2 + 10*(*(b+2)) -10*(*(y+1));
			blueSum1 += blueSum2 + 10*(*(b+3)) -10*(*(y+2));

			redSum2 -= (int)*u + (int)*v + (int)*w;
			u++; v++; w++;
			greenSum2 -= (int)*u + (int)*v + (int)*w;
			u++; v++; w++;
			blueSum2 -= (int)*u + (int)*v + (int)*w;
			u++; v++; w++;

			a+=4; x+=3; 
			b+=4; y+=3;
			c+=4; z+=3;

			// the equivalent of applykernel multiplication
			// redSum   -= (int)*ar + (int)*xr + (int)*ur;
			// greenSum -= (int)*ag + (int)*xg + (int)*ug;
			// blueSum  -= (int)*ab + (int)*xb + (int)*ub;

			// redSum   -= (int)*br - 9*(int)*yr + (int)*vr;
			// greenSum -= (int)*bg - 9*(int)*yg + (int)*vg;
			// blueSum  -= (int)*bb - 9*(int)*yb + (int)*vb;

			// redSum   -= (int)*cr + (int)*zr + (int)*wr;
			// greenSum -= (int)*cg + (int)*zg + (int)*wg;
			// blueSum  -= (int)*cb + (int)*zb + (int)*wb;
			

			// put the values in the target 'pixel'
			*destr = (unsigned char)(MIN(MAX(redSum1, 0), 255));
			*destg = (unsigned char)(MIN(MAX(greenSum1, 0), 255));
			*destb = (unsigned char)(MIN(MAX(blueSum1, 0), 255));
			// moving on to the next 'pixel'
			destr += 3*sizeof(unsigned char);
			destg += 3*sizeof(unsigned char);
			destb += 3*sizeof(unsigned char);
			// put the values in the target 'pixel'
			*destr = (unsigned char)(MIN(MAX(redSum2, 0), 255));
			*destg = (unsigned char)(MIN(MAX(greenSum2, 0), 255));
			*destb = (unsigned char)(MIN(MAX(blueSum2, 0), 255));
			// moving on to the next 'pixel'
			destr += 3*sizeof(unsigned char);
			destg += 3*sizeof(unsigned char);
			destb += 3*sizeof(unsigned char);
			// ar += 3*sizeof(unsigned char);
			// ag += 3*sizeof(unsigned char);
			// ab += 3*sizeof(unsigned char);
			// br += 3*sizeof(unsigned char);
			// bg += 3*sizeof(unsigned char);
			// bb += 3*sizeof(unsigned char);
			// cr += 3*sizeof(unsigned char);
			// cg += 3*sizeof(unsigned char);
			// cb += 3*sizeof(unsigned char);
			// xr += 3*sizeof(unsigned char);
			// xg += 3*sizeof(unsigned char);
			// xb += 3*sizeof(unsigned char);
			// yr += 3*sizeof(unsigned char);
			// yg += 3*sizeof(unsigned char);
			// yb += 3*sizeof(unsigned char);
			// zr += 3*sizeof(unsigned char);
			// zg += 3*sizeof(unsigned char);
			// zb += 3*sizeof(unsigned char);
			// ur += 3*sizeof(unsigned char);
			// ug += 3*sizeof(unsigned char);
			// ub += 3*sizeof(unsigned char);
			// vr += 3*sizeof(unsigned char);
			// vg += 3*sizeof(unsigned char);
			// vb += 3*sizeof(unsigned char);
			// wr += 3*sizeof(unsigned char);
			// wg += 3*sizeof(unsigned char);
			// wb += 3*sizeof(unsigned char);
		}
		// copy the last pixel to dst
		*(destr) = *(v - 3);
		*(destg) = *(v - 2);
		*(destb) = *(v - 1);
	}
}

void filterChars(int pdim, unsigned char *src, char *dst) {
	int dim = pdim * 3;
	// copy the first row of pixels to dst
	memcpy(dst, src, dim);
	// compy the last row of pixels to dst
	memcpy(dst + ((pdim - 1)*dim), src + ((pdim - 1)*dim), dim);
	
	unsigned char* ar = src;
	unsigned char* ag = src + 1;
	unsigned char* ab = src + 2;

	unsigned char* br = src + dim;
	unsigned char* bg = src + dim + 1;
	unsigned char* bb = src + dim + 2;

	unsigned char* cr = src + dim + dim;
	unsigned char* cg = src + dim + dim + 1;
	unsigned char* cb = src + dim + dim + 2;

	unsigned char* xr = src + 3;
	unsigned char* xg = src + 4;
	unsigned char* xb = src + 5;

	unsigned char* yr = src + dim + 3;
	unsigned char* yg = src + dim + 4;
	unsigned char* yb = src + dim + 5;

	unsigned char* zr = src + dim + dim + 3;
	unsigned char* zg = src + dim + dim + 4;
	unsigned char* zb = src + dim + dim + 5;
	
	unsigned char* ur = src + 6;
	unsigned char* ug = src + 7;
	unsigned char* ub = src + 8;

	unsigned char* vr = src + dim + 6;
	unsigned char* vg = src + dim + 7;
	unsigned char* vb = src + dim + 8;

	unsigned char* wr = src + dim + dim + 6;
	unsigned char* wg = src + dim + dim + 7;
	unsigned char* wb = src + dim + dim + 8;

	char* destr = dst + dim + 3;
	char* destg = dst + dim + 4;
	char* destb = dst + dim + 5;
	
	register int redSum, greenSum, blueSum;
	unsigned char *maxPixel, *minPixel;
	register int sumL, sumM, sumR, min, max;
	unsigned int i, j;
	for (i = 1; i < pdim - 1; i++) {

		destr = dst + dim*i + 3*sizeof(unsigned char);
		destg = dst + dim*i + 4*sizeof(unsigned char);
		destb = dst + dim*i + 5*sizeof(unsigned char);

		ar = src + dim*(i-1);
		ag = src + dim*(i-1) + 1*sizeof(unsigned char);
		ab = src + dim*(i-1) + 2*sizeof(unsigned char);
		br = src + dim*(i);
		bg = src + dim*(i) + 1*sizeof(unsigned char);
		bb = src + dim*(i) + 2*sizeof(unsigned char);
		cr = src + dim*(i+1);
		cg = src + dim*(i+1) + 1*sizeof(unsigned char);
		cb = src + dim*(i+1) + 2*sizeof(unsigned char);
		xr = src + dim*(i-1) + 3*sizeof(unsigned char);
		xg = src + dim*(i-1) + 4*sizeof(unsigned char);
		xb = src + dim*(i-1) + 5*sizeof(unsigned char);
		yr = src + dim*(i) + 3*sizeof(unsigned char);
		yg = src + dim*(i) + 4*sizeof(unsigned char);
		yb = src + dim*(i) + 5*sizeof(unsigned char);
		zr = src + dim*(i+1) + 3*sizeof(unsigned char);
		zg = src + dim*(i+1) + 4*sizeof(unsigned char);
		zb = src + dim*(i+1) + 5*sizeof(unsigned char);
		ur = src + dim*(i-1) + 6*sizeof(unsigned char);
		ug = src + dim*(i-1) + 7*sizeof(unsigned char);
		ub = src + dim*(i-1) + 8*sizeof(unsigned char);
		vr = src + dim*(i) + 6*sizeof(unsigned char);
		vg = src + dim*(i) + 7*sizeof(unsigned char);
		vb = src + dim*(i) + 8*sizeof(unsigned char);
		wr = src + dim*(i+1) + 6*sizeof(unsigned char);
		wg = src + dim*(i+1) + 7*sizeof(unsigned char);
		wb = src + dim*(i+1) + 8*sizeof(unsigned char);

		// copy the first pixel to dst
		*(destr - 3) = *br;
		*(destg - 3) = *bg;
		*(destb - 3) = *bb;

		for ( j = 1; j < pdim - 1; j++) {
			redSum = 0;
			greenSum = 0;
			blueSum = 0;
			min = 766;
			max = 0;

			// the equivalent of applykernel multiplication
			redSum   += (int)*ar + (int)*xr + (int)*ur;
			greenSum += (int)*ag + (int)*xg + (int)*ug;
			blueSum  += (int)*ab + (int)*xb + (int)*ub;
			sumL = (int)*ar + (int)*ag + (int)*ab;
			sumM = (int)*xr + (int)*xg + (int)*xb;
			sumR = (int)*ur + (int)*ug + (int)*ub;
			
			if (sumL <= min) {
				min = sumL;
				minPixel = ar; 
			}
			if (sumL > max) {
				max = sumL;
				maxPixel = ar;
			}
			if (sumM <= min) {
				min = sumM;
				minPixel = xr; 
			}
			if (sumM > max) {
				max = sumM;
				maxPixel = xr;
			}
			if (sumR <= min) {
				min = sumR;
				minPixel = ur; 
			}
			if (sumR > max) {
				max = sumR;
				maxPixel = ur;
			}
			
			
			
			// find min and max pixel
			/*
			if (sumL >= sumM) {
				if (sumL >= sumR) {
					max = sumL;
					maxPixel = ar;
					if (sumM < sumR) {
						min = sumM;
						minPixel = xr;
					} else {
						min = sumR;
						minPixel = ur;
					}
				} else {
					max = sumR;
					maxPixel = ur;
					min = sumM;
					minPixel = xr;
				}
			} else {
				if (sumM >= sumR) {
					max = sumM;
					maxPixel = xr;
					if (sumL < sumR) {
						min = sumL;
						minPixel = ar;
					} else {
						min = sumR;
						minPixel = ur;
					}					
				} else {
					max = sumR;
					maxPixel = ur;
					min = sumL;
					minPixel = ar;
				}				
			}	
			*/

			redSum   += (int)*br + (int)*yr + (int)*vr;
			greenSum += (int)*bg + (int)*yg + (int)*vg;
			blueSum  += (int)*bb + (int)*yb + (int)*vb;
			sumL = (int)*br + (int)*bg + (int)*bb;
			sumM = (int)*yr + (int)*yg + (int)*yb;
			sumR = (int)*vr + (int)*vg + (int)*vb;
			
			if (sumL <= min) {
				min = sumL;
				minPixel = br; 
			}
			if (sumL > max) {
				max = sumL;
				maxPixel = br;
			}if (sumM <= min) {
				min = sumM;
				minPixel = yr; 
			}
			if (sumM > max) {
				max = sumM;
				maxPixel = yr;
			}if (sumR <= min) {
				min = sumR;
				minPixel = vr; 
			}
			if (sumR > max) {
				max = sumR;
				maxPixel = vr;
			}
			
			/*
			if (sumL >= sumM) {
				if (sumL >= sumR) {
					if (sumL > max) {
						max = sumL;
					    maxPixel = br;
					}
					if (sumM < sumR) {
						if (sumM <= min) {
							min = sumM;
							minPixel = yr;
						}
					} else {
						if (sumR <= min) {
							min = sumR;
							minPixel = vr;
						}
					}
				} else {
					if (sumR > max) {
						max = sumR;
						maxPixel = vr;
					}
					if (sumM <= min) {
						min = sumM;
						minPixel = yr;
					}
				}
			} else {
				if (sumM >= sumR) {
					if (sumM > max) {
						max = sumM;
						maxPixel = yr;
					}
					if (sumL < sumR) {
						if (sumL <= min) {
							min = sumL;
							minPixel = br;
						}
					} else {
						if (sumR <= min) {
							min = sumR;
							minPixel = vr;
						}
					}					
				} else {
					if (sumR > max) {
						max = sumR;
						maxPixel = vr;
					}
					if (sumL <= min) {
						min = sumL;
						minPixel = br;
					}
				}				
			}
			*/
			
			redSum   += (int)*cr + (int)*zr + (int)*wr;
			greenSum += (int)*cg + (int)*zg + (int)*wg;
			blueSum  += (int)*cb + (int)*zb + (int)*wb;
			sumL = (int)*cr + (int)*cg + (int)*cb;
			sumM = (int)*zr + (int)*zg + (int)*zb;
			sumR = (int)*wr + (int)*wg + (int)*wb;
			
			if (sumL <= min) {
				min = sumL;
				minPixel = cr; 
			}
			if (sumL > max) {
				max = sumL;
				maxPixel = cr;
			}if (sumM <= min) {
				min = sumM;
				minPixel = zr; 
			}
			if (sumM > max) {
				max = sumM;
				maxPixel = zr;
			}if (sumR <= min) {
				min = sumR;
				minPixel = wr; 
			}
			if (sumR > max) {
				max = sumR;
				maxPixel = wr;
			}
			/*
			
			if (sumL >= sumM) {
				if (sumL >= sumR) {
					if (sumL > max) {
						max = sumL;
					    maxPixel = cr;
					}
					if (sumM < sumR) {
						if (sumM <= min) {
							min = sumM;
							minPixel = zr;
						}
					} else {
						if (sumR <= min) {
							min = sumR;
							minPixel = wr;
						}
					}
				} else {
					if (sumR > max) {
						max = sumR;
						maxPixel = wr;
					}
					if (sumM <= min) {
						min = sumM;
						minPixel = zr;
					}
				}
			} else {
				if (sumM >= sumR) {
					if (sumM > max) {
						max = sumM;
						maxPixel = zr;
					}
					if (sumL < sumR) {
						if (sumL <= min) {
							min = sumL;
							minPixel = cr;
						}
					} else {
						if (sumR <= min) {
							min = sumR;
							minPixel = wr;
						}
					}					
				} else {
					if (sumR > max) {
						max = sumR;
						maxPixel = wr;
					}
					if (sumL <= min) {
						min = sumL;
						minPixel = cr;
					}
				}				
			}
			*/
			// subtract the value of max and min pixels
			redSum   -= (int)*(maxPixel++)   + (int)*(minPixel++);
			greenSum -= (int)*(maxPixel++)   + (int)*(minPixel++);
			blueSum  -= (int)*(maxPixel)     + (int)*(minPixel);

			// put the values in the target 'pixel'
			redSum = (redSum / 7);
			greenSum = (greenSum / 7);
			blueSum = (blueSum / 7);
			*destr = (unsigned char)(MIN(MAX(redSum, 0), 255));
			*destg = (unsigned char)(MIN(MAX(greenSum, 0), 255));
			*destb = (unsigned char)(MIN(MAX(blueSum, 0), 255));

			// moving on to the next 'pixel'
			destr += 3*sizeof(unsigned char);
			destg += 3*sizeof(unsigned char);
			destb += 3*sizeof(unsigned char);
			ar += 3*sizeof(unsigned char);
			ag += 3*sizeof(unsigned char);
			ab += 3*sizeof(unsigned char);
			br += 3*sizeof(unsigned char);
			bg += 3*sizeof(unsigned char);
			bb += 3*sizeof(unsigned char);
			cr += 3*sizeof(unsigned char);
			cg += 3*sizeof(unsigned char);
			cb += 3*sizeof(unsigned char);
			xr += 3*sizeof(unsigned char);
			xg += 3*sizeof(unsigned char);
			xb += 3*sizeof(unsigned char);
			yr += 3*sizeof(unsigned char);
			yg += 3*sizeof(unsigned char);
			yb += 3*sizeof(unsigned char);
			zr += 3*sizeof(unsigned char);
			zg += 3*sizeof(unsigned char);
			zb += 3*sizeof(unsigned char);
			ur += 3*sizeof(unsigned char);
			ug += 3*sizeof(unsigned char);
			ub += 3*sizeof(unsigned char);
			vr += 3*sizeof(unsigned char);
			vg += 3*sizeof(unsigned char);
			vb += 3*sizeof(unsigned char);
			wr += 3*sizeof(unsigned char);
			wg += 3*sizeof(unsigned char);
			wb += 3*sizeof(unsigned char);
		}
		// copy the last pixel to dst
		*(destr) = *(vr - 3);
		*(destg) = *(vg - 3);
		*(destb) = *(vb - 3);
	}
}

void filterChars_less_vars(int pdim, unsigned char *src, char *dst) {
	int dim = pdim * 3;
	// copy the first row of pixels to dst
	memcpy(dst, src, dim);
	// copy the last row of pixels to dst
	memcpy(dst + ((pdim - 1)*dim), src + ((pdim - 1)*dim), dim);
	register unsigned char* a = src;
	register unsigned char* b = src + dim;
	register unsigned char* c = src + dim + dim;
	register unsigned char* x = src + 3;
	register unsigned char* y = src + dim + 3;
	register unsigned char* z = src + dim + dim + 3;
	register unsigned char* u = src + 6;
	register unsigned char* v = src + dim + 6;
	register unsigned char* w = src + dim + dim + 6;
	char* destr = dst + dim + 3;
	char* destg = dst + dim + 4;
	char* destb = dst + dim + 5;
	register int redSum, greenSum, blueSum;
	unsigned char *maxPixel, *minPixel;
	register int sumL, sumM, sumR, min, max;
	unsigned int i, j;
	for (i = 1; i < pdim - 1; i++) {

		// copy the first pixel to dst
		*(destr - 3) = *b;
		*(destg - 3) = *(b + 1);
		*(destb - 3) = *(b + 2);

		for ( j = 1; j < pdim - 1; j++) {
			redSum = 0;
			greenSum = 0;
			blueSum = 0;
			min = 766;
			max = -1;

			// the equivalent of applykernel multiplication
			redSum   += (int)*a + (int)*x + (int)*u;
			// calculate intensity
			sumL = (int)*a;
			sumM = (int)*x;
			sumR = (int)*u;
			// move to green
			a++; x++; u++;
			greenSum   += (int)*a + (int)*x + (int)*u;
			// calculate intensity
			sumL += (int)*a;
			sumM += (int)*x;
			sumR += (int)*u;
			// move to blue
			a++; x++; u++;
			blueSum   += (int)*a + (int)*x + (int)*u;
			// calculate intensity
			sumL += (int)*a;
			sumM += (int)*x;
			sumR += (int)*u;
			a++; x++; u++;
			
			if (sumL <= min) {
				min = sumL;
				minPixel = a - 3; 
			}
			if (sumL > max) {
				max = sumL;
				maxPixel = a - 3;
			}
			if (sumM <= min) {
				min = sumM;
				minPixel = a; // x - 3
			}
			if (sumM > max) {
				max = sumM;
				maxPixel = a; // x - 3
			}
			if (sumR <= min) {
				min = sumR;
				minPixel = x; // u - 3
			}
			if (sumR > max) {
				max = sumR;
				maxPixel = x; // u - 3
			}
			

			redSum   += (int)*b + (int)*y + (int)*v;
			// calculate intensity
			sumL = (int)*b;
			sumM = (int)*y;
			sumR = (int)*v;
			// move to green
			b++; y++; v++;
			greenSum   += (int)*b + (int)*y + (int)*v;
			// calculate intensity
			sumL += (int)*b;
			sumM += (int)*y;
			sumR += (int)*v;
			// move to blue
			b++; y++; v++;
			blueSum   += (int)*b + (int)*y + (int)*v;
			// calculate intensity
			sumL += (int)*b;
			sumM += (int)*y;
			sumR += (int)*v;
			b++; y++; v++;
			
			if (sumL <= min) {
				min = sumL;
				minPixel = b - 3; 
			}
			if (sumL > max) {
				max = sumL;
				maxPixel = b - 3;
			}if (sumM <= min) {
				min = sumM;
				minPixel = b; // y - 3
			}
			if (sumM > max) {
				max = sumM;
				maxPixel = b; // y - 3
			}if (sumR <= min) {
				min = sumR;
				minPixel = y; // v - 3
			}
			if (sumR > max) {
				max = sumR;
				maxPixel = y; // v - 3
			}
			
			
			redSum  += (int)*c + (int)*z + (int)*w;
			// calculate intensity
			sumL = (int)*c;
			sumM = (int)*z;
			sumR = (int)*w;
			// move to green
			c++; z++; w++;
			greenSum  += (int)*c + (int)*z + (int)*w;
			// calculate intensity
			sumL += (int)*c;
			sumM += (int)*z;
			sumR += (int)*w;
			// move to blue
			c++; z++; w++;
			blueSum  += (int)*c + (int)*z + (int)*w;
			// calculate intensity
			sumL += (int)*c;
			sumM += (int)*z;
			sumR += (int)*w;
			c++; z++; w++;
			
			if (sumL <= min) {
				min = sumL;
				minPixel = c - 3; 
			}
			if (sumL > max) {
				max = sumL;
				maxPixel = c - 3;
			}if (sumM <= min) {
				min = sumM;
				minPixel = c; // z - 3
			}
			if (sumM > max) {
				max = sumM;
				maxPixel = c; // z - 3
			}if (sumR <= min) {
				min = sumR;
				minPixel =  z; // w - 3
			}if (sumR > max) {
				max = sumR;
				maxPixel = z; // w - 3
			}
			
			// subtract the value of max and min pixels
			redSum   -= (int)*(maxPixel)     + (int)*(minPixel);
			greenSum -= (int)*(maxPixel + 1) + (int)*(minPixel + 1);
			blueSum  -= (int)*(maxPixel + 2) + (int)*(minPixel + 2);

			// put the values in the target 'pixel'
			redSum = (redSum / 7);
			greenSum = (greenSum / 7);
			blueSum = (blueSum / 7);

			// truncate the pixel values [0,255]
			if (0 != (redSum & 0b10000000000000000000000000000000)) {
				*destr = (unsigned char) 0;
			} 
			else if (0 != (redSum & 0b11111111111111111111111100000000)) {
				*destr = (unsigned char)255;
			} else {
				*destr = redSum;
			}
			if (0 != (greenSum & 0b10000000000000000000000000000000)) {
				*destg = (unsigned char)0;
			} 
			else if (0 != (greenSum & 0b11111111111111111111111100000000)) {
				*destg = (unsigned char)255;
			} else {
				*destg = greenSum;
			}
			if (0 != (blueSum & 0b10000000000000000000000000000000)) {
				*destb = (unsigned char)0;
			} 
			else if (0 != (blueSum & 0b11111111111111111111111100000000)) {
				*destb = (unsigned char)255;
			} else {
				*destb = blueSum;
			}

			// moving on to the next 'pixel'
			destr += 3;
			destg += 3;
			destb += 3;
		}
		// copy the last pixel to dst
		*(destr) = *(y);
		*(destg) = *(y + 1);
		*(destb) = *(y +2);

		a+=6; x+=6; u+=6;
		b+=6; y+=6; v+=6;
		c+=6; z+=6; w+=6;
		destr +=6; destg+=6; destb+=6;
	}
 
}


void doConvolution1(Image *image) {

	char* newImage = (char*)malloc(3*m*n*sizeof(unsigned char));

	smoothNoFilter(m, image->data, newImage);
	image->data = newImage;
}

void doConvolution2(Image *image) {

	char* newImage = (unsigned char*)malloc(3*m*n*sizeof(unsigned char));
	
	sharp_less_vars(m, image->data, newImage);
	image->data = newImage;
}

void doConvolution3(Image *image) {

	char* newImage = (unsigned char*)malloc(3*m*n*sizeof(unsigned char));
	
	filterChars_less_vars(m, image->data, newImage);
	image->data = newImage;
}

void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

	if (flag == '1') {	
		// blur image
		doConvolution1(image);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		doConvolution2(image);
		
		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);	
	} else {
		// apply extermum filtered kernel to blur image
		doConvolution3(image);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		doConvolution2(image);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}