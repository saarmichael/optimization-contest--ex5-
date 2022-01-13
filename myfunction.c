#include <stdbool.h> 
#include <string.h>

#define MIN(x,y) ((x<y)?x:y)
#define MAX(x,y) ((x>y)?x:y)
#define MULT9(x) ((x << 3) + x)

/*
	the optimizations in this function are as follows:
	1. LOOP UNROLLING: calculating and updating two pixels each iteration
	2. SPATIAL LOCALITY: calculations mostly applied over closly located values (ex. a+x+u which are in the same row)
	3. TEMPORAL LOCALITY: using same variables closly over time 	
*/
void smoothNoFilter(unsigned int pdim, unsigned char *src, char *dst) {

	int dim = pdim * 3;
	// copy the first row of pixels to dst
	memcpy(dst, src, dim);
	// copy the last row of pixels to dst
	memcpy(dst + ((pdim - 1)*dim), src + ((pdim - 1)*dim), dim);
	int range = pdim - 1;
	/*
	a , x , u
	b , y , v
	c , z , w
	*/
	register unsigned char* a = src;
	register unsigned char* x = src + 3;
	register unsigned char* u = src + 6;
	register unsigned char* b = src + dim;
	register unsigned char* y = src + dim + 3;
	register unsigned char* v = src + dim + 6;
	register unsigned char* c = src + dim + dim;
	register unsigned char* z = src + dim + dim + 3;
	register unsigned char* w = src + dim + dim + 6;
	char* destr = dst + dim;
	char* destg = dst + dim+1;
	char* destb = dst + dim+2;
	register int redSum1, greenSum1, blueSum1;
	unsigned int i, j;
	// for the even case:
	if (pdim % 2  == 0) {
		for (i = 1; i < pdim - 1; i++) {
			// copy the first pixel in the row
			*(destr) = *b;
			*(destg) = *(b + 1);
			*(destb) = *(b + 2);
			// move to the second pixel in the row and start iterating through the row
			destr+=3;
			destg+=3;
			destb+=3;
			for (j = 1; j < pdim - 1; j+=2) {
				redSum1 = 0;
				greenSum1 = 0;
				blueSum1 = 0;
				// sum the middle vectors (xyz),(uvw)
				redSum1   += (int)*(x)   + (int)*(u);
				greenSum1 += (int)*(x+1) + (int)*(u+1);
				blueSum1  += (int)*(x+2) + (int)*(u+2);

				redSum1   += (int)*(y)   + (int)*(v);
				greenSum1 += (int)*(y+1) + (int)*(v+1);
				blueSum1  += (int)*(y+2) + (int)*(v+2);

				redSum1   += (int)*(z)   + (int)*(w);
				greenSum1 += (int)*(z+1) + (int)*(w+1);
				blueSum1  += (int)*(z+2) + (int)*(w+2);
				// add the fourth vector (u+3, v+3, w+3)
				register int redSum2 = 0;
				register int greenSum2 = 0;
				register int blueSum2 = 0;
				
				redSum2   += redSum1   + (int)*(u+3) + (int)*(v+3) + (int)*(w+3);
				greenSum2 += greenSum1 + (int)*(u+4) + (int)*(v+4) + (int)*(w+4);
				blueSum2  += blueSum1  + (int)*(u+5) + (int)*(v+5) + (int)*(w+5);
				// add the first vector (abc)
				redSum1   += (int)*a     + (int)*b     + (int)*c;
				greenSum1 += (int)*(a+1) + (int)*(b+1) + (int)*(c+1);
				blueSum1  += (int)*(a+2) + (int)*(b+2) + (int)*(c+2);

				// put the values in the first target 'pixel'
				*destr = (unsigned char)(redSum1 / 9);
				*destg = (unsigned char)(greenSum1 / 9);
				*destb = (unsigned char)(blueSum1 / 9);
				// moving on to the next 'pixel'
				destr += 3;
				destg += 3;
				destb += 3;
				// put the values in the second target 'pixel'
				*destr = (unsigned char)(redSum2 / 9);
				*destg = (unsigned char)(greenSum2 / 9);
				*destb = (unsigned char)(blueSum2 / 9);
				// moving on to the next 'pixel'
				destr += 3;
				destg += 3;
				destb += 3;
				// move on to the next pixel
				a+=6; x+=6; u+=6;
				b+=6; y+=6; v+=6;
				c+=6; z+=6; w+=6;
			}
			// copy the last pixel to dst
			*(destr) = *(y);
			*(destg) = *(y+1);
			*(destb) = *(y+2);
			// move to the next row
			a+=6; x+=6; u+=6;
			b+=6; y+=6; v+=6;
			c+=6; z+=6; w+=6;
			destg += 3;
			destb += 3;
		}
	// for the odd case	
	} else {
		// basically the same thing is happening here, only difference is at the end of the inner loop
		for (i = 1; i < pdim - 1; i++) {
			// // copy the first pixel to dst
			*(destr ) = *b;
			*(destg ) = *(b + 1);
			*(destb ) = *(b + 2);
			destr += 3;
			destg += 3;
			destb += 3;
			for ( j = 1; j < pdim - 1 ; j+=2) {
				redSum1 = 0;
				greenSum1 = 0;
				blueSum1 = 0;

				redSum1   += (int)*(x)   + (int)*(u);
				greenSum1 += (int)*(x+1) + (int)*(u+1);
				blueSum1  += (int)*(x+2) + (int)*(u+2);
				
				redSum1   += (int)*(y)   + (int)*(v);
				greenSum1 += (int)*(y+1) + (int)*(v+1);
				blueSum1  += (int)*(y+2) + (int)*(v+2);

				redSum1   += (int)*(z)  + (int)*(w);
				greenSum1 += (int)*(z+1) + (int)*(w+1);
				blueSum1  += (int)*(z+2) + (int)*(w+2);

				register int redSum2 = 0;
				register int greenSum2 = 0;
				register int blueSum2 = 0;

				redSum2   += redSum1   + (int)*(u+3) + (int)*(v+3) + (int)*(w+3);
				greenSum2 += greenSum1 + (int)*(u+4) + (int)*(v+4) + (int)*(w+4);
				blueSum2  += blueSum1  + (int)*(u+5) + (int)*(v+5) + (int)*(w+5);
				
				redSum1   += (int)*a     + (int)*b     + (int)*c;
				greenSum1 += (int)*(a+1) + (int)*(b+1) + (int)*(c+1);
				blueSum1  += (int)*(a+2) + (int)*(b+2) + (int)*(c+2);

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
				a+=6; x+=6; u+=6;
				b+=6; y+=6; v+=6;
				c+=6; z+=6; w+=6;
			}
			// copy the last pixel to dst
			*(destr - 3) = *(b);
			*(destg - 3) = *(b+1);
			*(destb - 3) = *(b+2);
			// moving on to the next row
			a+=3; x+=3; u+=3;
			b+=3; y+=3; v+=3;
			c+=3; z+=3; w+=3;
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
	register unsigned char* x = src + 3;
	register unsigned char* u = src + 6;
	register unsigned char* b = src + dim;
	register unsigned char* y = src + dim + 3;
	register unsigned char* v = src + dim + 6;
	register unsigned char* c = src + dim + dim;
	register unsigned char* z = src + dim + dim + 3;
	register unsigned char* w = src + dim + dim + 6;
	register char* destr = dst + dim;
	register char* destg = dst + dim + 1;
	register char* destb = dst + dim + 2;
	register int redSum, greenSum, blueSum;
	unsigned int i, j;
	for (i = 1; i < pdim - 1; i++) {
		// copy the first pixel to dst
		*(destr) = *b;
		*(destg) = *(b + 1);
		*(destb) = *(b + 2);
		destr += 3;
		destg += 3;
		destb += 3;
		for ( j = 1; j < pdim - 1; j++) {
			redSum = 0;
			greenSum = 0;
			blueSum = 0;

			redSum -=   (int)*(a) + (int)*(x) + (int)*(u) + (int)*(b) 
			          + (int)*(v) + (int)*(c) + (int)*(z) + (int)*(w);
			
			redSum += MULT9((int)*(y));
			// a++; x++; u++;
			// b++; y++; v++;
			// c++; z++; w++;

			greenSum -=   (int)*(a+1) + (int)*(x+1) + (int)*(u+1) + (int)*(b+1) 
			            + (int)*(v+1) + (int)*(c+1) + (int)*(z+1) + (int)*(w+1);
			
			greenSum += MULT9((int)*(y+1));
			// a++; x++; u++;
			// b++; y++; v++;
			// c++; z++; w++;

			blueSum -=   (int)*(a+2) + (int)*(x+2) + (int)*(u+2) + (int)*(b+2) 
			           + (int)*(v+2) + (int)*(c+2) + (int)*(z+2) + (int)*(w+2);
			
			blueSum += MULT9((int)*(y+2));
			// a++; x++; u++;
			// b++; y++; v++;
			// c++; z++; w++;
			
		
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
			a+=3; x+=3; u+=3;
			b+=3; y+=3; v+=3;
			c+=3; z+=3; w+=3;
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
		destr +=3; destg+=3; destb+=3;
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
	/*
	{a, x, u}
	{b, y, v}
	{c, z, w}
	*/
	register unsigned char* a = src;
	register unsigned char* x = src + 3;
	register unsigned char* u = src + 6;
	register unsigned char* b = src + dim;
	register unsigned char* y = src + dim + 3;
	register unsigned char* v = src + dim + 6;
	register unsigned char* c = src + dim + dim;
	register unsigned char* z = src + dim + dim + 3;
	register unsigned char* w = src + dim + dim + 6;
	char* destr = dst + dim;
	char* destg = dst + dim + 1;
	char* destb = dst + dim + 2;
	int range = pdim - 1;
	register int redSum, greenSum, blueSum;
	unsigned char *maxPixel, *minPixel;
	register int min, max;
	unsigned int i, j;
	for (i = 1; i < range; i++) {

		// copy the first pixel to dst
		*(destr) = *b;
		*(destg) = *(b + 1);
		*(destb) = *(b + 2);
		// moving on to the next 'pixel'
		destr += 3;
		destg += 3;
		destb += 3;

		for ( j = 1; j < range; j++) {
			redSum = 0;
			greenSum = 0;
			blueSum = 0;
			min = 766;
			max = -1;
			register int sumL, sumM, sumR;
			// the equivalent of applykernel multiplication
			redSum   += (int)*a + (int)*x + (int)*u;
			// calculate intensity
			sumL = (int)*a;
			sumM = (int)*x;
			sumR = (int)*u;
			// move to green
			//a++; x++; u++;
			greenSum   += (int)*(a+1) + (int)*(x+1) + (int)*(u+1);
			// calculate intensity
			sumL += (int)*(a+1);
			sumM += (int)*(x+1);
			sumR += (int)*(u+1);
			// move to blue
			//a++; x++; u++;
			blueSum   += (int)*(a+2) + (int)*(x+2) + (int)*(u+2);
			// calculate intensity
			sumL += (int)*(a+2);
			sumM += (int)*(x+2);
			sumR += (int)*(u+2);
			//a++; x++; u++;
			// a+=3; x+=3; u+=3;
			
			if (sumL <= min) {
				min = sumL;
				minPixel = a; 
			}
			if (sumL > max) {
				max = sumL;
				maxPixel = a;
			}
			if (sumM <= min) {
				min = sumM;
				minPixel = x; // x - 3
			}
			if (sumM > max) {
				max = sumM;
				maxPixel = x; // x - 3
			}
			if (sumR <= min) {
				min = sumR;
				minPixel = u; // u - 3
			}
			if (sumR > max) {
				max = sumR;
				maxPixel = u; // u - 3
			}
			a+=3; x+=3; u+=3;
			
			register int sumL1, sumM1, sumR1;
			redSum   += (int)*b + (int)*y + (int)*v;
			// calculate intensity
			sumL1 = (int)*b;
			sumM1 = (int)*y;
			sumR1 = (int)*v;
			// move to green
			//b++; y++; v++;
			greenSum   += (int)*(b+1) + (int)*(y+1) + (int)*(v+1);
			// calculate intensity
			sumL1 += (int)*(b+1);
			sumM1 += (int)*(y+1);
			sumR1 += (int)*(v+1);
			// move to blue
			//b++; y++; v++;
			blueSum   += (int)*(b+2) + (int)*(y+2) + (int)*(v+2);
			// calculate intensity
			sumL1 += (int)*(b+2);
			sumM1 += (int)*(y+2);
			sumR1 += (int)*(v+2);
			// b++; y++; v++;
			
			if (sumL1 <= min) {
				min = sumL1;
				minPixel = b; 
			}
			if (sumL1 > max) {
				max = sumL1;
				maxPixel = b;
			}if (sumM1 <= min) {
				min = sumM1;
				minPixel = y; // y - 3
			}
			if (sumM1 > max) {
				max = sumM1;
				maxPixel = y; // y - 3
			}if (sumR1 <= min) {
				min = sumR1;
				minPixel = v; // v - 3
			}
			if (sumR1 > max) {
				max = sumR1;
				maxPixel = v; // v - 3
			}
			b+=3; y+=3; v+=3;
			register int sumL2, sumM2, sumR2;
			redSum  += (int)*c + (int)*z + (int)*w;
			// calculate intensity
			sumL2 = (int)*c;
			sumM2 = (int)*z;
			sumR2 = (int)*w;
			// move to green
			//c++; z++; w++;
			greenSum  += (int)*(c+1) + (int)*(z+1) + (int)*(w+1);
			// calculate intensity
			sumL2 += (int)*(c+1);
			sumM2 += (int)*(z+1);
			sumR2 += (int)*(w+1);
			// move to blue
			//c++; z++; w++;
			blueSum  += (int)*(c+2) + (int)*(z+2) + (int)*(w+2);
			// calculate intensity
			sumL2 += (int)*(c+2);
			sumM2 += (int)*(z+2);
			sumR2 += (int)*(w+2);
			//c++; z++; w++;
			
			if (sumL2 <= min) {
				min = sumL2;
				minPixel = c; 
			}
			if (sumL2 > max) {
				max = sumL2;
				maxPixel = c;
			}if (sumM2 <= min) {
				min = sumM2;
				minPixel = z; // z - 3
			}
			if (sumM2 > max) {
				max = sumM2;
				maxPixel = z; // z - 3
			}if (sumR2 <= min) {
				min = sumR2;
				minPixel =  w; // w - 3
			}if (sumR2 > max) {
				max = sumR2;
				maxPixel = w; // w - 3
			}
			c+=3;z+=3;w+=3;
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
		destr +=3; destg+=3; destb+=3;
	}
 
}

void filterChars_less_vars_arrays(int pdim, unsigned char *src, char *dst) {
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
	register int minmax[2] = {0, 0};
	register int intensity[3] = {0, 0, 0};
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
			minmax[0] = 766;
			minmax[1] = -1;

			// the equivalent of applykernel multiplication
			redSum   += (int)*a + (int)*x + (int)*u;
			// calculate intensity
			intensity[0] = (int)*a;
			intensity[1] = (int)*x;
			intensity[2] = (int)*u;
			// move to green
			a++; x++; u++;
			greenSum   += (int)*a + (int)*x + (int)*u;
			// calculate intensity
			intensity[0] += (int)*a;
			intensity[1] += (int)*x;
			intensity[2] += (int)*u;
			// move to blue
			a++; x++; u++;
			blueSum   += (int)*a + (int)*x + (int)*u;
			// calculate intensity
			intensity[0] += (int)*a;
			intensity[1] += (int)*x;
			intensity[2] += (int)*u;
			a++; x++; u++;
			
			if (intensity[0] <= minmax[0]) {
				minmax[0] = intensity[0];
				minPixel = a - 3; 
			}
			if (intensity[0] > minmax[1]) {
				minmax[1] = intensity[0];
				maxPixel = a - 3;
			}
			if (intensity[1] <= minmax[0]) {
				minmax[0] = intensity[1];
				minPixel = a; // x - 3
			}
			if (intensity[1] > minmax[1]) {
				minmax[1] = intensity[1];
				maxPixel = a; // x - 3
			}
			if (intensity[2] <= minmax[0]) {
				minmax[0] = intensity[2];
				minPixel = x; // u - 3
			}
			if (intensity[2] > minmax[1]) {
				minmax[1] = intensity[2];
				maxPixel = x; // u - 3
			}
			

			redSum   += (int)*b + (int)*y + (int)*v;
			// calculate intensity
			intensity[0] = (int)*b;
			intensity[1] = (int)*y;
			intensity[2] = (int)*v;
			// move to green
			b++; y++; v++;
			greenSum   += (int)*b + (int)*y + (int)*v;
			// calculate intensity
			intensity[0] += (int)*b;
			intensity[1] += (int)*y;
			intensity[2] += (int)*v;
			// move to blue
			b++; y++; v++;
			blueSum   += (int)*b + (int)*y + (int)*v;
			// calculate intensity
			intensity[0] += (int)*b;
			intensity[1] += (int)*y;
			intensity[2] += (int)*v;
			b++; y++; v++;
			
			if (intensity[0] <= minmax[0]) {
				minmax[0] = intensity[0];
				minPixel = b - 3; 
			}
			if (intensity[0] > minmax[1]) {
				minmax[1] = intensity[0];
				maxPixel = b - 3;
			}if (intensity[1] <= minmax[0]) {
				minmax[0] = intensity[1];
				minPixel = b; // y - 3
			}
			if (intensity[1] > minmax[1]) {
				minmax[1] = intensity[1];
				maxPixel = b; // y - 3
			}if (intensity[2] <= minmax[0]) {
				minmax[0] = intensity[2];
				minPixel = y; // v - 3
			}
			if (intensity[2] > minmax[1]) {
				minmax[1] = intensity[2];
				maxPixel = y; // v - 3
			}
			
			
			redSum  += (int)*c + (int)*z + (int)*w;
			// calculate intensity
			intensity[0] = (int)*c;
			intensity[1] = (int)*z;
			intensity[2] = (int)*w;
			// move to green
			c++; z++; w++;
			greenSum  += (int)*c + (int)*z + (int)*w;
			// calculate intensity
			intensity[0] += (int)*c;
			intensity[1] += (int)*z;
			intensity[2] += (int)*w;
			// move to blue
			c++; z++; w++;
			blueSum  += (int)*c + (int)*z + (int)*w;
			// calculate intensity
			intensity[0] += (int)*c;
			intensity[1] += (int)*z;
			intensity[2] += (int)*w;
			c++; z++; w++;
			
			if (intensity[0] <= minmax[0]) {
				minmax[0] = intensity[0];
				minPixel = c - 3; 
			}
			if (intensity[0] > minmax[1]) {
				minmax[1] = intensity[0];
				maxPixel = c - 3;
			}if (intensity[1] <= minmax[0]) {
				minmax[0] = intensity[1];
				minPixel = c; // z - 3
			}
			if (intensity[1] > minmax[1]) {
				minmax[1] = intensity[1];
				maxPixel = c; // z - 3
			}if (intensity[2] <= minmax[0]) {
				minmax[0] = intensity[2];
				minPixel =  z; // w - 3
			}if (intensity[2] > minmax[1]) {
				minmax[1] = intensity[2];
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