#ifndef FourierTransform_H
#define FourierTransform_H

// Analysis Equations (Forward Transforms - Not normalized by array size)
double** fft(double*,int);
double** fft(double**,int);
double** fft2(double*,int,int);
double** fft2(double**,int,int);
double** fft3(double*,int,int,int);
double** fft3(double**,int,int,int);
// Synthesis Equations (Backward Transforms - Normalized by array size)
double** ifft(double**,int);
double** ifft2(double**,int,int);
double** ifft3(double**,int,int,int);
// Zeropadding
double** zeropad(double**,int,int);
double** zeropad(double*,int,int);
double** zeropad2(double**,int,int,int,int);
double** zeropad2(double*,int,int,int,int);
double** zeropad3(double**,int,int,int,int,int,int);
double** zeropad3(double*,int,int,int,int,int,int);
/*/////////////////////// FT Operations ///////////////////////*/
// Convolution
double** conv(double** x,int Nx,double** y,int Ny);
double** conv(double* x,int Nx,double** y,int Ny);
double** conv(double** x,int Nx,double* y,int Ny);
double** conv(double* x,int Nx,double* y,int Ny);
// Correlation
double** corr_1d(double*,int,double*,int);
double** corr_1d(double**,int,double**,int);
// Time (or Spatial Shift)
double* circShift(double*,int,int);
double** circShift(double**,int,int);

double* circShift3(double*,int,int,int,int,int,int);
double** circShift3(double**,int,int,int,int,int,int);
// Reverse Order of Data Elements
double** reverse(double**,int);

// Covariance Matrix
double** cov(double** x,int numRows,int numCols,double** y);

// Basic Matrix Operations
double mean(double* Data,int Sz);
#endif
