// This file is part of libigl, a simple c++ geometry processing library.
//
// Copyright (C) 2013 Alec Jacobson <alecjacobson@gmail.com>
//
// This Source Code Form is subject to the terms of the Mozilla Public License
// v. 2.0. If a copy of the MPL was not distributed with this file, You can
// obtain one at http://mozilla.org/MPL/2.0/.
#include "colon.h"

#include <cstdio>

template <typename L,typename S,typename H,typename T>
IGL_INLINE void igl::colon(
  const L low,
  const S step,
  const H hi,
  Eigen::Matrix<T,Eigen::Dynamic,1> & I)
{
  const int size = ((hi-low)/step)+1;
  I = Eigen::Matrix<T,Eigen::Dynamic,1>::LinSpaced(size,low,low+step*(size-1));
}

template <typename L,typename H,typename T>
IGL_INLINE void igl::colon(
  const L low,
  const H hi,
  Eigen::Matrix<T,Eigen::Dynamic,1> & I)
{
  return igl::colon(low,(T)1,hi,I);
}

template <typename T,typename L,typename H>
IGL_INLINE Eigen::Matrix<T,Eigen::Dynamic,1> igl::colon(
  const L low,
  const H hi)
{
  Eigen::Matrix<T,Eigen::Dynamic,1> I;
  igl::colon(low,hi,I);
  return I;
}

#ifdef IGL_STATIC_LIBRARY
// Explicit template specialization
// generated by autoexplicit.sh
template Eigen::Matrix<int,-1,1,0,-1,1> igl::colon<int,int,int>(int, int);
template Eigen::Matrix<int,-1,1,0,-1,1> igl::colon<int,int,long>(int,long);
template Eigen::Matrix<int,-1,1,0,-1,1> igl::colon<int,int,long long int>(int,long long int);
template Eigen::Matrix<double, -1, 1, 0, -1, 1> igl::colon<double, double, double>(double, double);
// generated by autoexplicit.sh
template void igl::colon<int, long, int, int>(int, long, int, Eigen::Matrix<int, -1, 1, 0, -1, 1>&);
template void igl::colon<int, int, long, int>(int, int, long, Eigen::Matrix<int, -1, 1, 0, -1, 1>&);
template void igl::colon<int, long, int>(int, long, Eigen::Matrix<int, -1, 1, 0, -1, 1>&);
template void igl::colon<int, int, int>(int, int, Eigen::Matrix<int, -1, 1, 0, -1, 1>&);
template void igl::colon<int,long long int,int>(int,long long int,Eigen::Matrix<int,-1,1,0,-1,1> &);
template void igl::colon<int, int, int, int>(int, int, int, Eigen::Matrix<int, -1, 1, 0, -1, 1>&);
template void igl::colon<int, long, long>(int, long, Eigen::Matrix<long, -1, 1, 0, -1, 1>&);
template void igl::colon<double, double, double>(double, double, Eigen::Matrix<double, -1, 1, 0, -1, 1>&);
#ifdef WIN32
template void igl::colon<int, long long,long>(int, long long, class Eigen::Matrix<long,-1,1,0,-1,1> &);
#endif
#endif
