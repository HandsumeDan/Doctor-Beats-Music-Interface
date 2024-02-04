# include <cmath>
# include "fftw3.h"
# include "FourierTransform.h"

double* circShift(double* Data,int Lx,int nx)
	{// Lx is the correlation length and nx is the length of the original data
	 int N=Lx;
	 double* temp=new double[N];
	 for(int k=0;k<Lx;k++)
		{if(k<nx)
			{temp[k+(Lx-nx)]=Data[k];}
		 else if(k>=nx)
			{temp[k-nx]=Data[k];}}
	 return temp;}

double** circShift(double** Data,int Lx,int nx)
	{// Lx is the correlation length and nx is the length of the original data
	 int N=Lx;
	 double** temp=new double*[N];
	 for(int i=0;i<N;i++)
		{temp[i]=new double[2];}
	 for(int k=0;k<Lx;k++)
		{if(k<nx)
			{temp[k+(Lx-nx)][0]=Data[k][0];
			 temp[k+(Lx-nx)][1]=Data[k][1];}
		 else if(k>=nx)
			{temp[k-nx][0]=Data[k][0];
			 temp[k-nx][1]=Data[k][1];}}
	 return temp;}

double* circShift3(double* Data,int Lx,int Ly,int Lz,int nx,int ny,int nz)
	{// Lx,Ly,and Lz are the correlation lengths and nx,ny,and nz are the lengths of the original data
	 int N=Lx*Ly*Lz;
	 double* tempX=new double[N];
	 double* tempY=new double[N];	
	 double* temp=new double[N];
	 int index;

	 for(int i=0;i<Lx;i++)
		{for(int j=0;j<Ly;j++)
			{for(int k=0;k<Lz;k++)
				{index=i*Ly*Lz+j*Lz+k;
				 // Shift Z
				 if(k<nz)
					{temp[index+(Lz-nz)]=Data[index];}
				 else if(k>=nz)
					{temp[index-nz]=Data[index];}}
			 for(int k=0;k<Lz;k++)
				 {index=i*Ly*Lz+j*Lz+k;
				  // Shift Y
			 	  if(j<ny)
					{tempY[index+(Ly-ny)*Lz]=temp[index];}
			 	  else if(j>=ny)
					{tempY[index-ny*Lz]=temp[index];}}}		
		 for(int j=0;j<Ly;j++)
			{for(int k=0;k<Lz;k++)
				{index=i*Ly*Lz+j*Lz+k;
				 // Shift X
			 	 if(i<nx)
					{tempX[index+(Lx-nx)*Ly*Lz]=tempY[index];}
			 	 else if(i>=nx)
					{tempX[index-nx*Ly*Lz]=tempY[index];}}}}

	 delete [] temp;
	 delete [] tempY;
	 return tempX;}

double** circShift3(double** Data,int Lx,int Ly,int Lz,int nx,int ny,int nz)
	{// Lx,Ly,and Lz are the correlation lengths and nx,ny,and nz are the lengths of the original data
	 int N=Lx*Ly*Lz;
	 double** tempX=new double*[N];
	 double** tempY=new double*[N];	
	 double** temp=new double*[N];
	 for(int i=0;i<N;i++)
		{tempX[i]=new double[2];
		 tempY[i]=new double[2];
		 temp[i]=new double[2];}
	 int index;

	 for(int i=0;i<Lx;i++)
		{for(int j=0;j<Ly;j++)
			{for(int k=0;k<Lz;k++)
				{index=i*Ly*Lz+j*Lz+k;
				 // Shift Z
				 if(k<nz)
					{temp[index+(Lz-nz)][0]=Data[index][0];
					 temp[index+(Lz-nz)][1]=Data[index][1];}
				 else if(k>=nz)
					{temp[index-nz][0]=Data[index][0];
					 temp[index-nz][1]=Data[index][1];}}
			 for(int k=0;k<Lz;k++)
				 {index=i*Ly*Lz+j*Lz+k;
				  // Shift Y
			 	  if(j<ny)
					{tempY[index+(Ly-ny)*Lz][0]=temp[index][0];
					 tempY[index+(Ly-ny)*Lz][1]=temp[index][1];}
			 	  else if(j>=ny)
					{tempY[index-ny*Lz][0]=temp[index][0];
					 tempY[index-ny*Lz][1]=temp[index][1];}}}		
		 for(int j=0;j<Ly;j++)
			{for(int k=0;k<Lz;k++)
				{index=i*Ly*Lz+j*Lz+k;
				 // Shift X
			 	 if(i<nx)
					{tempX[index+(Lx-nx)*Ly*Lz][0]=tempY[index][0];
					 tempX[index+(Lx-nx)*Ly*Lz][1]=tempY[index][1];}
			 	 else if(i>=nx)
					{tempX[index-nx*Ly*Lz][0]=tempY[index][0];
					 tempX[index-nx*Ly*Lz][1]=tempY[index][1];}}}}

	 for(int i=0;i<N;i++)
		{delete [] tempY[i];
		 delete [] temp[i];}
	 delete [] temp;
	 delete [] tempY;
	 return tempX;}

double** conv(double** x,int Nx,double** y,int Ny)
	{int L=Nx+Ny-1;
	 double** X=new double*[L];
	 double** Y=new double*[L];
	 double** C=new double*[L];
	 double** c=new double*[L];
	 double** in_x=new double*[L];
	 double** in_y=new double*[L];
	 for(int i=0;i<L;i++)
		 {X[i]=new double[2];
		  Y[i]=new double[2];
		  C[i]=new double[2];
		  c[i]=new double[2];
		  in_x[i]=new double[2];
		  in_y[i]=new double[2];}
	 in_x=zeropad(x,Nx,L);
	 in_y=zeropad(y,Ny,L);
	 X=fft(in_x,L);
	 Y=fft(in_y,L);
	 //for(int i=0;i<L;i++)
	 //	{cout<<Y[i][0]<<"\t"<<Y[i][1]<<endl;} 
	 for(int i=0;i<L;i++)
		{C[i][0]=X[i][0]*Y[i][0]-X[i][1]*Y[i][1];
		 C[i][1]=X[i][0]*Y[i][1]+Y[i][0]*X[i][1];}
	 //for(int i=0;i<L;i++)
	//	{cout<<C[i][0]<<"\t"<<C[i][1]<<endl;}
	 c=ifft(C,L);
	 for(int i=0;i<L;i++)
		{delete [] X[i];
		 delete [] Y[i];
		 delete [] C[i];
		 delete [] in_x[i];
		 delete [] in_y[i];}
	 delete [] X;
	 delete [] Y;
	 delete [] C;
	 delete [] in_x;
	 delete [] in_y;
	 return c;}

double** conv(double* x,int Nx,double** y,int Ny)
	{int L=Nx+Ny-1;
	 double** X=new double*[L];
	 double** Y=new double*[L];
	 double** C=new double*[L];
	 double** c=new double*[L];
	 double** in_x=new double*[L];
	 double** in_y=new double*[L];
	 for(int i=0;i<L;i++)
		 {X[i]=new double[2];
		  Y[i]=new double[2];
		  C[i]=new double[2];
		  c[i]=new double[2];
		  in_x[i]=new double[2];
		  in_y[i]=new double[2];}
	 in_x=zeropad(x,Nx,L);
	 in_y=zeropad(y,Ny,L);
	 X=fft(in_x,L);
	 Y=fft(in_y,L);
	 for(int i=0;i<L;i++)
		{C[i][0]=X[i][0]*Y[i][0]-X[i][1]*Y[i][1];
		 C[i][1]=X[i][0]*Y[i][1]+Y[i][0]*X[i][1];}
	 c=ifft(C,L);
	 for(int i=0;i<L;i++)
		{delete [] X[i];
		 delete [] Y[i];
		 delete [] C[i];
		 delete [] in_x[i];
		 delete [] in_y[i];}
	 delete [] X;
	 delete [] Y;
	 delete [] C;
	 delete [] in_x;
	 delete [] in_y;
	 return c;}

double** conv(double** x,int Nx,double* y,int Ny)
	{int L=Nx+Ny-1;
	 double** X=new double*[L];
	 double** Y=new double*[L];
	 double** C=new double*[L];
	 double** c=new double*[L];
	 double** in_x=new double*[L];
	 double** in_y=new double*[L];
	 for(int i=0;i<L;i++)
		 {X[i]=new double[2];
		  Y[i]=new double[2];
		  C[i]=new double[2];
		  c[i]=new double[2];
		  in_x[i]=new double[2];
		  in_y[i]=new double[2];}
	 in_x=zeropad(x,Nx,L);
	 in_y=zeropad(y,Ny,L);
	 X=fft(in_x,L);
	 Y=fft(in_y,L);
	 for(int i=0;i<L;i++)
		{C[i][0]=X[i][0]*Y[i][0]-X[i][1]*Y[i][1];
		 C[i][1]=X[i][0]*Y[i][1]+Y[i][0]*X[i][1];}
	 c=ifft(C,L);
	 for(int i=0;i<L;i++)
		{delete [] X[i];
		 delete [] Y[i];
		 delete [] C[i];
		 delete [] in_x[i];
		 delete [] in_y[i];}
	 delete [] X;
	 delete [] Y;
	 delete [] C;
	 delete [] in_x;
	 delete [] in_y;
	 return c;}

double** conv(double* x,int Nx,double* y,int Ny)
	{int L=Nx+Ny-1;
	 double** X=new double*[L];
	 double** Y=new double*[L];
	 double** C=new double*[L];
	 double** c=new double*[L];
	 double** in_x=new double*[L];
	 double** in_y=new double*[L];
	 for(int i=0;i<L;i++)
		 {X[i]=new double[2];
		  Y[i]=new double[2];
		  C[i]=new double[2];
		  c[i]=new double[2];
		  in_x[i]=new double[2];
		  in_y[i]=new double[2];}
	 in_x=zeropad(x,Nx,L);
	 in_y=zeropad(y,Ny,L);
	 X=fft(in_x,L);
	 Y=fft(in_y,L);
	 for(int i=0;i<L;i++)
		{C[i][0]=X[i][0]*Y[i][0]-X[i][1]*Y[i][1];
		 C[i][1]=X[i][0]*Y[i][1]+Y[i][0]*X[i][1];}
	 c=ifft(C,L);
	 for(int i=0;i<L;i++)
		{delete [] X[i];
		 delete [] Y[i];
		 delete [] C[i];
		 delete [] in_x[i];
		 delete [] in_y[i];}
	 delete [] X;
	 delete [] Y;
	 delete [] C;
	 delete [] in_x;
	 delete [] in_y;
	 return c;}

double** corr_1d(double** x,int Nx,double** y,int Ny)
	{int L=Nx+Ny-1;
	 double** X=new double*[L];
	 double** Xconj=new double*[L];
	 double** Y=new double*[L];
	 double** C=new double*[L];
	 double** c=new double*[L];
	 double** c_out=new double*[L];
	 double** c_shift=new double*[L];
	 double** in_x=new double*[L];
	 double** in_y=new double*[L];
	 for(int i=0;i<L;i++)
		 {X[i]=new double[2];
		  Xconj[i]=new double[2];
		  Y[i]=new double[2];
		  C[i]=new double[2];
		  c[i]=new double[2];
		  c_out[i]=new double[2];
		  c_shift[i]=new double[2];
		  in_x[i]=new double[2];
		  in_y[i]=new double[2];}
	 in_x=zeropad(x,Nx,L);
	 in_y=zeropad(y,Ny,L);
	 X=fft(in_x,L);
	 Y=fft(in_y,L);	
	 // Create Complex Conjugate
	 for(int i=0;i<L;i++)
		{Xconj[i][0]=X[i][0];
		 Xconj[i][1]=-X[i][1];}
	 // Multiplication of Conjugate Pair in Freq Domain = Correlation in Time Domain 
	 for(int i=0;i<L;i++)
		{C[i][0]=Y[i][0]*Xconj[i][0]-Y[i][1]*Xconj[i][1];
		 C[i][1]=Y[i][0]*Xconj[i][1]+Xconj[i][0]*Y[i][1];}
	 // Inverse DFT
	 c=ifft(C,L);	 
	 // Apply Shift to order data in terms of increasing lag
	 if(Nx>=Ny)
		{c_shift=circShift(c,L,Nx);}
	 else if(Ny>Nx)
		{c_shift=circShift(c,L,Ny);}
	 // Reverse order of elements in array
	 c_out=reverse(c_shift,L);	
	 for(int i=0;i<L;i++)
		{delete [] X[i];
		 delete [] Xconj[i];
		 delete [] Y[i];
		 delete [] C[i];
		 delete [] c[i];
	 	 delete [] c_shift[i];
		 delete [] in_x[i];
		 delete [] in_y[i];}
	  delete [] X;
	  delete [] Xconj;
	  delete [] Y;
	  delete [] C;
	  delete [] c;
	  delete [] c_shift;
	  delete [] in_x;
	  delete [] in_y;
	  return c_out;}

double** corr_1d(double* x,int Nx,double* y,int Ny)
	{int L=Nx+Ny-1;
	 double** X=new double*[L];
	 double** Xconj=new double*[L];
	 double** Y=new double*[L];
	 double** C=new double*[L];
	 double** c=new double*[L];
	 double** c_out=new double*[L];
	 double** c_shift=new double*[L];
	 double** in_x=new double*[L];
	 double** in_y=new double*[L];
	 for(int i=0;i<L;i++)
		 {X[i]=new double[2];
		  Xconj[i]=new double[2];
		  Y[i]=new double[2];
		  C[i]=new double[2];
		  c[i]=new double[2];
		  c_out[i]=new double[2];
		  c_shift[i]=new double[2];
		  in_x[i]=new double[2];
		  in_y[i]=new double[2];}
	 in_x=zeropad(x,Nx,L);
	 in_y=zeropad(y,Ny,L);
	 X=fft(in_x,L);
	 Y=fft(in_y,L);	
	 // Create Complex Conjugate
	 for(int i=0;i<L;i++)
		{Xconj[i][0]=X[i][0];
		 Xconj[i][1]=-X[i][1];}
	 // Multiplication of Conjugate Pair in Freq Domain = Correlation in Time Domain 
	 for(int i=0;i<L;i++)
		{C[i][0]=Y[i][0]*Xconj[i][0]-Y[i][1]*Xconj[i][1];
		 C[i][1]=Y[i][0]*Xconj[i][1]+Xconj[i][0]*Y[i][1];}
	 //for(int i=0;i<L;i++)
	//	{cout<<C[i][0]<<"\t"<<C[i][1]<<endl;}	 
	 // Inverse DFT
	 c=ifft(C,L);	 
	 //for(int i=0;i<L;i++)
	 //	{cout<<c[i][0]<<"\t"<<c[i][1]<<endl;}
	 // Apply Shift to order data in terms of increasing lag
	 if(Nx>=Ny)
		{c_shift=circShift(c,L,Nx);}
	 else if(Ny>Nx)
		{c_shift=circShift(c,L,Ny);}
	 // Reverse order of elements in array
	 c_out=reverse(c_shift,L);
	 for(int i=0;i<L;i++)
		{delete [] X[i];
		 delete [] Xconj[i];
		 delete [] Y[i];
		 delete [] C[i];
		 delete [] c[i];
		 delete [] c_shift[i];
		 delete [] in_x[i];
		 delete [] in_y[i];}
	  delete [] X;
	  delete [] Xconj;
	  delete [] Y;
	  delete [] C;
	  delete [] c;
	  delete [] c_shift;	  
	  delete [] in_x;
	  delete [] in_y;
	  return c_out;}

double** cov(double** x,int numRows,int numCols,double** y)
	{double** output=new double*[numRows];
	for(int i=0;i<numRows;i++)
		{output[i]=new double[numCols];}

	double avgX,avgY,tmp;
	for(int i=0;i<numCols;i++)
		{for(int j=i;j<numCols;j++)
			{for(int k=0;k<numRows;k++)
				{tmp+=(x[k][i]-avgX)*(y[k][j]-avgY);}
           output[i][j]=tmp; // matrix
           output[j][i]=tmp;}}

	return output;}

double** fft(double* Data,int Nx)
	{// Declare Variables
	 const double PI=2*asin(1);
	 fftw_complex *x,*X;
	 fftw_plan plan;
	 int N=Nx;	

	 // Allocate Memory for Input Array
	 x=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);	 	 
	 // Allocate Memory for Output Array
	 X=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
	 // Create 3D FFT Plan (Must create plan before initializing the input array!!!)
	 plan=fftw_plan_dft_1d(Nx,x,X,FFTW_FORWARD,FFTW_ESTIMATE);

	 // With Plan Created Define Input Array
	 for(int i=0;i<Nx;i++)
		{// Real Component
	 	 x[i][0]=Data[i];
	 	 // Complex Component
	 	 x[i][1]=0;}

	 // Execute FFT
	 fftw_execute(plan);		 
	 // Output Data
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // The synthesis equation (i.e. the inverse DFT) does not normalize to the array length, so it must be done manually
	 for(int i=0;i<N;i++)
		{output[i][0]=X[i][0];
		 output[i][1]=X[i][1];}
	
	 // Free Memory Occupied by FFT plan and Data Arrays
	 fftw_destroy_plan(plan);
	 fftw_free(x);
	 fftw_free(X);
	 return output;}

double** fft(double** Data,int Nx)
	{// Declare Variables
	 const double PI=2*asin(1);
	 fftw_complex *x,*X;
	 fftw_plan plan;
	 int N=Nx;	

	 // Allocate Memory for Input Array
	 x=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);	 	 
	 // Allocate Memory for Output Array
	 X=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
	 // Create 3D FFT Plan (Must create plan before initializing the input array!!!)
	 plan=fftw_plan_dft_1d(Nx,x,X,FFTW_FORWARD,FFTW_ESTIMATE);

	 // With Plan Created Define Input Array
	 for(int i=0;i<Nx;i++)
		{// Real Component
	 	 x[i][0]=Data[i][0];
	 	 // Complex Component
	 	 x[i][1]=Data[i][1];}	

	 // Execute FFT
	 fftw_execute(plan);		 
	 // Output Data
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // The synthesis equation (i.e. the inverse DFT) does not normalize to the array length, so it must be done manually
	 for(int i=0;i<N;i++)
		{output[i][0]=X[i][0];
		 output[i][1]=X[i][1];}
	
	 // Free Memory Occupied by FFT plan and Data Arrays
	 fftw_destroy_plan(plan);
	 fftw_free(x);
	 fftw_free(X);
	 return output;}

double** fft2(double* Data,int Nx,int Ny)
	{// Declare Variables
	 const double PI=2*asin(1);
	 fftw_complex *x,*X;
	 fftw_plan plan;
	 int N=Nx*Ny;	

	 // Allocate Memory for Input Array
	 x=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);	 	 
	 // Allocate Memory for Output Array
	 X=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
	 // Create 3D FFT Plan (Must create plan before initializing the input array!!!)
	 plan=fftw_plan_dft_2d(Nx,Ny,x,X,FFTW_FORWARD,FFTW_ESTIMATE);

	 // With Plan Created Define Input Array
	 for(int i=0;i<N;i++)
		{// Real Component
		 x[i][0]=Data[i];
		 // Complex Component
		 x[i][1]=0;}

	 // Execute FFT
	 fftw_execute(plan);		 
	 // Output Data
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // The synthesis equation (i.e. the inverse DFT) does not normalize to the array length, so it must be done manually
	 for(int i=0;i<N;i++)
		{output[i][0]=X[i][0];
		 output[i][1]=X[i][1];}
	
	 // Free Memory Occupied by FFT plan and Data Arrays
	 fftw_destroy_plan(plan);
	 fftw_free(x);
	 fftw_free(X);
	 return output;}

double** fft2(double** Data,int Nx,int Ny)
	{// Declare Variables
	 const double PI=2*asin(1);
	 fftw_complex *x,*X;
	 fftw_plan plan;
	 int N=Nx*Ny;	

	 // Allocate Memory for Input Array
	 x=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);	 	 
	 // Allocate Memory for Output Array
	 X=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
	 // Create 3D FFT Plan (Must create plan before initializing the input array!!!)
	 plan=fftw_plan_dft_2d(Nx,Ny,x,X,FFTW_FORWARD,FFTW_ESTIMATE);

	 // With Plan Created Define Input Array
	 	 // With Plan Created Define Input Array
	 for(int i=0;i<N;i++)
		{// Real Component
		 x[i][0]=Data[i][0];
		 // Complex Component
		 x[i][1]=Data[i][1];}

	 // Execute FFT
	 fftw_execute(plan);		 
	 // Output Data
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // The synthesis equation (i.e. the inverse DFT) does not normalize to the array length, so it must be done manually
	 for(int i=0;i<N;i++)
		{output[i][0]=X[i][0];
		 output[i][1]=X[i][1];}
	
	 // Free Memory Occupied by FFT plan and Data Arrays
	 fftw_destroy_plan(plan);
	 fftw_free(x);
	 fftw_free(X);
	 return output;}

double** fft3(double* Data,int Nx,int Ny,int Nz)
	{// Declare Variables
	 const double PI=2*asin(1);
	 fftw_complex *x,*X;
	 fftw_plan plan;
	 //int Nx=Data.nx;
	 //int Ny=Data.ny;
	 //int Nz=Data.nz;
	 //cout<<Nx<<" "<<Ny<<" "<<Nz<<" "<<Nx*Ny*Nz<<endl;
	 int N=Nx*Ny*Nz;	

	 // Allocate Memory for Input Array
	 x=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);	 	 
	 // Allocate Memory for Output Array
	 X=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
	 // Create 3D FFT Plan (Must create plan before initializing the input array!!!)
	 plan=fftw_plan_dft_3d(Nx,Ny,Nz,x,X,FFTW_FORWARD,FFTW_ESTIMATE);

	 // With Plan Created Define Input Array
	 for(int i=0;i<N;i++)
		{// Real Component
	 	 x[i][0]=Data[i];
	 	 // Complex Component
	 	 x[i][1]=0;}

	 // Execute FFT
	 fftw_execute(plan);		 
	 // Output Data
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // The synthesis equation (i.e. the inverse DFT) does not normalize to the array length, so it must be done manually
	 for(int i=0;i<N;i++)
		{output[i][0]=X[i][0];
		 output[i][1]=X[i][1];}
	
	 // Free Memory Occupied by FFT plan and Data Arrays
	 fftw_destroy_plan(plan);
	 fftw_free(x);
	 fftw_free(X);
	 return output;}

double** fft3(double** Data,int Nx,int Ny,int Nz)
	{// Declare Variables
	 const double PI=2*asin(1);
	 fftw_complex *x,*X;
	 fftw_plan plan;
	 //int Nx=Data.nx;
	 //int Ny=Data.ny;
	 //int Nz=Data.nz;
	 //cout<<Nx<<" "<<Ny<<" "<<Nz<<" "<<Nx*Ny*Nz<<endl;
	 int N=Nx*Ny*Nz;	

	 // Allocate Memory for Input Array
	 x=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);	 	 
	 // Allocate Memory for Output Array
	 X=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
	 // Create 3D FFT Plan (Must create plan before initializing the input array!!!)
	 plan=fftw_plan_dft_3d(Nx,Ny,Nz,x,X,FFTW_FORWARD,FFTW_ESTIMATE);

	 // With Plan Created Define Input Array
	 for(int i=0;i<N;i++)
		{// Real Component
	 	 x[i][0]=Data[i][0];
	 	 // Complex Component
	 	 x[i][1]=Data[i][1];}

	 // Execute FFT
	 fftw_execute(plan);		 
	 // Output Data
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // The synthesis equation (i.e. the inverse DFT) does not normalize to the array length, so it must be done manually
	 for(int i=0;i<N;i++)
		{output[i][0]=X[i][0];
		 output[i][1]=X[i][1];}
	
	 // Free Memory Occupied by FFT plan and Data Arrays
	 fftw_destroy_plan(plan);
	 fftw_free(x);
	 fftw_free(X);
	 return output;}

double** ifft(double** Data,int Nx)
	{// Declare Variables
	 const double PI=2*asin(1);
	 fftw_complex *x,*X;
	 fftw_plan plan;
	 int N=Nx;	

	 // Allocate Memory for Input Array
	 x=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);	 	 
	 // Allocate Memory for Output Array
	 X=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
	 // Create 3D FFT Plan (Must create plan before initializing the input array!!!)
	 plan=fftw_plan_dft_1d(Nx,X,x,FFTW_BACKWARD,FFTW_ESTIMATE);

	 // With Plan Created Define Input Array
	 for(int i=0;i<Nx;i++)
		{// Real Component
	 	 X[i][0]=Data[i][0];
	 	 // Complex Component
	 	 X[i][1]=Data[i][1];}

	 // Execute FFT
	 fftw_execute(plan);		 
	 // Output Data
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // The synthesis equation (i.e. the inverse DFT) does not normalize to the array length, so it must be done manually
	 for(int i=0;i<N;i++)
		{output[i][0]=x[i][0]/N;
		 output[i][1]=x[i][1]/N;}
	
	 // Free Memory Occupied by FFT plan and Data Arrays
	 fftw_destroy_plan(plan);
	 fftw_free(x);
	 fftw_free(X);
	 return output;}

double** ifft2(double** Data,int Nx,int Ny)
	{// Declare Variables
	 const double PI=2*asin(1);
	 fftw_complex *x,*X;
	 fftw_plan plan;
	 int N=Nx*Ny;	

	 // Allocate Memory for Input Array
	 x=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);	 	 
	 // Allocate Memory for Output Array
	 X=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
	 // Create 3D FFT Plan (Must create plan before initializing the input array!!!)
	 plan=fftw_plan_dft_2d(Nx,Ny,X,x,FFTW_BACKWARD,FFTW_ESTIMATE);

	 // With Plan Created Define Input Array
	 int index=0,Count=0;
	 for(int i=0;i<Nx;i++)
		{for(int j=0;j<Ny;j++)
			{index=i*Ny+j;
			 if(i<Nx && j<Ny)
				{// Real Component
			 	 X[index][0]=Data[Count][0];
			 	 // Complex Component
			 	 X[index][1]=Data[Count][1];
				 Count++;}
			 else
				{// Zero-Pad
				 X[index][0]=0;
				 X[index][1]=0;}}}

	 // Execute FFT
	 fftw_execute(plan);		 
	 // Output Data
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // The synthesis equation (i.e. the inverse DFT) does not normalize to the array length, so it must be done manually
	 for(int i=0;i<N;i++)
		{output[i][0]=x[i][0]/N;
		 output[i][1]=x[i][1]/N;}
	
	 // Free Memory Occupied by FFT plan and Data Arrays
	 fftw_destroy_plan(plan);
	 fftw_free(x);
	 fftw_free(X);
	 return output;}

double** ifft3(double** Data,int Nx,int Ny,int Nz)
	{// Declare Variables
	 const double PI=2*asin(1);
	 fftw_complex *x,*X;
	 fftw_plan plan;
	 int N=Nx*Ny*Nz;	

	 // Allocate Memory for Input Array
	 x=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);	 	 
	 // Allocate Memory for Output Array
	 X=(fftw_complex*) fftw_malloc(sizeof(fftw_complex)*N);
	 // Create 3D FFT Plan (Must create plan before initializing the input array!!!)
	 plan=fftw_plan_dft_3d(Nx,Ny,Nz,X,x,FFTW_BACKWARD,FFTW_ESTIMATE);

	 // With Plan Created Define Input Array
	 int index=0,Count=0;
	 for(int i=0;i<Nx;i++)
		{for(int j=0;j<Ny;j++)
			{for(int k=0;k<Nz;k++)				
				{index=i*Ny*Nz+j*Nz+k;
				 if(i<Nx && j<Ny && k<Nz)
					{// Real Component
				 	 X[index][0]=Data[Count][0];
				 	 // Complex Component
				 	 X[index][1]=Data[Count][1];
					 Count++;}
				 else
					{// Zero-Pad
					 X[index][0]=0;
					 X[index][1]=0;}}}}

	 // Execute FFT
	 fftw_execute(plan);		 
	 // Output Data
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // The synthesis equation (i.e. the inverse DFT) does not normalize to the array length, so it must be done manually
	 for(int i=0;i<N;i++)
		{output[i][0]=x[i][0]/N;
		 output[i][1]=x[i][1]/N;}
	
	 // Free Memory Occupied by FFT plan and Data Arrays
	 fftw_destroy_plan(plan);
	 fftw_free(x);
	 fftw_free(X);
	 return output;}

double mean(double* Data,int Sz)
	{double output=0;
	for(int i=0;i<Sz;i++)
		{output+=Data[i];}
	output/=Sz;
	return output;}

double** reverse(double** input,int Nx)
	{double** output=new double*[Nx];
	 for(int i=0;i<Nx;i++)
		{output[i]=new double[2];}
	 // Reverse Order Input Array
	 for(int i=0;i<Nx;i++)
		{output[(Nx-1)-i][0]=input[i][0];
		 output[(Nx-1)-i][1]=input[i][1];}
	 return output;}

double** zeropad(double** input,int Nx,int newNx)
	{int index=0,Count=0;
	 double** output=new double*[newNx];
	 for(int i=0;i<newNx;i++)
		{output[i]=new double[2];
		 index=i;
		 if(i<Nx)
			{// Real Component
		 	 output[index][0]=input[Count][0];
		 	 // Complex Component
		 	 output[index][1]=input[Count][1];
			 Count++;}
		 else
			{// Zero-Pad
			 output[index][0]=0;
			 output[index][1]=0;}}
	 return output;}

double** zeropad(double* input,int Nx,int newNx)
	{int index=0,Count=0;
	 double** output=new double*[newNx];
	 for(int i=0;i<newNx;i++)
		{output[i]=new double[2];
		 index=i;
		 if(i<Nx)
			{// Real Component
		 	 output[index][0]=input[Count];
		 	 // Complex Component
		 	 output[index][1]=0;
			 Count++;}
		 else
			{// Zero-Pad
			 output[index][0]=0;
			 output[index][1]=0;}}
	 return output;}

double** zeropad2(double** input,int Nx,int Ny,int newNx,int newNy)
	{int N=newNx*newNy;
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // With Plan Created Define Input Array
	 int index=0,Count=0;
	 for(int i=0;i<newNx;i++)
		{for(int j=0;j<newNy;j++)
			{index=i*newNy+j;
			 if(i<Nx && j<Ny)
				{// Real Component
			 	 output[index][0]=input[Count][0];
			 	 // Complex Component
			 	 output[index][1]=input[Count][1];
				 Count++;}
			 else
				{// Zero-Pad
				 output[index][0]=0;
				 output[index][1]=0;}}}
	 return output;}

double** zeropad2(double* input,int Nx,int Ny,int newNx,int newNy)
	{int N=newNx*newNy;
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // With Plan Created Define Input Array
	 int index=0,Count=0;
	 for(int i=0;i<newNx;i++)
		{for(int j=0;j<newNy;j++)
			{index=i*newNy+j;
			 if(i<Nx && j<Ny)
				{// Real Component
			 	 output[index][0]=input[Count];
			 	 // Complex Component
			 	 output[index][1]=0;
				 Count++;}
			 else
				{// Zero-Pad
				 output[index][0]=0;
				 output[index][1]=0;}}}
	 return output;}

double** zeropad3(double** input,int Nx,int Ny,int Nz,int newNx,int newNy,int newNz)
	{int N=newNx*newNy*newNz;
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // With Plan Created Define Input Array
	 int index=0,Count=0;
	 for(int i=0;i<newNx;i++)
		{for(int j=0;j<newNy;j++)
			{for(int k=0;k<newNz;k++)				
				{index=i*newNy*newNz+j*newNz+k;
				 if(i<Nx && j<Ny && k<Nz)
					{// Real Component
				 	 output[index][0]=input[Count][0];
				 	 // Complex Component
				 	 output[index][1]=input[Count][1];
					 Count++;}
				 else
					{// Zero-Pad
					 output[index][0]=0;
					 output[index][1]=0;}}}}
	 return output;}

double** zeropad3(double* input,int Nx,int Ny,int Nz,int newNx,int newNy,int newNz)
	{int N=newNx*newNy*newNz;
	 double** output=new double*[N];
	 for(int i=0;i<N;i++)
		{output[i]=new double[2];}
	 // With Plan Created Define Input Array
	 int index=0,Count=0;
	 for(int i=0;i<newNx;i++)
		{for(int j=0;j<newNy;j++)
			{for(int k=0;k<newNz;k++)				
				{index=i*newNy*newNz+j*newNz+k;
				 if(i<Nx && j<Ny && k<Nz)
					{// Real Component
				 	 output[index][0]=input[Count];
				 	 // Complex Component
				 	 output[index][1]=0;
					 Count++;}
				 else
					{// Zero-Pad
					 output[index][0]=0;
					 output[index][1]=0;}}}}
	 return output;}

