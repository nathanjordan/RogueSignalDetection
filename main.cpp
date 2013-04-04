#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <complex>
#include <fftw3.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include <map>

#define PI 3.14159265359
#define BLOCK_SIZE 4096
#define SIGNAL_THRESHOLD 10

typedef char byte;
typedef double fftw_real;

struct transmission
{
	float frequency;

	int timeStart;

	int timeEnd;

};

int main( int argc , char** argv )
{
	
	std::string filename = std::string( argv[1] );

	std::ifstream f;

	struct stat filestatus;

	stat( filename.c_str() , &filestatus );

	size_t filesize = filestatus.st_size;

	f.open( filename.c_str() , std::ios::in | std::ios::binary );

	if( !f.good() )
	{
	
		std::cerr << "Can't open file" << std::endl;

		exit( 1 );

	}

	fftw_real* original = new fftw_real[ filesize ];

	fftw_complex* result = new fftw_complex[ filesize ];

	for( unsigned int i = 0 ; i < filesize ; i++ )
	{
	
		original[i] = (float) (byte) f.get();

	}

	f.close();

	//create a new input for the windowing function
	fftw_real* windowed = new fftw_real[ filesize ];
	
	float* resultScaled = new float[ BLOCK_SIZE / 2 ];

	float* resultScaledOld = new float[ BLOCK_SIZE / 2 ];
	
	float* difference = new float[ BLOCK_SIZE / 2 ];

	std::map< int ,  struct transmission > spikes;

	for( unsigned int j = 0 ; j < filesize - BLOCK_SIZE  ; j += BLOCK_SIZE )
	{
		
		int fft_size = 0, fft_size_old;

		//if( j > filesize )
		//{
			
		//	fft_size = filesize % BLOCK_SIZE;

		//	memcpy( windowed , original + j * BLOCK_SIZE * sizeof( fftw_real ) , fft_size * sizeof( fftw_real ) );
		
		//}

		//else
		//{
			
		fft_size = BLOCK_SIZE;

		memcpy( windowed , original + j * BLOCK_SIZE * sizeof( fftw_real ) , BLOCK_SIZE * sizeof( fftw_real ) );
		
		//}
		
		//prepare the FFT
		fftw_plan p = fftw_plan_dft_r2c_1d( fft_size , windowed , result , FFTW_ESTIMATE );

		//Run the FFT
		fftw_execute( p );

		//calculate amplitude of first N/2 bins (Nyquist Limit?)
		for( unsigned int i = 0 ; i < BLOCK_SIZE / 2 ; i++ )
		{

			//get magnitude using complex conjugate
			resultScaled[ i ] = sqrt( result[ i ][ 0 ] * result[ i ][ 0 ] + result[ i ][ 1 ] * result[ i ][ 1 ] );

			//put into decibels
			//resultScaled[ i ] = 20 * log10( resultScaled[ i ] );

			difference[ i ] = 20.0f * log10( resultScaled[ i ] - resultScaledOld[ i ] );
	
			if( difference[ i ] > SIGNAL_THRESHOLD )
			{
				
				struct transmission trans;

				trans.frequency = i;

				trans.timeStart = j;

				spikes.insert( new std::pair<int, transmission>( i ,  ) );

			}
		}

		memcpy( resultScaledOld , resultScaled , fft_size * sizeof( fftw_complex ) );

		fft_size_old = fft_size;
	
	}

	std::ofstream fout;

	fout.open( "out.csv" );

	for( unsigned int i = 0 ; i < filesize / 2 ; i++ )
	{

		fout << resultScaled[ i ] << std::endl;

	}

	return 0;

}
