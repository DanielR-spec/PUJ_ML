## =========================================================================
## @author Leonardo Florez-Valencia (florez-l@javeriana.edu.co)
## =========================================================================

import argparse, numpy, os, random, sys
sys.path.append( os.path.join( os.getcwd( ), '../../lib/python3' ) )

import PUJ.Model.Linear, PUJ.Regression.MSE
import PUJ.Data.Normalize, PUJ.Optimizer.GradientDescent
import PUJ.Debug.Polynomial

## -- Parse command line arguments
parser = argparse.ArgumentParser( )
parser.add_argument( 'filename', type = str )
parser.add_argument( '-a', '--learning_rate', type = float )
parser.add_argument( '-I', '--max_iterations', type = int )
parser.add_argument( '-D', '--debug_step', type = int )
parser.add_argument( '-e', '--epsilon', type = float )
parser.add_argument( '--init', type = str, choices = [ 'zeros', 'ones', 'random' ] )
args = parser.parse_args( )

lr = 1e-2
I = 1000
e = 1e-8
d = 10
init = 'zeros'

if args.learning_rate != None: lr = args.learning_rate
if args.max_iterations != None: I = args.max_iterations
if args.debug_step != None: d = args.debug_step
if args.epsilon != None: e = args.epsilon
if args.init != None: init = args.init

# -- Data
reader = PUJ.Data.Reader( 0.5 )
D = reader.FromCSV( args.filename, output_size = 1, shuffle = True )
x0 = D[ 0 ]
y0 = D[ 1 ]

# -- Initial parameters
if init == 'zeros':
  init_theta = numpy.zeros( x0.shape[ 1 ] + 1 )
elif init == 'ones':
  init_theta = numpy.ones( x0.shape[ 1 ] + 1 )
else:
  init_theta = \
    numpy.random.uniform( low = -1.0, high = 1.0, size = x0.shape[ 1 ] + 1 )
# end if

# -- Prepare regression
cost = PUJ.Regression.MSE( x0, y0 )

# -- Analitical solution
tA = cost.AnalyticSolve( )

# -- Prepare debug
debug = PUJ.Debug.Polynomial( x0[ : , 0 ], y0 )

# -- Iterative solution
tI, nI = PUJ.Optimizer.GradientDescent(
  cost,
  learning_rate = lr,
  init_theta = init_theta,
  max_iter = I,
  epsilon = e,
  debug_step = d,
  debug_function = debug
  )

print( '=================================================================' )
print( '* Analitical solution  :', tA )
print( '* Iterative solution   :', tI )
print(
  '* Difference           :',
  ( ( tI - tA ) @ ( tI - tA ).T )[ 0, 0 ] ** 0.5
  )
print( '* Number of iterations :', nI )
print( '=================================================================' )

debug.KeepFigures( )

## eof - $RCSfile$