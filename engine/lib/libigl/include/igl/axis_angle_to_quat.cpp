// This file is part of libigl, a simple c++ geometry processing library.
// 
// Copyright (C) 2013 Alec Jacobson <alecjacobson@gmail.com>
// 
// This Source Code Form is subject to the terms of the Mozilla Public License 
// v. 2.0. If a copy of the MPL was not distributed with this file, You can 
// obtain one at http://mozilla.org/MPL/2.0/.
#include "axis_angle_to_quat.h"
#include "EPS.h"
#include <cmath>

// http://www.antisphere.com/Wiki/tools:anttweakbar
template <typename Q_type>
IGL_INLINE void igl::axis_angle_to_quat(
  const Q_type *axis, 
  const Q_type angle,
  Q_type *out)
{
    Q_type n = axis[0]*axis[0] + axis[1]*axis[1] + axis[2]*axis[2];
    if( fabs(n)>igl::EPS<Q_type>())
    {
        Q_type f = 0.5*angle;
        out[3] = cos(f);
        f = sin(f)/sqrt(n);
        out[0] = axis[0]*f;
        out[1] = axis[1]*f;
        out[2] = axis[2]*f;
    }
    else
    {
        out[3] = 1.0;
        out[0] = out[1] = out[2] = 0.0;
    }
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template specialization
// generated by autoexplicit.sh
template void igl::axis_angle_to_quat<double>(double const*, double, double*);
// generated by autoexplicit.sh
template void igl::axis_angle_to_quat<float>(float const*, float, float*);
#endif
