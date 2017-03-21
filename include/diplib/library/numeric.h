/*
 * DIPlib 3.0
 * This file contains numeric algorithms unrelated to images.
 *
 * (c)2015-2017, Cris Luengo.
 * Based on original DIPlib code: (c)1995-2014, Delft University of Technology.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


//
// NOTE!
// This file is included through diplib.h -- no need to include directly
//


#ifndef DIP_NUMERIC_H
#define DIP_NUMERIC_H

#include <cmath>
#include <algorithm>

#include "diplib/library/types.h"
#include "diplib/library/sample_iterator.h"


/// \file
/// \brief Numeric algorithms and constants unrelated to images. This file is always included through `diplib.h`.
/// \see numeric


namespace dip {


/// \defgroup numeric Numeric algorithms and constants
/// \ingroup infrastructure
/// \brief Functions and constants to be used in numeric computation, unrelated to images.
///
/// These functions and constants are made available when including `diplib.h`.
/// \{


/// \brief The constant pi.
constexpr dfloat pi = 3.14159265358979323846264338327950288;
// std::acos( -1 ) is the good way of definig pi, but it's not constexpr.

/// \brief Compute the greatest common denominator of two positive integers.
// `std::gcd` will be available in C++17.
inline dip::uint gcd( dip::uint a, dip::uint b ) {
   return b == 0 ? a : gcd( b, a % b );
}


/// \brief Integer division, return ceil.
inline dip::uint div_ceil( dip::uint lhs, dip::uint rhs ) {
   if( lhs * rhs == 0 ) {
      return 0;
   }
   return ( lhs - 1 ) / rhs + 1;
}

/// \brief Integer division, return ceil.
inline dip::sint div_ceil( dip::sint lhs, dip::sint rhs ) {
   if( lhs * rhs == 0 ) {
      return 0;
   }
   if( lhs * rhs < 0 ) {
      return lhs / rhs;
   } else {
      if( lhs < 0 ) {
         return ( lhs + 1 ) / rhs + 1;
      } else {
         return ( lhs - 1 ) / rhs + 1;
      }
   }
}

/// \brief Integer division, return floor.
inline dip::uint div_floor( dip::uint lhs, dip::uint rhs ) {
   if( lhs * rhs == 0 ) {
      return 0;
   }
   return lhs / rhs;
}

/// \brief Integer division, return floor.
inline dip::sint div_floor( dip::sint lhs, dip::sint rhs ) {
   if( lhs * rhs == 0 ) {
      return 0;
   }
   if( lhs * rhs < 0 ) {
      if( lhs < 0 ) {
         return ( lhs + 1 ) / rhs - 1;
      } else {
         return ( lhs - 1 ) / rhs - 1;
      }
   } else {
      return lhs / rhs;
   }
}

/// \brief Integer division, return rounded.
inline dip::uint div_round( dip::uint lhs, dip::uint rhs ) {
   return div_floor( lhs + rhs / 2, rhs );
}

/// \brief Integer division, return rounded.
inline dip::sint div_round( dip::sint lhs, dip::sint rhs ) {
   return div_floor( lhs + rhs / 2, rhs );
}


/// \brief Clamps a value between a min and max value (a.k.a. clip, saturate, etc.).
// `std::clamp` will be available in C++17.
template< typename T >
constexpr inline const T& clamp( const T& v, const T& lo, const T& hi ) {
   return std::min( std::max( v, lo ), hi );
}

/// \brief Computes integer powers of 10, assuming the power is relatively small.
inline dfloat pow10( dip::sint power ) {
   switch( power ) {
      case -6: return 1e-6;
      case -5: return 1e-5;
      case -4: return 1e-4;
      case -3: return 1e-3;
      case -2: return 1e-2;
      case -1: return 1e-1;
      case  0: return 1;
      case  1: return 1e1;
      case  2: return 1e2;
      case  3: return 1e3;
      case  4: return 1e4;
      case  5: return 1e5;
      case  6: return 1e6;
      default:
         if( power > 0 ) {
            return 1e6 * pow10( power - 6 );
         } else {
            return 1e-6 * pow10( power + 6 );
         }
   }
}

/// \brief Computes the Bessel function J of the order 0.
dfloat BesselJ0( dfloat x );

/// \brief Computes the Bessel function J of the order 1.
dfloat BesselJ1( dfloat x );

/// \brief Computes the Bessel function J of the order `n`.
dfloat BesselJN( dfloat x, dip::uint n );

/// \brief Computes the Bessel function Y of the order 0.
dfloat BesselY0( dfloat x );

/// \brief Computes the Bessel function Y of the order 1.
dfloat BesselY1( dfloat x );

/// \brief Computes the Bessel function Y of the order `n`.
dfloat BesselYN( dfloat x, dip::uint n );

/// \brief Computes the natural logarithm of the gamma function.
dfloat LnGamma( dfloat value );

/// \brief Computes the incomplete gamma function.
dfloat GammaP( dfloat a, dfloat x );

/// \brief Computes the complementary incomplete gamma function.
dfloat GammaQ( dfloat a, dfloat x );

/// \brief Computes the error function.
inline dfloat Erf( dfloat x ) {
   return ( x < 0.0 ? -GammaP( 0.5, x * x ) : GammaP( 0.5, x * x ));
}

/// \brief Computes the complementary error function.
inline dfloat Erfc( dfloat x ) {
   return ( x < 0.0 ? 1.0 + GammaP( 0.5, x * x ) : GammaQ( 0.5, x * x ));
}

/// \brief Computes the sinc function.
inline dfloat Sinc( dfloat x ) {
   return x == 0.0 ? 1.0 : std::sin( x ) / x;
}

/// \brief Finds the eigenvalues and eigenvectors of a symmetric real matrix.
///
/// `input` is a pointer to `n*n` values, in column-major order; only the lower triangle will be used.
///
/// `lambdas` is a pointer to space for `n` values, which will be written sorted largest to smallest.
///
/// `vectors` is a pointer to space for `n*n` values and will receive the `n` eigenvectors. The eigenvectors
/// can be accessed at `&vectors[ 0 ]`, `&vectors[ n ]`, `&vectors[ 2*n ]`, etc.
/// If `vectors` is `false`, no eigenvectors are computed.
void SymmetricEigenDecomposition(
      dip::uint n,
      ConstSampleIterator< dfloat > input,
      SampleIterator< dfloat > lambdas,
      SampleIterator< dfloat > vectors = nullptr
);

/// \brief Finds the eigenvalues and eigenvectors of a symmetric real matrix, where only the unique values are given.
///
/// Calls `dip::SymmetricEigenDecomposition` after copying over the input values to a temporary buffer. `n`
/// must be either 2 or 3.
///
/// `input` is a pointer to 3 or 6 values: { xx, yy, xy }, { xx, yy, zz, xy, xz, yz }.
///
/// See `dip::SymmetricEigenDecomposition` for information on `lambdas` and `vectors`.
inline void SymmetricEigenDecompositionPacked(
      dip::uint n,
      ConstSampleIterator< dfloat > input,
      SampleIterator< dfloat > lambdas,
      SampleIterator< dfloat > vectors = nullptr
) {
   dfloat matrix[ 9 ];
   if( n == 2 ) {
      matrix[ 0 ] = input[ 0 ];
      matrix[ 1 ] = input[ 2 ];
      // matrix[ 2 ] is never used
      matrix[ 3 ] = input[ 1 ];
   } else if( n == 3 ) {
      matrix[ 0 ] = input[ 0 ];
      matrix[ 1 ] = input[ 3 ];
      matrix[ 2 ] = input[ 4 ];
      // matrix[ 3 ] is never used
      matrix[ 4 ] = input[ 1 ];
      matrix[ 5 ] = input[ 5 ];
      // matrix[ 6 ] is never used
      // matrix[ 7 ] is never used
      matrix[ 8 ] = input[ 2 ];
   } else {
      DIP_THROW( "dip::SymmetricEigenDecompositionPacked only defined for n=2 or n=3" );
   }
   SymmetricEigenDecomposition( n, matrix, lambdas, vectors );
}

/// \brief Finds the eigenvalues and eigenvectors of a square real matrix.
///
/// `input` is a pointer to `n*n` values, in column-major order.
///
/// `lambdas` is a pointer to space for `n` values, which don't have any specific ordering.
///
/// `vectors` is a pointer to space for `n*n` values and will receive the `n` eigenvectors. The eigenvectors
/// can be accessed at `&vectors[ 0 ]`, `&vectors[ n ]`, `&vectors[ 2*n ]`, etc.
/// If `vectors` is `false`, no eigenvectors are computed.
void EigenDecomposition(
      dip::uint n,
      ConstSampleIterator< dfloat > input,
      SampleIterator< dcomplex > lambdas,
      SampleIterator< dcomplex > vectors = nullptr
);

/// \brief Finds the eigenvalues and eigenvectors of a square complex matrix.
///
/// `input` is a pointer to `n*n` values, in column-major order.
///
/// `lambdas` is a pointer to space for `n` values, which don't have any specific ordering.
///
/// `vectors` is a pointer to space for `n*n` values and will receive the `n` eigenvectors. The eigenvectors
/// can be accessed at `&vectors[ 0 ]`, `&vectors[ n ]`, `&vectors[ 2*n ]`, etc.
/// If `vectors` is `false`, no eigenvectors are computed.
void EigenDecomposition(
      dip::uint n,
      ConstSampleIterator< dcomplex > input,
      SampleIterator< dcomplex > lambdas,
      SampleIterator< dcomplex > vectors = nullptr
);

/// \brief Computes the sum of the values of a vector.
///
/// `input` is a pointer to `n` values.
template< typename T >
inline T Sum( dip::uint n, ConstSampleIterator< T > input ) {
   return std::accumulate( input, input + n, T( 0.0 ));
}

/// \brief Computes the sum of the square of the values of a vector.
///
/// `input` is a pointer to `n` values.
template< typename T >
inline FloatType< T > SumAbsSquare( dip::uint n, ConstSampleIterator< T > input ) {
   return std::accumulate( input, input + n, 0.0, []( FloatType< T > a, T b ){ return a + std::abs( b ) * std::abs( b ); } );
}

/// \brief Computes the norm of a vector.
///
/// `input` is a pointer to `n` values.
template< typename T >
inline FloatType< T > Norm( dip::uint n, ConstSampleIterator< T > input ) {
   return std::sqrt( SumAbsSquare( n, input ));
}

/// \brief Computes the determinant of a square real matrix.
///
/// `input` is a pointer to `n*n` values, in column-major order.
dfloat Determinant( dip::uint n, ConstSampleIterator< dfloat > input );

/// \brief Computes the determinant of a square complex matrix.
///
/// `input` is a pointer to `n*n` values, in column-major order.
dcomplex Determinant( dip::uint n, ConstSampleIterator< dcomplex > input );

/// \brief Computes the determinant of a diagonal real matrix.
///
/// `input` is a pointer to `n` values, representing the matrix's main diagonal.
inline dfloat DeterminantDiagonal( dip::uint n, ConstSampleIterator< dfloat > input ) {
   return std::accumulate( input, input + n, 1.0, std::multiplies< dfloat >());
}

/// \brief Computes the determinant of a diagonal complex matrix.
///
/// `input` is a pointer to `n` values, representing the matrix's main diagonal.
inline dcomplex DeterminantDiagonal( dip::uint n, ConstSampleIterator< dcomplex > input ) {
   return std::accumulate( input, input + n, dcomplex( 1.0 ), std::multiplies< dcomplex >());
}

/// \brief Computes the trace of a square real matrix.
///
/// `input` is a pointer to `n*n` values, in column-major order.
inline dfloat Trace( dip::uint n, ConstSampleIterator< dfloat > input ) {
   return Sum( n, ConstSampleIterator< dfloat >( input.Pointer(), input.Stride() * ( n + 1 )) );
}

/// \brief Computes the trace of a square complex matrix.
///
/// `input` is a pointer to `n*n` values, in column-major order.
inline dcomplex Trace( dip::uint n, ConstSampleIterator< dcomplex > input ) {
   return Sum( n, ConstSampleIterator< dcomplex >( input.Pointer(), input.Stride() * ( n + 1 )) );
}

/// \brief Computes the trace of a diagonal real matrix.
///
/// `input` is a pointer to `n` values, representing the matrix's main diagonal.
inline dfloat TraceDiagonal( dip::uint n, ConstSampleIterator< dfloat > input ) {
   return Sum( n, input );
}

/// \brief Computes the trace of a diagonal complex matrix.
///
/// `input` is a pointer to `n` values, representing the matrix's main diagonal.
inline dcomplex TraceDiagonal( dip::uint n, ConstSampleIterator< dcomplex > input ) {
   return Sum( n, input );
}

/// \brief Computes the "thin" singular value decomposition of a real matrix
///
/// `input` is a pointer to `m*n` values, in column-major order.
///
/// `output` is a pointer to `p` values, where `p = std::min( m, n )`. It contains the
/// singular values of `input`, sorted in decreasing order.
///
/// `U` and `V` are pointers to `m*p` and `n*p` values, respectively. The left and right
/// singular vectors will be written to then.
/// If either of them is `false`, neither is computed, and only `output` is filled.
void SingularValueDecomposition(
      dip::uint m,
      dip::uint n,
      ConstSampleIterator< dfloat > input,
      SampleIterator< dfloat > output,
      SampleIterator< dfloat > U = nullptr,
      SampleIterator< dfloat > V = nullptr
);

/// \brief Computes the "thin" singular value decomposition of a complex matrix
///
/// `input` is a pointer to `m*n` values, in column-major order.
///
/// `output` is a pointer to `p` values, where `p = std::min( m, n )`. It contains the
/// singular values of `input`, sorted in decreasing order.
///
/// `U` and `V` are pointers to `m*p` and `n*p` values, respectively. The left and right
/// singular vectors will be written to then.
/// If either of them is `false`, neither is computed, and only `output` is filled.
void SingularValueDecomposition(
      dip::uint m,
      dip::uint n,
      ConstSampleIterator< dcomplex > input,
      SampleIterator< dcomplex > output,
      SampleIterator< dcomplex > U = nullptr,
      SampleIterator< dcomplex > V = nullptr
);

/// \brief Computes the inverse of a square real matrix.
///
/// `input` and `output` are pointers to `n*n` values, in column-major order.
void Inverse( dip::uint n, ConstSampleIterator< dfloat > input, SampleIterator< dfloat > output );

/// \brief Computes the inverse of a square complex matrix.
///
/// `input` and `output` are pointers to `n*n` values, in column-major order.
void Inverse( dip::uint n, ConstSampleIterator< dcomplex > input, SampleIterator< dcomplex > output );

/// \brief Computes the pseudo-inverse of a real matrix.
///
/// `input` is a pointer to `m*n` values, in column-major order.
///
/// `output` is a pointer to `n*m` values, in column-major order.
void PseudoInverse( dip::uint m, dip::uint n, ConstSampleIterator< dfloat > input, SampleIterator< dfloat > output );

/// \brief Computes the pseudo-inverse of a complex matrix.
///
/// `input` and `output` are pointers to `m*n` values, in column-major order.
///
/// `output` is a pointer to `n*m` values, in column-major order.
void PseudoInverse( dip::uint m, dip::uint n, ConstSampleIterator< dcomplex > input, SampleIterator< dcomplex > output );

/// \brief Computes the rank of a real matrix.
///
/// `input` is a pointer to `m*n` values, in column-major order.
dip::uint Rank( dip::uint m, dip::uint n, ConstSampleIterator< dfloat > input );

/// \brief Computes the rank of a complex matrix.
///
/// `input` is a pointer to `m*n` values, in column-major order.
dip::uint Rank( dip::uint m, dip::uint n, ConstSampleIterator< dcomplex > input );



/// \brief Computes the rank of the square matrix at each pixel in image `in`.


/// \brief `%StatisticsAccumulator` computes population statistics by accumulating the first four central moments.
///
/// Samples are added one by one, using the `Push` method. Other members are used to retrieve estimates of
/// the population statistics based on the samples seen up to that point. Formula used to compute population
/// statistics are corrected, though the standard deviation, skewness and excess kurtosis are not unbiased
/// estimators. The accumulator uses a stable algorithm to prevent catastrophic cancellation.
///
/// It is possible to accumulate samples in different objects (e.g. when processing with multiple threads),
/// and add the accumulators together using the `+` operator.
///
/// \see VarianceAccumulator, MinMaxAccumulator
///
/// ###Source
///
/// Code modified from <a href="http://www.johndcook.com/blog/skewness_kurtosis/">John D. Cook</a>,
/// but the same code appears on <a href="https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance">Wikipedia</a>.
/// Method for 3rd and 4th order moments was first published by:
///    <a href="http://people.xiph.org/~tterribe/notes/homs.html">T. B. Terriberry,
///    "Computing higher-order moments online", 2008</a>.
/// For more information:
///    <a href="http://infoserve.sandia.gov/sand_doc/2008/086212.pdf">Philippe P. Pébay, "Formulas for Robust,
///    One-Pass Parallel Computation of Covariances and Arbitrary-Order Statistical Moments",
///    Technical Report SAND2008-6212, Sandia National Laboratories, September 2008</a>.
///
/// Computation of statistics from moments according to Wikipedia:
///    <a href="https://en.wikipedia.org/wiki/Skewness#Sample_skewness">Skewness</a> and
///    <a href="https://en.wikipedia.org/wiki/Kurtosis#Estimators_of_population_kurtosis">Kurtosis</a>.
class StatisticsAccumulator {
   public:
      /// Add a sample to the accumulator
      void Push( dfloat x ) {
         ++n_;
         dfloat delta = x - m1_;
         dfloat term1 = delta / n_;
         dfloat term2 = term1 * term1;
         dfloat term3 = delta * term1 * ( n_ - 1 );
         m4_ += term3 * term2 * ( n_ * n_ - 3.0 * n_ + 3 ) + 6.0 * term2 * m2_ - 4.0 * term1 * m3_;
         m3_ += term3 * term1 * ( n_ - 2 ) - 3.0 * term1 * m2_; // old value used for m4_ calculation
         m2_ += term3; // old value used for m3_ and m4_ calculation.
         m1_ += term1;
      }

      /// Combine two accumulators
      StatisticsAccumulator& operator+=( StatisticsAccumulator const& b ) {
         dip::uint an = n_;
         dip::uint an2 = an * an;
         dip::uint bn2 = b.n_ * b.n_;
         dip::uint xn2 = an * b.n_;
         n_ += b.n_;
         dip::uint n2 = n_ * n_;
         dfloat delta = b.m1_ - m1_;
         dfloat delta2 = delta * delta;
         m4_ += b.m4_ + delta2 * delta2 * xn2 * ( an2 - xn2 + bn2 ) / ( n2 * n_ )
               + 6.0 * delta2 * ( an2 * b.m2_ + bn2 * m2_ ) / n2
               + 4.0 * delta * ( an * b.m3_ - b.n_ * m3_ ) / n_;
         m3_ += b.m3_ + delta * delta2 * xn2 * ( an - b.n_ ) / n2
                + 3.0 * delta * ( an * b.m2_ - b.n_ * m2_ ) / n_;
         m2_ += b.m2_ + delta2 * xn2 / n_;
         m1_ = ( an * m1_ + b.n_ * b.m1_ ) / n_;
         return *this;
      }

      /// Number of samples
      dip::uint Number() const {
         return n_;
      }
      /// Unbiased estimator of population mean
      dfloat Mean() const {
         return m1_;
      }
      /// Unbiased estimator of population variance
      dfloat Variance() const {
         return ( n_ > 1 ) ? ( m2_ / ( n_ - 1 )) : ( 0.0 );
      }
      /// Estimator of population standard deviation (it is not possible to derive an unbiased estimator)
      dfloat StandardDeviation() const {
         return std::sqrt( Variance() );
      }
      /// \brief Estimator of population skewness. This estimator is unbiased only for symetric distributions
      /// (it is not possible to derive an unbiased estimator).
      dfloat Skewness() const {
         if(( n_ > 2 ) && ( m2_ != 0 )) {
            dfloat n = n_;
            return (( n * n ) / (( n - 1 ) * ( n - 2 ))) * ( m3_ / ( n * std::pow( Variance(), 1.5 )));
         }
         return 0;
      }
      /// \brief Estimator of population excess kurtosis. This estimator is only unbiased for normally
      /// distributed data (it is not possible to derive an unbiased estimator).
      dfloat ExcessKurtosis() const {
         if( n_ > 3 && ( m2_ != 0 )) {
            dfloat n = n_;
            return ( n - 1 ) / (( n - 2 ) * ( n - 3 )) * (( n + 1 ) * n * m4_ / ( m2_ * m2_ ) - 3 * ( n - 1 ));
         }
         return 0;
      }

   private:
      dip::uint n_ = 0; // number of values x collected
      dfloat m1_ = 0;   // mean of values x
      dfloat m2_ = 0;   // sum of (x-mean(x))^2  --  `m2_ / n_` is second order central moment
      dfloat m3_ = 0;   // sum of (x-mean(x))^3  --  `m3_ / n_` is third order central moment
      dfloat m4_ = 0;   // sum of (x-mean(x))^4  --  `m4_ / n_` is fourth order central moment
};

/// \brief Combine two accumulators
inline StatisticsAccumulator operator+( StatisticsAccumulator lhs, StatisticsAccumulator const& rhs ) {
   lhs += rhs;
   return lhs;
}


/// \brief `%VarianceAccumulator` computes mean and standard deviation by accumulating the first two
/// central moments.
///
/// Samples are added one by one, using the `Push` method. Other members are used to retrieve estimates of
/// the population statistics based on the samples seen up to that point. Formula used to compute population
/// statistics are corrected, though the standard deviation is not an unbiased estimator. The accumulator
/// uses a stable algorithm to prevent catastrophic cancellation.
///
/// It is possible to accumulate samples in different objects (e.g. when processing with multiple threads),
/// and add the accumulators together using the `+` operator.
///
/// \see StatisticsAccumulator, MinMaxAccumulator
///
/// ### Source
///
/// Donald E. Knuth, "The Art of Computer Programming, Volume 2: Seminumerical Algorithms", 3rd Ed., 1998.
class VarianceAccumulator {
   public:
      /// Add a sample to the accumulator
      void Push( dfloat x ) {
         ++n_;
         dfloat delta = x - m1_;
         m1_ += delta / n_;
         m2_ += delta * ( x - m1_ );
      }

      /// Combine two accumulators
      VarianceAccumulator& operator+=( VarianceAccumulator const& b ) {
         dip::uint oldn = n_;
         n_ += b.n_;
         dfloat delta = b.m1_ - m1_;
         m1_ = ( oldn * m1_ + b.n_ * b.m1_ ) / n_;
         m2_ += b.m2_ + delta * delta * ( oldn * b.n_ ) / n_;
         return *this;
      }

      /// Number of samples
      dip::uint Number() const {
         return n_;
      }
      /// Unbiased estimator of population mean
      dfloat Mean() const {
         return m1_;
      }
      /// Unbiased estimator of population variance
      dfloat Variance() const {
         return ( n_ > 1 ) ? ( m2_ / ( n_ - 1 )) : ( 0.0 );
      }
      /// Estimator of population standard deviation (it is not possible to derive an unbiased estimator)
      dfloat StandardDeviation() const {
         return std::sqrt( Variance() );
      }

   private:
      dip::uint n_ = 0; // number of values x collected
      dfloat m1_ = 0;   // mean of values x
      dfloat m2_ = 0;   // sum of (x-mean(x))^2  --  `m2_ / n_` is second order central moment
};

/// \brief Combine two accumulators
inline VarianceAccumulator operator+( VarianceAccumulator lhs, VarianceAccumulator const& rhs ) {
   lhs += rhs;
   return lhs;
}


/// \brief `%MinMaxAccumulator` computes minimum and maximum values of a sequence of values.
///
/// Samples are added one by one or two by two, using the `Push` method. Other members are used to retrieve the results.
///
/// It is possible to accumulate samples in different objects (e.g. when processing with multiple threads),
/// and add the accumulators together using the `+` operator.
///
/// \see StatisticsAccumulator, VarianceAccumulator
class MinMaxAccumulator {
   public:
      /// Add a sample to the accumulator
      void Push( dfloat x ) {
         max_ = std::max( max_, x );
         min_ = std::min( min_, x );
      }

      /// \brief Add two samples to the accumulator. Prefer this over adding one value at the time.
      void Push( dfloat x, dfloat y ) {
         if( x > y ) {
            max_ = std::max( max_, x );
            min_ = std::min( min_, y );
         } else { // y >= x
            max_ = std::max( max_, y );
            min_ = std::min( min_, x );
         }
      }

      /// Combine two accumulators
      MinMaxAccumulator& operator+=( MinMaxAccumulator const& other ) {
         min_ = std::min( min_, other.min_ );
         max_ = std::max( max_, other.max_ );
         return *this;
      }

      /// Minimum value seen so far
      dfloat Minimum() const {
         return min_;
      }

      /// Maximum value seen so far
      dfloat Maximum() const {
         return max_;
      }

   private:
      dfloat min_ = std::numeric_limits< dfloat >::max();
      dfloat max_ = std::numeric_limits< dfloat >::lowest();
};





/// \}

} // namespace dip


#ifdef DIP__ENABLE_DOCTEST
#include "doctest.h"

DOCTEST_TEST_CASE("[DIPlib] testing the dip::gcd function") {
   DOCTEST_CHECK( dip::gcd( 10, 10 ) == 10 );
   DOCTEST_CHECK( dip::gcd( 10, 5 ) == 5 );
   DOCTEST_CHECK( dip::gcd( 10, 1 ) == 1 );
   DOCTEST_CHECK( dip::gcd( 10, 12 ) == 2 );
   DOCTEST_CHECK( dip::gcd( 10, 15 ) == 5 );
   DOCTEST_CHECK( dip::gcd( 15, 10 ) == 5 );
}

DOCTEST_TEST_CASE("[DIPlib] testing the dip::div_ceil function") {
   DOCTEST_CHECK( dip::div_ceil( 11l, 11l ) == 1 );
   DOCTEST_CHECK( dip::div_ceil( 11l, 6l ) == 2 );
   DOCTEST_CHECK( dip::div_ceil( 11l, 5l ) == 3 );
   DOCTEST_CHECK( dip::div_ceil( 11l, 4l ) == 3 );
   DOCTEST_CHECK( dip::div_ceil( 11l, 3l ) == 4 );
   DOCTEST_CHECK( dip::div_ceil( -11l, 3l ) == -3 );
   DOCTEST_CHECK( dip::div_ceil( -11l, 4l ) == -2 );
   DOCTEST_CHECK( dip::div_ceil( -11l, 5l ) == -2 );
   DOCTEST_CHECK( dip::div_ceil( -11l, 6l ) == -1 );
   DOCTEST_CHECK( dip::div_ceil( 11l, -3l ) == -3 );
   DOCTEST_CHECK( dip::div_ceil( 11l, -4l ) == -2 );
   DOCTEST_CHECK( dip::div_ceil( 11l, -5l ) == -2 );
   DOCTEST_CHECK( dip::div_ceil( 11l, -6l ) == -1 );
   DOCTEST_CHECK( dip::div_ceil( -11l, -6l ) == 2 );
   DOCTEST_CHECK( dip::div_ceil( -11l, -5l ) == 3 );
   DOCTEST_CHECK( dip::div_ceil( -11l, -4l ) == 3 );
   DOCTEST_CHECK( dip::div_ceil( -11l, -3l ) == 4 );
}

DOCTEST_TEST_CASE("[DIPlib] testing the dip::div_floor function") {
   DOCTEST_CHECK( dip::div_floor( 10l, 10l ) == 1 );
   DOCTEST_CHECK( dip::div_floor( 11l, 6l ) == 1 );
   DOCTEST_CHECK( dip::div_floor( 11l, 5l ) == 2 );
   DOCTEST_CHECK( dip::div_floor( 11l, 4l ) == 2 );
   DOCTEST_CHECK( dip::div_floor( 11l, 3l ) == 3 );
   DOCTEST_CHECK( dip::div_floor( -11l, 3l ) == -4 );
   DOCTEST_CHECK( dip::div_floor( -11l, 4l ) == -3 );
   DOCTEST_CHECK( dip::div_floor( -11l, 5l ) == -3 );
   DOCTEST_CHECK( dip::div_floor( -11l, 6l ) == -2 );
   DOCTEST_CHECK( dip::div_floor( 11l, -3l ) == -4 );
   DOCTEST_CHECK( dip::div_floor( 11l, -4l ) == -3 );
   DOCTEST_CHECK( dip::div_floor( 11l, -5l ) == -3 );
   DOCTEST_CHECK( dip::div_floor( 11l, -6l ) == -2 );
   DOCTEST_CHECK( dip::div_floor( -11l, -6l ) == 1 );
   DOCTEST_CHECK( dip::div_floor( -11l, -5l ) == 2 );
   DOCTEST_CHECK( dip::div_floor( -11l, -4l ) == 2 );
   DOCTEST_CHECK( dip::div_floor( -11l, -3l ) == 3 );
}

DOCTEST_TEST_CASE("[DIPlib] testing the dip::div_round function") {
   DOCTEST_CHECK( dip::div_round( 10l, 10l ) == 1 );
   DOCTEST_CHECK( dip::div_round( 11l, 6l ) == 2 );
   DOCTEST_CHECK( dip::div_round( 11l, 5l ) == 2 );
   DOCTEST_CHECK( dip::div_round( 11l, 4l ) == 3 );
   DOCTEST_CHECK( dip::div_round( 11l, 3l ) == 4 );
   DOCTEST_CHECK( dip::div_round( -11l, 3l ) == -4 );
   DOCTEST_CHECK( dip::div_round( -11l, 4l ) == -3 );
   DOCTEST_CHECK( dip::div_round( -11l, 5l ) == -2 );
   DOCTEST_CHECK( dip::div_round( -11l, 6l ) == -2 );
   DOCTEST_CHECK( dip::div_round( 11l, -3l ) == -4 );
   DOCTEST_CHECK( dip::div_round( 11l, -4l ) == -3 );
   DOCTEST_CHECK( dip::div_round( 11l, -5l ) == -2 );
   DOCTEST_CHECK( dip::div_round( 11l, -6l ) == -2 );
   DOCTEST_CHECK( dip::div_round( -11l, -6l ) == 2 );
   DOCTEST_CHECK( dip::div_round( -11l, -5l ) == 2 );
   DOCTEST_CHECK( dip::div_round( -11l, -4l ) == 3 );
   DOCTEST_CHECK( dip::div_round( -11l, -3l ) == 4 );
}

DOCTEST_TEST_CASE("[DIPlib] testing the dip::pow10 function") {
   DOCTEST_CHECK( dip::pow10( 25 ) == doctest::Approx( std::pow( 10, 25 )) );
   DOCTEST_CHECK( dip::pow10( 10 ) == std::pow( 10, 10 ) );
   DOCTEST_CHECK( dip::pow10( 1 ) == std::pow( 10, 1 ) );
   DOCTEST_CHECK( dip::pow10( 0 ) == std::pow( 10, 0 ) );
   DOCTEST_CHECK( dip::pow10( -5 ) == std::pow( 10, -5 ) );
   DOCTEST_CHECK( dip::pow10( -21 ) == doctest::Approx( std::pow( 10, -21 )) );
}

#endif // DIP__ENABLE_DOCTEST

#endif // DIP_NUMERIC_H
