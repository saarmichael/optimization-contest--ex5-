// 315240937
// Michael Saar

/*
This code is a reconstruction of a another code. The latter is a non-efficient, complicated code
and this one is an optimized version of it.
myfunction recives an Image and a user argument, and runs a series of convolutions give a new Image.

The main approach I took was to give up on using the Pixel and pixel_sum structs, and rather work straight
on the Images->data which is a char* array of r-g-b bytes.
This sagnificantlly saves time of copying information from the original Image data to a new pixels array
*/

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
	//int pdim - 1 = pdim - 1;
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
			destr += 3;
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
			for ( j = 1; j < pdim - 1; j+=2) {
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

void sharp_less_vars(int pdim, unsigned char* src, char* dst) {
	int dim = pdim * 3;
	// copy the first row of pixels to dst
	memcpy(dst, src, dim);
	// copy the last row of pixels to dst
	memcpy(dst + ((pdim - 1)*dim), src + ((pdim - 1)*dim), dim);
	//int pdim - 1 = pdim - 1;
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

			greenSum -=   (int)*(a+1) + (int)*(x+1) + (int)*(u+1) + (int)*(b+1) 
			            + (int)*(v+1) + (int)*(c+1) + (int)*(z+1) + (int)*(w+1);
			
			greenSum += MULT9((int)*(y+1));
	
			blueSum -=   (int)*(a+2) + (int)*(x+2) + (int)*(u+2) + (int)*(b+2) 
			           + (int)*(v+2) + (int)*(c+2) + (int)*(z+2) + (int)*(w+2);
			
			blueSum += MULT9((int)*(y+2));
		    
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
	register int redSum, greenSum, blueSum;
	unsigned char *maxPixel, *minPixel;
	register int min, max;
	unsigned int i, j;
	for (i = 1; i < pdim - 1; i++) {
		// copy the first pixel to dst
		*(destr) = *b;
		*(destg) = *(b + 1);
		*(destb) = *(b + 2);
		// moving on to the next 'pixel'
		destr += 3;
		destg += 3;
		destb += 3;

		for ( j = 1; j < pdim - 1; j++) {
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
			greenSum   += (int)*(a+1) + (int)*(x+1) + (int)*(u+1);
			// calculate intensity
			sumL += (int)*(a+1);
			sumM += (int)*(x+1);
			sumR += (int)*(u+1);
			// move to blue
			blueSum   += (int)*(a+2) + (int)*(x+2) + (int)*(u+2);
			// calculate intensity
			sumL += (int)*(a+2);
			sumM += (int)*(x+2);
			sumR += (int)*(u+2);
			
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
			greenSum   += (int)*(b+1) + (int)*(y+1) + (int)*(v+1);
			// calculate intensity
			sumL1 += (int)*(b+1);
			sumM1 += (int)*(y+1);
			sumR1 += (int)*(v+1);
			// move to blue
			blueSum   += (int)*(b+2) + (int)*(y+2) + (int)*(v+2);
			// calculate intensity
			sumL1 += (int)*(b+2);
			sumM1 += (int)*(y+2);
			sumR1 += (int)*(v+2);
			
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
			greenSum  += (int)*(c+1) + (int)*(z+1) + (int)*(w+1);
			// calculate intensity
			sumL2 += (int)*(c+1);
			sumM2 += (int)*(z+1);
			sumR2 += (int)*(w+1);
			// move to blue
			blueSum  += (int)*(c+2) + (int)*(z+2) + (int)*(w+2);
			// calculate intensity
			sumL2 += (int)*(c+2);
			sumM2 += (int)*(z+2);
			sumR2 += (int)*(w+2);
			
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
			else {
				*destr = redSum;
			}
			if (0 != (greenSum & 0b10000000000000000000000000000000)) {
				*destg = (unsigned char)0;
			} 
			else {
				*destg = greenSum;
			}
			if (0 != (blueSum & 0b10000000000000000000000000000000)) {
				*destb = (unsigned char)0;
			} 
			else {
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

/*
The main approach I took was to give up on using the Pixel and pixel_sum structs, and rather work straight
on the Images->data which is a char* array of r-g-b bytes.
This sagnificantlly saves time of copying information from the original Image data to a new pixels array
*/

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