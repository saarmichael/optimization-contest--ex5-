#include <stdbool.h> 
#include <string.h>

#define MIN(x,y) ((x<y)?x:y)
#define MAX(x,y) ((x>y)?x:y)

typedef struct {
   unsigned char red;
   unsigned char green;
   unsigned char blue;
} pixel;

typedef struct {
    int red;
    int green;
    int blue;
    // int num;
} pixel_sum;


/* Compute min and max of two integers, respectively */
int min(int a, int b) { return (a < b ? a : b); }
int max(int a, int b) { return (a > b ? a : b); }

int calcIndex(int i, int j, int n) {
	return ((i)*(n)+(j));
}

/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
void initialize_pixel_sum(pixel_sum *sum) {
	sum->red = sum->green = sum->blue = 0;
	// sum->num = 0;
	return;
}

/*
 * assign_sum_to_pixel - Truncates pixel's new value to match the range [0,255]
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum, int kernelScale) {

	// divide by kernel's weight
	sum.red = sum.red / kernelScale;
	sum.green = sum.green / kernelScale;
	sum.blue = sum.blue / kernelScale;

	// truncate each pixel's color values to match the range [0,255]
	current_pixel->red = (unsigned char) (min(max(sum.red, 0), 255));
	current_pixel->green = (unsigned char) (min(max(sum.green, 0), 255));
	current_pixel->blue = (unsigned char) (min(max(sum.blue, 0), 255));
	return;
}

/*
* sum_pixels_by_weight - Sums pixel values, scaled by given weight
*/
static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight) {
	sum->red += ((int) p.red) * weight;
	sum->green += ((int) p.green) * weight;
	sum->blue += ((int) p.blue) * weight;
	// sum->num++;
	return;
}

/*
 *  Applies kernel for pixel at (i,j)
 */
static pixel applyKernel(int dim, int i, int j, pixel *src, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	int ii, jj;
	int currRow, currCol;
	pixel_sum sum;
	pixel current_pixel;
	int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
	int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
	int min_row, min_col, max_row, max_col;
	pixel loop_pixel;

	initialize_pixel_sum(&sum);

	int istart = max(i-1, 0);
	int istop = min(i+1, dim-1);
	for(ii = istart; ii <= istop; ii++) {
		for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {

			int kRow, kCol;

			// compute row index in kernel
			if (ii < i) {
				kRow = 0;
			} else if (ii > i) {
				kRow = 2;
			} else {
				kRow = 1;
			}

			// compute column index in kernel
			if (jj < j) {
				kCol = 0;
			} else if (jj > j) {
				kCol = 2;
			} else {
				kCol = 1;
			}

			// apply kernel on pixel at [ii,jj]
			sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
		}
	}

	if (filter) {
		// find min and max coordinates
		for(ii = max(i-1, 0); ii <= min(i+1, dim-1); ii++) {
			for(jj = max(j-1, 0); jj <= min(j+1, dim-1); jj++) {
				// check if smaller than min or higher than max and update
				loop_pixel = src[calcIndex(ii, jj, dim)];
				if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
					min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
					min_row = ii;
					min_col = jj;
				}
				if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
					max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
					max_row = ii;
					max_col = jj;
				}
			}
		}
		// filter out min and max
		sum_pixels_by_weight(&sum, src[calcIndex(min_row, min_col, dim)], -1);
		sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
	}

	// assign kernel's result to pixel at [i,j]
	assign_sum_to_pixel(&current_pixel, sum, kernelScale);
	return current_pixel;
}

/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(int dim, pixel *src, pixel *dst, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	int i, j;
	for (i = kernelSize / 2 ; i < dim - kernelSize / 2; i++) {
		for (j =  kernelSize / 2 ; j < dim - kernelSize / 2 ; j++) {
			dst[calcIndex(i, j, dim)] = applyKernel(dim, i, j, src, kernelSize, kernel, kernelScale, filter);
		}
	}
}


void smoothNoFilter(int pdim, unsigned char *src, char *dst) {
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
			redSum   += (int)*ar + (int)*xr + (int)*ur;
			greenSum += (int)*ag + (int)*xg + (int)*ug;
			blueSum  += (int)*ab + (int)*xb + (int)*ub;

			redSum   += (int)*br + (int)*yr + (int)*vr;
			greenSum += (int)*bg + (int)*yg + (int)*vg;
			blueSum  += (int)*bb + (int)*yb + (int)*vb;

			redSum   += (int)*cr + (int)*zr + (int)*wr;
			greenSum += (int)*cg + (int)*zg + (int)*wg;
			blueSum  += (int)*cb + (int)*zb + (int)*wb;
			

			// put the values in the target 'pixel'
			*destr = (unsigned char)(redSum / 9);
			*destg = (unsigned char)(greenSum / 9);
			*destb = (unsigned char)(blueSum / 9);
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

void sharpNofilterChars(int pdim, unsigned char* src, char* dst) {
	int dim = pdim * 3;
	
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

		for ( j = 1; j < pdim - 1; j++) {
			redSum = 0;
			greenSum = 0;
			blueSum = 0;

			// the equivalent of applykernel multiplication
			redSum   += (int)*ar + (int)*xr + (int)*ur;
			greenSum += (int)*ag + (int)*xg + (int)*ug;
			blueSum  += (int)*ab + (int)*xb + (int)*ub;

			redSum   += (int)*br + (int)*yr + (int)*vr;
			greenSum += (int)*bg + (int)*yg + (int)*vg;
			blueSum  += (int)*bb + (int)*yb + (int)*vb;

			redSum   += (int)*cr + (int)*zr + (int)*wr;
			greenSum += (int)*cg + (int)*zg + (int)*wg;
			blueSum  += (int)*cb + (int)*zb + (int)*wb;
			

			// put the values in the target 'pixel'
			*destr = (unsigned char)(redSum / 9);
			*destg = (unsigned char)(greenSum / 9);
			*destb = (unsigned char)(blueSum / 9);
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
	}
}

void sharpNoFilter(int dim, pixel *src, pixel *dst, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale) {
	// int div = dim % 3;
	// int jump = 2*dim;
	// create 9 pointers representing the template to work on
	pixel* a = src;
	pixel* x = src + 1;
	pixel* u = src + 2;
	pixel* b = src + dim;
	pixel* y = src + dim + 1;
	pixel* v = src + dim + 2;
	pixel* c = src + 2*dim;
	pixel* z = src + 2*dim + 1;
	pixel* w = src + 2*dim + 2;

	pixel* dstPixel = dst + dim + 1;

	//pixel ka, kb, kc, kx, ky, kz, ku, kv, kw;
	register int redSum, greenSum, blueSum;
	unsigned int i, j;
	for ( i = 1; i < dim - 1; i++) {
		for ( j = 1; j < dim - 1; j++) {
			//printf("i = %d, j = %d\n", i ,j);
			redSum = 0;
			greenSum = 0;
			blueSum = 0;

			// the equivalent of applykernel multiplication for kernel {{-1,-1,-1}, {-1, 9, -1}, {-1, -1 ,-1}}
			redSum   -= a->red   + x->red   + u->red;
			greenSum -= a->green + x->green + u->green;
			blueSum  -= a->blue  + x->blue  + u->blue;

			redSum   += -1*b->red   + 9*y->red   + -1*v->red;
			greenSum += -1*b->green + 9*y->green + -1*v->green;
			blueSum  += -1*b->blue  + 9*y->blue  + -1*v->blue;

			redSum   -= c->red   + z->red   + w->red;
			greenSum -= c->green + z->green + w->green;
			blueSum  -= c->blue  + z->blue  + w->blue;
			
			
			dstPixel->red   = MIN(MAX(redSum,0), 255);
			dstPixel->green =  MIN(MAX(greenSum,0), 255);
			dstPixel->blue  =  MIN(MAX(blueSum,0), 255);
			// moving on to the next pixel
			++dstPixel;
			++a;
			++b;
			++c;
			++x;
			++y;
			++z;
			++u;
			++v;
			++w;
		}
	}
	
}


void charsToPixels(Image *charsImg, pixel* pixels) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {
			int position = row*n + col;
			int position3 = 3*row*n + 3*col;	
			pixels[position].red = image->data[position3];
			pixels[position].green = image->data[position3 + 1];
			pixels[position].blue = image->data[position3 + 2];
		}
	}
}

void pixelsToChars(pixel* pixels, Image *charsImg) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {
			int position = row*n + col;
			int position3 = 3*row*n + 3*col;	
			image->data[position3] = pixels[position].red;
			image->data[position3 + 1] = pixels[position].green;
			image->data[position3 + 2] = pixels[position].blue;
		}
	}
}

void copyPixels(pixel* src, pixel* dst) {

	int row, col;
	for (row = 0 ; row < m ; row++) {
		for (col = 0 ; col < n ; col++) {
			int position = row*n + col;
			dst[position].red = src[position].red;
			dst[position].green = src[position].green;
			dst[position].blue = src[position].blue;
		}
	}
}

void doConvolution1(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	//pixel* pixelsImg = malloc(m*n*sizeof(pixel));
	//pixel* backupOrg = malloc(m*n*sizeof(pixel));

	//charsToPixels(image, pixelsImg);
	//copyPixels(pixelsImg, backupOrg);

	char* newImage = (char*)malloc(3*m*n*sizeof(unsigned char));

	if (filter)
	{
		//smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	} else {
		smoothNoFilter(m, image->data, newImage);
		image->data = newImage;
	}
	

	//pixelsToChars(pixelsImg, image);

	//free(pixelsImg);
	//free(backupOrg);
}

void doConvolution2(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	//pixel* pixelsImg = malloc(m*n*sizeof(pixel));
	//pixel* backupOrg = malloc(m*n*sizeof(pixel));

	char* newImage = (unsigned char*)malloc(3*m*n*sizeof(unsigned char));
	
	//charsToPixels(image, pixelsImg);
	//copyPixels(pixelsImg, backupOrg);
	if (filter)
	{
		//smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	} else {
		sharpNofilterChars(m, image->data, newImage);
		image->data = newImage;
	}
	

	//pixelsToChars(pixelsImg, image);

	//free(pixelsImg);
	//free(backupOrg);
	//free(newImage);
}

void doConvolution3(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	pixel* pixelsImg = malloc(m*n*sizeof(pixel));
	pixel* backupOrg = malloc(m*n*sizeof(pixel));

	charsToPixels(image, pixelsImg);
	copyPixels(pixelsImg, backupOrg);

	smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);

	pixelsToChars(pixelsImg, image);

	free(pixelsImg);
	free(backupOrg);
}

void myfunction(Image *image, char* srcImgpName, char* blurRsltImgName, char* sharpRsltImgName, char* filteredBlurRsltImgName, char* filteredSharpRsltImgName, char flag) {

	/*
	* [1, 1, 1]
	* [1, 1, 1]
	* [1, 1, 1]
	*/
	int blurKernel[3][3] = {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}};

	/*
	* [-1, -1, -1]
	* [-1, 9, -1]
	* [-1, -1, -1]
	*/
	int sharpKernel[3][3] = {{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};

	if (flag == '1') {	
		// blur image
		doConvolution1(image, 3, blurKernel, 9, false);

		// write result image to file
		writeBMP(image, srcImgpName, blurRsltImgName);	

		// sharpen the resulting image
		doConvolution2(image, 3, sharpKernel, 1, false);
		
		// write result image to file
		writeBMP(image, srcImgpName, sharpRsltImgName);	
	} else {
		// apply extermum filtered kernel to blur image
		doConvolution3(image, 3, blurKernel, 7, true);

		// write result image to file
		writeBMP(image, srcImgpName, filteredBlurRsltImgName);

		// sharpen the resulting image
		doConvolution3(image, 3, sharpKernel, 1, false);

		// write result image to file
		writeBMP(image, srcImgpName, filteredSharpRsltImgName);	
	}
}