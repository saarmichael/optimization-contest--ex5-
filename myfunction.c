#include <stdbool.h> 

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


void smoothNoFilter(int dim, pixel *src, pixel *dst, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale) {
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
	for (i = 1; i < dim - 1; i++) {
		for ( j = 1; j < dim - 1; j++) {
			//printf("i = %d, j = %d\n", i ,j);
			redSum = 0;
			greenSum = 0;
			blueSum = 0;
			// ka =  *a;
			// kx =  *x;
			// ku =  *u;
			// kb =  *b;
			// ky =  *y;
			// kv =  *v;
			// kc =  *c;
			// kz =  *z;
			// kw =  *w;

			// the equivalent of applykernel multiplication
			redSum   += a->red   + x->red   + u->red;
			greenSum += a->green + x->green + u->green;
			blueSum  += a->blue  + x->blue  + u->blue;

			redSum   += b->red   + y->red   + v->red;
			greenSum += b->green + y->green + v->green;
			blueSum  += b->blue  + y->blue  + v->blue;

			redSum   += c->red   + z->red   + w->red;
			greenSum += c->green + z->green + w->green;
			blueSum  += c->blue  + z->blue  + w->blue;
			
			
			dstPixel->red   = redSum / kernelScale;
			dstPixel->green = greenSum / kernelScale;
			dstPixel->blue  = blueSum / kernelScale;
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
			// ka =  *a;
			// kx =  *x;
			// ku =  *u;
			// kb =  *b;
			// ky =  *y;
			// kv =  *v;
			// kc =  *c;
			// kz =  *z;
			// kw =  *w;

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

void charsToPixelsUnrolled(Image *charsImg, pixel* pixels) {
	pixel* p1 = pixel;
	
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

	pixel* pixelsImg = malloc(m*n*sizeof(pixel));
	pixel* backupOrg = malloc(m*n*sizeof(pixel));

	charsToPixels(image, pixelsImg);
	copyPixels(pixelsImg, backupOrg);
	if (filter)
	{
		smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	} else {
		smoothNoFilter(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale);
	}
	

	pixelsToChars(pixelsImg, image);

	free(pixelsImg);
	free(backupOrg);
}

void doConvolution2(Image *image, int kernelSize, int kernel[kernelSize][kernelSize], int kernelScale, bool filter) {

	pixel* pixelsImg = malloc(m*n*sizeof(pixel));
	pixel* backupOrg = malloc(m*n*sizeof(pixel));

	charsToPixels(image, pixelsImg);
	copyPixels(pixelsImg, backupOrg);
	if (filter)
	{
		smooth(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale, filter);
	} else {
		sharpNoFilter(m, backupOrg, pixelsImg, kernelSize, kernel, kernelScale);
	}
	

	pixelsToChars(pixelsImg, image);

	free(pixelsImg);
	free(backupOrg);
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

