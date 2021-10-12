// =========================================================================
// @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
// =========================================================================

#include <cmath>
#include <iostream>

#include <PUJ/Data/Algorithms.h>
#include <PUJ/Model/Linear.h>
#include <PUJ/Optimizer/GradientDescent.h>

// -- Typedef
using TScalar    = double;
using TModel     = PUJ::Model::Linear< TScalar >;
using TOptimizer = PUJ::Optimizer::GradientDescent< TModel >;

// -------------------------------------------------------------------------
bool debug( unsigned long long i, TScalar J, bool show )
{
  if( show )
    std::cout << i << " " << J << std::endl;
  return( false );
}

// -------------------------------------------------------------------------
int main( int argc, char** argv )
{
  TScalar min_v = -10;
  TScalar max_v =  10;
  unsigned long long m = 10;
  TScalar dif_v = ( max_v - min_v ) / TScalar( m - 1 );

  TModel real_model( 2, 10 );
  unsigned long long n = real_model.GetDimensions( );

  TModel::TMatrix X_real =
    TModel::TMatrix::NullaryExpr(
      m, n,
      [=]( TModel::TMatrix::Index i ) -> TScalar
      {
        return(
          std::pow( ( dif_v * TScalar( i % m ) ) + min_v, 1 + ( i / m ) )
          );
      }
      );
  PUJ::Algorithms::Shuffle( X_real );
  TModel::TCol y_real = real_model( X_real );

  TModel opt_model;
  opt_model.Init( n, PUJ::Random );
  TModel::Cost J( &opt_model, X_real, y_real );
  TOptimizer opt( &J );
  opt.SetAlpha( 1e-4 );
  opt.SetMaximumNumberOfIterations( 100000 );
  opt.SetDebugIterations( 1000 );
  opt.SetDebug( debug );
  opt.Fit( );

  TModel::TCol y_diff = y_real - opt_model( X_real );

  std::cout << "=======================================" << std::endl;
  std::cout << "Real model      : " << real_model << std::endl;
  std::cout << "Optimized model : " << opt_model << std::endl;
  std::cout << "Iterations      : " << opt.GetRealIterations( ) << std::endl;
  std::cout << "Difference      : " << y_diff.norm( ) << std::endl;
  std::cout << "=======================================" << std::endl;

  return( EXIT_SUCCESS );
}

// eof - $RCSfile$
