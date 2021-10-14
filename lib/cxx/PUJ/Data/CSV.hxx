// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================
#ifndef __PUJ__Data__CSV__hxx__
#define __PUJ__Data__CSV__hxx__

#include <filesystem>
#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>

// -------------------------------------------------------------------------
template< class _TMatrix >
_TMatrix PUJ::CSV::Read(
  const std::string& fname,
  bool ignore_first_row,
  const std::string& separator
  )
{
  // Load buffer
  std::FILE* ifs = std::fopen( fname.c_str( ), "r" );
  std::fseek( ifs, 0, SEEK_END );
  std::size_t size = std::ftell( ifs );

  char* buffer = new char[ size ];
  std::rewind( ifs );
  std::fread( buffer, sizeof( char ), size, ifs );
  std::istringstream input( buffer );

  // Read line by line
  std::vector< std::stringstream > lines;
  std::string line;
  unsigned long long n = 0;
  while( std::getline( input, line ) )
  {
    std::vector< std::string > tokens;
    boost::split( tokens, line, boost::is_any_of( separator ) );
    n = ( n < tokens.size( ) )? tokens.size( ): n;

    lines.push_back( std::stringstream( ) );
    for( const std::string& t: tokens )
      *lines.rbegin( ) << t << " ";
  } // end while

  // Pass to Eigen::Matrix
  unsigned long long m = lines.size( );
  if( ignore_first_row )
    m--;
  _TMatrix data = _TMatrix::Zero( m, n );
  for( unsigned long long r = 0; r < m; ++r )
    for( unsigned long long c = 0; c < n; ++c )
      lines[ r ] >> data( r, c );

  // Finish
  delete [] buffer;
  return( data );
}

#endif // __PUJ__Data__CSV__hxx__

// eof - $RCSfile$
