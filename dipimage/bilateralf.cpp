/*
 * DIPimage 3.0
 * This MEX-file implements the `bilateralf` function
 *
 * (c)2018, Cris Luengo.
 * Based on original DIPlib code: (c)1995-2014, Delft University of Technology.
 * Based on original DIPimage code: (c)1999-2014, Delft University of Technology.
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

#include "dip_matlab_interface.h"
#include "diplib/nonlinear.h"

void mexFunction( int /*nlhs*/, mxArray* plhs[], int nrhs, const mxArray* prhs[] ) {
   try {

      DML_MIN_ARGS( 1 );
      DML_MAX_ARGS( 6 );

      dml::MatlabInterface mi;
      dip::Image const in = dml::GetImage( prhs[ 0 ] );
      dip::Image out = mi.NewImage();

      dip::FloatArray spatialSigmas = nrhs > 1 ? dml::GetFloatArray( prhs[ 1 ] ) : dip::FloatArray{ 2.0 };
      dip::dfloat tonalSigma = nrhs > 2 ? dml::GetFloat( prhs[ 2 ] ) : 30.0;
      dip::dfloat truncation = nrhs > 3 ? dml::GetFloat( prhs[ 3 ] ) : 2.0;
      dip::String method = nrhs > 4 ? dml::GetString( prhs[ 4 ] ) : dip::String( "xysep" );
      dip::StringArray bc = nrhs > 5 ? dml::GetStringArray( prhs[ 5 ] ) : dip::StringArray{};

      dip::BilateralFilter( in, {}, out, spatialSigmas, tonalSigma, truncation, method, bc );

      plhs[ 0 ] = dml::GetArray( out );

   } DML_CATCH
}