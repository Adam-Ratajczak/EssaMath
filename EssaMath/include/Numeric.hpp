/*
 ******************************************************************
 *           C++ Mathematical Expression Toolkit Library          *
 *                                                                *
 * Author: Arash Partow (1999-2023)                               *
 * URL: https://www.partow.net/programming/exprtk/index.html      *
 *                                                                *
 * Copyright notice:                                              *
 * Free use of the C++ Mathematical Expression Toolkit Library is *
 * permitted under the guidelines and in accordance with the most *
 * current version of the MIT License.                            *
 * https://www.opensource.org/licenses/MIT                        *
 *                                                                *
 * Example expressions:                                           *
 * (00) (y + x / y) * (x - y / x)                                 *
 * (01) (x^2 / sin(2 * pi / y)) - x / 2                           *
 * (02) sqrt(1 - (x^2))                                           *
 * (03) 1 - sin(2 * x) + cos(pi / y)                              *
 * (04) a * exp(2 * t) + c                                        *
 * (05) if(((x + 2) == 3) and ((y + 5) <= 9),1 + w, 2 / z)        *
 * (06) (avg(x,y) <= x + y ? x - y : x * y) + 2 * pi / x          *
 * (07) z := x + sin(2 * pi / y)                                  *
 * (08) u := 2 * (pi * z) / (w := x + cos(y / pi))                *
 * (09) clamp(-1,sin(2 * pi * x) + cos(y / 2 * pi),+1)            *
 * (10) inrange(-2,m,+2) == if(({-2 <= m} and [m <= +2]),1,0)     *
 * (11) (2sin(x)cos(2y)7 + 1) == (2 * sin(x) * cos(2*y) * 7 + 1)  *
 * (12) (x ilike 's*ri?g') and [y < (3 z^7 + w)]                  *
 *                                                                *
 ******************************************************************
*/

#pragma once

#include "Defines.hpp"
#include "Faddeeva.hpp"
#include <cmath>
#include <limits>
#include <sstream>
#include <string>

namespace Essa::Math{
   namespace details{
         namespace numeric{
            namespace details{
               
            template <typename T>
            inline bool is_nan_impl(const T v, complex_type_tag)
            {
               return std::not_equal_to<T>()(v,v);
            }

            template <typename T>
            inline bool is_nan_impl(const T v, real_type_tag)
            {
               return std::not_equal_to<T>()(v,v);
            }
               
            template <typename T>
            inline bool is_i_impl(const T v, complex_type_tag)
            {
               return std::equal_to<T>()(v,T(0, 1));
            }

            template <typename T>
            inline bool is_i_impl(const T v, real_type_tag)
            {
               return false;
            }
               
            template <typename T>
            inline std::string to_string_impl(const T v, complex_type_tag)
            {
               std::string _result;
               if(v.real() != 0){
                  std::stringstream ss;
                  ss << v.real();
                  _result += ss.str();
               }

               if(v.imag() != 0){
                  std::stringstream ss;
                  ss << v.imag();
                  if(v.imag() > 0)
                     _result += "+";
                  _result += ss.str() + "*%i";
               }

               if(_result.empty())
                  _result = "=";

               return _result;
            }
               
            template <typename T>
            inline std::string to_string_impl(const T v, real_type_tag)
            {
               std::stringstream ss;
               ss << v;
               return ss.str();
            }

            template <typename T>
            inline int to_int32_impl(const T v, complex_type_tag)
            {
               return static_cast<int>(v.real());
            }

            template <typename T>
            inline int to_int32_impl(const T v, real_type_tag)
            {
               return static_cast<int>(v);
            }

            template <typename T>
            inline _int64_t to_int64_impl(const T v, complex_type_tag)
            {
               return static_cast<_int64_t>(v.real());
            }

            template <typename T>
            inline _int64_t to_int64_impl(const T v, real_type_tag)
            {
               return static_cast<_int64_t>(v);
            }

            template <typename T>
            inline bool is_true_impl(const T v)
            {
               return std::not_equal_to<T>()(T(0),v);
            }

            template <typename T>
            inline bool is_false_impl(const T v)
            {
               return std::equal_to<T>()(T(0),v);
            }

            template <typename T>
            inline T abs_impl(const T v, complex_type_tag)
            {
               return T(std::fabs(v.real()), std::fabs(v.imag()));
            }

            template <typename T>
            inline T abs_impl(const T v, real_type_tag)
            {
               return std::fabs(v);
            }

            template <typename T>
            inline T lth_impl(const T v0, const T v1, complex_type_tag)
            {
               return v0.real() < v1.real() ? T(1) : T(0);
            }

            template <typename T>
            inline T lth_impl(const T v0, const T v1, real_type_tag)
            {
               return v0 < v1 ? T(1) : T(0);
            }

            template <typename T>
            inline T leq_impl(const T v0, const T v1, complex_type_tag)
            {
               return v0.real() <= v1.real() ? T(1) : T(0);
            }

            template <typename T>
            inline T leq_impl(const T v0, const T v1, real_type_tag)
            {
               return v0 <= v1 ? T(1) : T(0);
            }

            template <typename T>
            inline T geq_impl(const T v0, const T v1, complex_type_tag)
            {
               return v0.real() >= v1.real() ? T(1) : T(0);
            }

            template <typename T>
            inline T geq_impl(const T v0, const T v1, real_type_tag)
            {
               return v0 >= v1 ? T(1) : T(0);
            }

            template <typename T>
            inline T gth_impl(const T v0, const T v1, complex_type_tag)
            {
               return v0.real() > v1.real() ? T(1) : T(0);
            }

            template <typename T>
            inline T gth_impl(const T v0, const T v1, real_type_tag)
            {
               return v0 > v1 ? T(1) : T(0);
            }

            template <typename T>
            inline T equal_impl(const T v0, const T v1, complex_type_tag)
            {
               return (v0 == v1) ? T(1, 0) : T(0, 0);  
            }

            template <typename T>
            inline T equal_impl(const T v0, const T v1, real_type_tag)
            {
               const T epsilon = epsilon_type<T>::value();
               return (abs_impl(v0 - v1,real_type_tag()) <= (std::max(T(1),std::max(abs_impl(v0,real_type_tag()),abs_impl(v1,real_type_tag()))) * epsilon)) ? T(1) : T(0);
            }

            inline float equal_impl(const float v0, const float v1, real_type_tag)
            {
               const float epsilon = epsilon_type<float>::value();
               return (abs_impl(v0 - v1,real_type_tag()) <= (std::max(1.0f,std::max(abs_impl(v0,real_type_tag()),abs_impl(v1,real_type_tag()))) * epsilon)) ? 1.0f : 0.0f;
            }

            template <typename T>
            inline T expm1_impl(const T v, complex_type_tag)
            {
               return std::exp(v) - T(1, 0);
            }

            template <typename T>
            inline T expm1_impl(const T v, real_type_tag)
            {
               // return std::expm1<T>(v);
               if (abs_impl(v,real_type_tag()) < T(0.00001))
                  return v + (T(0.5) * v * v);
               else
                  return std::exp(v) - T(1);
            }

            template <typename T>
            inline T nequal_impl(const T v0, const T v1, complex_type_tag)
            {
               typedef real_type_tag rtg;
               const T epsilon = epsilon_type<T>::value();
               return v0 != v1 ? T(1, 0) : T(0, 0);
            }

            template <typename T>
            inline T nequal_impl(const T v0, const T v1, real_type_tag)
            {
               typedef real_type_tag rtg;
               const T epsilon = epsilon_type<T>::value();
               return (abs_impl(v0 - v1,rtg()) > (std::max(T(1),std::max(abs_impl(v0,rtg()),abs_impl(v1,rtg()))) * epsilon)) ? T(1) : T(0);
            }

            inline float nequal_impl(const float v0, const float v1, real_type_tag)
            {
               typedef real_type_tag rtg;
               const float epsilon = epsilon_type<float>::value();
               return (abs_impl(v0 - v1,rtg()) > (std::max(1.0f,std::max(abs_impl(v0,rtg()),abs_impl(v1,rtg()))) * epsilon)) ? 1.0f : 0.0f;
            }

            template <typename T>
            inline T max_num_impl(const T v0, const T v1, complex_type_tag)
            {
               return std::max(v0.real(),v1.real());
            }

            template <typename T>
            inline T max_num_impl(const T v0, const T v1, real_type_tag)
            {
               return std::max(v0,v1);
            }
            template <typename T>
            inline T min_num_impl(const T v0, const T v1, complex_type_tag)
            {
               return std::min(v0.real(),v1.real());
            }

            template <typename T>
            inline T min_num_impl(const T v0, const T v1, real_type_tag)
            {
               return std::min(v0,v1);
            }

            template <typename T>
            inline T pow_impl(const T v0, const T v1, complex_type_tag)
            {
               return std::pow(v0,v1);
            }


            template <typename T>
            inline T pow_impl(const T v0, const T v1, real_type_tag)
            {
               return std::pow(v0,v1);
            }

            template <typename T>
            inline T logn_impl(const T v0, const T v1, complex_type_tag)
            {
               return std::log(v0) / std::log(v1);
            }

            template <typename T>
            inline T logn_impl(const T v0, const T v1, real_type_tag)
            {
               return std::log(v0) / std::log(v1);
            }

            template <typename T>
            inline T log1p_impl(const T v, complex_type_tag)
            {
               return std::log(T(1, 0) + v);
            }

            template <typename T>
            inline T log1p_impl(const T v, real_type_tag)
            {
               if (v > T(-1))
               {
                  if (abs_impl(v,real_type_tag()) > T(0.0001))
                  {
                     return std::log(T(1) + v);
                  }
                  else
                     return (T(-0.5) * v + T(1)) * v;
               }
               else
                  return std::numeric_limits<T>::quiet_NaN();
            }

            template <typename T>
            inline T root_impl(const T v0, const T v1, complex_type_tag)
            {
               return std::pow(v0, T(1, 0) / v1);
            }

            template <typename T>
            inline T root_impl(const T v0, const T v1, real_type_tag)
            {
               if (v1 < T(0))
                  return std::numeric_limits<T>::quiet_NaN();

               const std::size_t n = static_cast<std::size_t>(v1);

               if ((v0 < T(0)) && (0 == (n % 2)))
                  return std::numeric_limits<T>::quiet_NaN();

               return std::pow(v0, T(1) / n);
            }

            template <typename T>
            inline T atan2_impl(const T v0, const T v1, complex_type_tag)
            {
               return std::atan(v0/v1);
            }

            template <typename T>
            inline T atan2_impl(const T v0, const T v1, real_type_tag)
            {
               return std::atan2(v0,v1);
            }

            template <typename T>
            inline T erf_impl(const T v, complex_type_tag)
            {
               return erf(v);
            }

            template <typename T>
            inline T erf_impl(const T v, real_type_tag)
            {
               return erf(v);
            }

            template <typename T>
            inline T erfc_impl(const T v, complex_type_tag)
            {
               return erfc(v);
            }

            template <typename T>
            inline T erfc_impl(const T v, real_type_tag)
            {
               return erfc(v);
            }

            template <typename T>
            inline T ncdf_impl(const T v, complex_type_tag)
            {
               return Dawson(v);
            }

            template <typename T>
            inline T ncdf_impl(const T v, real_type_tag)
            {
               return Dawson(v);
            }

            template <typename T>
            inline T sinc_impl(const T v, complex_type_tag)
            {
               return(std::sin(v) / v);
            }

            template <typename T>
            inline T sinc_impl(const T v, real_type_tag)
            {
               if (std::abs(v) >= std::numeric_limits<T>::epsilon())
                   return(std::sin(v) / v);
               else
                  return T(1);
            }

            template <typename T> inline T  acos_impl(const T v, complex_type_tag) { return std::acos (v); }
            template <typename T> inline T acosh_impl(const T v, complex_type_tag) { return std::log(v + std::sqrt((v * v) - T(1))); }
            template <typename T> inline T  asin_impl(const T v, complex_type_tag) { return std::asin (v); }
            template <typename T> inline T asinh_impl(const T v, complex_type_tag) { return std::log(v + std::sqrt((v * v) + T(1))); }
            template <typename T> inline T  atan_impl(const T v, complex_type_tag) { return std::atan (v); }
            template <typename T> inline T atanh_impl(const T v, complex_type_tag) { return (std::log(T(1) + v) - std::log(T(1) - v)) / T(2); }
            template <typename T> inline T   cos_impl(const T v, complex_type_tag) { return std::cos  (v); }
            template <typename T> inline T  cosh_impl(const T v, complex_type_tag) { return std::cosh (v); }
            template <typename T> inline T   exp_impl(const T v, complex_type_tag) { return std::exp  (v); }
            template <typename T> inline T   log_impl(const T v, complex_type_tag) { return std::log  (v); }
            template <typename T> inline T log10_impl(const T v, complex_type_tag) { return std::log10(v); }
            template <typename T> inline T  log2_impl(const T v, complex_type_tag) { return std::log(v)/T(numeric::constant::log2); }
            template <typename T> inline T   neg_impl(const T v, complex_type_tag) { return -v;            }
            template <typename T> inline T   pos_impl(const T v, complex_type_tag) { return +v;            }
            template <typename T> inline T   sin_impl(const T v, complex_type_tag) { return std::sin  (v); }
            template <typename T> inline T  sinh_impl(const T v, complex_type_tag) { return std::sinh (v); }
            template <typename T> inline T  sqrt_impl(const T v, complex_type_tag) { return std::sqrt (v); }
            template <typename T> inline T   tan_impl(const T v, complex_type_tag) { return std::tan  (v); }
            template <typename T> inline T  tanh_impl(const T v, complex_type_tag) { return std::tanh (v); }
            template <typename T> inline T   cot_impl(const T v, complex_type_tag) { return T(1) / std::tan(v); }
            template <typename T> inline T   sec_impl(const T v, complex_type_tag) { return T(1) / std::cos(v); }
            template <typename T> inline T   csc_impl(const T v, complex_type_tag) { return T(1) / std::sin(v); }

            template <typename T> inline T   const_pi_impl(complex_type_tag) { return T(numeric::constant::pi, 0.0);       }
            template <typename T> inline T    const_e_impl(complex_type_tag) { return T(numeric::constant::e, 0.0);        }
            template <typename T> inline T const_qnan_impl(complex_type_tag) { return std::numeric_limits<T>::quiet_NaN(); }
            template <typename T> inline T   const_i_impl(complex_type_tag) { return T(0.0, 1.0);                          }

            template <typename T> inline T  acos_impl(const T v, real_type_tag) { return std::acos (v); }
            template <typename T> inline T acosh_impl(const T v, real_type_tag) { return std::log(v + std::sqrt((v * v) - T(1))); }
            template <typename T> inline T  asin_impl(const T v, real_type_tag) { return std::asin (v); }
            template <typename T> inline T asinh_impl(const T v, real_type_tag) { return std::log(v + std::sqrt((v * v) + T(1))); }
            template <typename T> inline T  atan_impl(const T v, real_type_tag) { return std::atan (v); }
            template <typename T> inline T atanh_impl(const T v, real_type_tag) { return (std::log(T(1) + v) - std::log(T(1) - v)) / T(2); }
            template <typename T> inline T   cos_impl(const T v, real_type_tag) { return std::cos  (v); }
            template <typename T> inline T  cosh_impl(const T v, real_type_tag) { return std::cosh (v); }
            template <typename T> inline T   exp_impl(const T v, real_type_tag) { return std::exp  (v); }
            template <typename T> inline T   log_impl(const T v, real_type_tag) { return std::log  (v); }
            template <typename T> inline T log10_impl(const T v, real_type_tag) { return std::log10(v); }
            template <typename T> inline T  log2_impl(const T v, real_type_tag) { return std::log(v)/T(numeric::constant::log2); }
            template <typename T> inline T   neg_impl(const T v, real_type_tag) { return -v;            }
            template <typename T> inline T   pos_impl(const T v, real_type_tag) { return +v;            }
            template <typename T> inline T   sin_impl(const T v, real_type_tag) { return std::sin  (v); }
            template <typename T> inline T  sinh_impl(const T v, real_type_tag) { return std::sinh (v); }
            template <typename T> inline T  sqrt_impl(const T v, real_type_tag) { return std::sqrt (v); }
            template <typename T> inline T   tan_impl(const T v, real_type_tag) { return std::tan  (v); }
            template <typename T> inline T  tanh_impl(const T v, real_type_tag) { return std::tanh (v); }
            template <typename T> inline T   cot_impl(const T v, real_type_tag) { return T(1) / std::tan(v); }
            template <typename T> inline T   sec_impl(const T v, real_type_tag) { return T(1) / std::cos(v); }
            template <typename T> inline T   csc_impl(const T v, real_type_tag) { return T(1) / std::sin(v); }

            template <typename T> inline T   const_pi_impl(real_type_tag) { return T(numeric::constant::pi);            }
            template <typename T> inline T    const_e_impl(real_type_tag) { return T(numeric::constant::e);             }
            template <typename T> inline T const_qnan_impl(real_type_tag) { return std::numeric_limits<T>::quiet_NaN(); }
            template <typename T> inline T    const_i_impl(real_type_tag) { return std::numeric_limits<T>::quiet_NaN(); }

            template <typename T>
            inline bool is_integer_impl(const T& v, complex_type_tag)
            {
               if(v.imag() == 0)
                  return std::equal_to<T>()(T(0),std::fmod(v.real(),1.0));
               return false;
            }
            template <typename T>
            inline bool is_integer_impl(const T& v, real_type_tag)
            {
               return std::equal_to<T>()(T(0),std::fmod(v,T(1)));
            }
         }

         template <typename Type>
         struct numeric_info { enum { length = 0, size = 32, bound_length = 0, min_exp = 0, max_exp = 0 }; };

         template <> struct numeric_info<float                       > { enum { min_exp =  -38, max_exp =  +38 }; };
         template <> struct numeric_info<double                      > { enum { min_exp = -308, max_exp = +308 }; };
         template <> struct numeric_info<long double                 > { enum { min_exp = -308, max_exp = +308 }; };
         template <> struct numeric_info<std::complex<float>         > { enum { min_exp =  -38, max_exp =  +38 }; };
         template <> struct numeric_info<std::complex<double>        > { enum { min_exp = -308, max_exp = +308 }; };
         template <> struct numeric_info<std::complex<long double>   > { enum { min_exp = -308, max_exp = +308 }; };

         template <typename T>
         inline int to_int32(const T v)
         {
            const typename details::number_type<T>::type num_type;
            return to_int32_impl(v, num_type);
         }

         template <typename T>
         inline _int64_t to_int64(const T v)
         {
            const typename details::number_type<T>::type num_type;
            return to_int64_impl(v, num_type);
         }

         template <typename T>
         inline bool is_nan(const T v)
         {
            const typename details::number_type<T>::type num_type;
            return is_nan_impl(v, num_type);
         }

         template <typename T>
         inline bool is_i(const T v)
         {
            const typename details::number_type<T>::type num_type;
            return is_i_impl(v, num_type);
         }

         template <typename T>
         inline std::string num_to_string(const T v)
         {
            const typename details::number_type<T>::type num_type;
            return to_string_impl(v, num_type);
         }

         template <typename T>
         inline T lth(const T v0, const T v1)
         {
            const typename details::number_type<T>::type num_type;
            return lth_impl(v0, v1, num_type);
         }

         template <typename T>
         inline T leq(const T v0, const T v1)
         {
            const typename details::number_type<T>::type num_type;
            return leq_impl(v0, v1, num_type);
         }

         template <typename T>
         inline T geq(const T v0, const T v1)
         {
            const typename details::number_type<T>::type num_type;
            return geq_impl(v0, v1, num_type);
         }

         template <typename T>
         inline T gth(const T v0, const T v1)
         {
            const typename details::number_type<T>::type num_type;
            return gth_impl(v0, v1, num_type);
         }

         template <typename T>
         inline T equal(const T v0, const T v1)
         {
            const typename details::number_type<T>::type num_type;
            return equal_impl(v0, v1, num_type);
         }

         template <typename T>
         inline T nequal(const T v0, const T v1)
         {
            const typename details::number_type<T>::type num_type;
            return nequal_impl(v0, v1, num_type);
         }

         template <typename T>
         inline T max_num(const T v0, const T v1)
         {
            const typename details::number_type<T>::type num_type;
            return max_num_impl(v0, v1, num_type);
         }

         template <typename T>
         inline T min_num(const T v0, const T v1)
         {
            const typename details::number_type<T>::type num_type;
            return min_num_impl(v0, v1, num_type);
         }

         template <typename T>
         inline T pow(const T v0, const T v1)
         {
            const typename details::number_type<T>::type num_type;
            return pow_impl(v0, v1, num_type);
         }

         template <typename T>
         inline T logn(const T v0, const T v1)
         {
            const typename details::number_type<T>::type num_type;
            return logn_impl(v0, v1, num_type);
         }

         template <typename T>
         inline T root(const T v0, const T v1)
         {
            const typename details::number_type<T>::type num_type;
            return root_impl(v0, v1, num_type);
         }

         template <typename T>
         inline T atan2(const T v0, const T v1)
         {
            const typename details::number_type<T>::type num_type;
            return atan2_impl(v0, v1, num_type);
         }

         template <typename T>
         inline bool is_integer(const T v)
         {
            const typename details::number_type<T>::type num_type;
            return is_integer_impl(v, num_type);
         }

         template <typename T, unsigned int N>
         struct fast_exp
         {
            static inline T result(T v)
            {
               unsigned int k = N;
               T l = T(1);

               while (k)
               {
                  if (1 == (k % 2))
                  {
                     l *= v;
                     --k;
                  }

                  v *= v;
                  k /= 2;
               }

               return l;
            }
         };

         template <typename T> struct fast_exp<T,10> { static inline T result(const T v) { T v_5 = fast_exp<T,5>::result(v); return v_5 * v_5; } };
         template <typename T> struct fast_exp<T, 9> { static inline T result(const T v) { return fast_exp<T,8>::result(v) * v; } };
         template <typename T> struct fast_exp<T, 8> { static inline T result(const T v) { T v_4 = fast_exp<T,4>::result(v); return v_4 * v_4; } };
         template <typename T> struct fast_exp<T, 7> { static inline T result(const T v) { return fast_exp<T,6>::result(v) * v; } };
         template <typename T> struct fast_exp<T, 6> { static inline T result(const T v) { T v_3 = fast_exp<T,3>::result(v); return v_3 * v_3; } };
         template <typename T> struct fast_exp<T, 5> { static inline T result(const T v) { return fast_exp<T,4>::result(v) * v; } };
         template <typename T> struct fast_exp<T, 4> { static inline T result(const T v) { T v_2 = v * v; return v_2 * v_2; } };
         template <typename T> struct fast_exp<T, 3> { static inline T result(const T v) { return v * v * v; } };
         template <typename T> struct fast_exp<T, 2> { static inline T result(const T v) { return v * v;     } };
         template <typename T> struct fast_exp<T, 1> { static inline T result(const T v) { return v;         } };
         template <typename T> struct fast_exp<T, 0> { static inline T result(const T  ) { return T(1);      } };

         #define exprtk_define_unary_function(FunctionName)        \
         template <typename T>                                     \
         inline T FunctionName (const T v)                         \
         {                                                         \
            const typename details::number_type<T>::type num_type; \
            return  FunctionName##_impl(v,num_type);               \
         }                                                         \

         exprtk_define_unary_function(abs  )
         exprtk_define_unary_function(acos )
         exprtk_define_unary_function(acosh)
         exprtk_define_unary_function(asin )
         exprtk_define_unary_function(asinh)
         exprtk_define_unary_function(atan )
         exprtk_define_unary_function(atanh)
         exprtk_define_unary_function(cos  )
         exprtk_define_unary_function(cosh )
         exprtk_define_unary_function(exp  )
         exprtk_define_unary_function(expm1)
         exprtk_define_unary_function(log  )
         exprtk_define_unary_function(log10)
         exprtk_define_unary_function(log2 )
         exprtk_define_unary_function(log1p)
         exprtk_define_unary_function(neg  )
         exprtk_define_unary_function(pos  )
         exprtk_define_unary_function(sin  )
         exprtk_define_unary_function(sinc )
         exprtk_define_unary_function(sinh )
         exprtk_define_unary_function(sqrt )
         exprtk_define_unary_function(tan  )
         exprtk_define_unary_function(tanh )
         exprtk_define_unary_function(cot  )
         exprtk_define_unary_function(sec  )
         exprtk_define_unary_function(csc  )
         exprtk_define_unary_function(erf  )
         exprtk_define_unary_function(erfc )
         exprtk_define_unary_function(ncdf )
         #undef exprtk_define_unary_function
      }

      template <typename T>
      inline T compute_pow10(T d, const int exponent)
      {
         static const double fract10[] =
         {
           0.0,
           1.0E+001, 1.0E+002, 1.0E+003, 1.0E+004, 1.0E+005, 1.0E+006, 1.0E+007, 1.0E+008, 1.0E+009, 1.0E+010,
           1.0E+011, 1.0E+012, 1.0E+013, 1.0E+014, 1.0E+015, 1.0E+016, 1.0E+017, 1.0E+018, 1.0E+019, 1.0E+020,
           1.0E+021, 1.0E+022, 1.0E+023, 1.0E+024, 1.0E+025, 1.0E+026, 1.0E+027, 1.0E+028, 1.0E+029, 1.0E+030,
           1.0E+031, 1.0E+032, 1.0E+033, 1.0E+034, 1.0E+035, 1.0E+036, 1.0E+037, 1.0E+038, 1.0E+039, 1.0E+040,
           1.0E+041, 1.0E+042, 1.0E+043, 1.0E+044, 1.0E+045, 1.0E+046, 1.0E+047, 1.0E+048, 1.0E+049, 1.0E+050,
           1.0E+051, 1.0E+052, 1.0E+053, 1.0E+054, 1.0E+055, 1.0E+056, 1.0E+057, 1.0E+058, 1.0E+059, 1.0E+060,
           1.0E+061, 1.0E+062, 1.0E+063, 1.0E+064, 1.0E+065, 1.0E+066, 1.0E+067, 1.0E+068, 1.0E+069, 1.0E+070,
           1.0E+071, 1.0E+072, 1.0E+073, 1.0E+074, 1.0E+075, 1.0E+076, 1.0E+077, 1.0E+078, 1.0E+079, 1.0E+080,
           1.0E+081, 1.0E+082, 1.0E+083, 1.0E+084, 1.0E+085, 1.0E+086, 1.0E+087, 1.0E+088, 1.0E+089, 1.0E+090,
           1.0E+091, 1.0E+092, 1.0E+093, 1.0E+094, 1.0E+095, 1.0E+096, 1.0E+097, 1.0E+098, 1.0E+099, 1.0E+100,
           1.0E+101, 1.0E+102, 1.0E+103, 1.0E+104, 1.0E+105, 1.0E+106, 1.0E+107, 1.0E+108, 1.0E+109, 1.0E+110,
           1.0E+111, 1.0E+112, 1.0E+113, 1.0E+114, 1.0E+115, 1.0E+116, 1.0E+117, 1.0E+118, 1.0E+119, 1.0E+120,
           1.0E+121, 1.0E+122, 1.0E+123, 1.0E+124, 1.0E+125, 1.0E+126, 1.0E+127, 1.0E+128, 1.0E+129, 1.0E+130,
           1.0E+131, 1.0E+132, 1.0E+133, 1.0E+134, 1.0E+135, 1.0E+136, 1.0E+137, 1.0E+138, 1.0E+139, 1.0E+140,
           1.0E+141, 1.0E+142, 1.0E+143, 1.0E+144, 1.0E+145, 1.0E+146, 1.0E+147, 1.0E+148, 1.0E+149, 1.0E+150,
           1.0E+151, 1.0E+152, 1.0E+153, 1.0E+154, 1.0E+155, 1.0E+156, 1.0E+157, 1.0E+158, 1.0E+159, 1.0E+160,
           1.0E+161, 1.0E+162, 1.0E+163, 1.0E+164, 1.0E+165, 1.0E+166, 1.0E+167, 1.0E+168, 1.0E+169, 1.0E+170,
           1.0E+171, 1.0E+172, 1.0E+173, 1.0E+174, 1.0E+175, 1.0E+176, 1.0E+177, 1.0E+178, 1.0E+179, 1.0E+180,
           1.0E+181, 1.0E+182, 1.0E+183, 1.0E+184, 1.0E+185, 1.0E+186, 1.0E+187, 1.0E+188, 1.0E+189, 1.0E+190,
           1.0E+191, 1.0E+192, 1.0E+193, 1.0E+194, 1.0E+195, 1.0E+196, 1.0E+197, 1.0E+198, 1.0E+199, 1.0E+200,
           1.0E+201, 1.0E+202, 1.0E+203, 1.0E+204, 1.0E+205, 1.0E+206, 1.0E+207, 1.0E+208, 1.0E+209, 1.0E+210,
           1.0E+211, 1.0E+212, 1.0E+213, 1.0E+214, 1.0E+215, 1.0E+216, 1.0E+217, 1.0E+218, 1.0E+219, 1.0E+220,
           1.0E+221, 1.0E+222, 1.0E+223, 1.0E+224, 1.0E+225, 1.0E+226, 1.0E+227, 1.0E+228, 1.0E+229, 1.0E+230,
           1.0E+231, 1.0E+232, 1.0E+233, 1.0E+234, 1.0E+235, 1.0E+236, 1.0E+237, 1.0E+238, 1.0E+239, 1.0E+240,
           1.0E+241, 1.0E+242, 1.0E+243, 1.0E+244, 1.0E+245, 1.0E+246, 1.0E+247, 1.0E+248, 1.0E+249, 1.0E+250,
           1.0E+251, 1.0E+252, 1.0E+253, 1.0E+254, 1.0E+255, 1.0E+256, 1.0E+257, 1.0E+258, 1.0E+259, 1.0E+260,
           1.0E+261, 1.0E+262, 1.0E+263, 1.0E+264, 1.0E+265, 1.0E+266, 1.0E+267, 1.0E+268, 1.0E+269, 1.0E+270,
           1.0E+271, 1.0E+272, 1.0E+273, 1.0E+274, 1.0E+275, 1.0E+276, 1.0E+277, 1.0E+278, 1.0E+279, 1.0E+280,
           1.0E+281, 1.0E+282, 1.0E+283, 1.0E+284, 1.0E+285, 1.0E+286, 1.0E+287, 1.0E+288, 1.0E+289, 1.0E+290,
           1.0E+291, 1.0E+292, 1.0E+293, 1.0E+294, 1.0E+295, 1.0E+296, 1.0E+297, 1.0E+298, 1.0E+299, 1.0E+300,
           1.0E+301, 1.0E+302, 1.0E+303, 1.0E+304, 1.0E+305, 1.0E+306, 1.0E+307, 1.0E+308
         };

         static const int fract10_size = static_cast<int>(sizeof(fract10) / sizeof(double));

         const int e = std::abs(exponent);

         if (exponent >= std::numeric_limits<double>::min_exponent10)
         {
            if (e < fract10_size)
            {
               if (exponent > 0)
                  return T(d * T(fract10[e]));
               else
                  return T(d / T(fract10[e]));
            }
            else
               return T(d * T(std::pow(10.0, 10.0 * exponent)));
         }
         else
         {
                     d /= T(fract10[               - std::numeric_limits<double>::min_exponent10]);
            return T(d /  T(fract10[-exponent      + std::numeric_limits<double>::min_exponent10]));
         }
      }
   }
}