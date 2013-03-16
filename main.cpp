#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

typedef char byte;

int main( int argc , char** argv )
{
	
	std::string filename = std::string( argv[1] );

	std::ifstream f;

	f.open( filename.c_str() , std::ios::in | std::ios::binary );
		
	f.seekg( 0 , std::ios::end );
	
	size_t filesize = f.tellg();



	if( !f.good() )
	{
	
		std::cerr << "Cant open file" << std::endl;

		exit( 1 );

	}

	std::vector< char > data;

	while( f.good() )
	{
	
		data.push_back( f.get() );  

	}
	
	std::cout << data.size() << std::endl;

	return 0;

}
