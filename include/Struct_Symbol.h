// Struct supports the dimensional function. Used internally, not for use outside of SOU.
#ifndef SYSTEM_OF_UNITS_HELPERS_STRUCT_T_SYMBOL_H
#define SYSTEM_OF_UNITS_HELPERS_STRUCT_T_SYMBOL_H
#include <cstdint>
namespace SystemOfUnits {
   namespace helpers {
         template<unsigned char C > struct T_Symbol 
         { 
            enum:uint8_t{ sym = C };  /// used on the input of template OneDim<>

            // ideally the function is used by the sorting template and its output.  Currently not used.
            template<typename t_char = char>
            constexpr static inline auto Symstr() noexcept-> t_char const *
            {
               // creation of a static character string array.
               constexpr t_char static str[] = { '[', sym ,']', '\0' };
               return str;
            }
         };
   }

   template< typename BASE_UNIT > struct is_LENGTH
   {
      enum :bool { value = BASE_UNIT::sym == 'L' };
   };

   template<typename BASE_UNIT > struct is_TIME
   {
      enum:bool{ value = BASE_UNIT::sym == 'T' };
   };
   template<typename BASE_UNIT > struct is_MASS
   {
      enum :bool { value = BASE_UNIT::sym == 'M' };
   };
   template<typename BASE_UNIT > struct is_TEMPERATURE
   {
      enum :bool { value = BASE_UNIT::sym == 233 };
   };
   template<typename BASE_UNIT > struct is_CURRENT
   {
      enum :bool { value = BASE_UNIT::sym == 'A' };
   };

}
#endif

// Copyright � 2005-2018 "Curt" Leslie L. Martin, All rights reserved.
// curt.leslie.lewis.martin@gmail.com
//
// Permission to use, copy, modify, and distribute this software for any
// purpose is hereby granted without fee, provided that this copyright and
// permissions notice appear in all copies and derivatives.
//
// This software is provided "as is" without express or implied warranty.
