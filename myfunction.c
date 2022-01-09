#include <stdbool.h> 
#include <string.h>

#define MIN(x,y) ((x<y)?x:y)
#define MAX(x,y) ((x>y)?x:y)


void smoothNoFilter(int pdim, unsigned char *src, char *dst) {
	int dim = pdim * 3;
	// copy the first row of pixels to dst
	memcpy(dst, src, dim);
	// copy the last row of pixels to dst
	memcpy(dst + ((pdim - 1)*dim), src + ((pdim - 1)*dim), dim);
	
	register unsigned char* a = src;
	// unsigned char* ag = src + 1;
	// unsigned char* ab = src + 2;

	register unsigned char* b = src + dim;
	// unsigned char* bg = src + dim + 1;
	// unsigned char* bb = src + dim + 2;

	register unsigned char* c = src + dim + dim;
	// unsigned char* cg = src + dim + dim + 1;
	// unsigned char* cb = src + dim + dim + 2;

	register unsigned char* x = src + 3;
	// unsigned char* xg = src + 4;
	// unsigned char* xb = src + 5;

	register unsigned char* y = src + dim + 3;
	// unsigned char* yg = src + dim + 4;
	// unsigned char* yb = src + dim + 5;

	register unsigned char* z = src + dim + dim + 3;
	// unsigned char* zg = src + dim + dim + 4;
	// unsigned char* zb = src + dim + dim + 5;
	
	register unsigned char* u = src + 6;
	// unsigned char* ug = src + 7;
	// unsigned char* ub = src + 8;

	register unsigned char* v = src + dim + 6;
	// unsigned char* vg = src + dim + 7;
	// unsigned char* vb = src + dim + 8;

	register unsigned char* w = src + dim + dim + 6;
	// unsigned char* wg = src + dim + dim + 7;
	// unsigned char* wb = src + dim + dim + 8;

	char* destr = dst + dim + 3;
	char* destg = dst + dim + 4;
	char* destb = dst + dim + 5;
	
	register int redSum, greenSum, blueSum;
	unsigned int i, j;
	for (i = 1; i < pdim - 1; i++) {

		destr = dst + dim*i + 3*sizeof(unsigned char);
		destg = dst + dim*i + 4*sizeof(unsigned char);
		destb = dst + dim*i + 5*sizeof(unsigned char);

		a = src + dim*(i-1);
		// ag = src + dim*(i-1) + 1*sizeof(unsigned char);
		// ab = src + dim*(i-1) + 2*sizeof(unsigned char);
		b = src + dim*(i);
		// bg = src + dim*(i) + 1*sizeof(unsigned char);
		// bb = src + dim*(i) + 2*sizeof(unsigned char);
		c = src + dim*(i+1);
		// cg = src + dim*(i+1) + 1*sizeof(unsigned char);
		// cb = src + dim*(i+1) + 2*sizeof(unsigned char);
		x = src + dim*(i-1) + 3*sizeof(unsigned char);
		// xg = src + dim*(i-1) + 4*sizeof(unsigned char);
		// xb = src + dim*(i-1) + 5*sizeof(unsigned char);
		y = src + dim*(i) + 3*sizeof(unsigned char);
		// yg = src + dim*(i) + 4*sizeof(unsigned char);
		// yb = src + dim*(i) + 5*sizeof(unsigned char);
		z = src + dim*(i+1) + 3*sizeof(unsigned char);
		// zg = src + dim*(i+1) + 4*sizeof(unsigned char);
		// zb = src + dim*(i+1) + 5*sizeof(unsigned char);
		u = src + dim*(i-1) + 6*sizeof(unsigned char);
		// ug = src + dim*(i-1) + 7*sizeof(unsigned char);
		// ub = src + dim*(i-1) + 8*sizeof(unsigned char);
		v = src + dim*(i) + 6*sizeof(unsigned char);
		// vg = src + dim*(i) + 7*sizeof(unsigned char);
		// vb = src + dim*(i) + 8*sizeof(unsigned char);
		w = src + dim*(i+1) + 6*sizeof(unsigned char);
		// wg = src + dim*(i+1) + 7*sizeof(unsigned char);
		// wb = src + dim*(i+1) + 8*sizeof(unsigned char);

		// copy the first pixel to dst
		*(destr - 3) = *b;
		*(destg - 3) = *(b + 1);
		*(destb - 3) = *(b + 2);

		for ( j = 1; j < pdim - 1; j++) {
			redSum = 0;
			greenSum = 0;
			blueSum = 0;

			// the equivalent of applykernel multiplication
			// redSum   += (int)*(a++) + (int)*(x++) + (int)*(u++);
			// greenSum += (int)*(a++) + (int)*(x++) + (int)*(u++);
			// blueSum  += (int)*(a++) + (int)*(x++) + (int)*(u++);

			// redSum   += (int)*(b++) + (int)*(y++) + (int)*(v++);
			// greenSum += (int)*(b++) + (int)*(y++) + (int)*(v++);
			// blueSum  += (int)*(b++) + (int)*(y++) + (int)*(v++);

			// redSum   += (int)*(c++) + (int)*(z++) + (int)*(w++);
			// greenSum += (int)*(c++) + (int)*(z++) + (int)*(w++);
			// blueSum  += (int)*(c++) + (int)*(z++) + (int)*(w++);

			redSum   += (int)*(a) + (int)*(x) + (int)*(u);
			a++; x++; u++;
			greenSum += (int)*(a) + (int)*(x) + (int)*(u);
			a++; x++; u++;
			blueSum  += (int)*(a) + (int)*(x) + (int)*(u);
			a++; x++; u++;

			redSum   += (int)*(b) + (int)*(y) + (int)*(v);
			b++; y++; v++;
			greenSum += (int)*(b) + (int)*(y) + (int)*(v);
			b++; y++; v++;
			blueSum  += (int)*(b) + (int)*(y) + (int)*(v);
			b++; y++; v++;

			redSum   += (int)*(c) + (int)*(z) + (int)*(w);
			c++; z++; w++;
			greenSum += (int)*(c) + (int)*(z) + (int)*(w);
			c++; z++; w++;
			blueSum  += (int)*(c) + (int)*(z) + (int)*(w);
			c++; z++; w++;

			// put the values in the target 'pixel'
			*destr = (unsigned char)(redSum / 9);
			*destg = (unsigned char)(greenSum / 9);
			*destb = (unsigned char)(blueSum / 9);
			// moving on to the next 'pixel'
			destr += 3*sizeof(unsigned char);
			destg += 3*sizeof(unsigned char);
			destb += 3*sizeof(unsigned char);
			// a += 3*sizeof(unsigned char);
			// // ag += 3*sizeof(unsigned char);
			// // ab += 3*sizeof(unsigned char);
			// b += 3*sizeof(unsigned char);
			// // bg += 3*sizeof(unsigned char);
			// // bb += 3*sizeof(unsigned char);
			// c += 3*sizeof(unsigned char);
			// // cg += 3*sizeof(unsigned char);
			// // cb += 3*sizeof(unsigned char);
			// x += 3*sizeof(unsigned char);
			// // xg += 3*sizeof(unsigned char);
			// // xb += 3*sizeof(unsigned char);
			// y += 3*sizeof(unsigned char);
			// // yg += 3*sizeof(unsigned char);
			// // yb += 3*sizeof(unsigned char);
			// z += 3*sizeof(unsigned char);
			// // zg += 3*sizeof(unsigned char);
			// // zb += 3*sizeof(unsigned char);
			// u += 3*sizeof(unsigned char);
			// // ug += 3*sizeof(unsigned char);
			// // ub += 3*sizeof(unsigned char);
			// v += 3*sizeof(unsigned char);
			// // vg += 3*sizeof(unsigned char);
			// // vb += 3*sizeof(unsigned char);
			// w += 3*sizeof(unsigned char);
			// // wg += 3*sizeof(unsigned char);
			// // wb += 3*sizeof(unsigned char);
		}
		// copy the last pixel to dst
		*(destr) = *(v - 3);
		*(destg) = *(v - 2);
		*(destb) = *(v - 1);
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

			redSum   -= (int)*br - 9*(int)*yr + (int)*vr;
			greenSum -= (int)*bg - 9*(int)*yg + (int)*vg;
			blueSum  -= (int)*bb - 9*(int)*yb + (int)*vb;

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


void doConvolution1(Image *image) {

	char* newImage = (char*)malloc(3*m*n*sizeof(unsigned char));

	smoothNoFilter(m, image->data, newImage);
	image->data = newImage;
}

void doConvolution2(Image *image) {

	char* newImage = (unsigned char*)malloc(3*m*n*sizeof(unsigned char));
	
	sharp(m, image->data, newImage);
	image->data = newImage;
}

void doConvolution3(Image *image) {

	char* newImage = (unsigned char*)malloc(3*m*n*sizeof(unsigned char));
	
	filterChars(m, image->data, newImage);
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