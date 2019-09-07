#include <gtest/gtest.h>
#include "list.hpp"
#include "Struct_Symbol.h"
#include "WhatAmI.h"

template< int D > struct t_Test 
{ 
   enum { DIM = D }; 
   template< typename OUT> friend static OUT &operator<<(OUT &out, t_Test<D> )
   {
      return out << DIM;
   }
};

using myList = Meta::LIST5<t_Test<10>, t_Test<23>, t_Test<4>, t_Test<2>, t_Test<9> >::TYPE ;


TEST(MetaList, LIST5) {
   EXPECT_EQ(myList::LENGTH, 5);
   //using t_list = Meta::LIST5< 2, 1, 5, 3, 10>::TYPE; does not compile
}

TEST(MetaList, UnSortedInt) {
   std::stringstream strm;
   Meta::ListPrinter<myList>::print(strm);
   EXPECT_EQ( strm.str(), "10, 23, 4, 2, 9");
}

TEST(MetaList, SortedInt) {
   std::stringstream strm;
   Meta::ListPrinter< Meta::SORT< Meta::DIM_GT, myList>::TYPE >::print(strm);
   EXPECT_EQ(strm.str(), "23, 10, 9, 4, 2") << "an example of sort at compile time";
}

using LENGTH = SystemOfUnits::helpers::T_Symbol<'L'>;
using MASS = SystemOfUnits::helpers::T_Symbol<'M'>;
using Len1 = SystemOfUnits::helpers::t_SingleDim< LENGTH, 1 >;

TEST(MetaList, PrintSymbol) {
   EXPECT_EQ(std::string("[L]"), LENGTH::Symstr() ) << "Ensure base print works";
   EXPECT_EQ(std::string("[M]"), MASS::Symstr());
   //EXPECT_STREQ
}

TEST(MetaList, PrintSymbolWchar) {
   EXPECT_EQ(std::wstring(L"[L]"), LENGTH::Symstr<wchar_t>() ) << "Ensure base print works";
   EXPECT_EQ(std::wstring(L"[M]"), MASS::Symstr<wchar_t>());
}

TEST( MetaList, PrintBaseDim) {
   EXPECT_EQ(std::string("[L]"), Len1::t_BaseUnit::Symstr());
   EXPECT_EQ(std::string("[L]"), Len1::c_str());
}

TEST(MetaList, SortWithNeg) {
   using t_List = Meta::LIST5<t_Test<-10>, t_Test<23>, t_Test<4>, t_Test<2>, t_Test<9> >::TYPE;
   using t_Sorted = Meta::SORT<Meta::DIM_GT, t_List>::TYPE;
   using t_Last = Meta::At<t_Sorted, 4 >::RET;
   EXPECT_EQ(t_Last::DIM, -10);
}

TEST(MetaList, ListAtZero ) {
   using t_Sorted = Meta::SORT<Meta::DIM_GT, myList>::TYPE;
   using t_num0 = Meta::At< t_Sorted, 0 >::RET;
   EXPECT_EQ(t_num0::DIM, 23);
   using t_numUn = Meta::At< myList, 0 >::RET;
   EXPECT_EQ(t_numUn::DIM, 10) << "before the sort";
}

TEST(MetaList, ListAtFirst) {
   using t_Sorted = Meta::SORT<Meta::DIM_GT, myList>::TYPE;
   using t_num1 = Meta::At< t_Sorted, 1 >::RET;
   EXPECT_EQ(t_num1::DIM, 10);
}

TEST(MetaList, ListAtForth) {
   using t_Sorted = Meta::SORT<Meta::DIM_GT, myList>::TYPE;
   using t_num4 = Meta::At< t_Sorted, 4 >::RET;
   EXPECT_EQ(t_num4::DIM, 2);
}

TEST(MetaList, SizeSorted) {
   using t_Sorted = Meta::SORT<Meta::DIM_GT, myList>::TYPE;
   EXPECT_EQ(t_Sorted::LENGTH, 5);
}

#include "MetricTypes.h"

using t_MakeType = SOU::MakeType< Metric::AtomicUnit::Meter, AT::second, Metric::AtomicUnit::kilogram, Metric::AtomicUnit::kelvin, Metric::AtomicUnit::coulomb >;
using t_Joule = t_MakeType::MakeDim<2, -2, 1, 0, 0>::type;
using t_kilogram = t_MakeType::MakeDim<0, 0, 1, 0, 0>::type;

using t_Grav = t_MakeType::MakeDim< 3, -2, -1, 0, 0 >::type;

TEST(MetaList, BuildListWithUnit) {

   using T = t_Joule;
   using SystemOfUnits::t_BaseDim;

   using t_List = Meta::LIST5< t_BaseDim< typename T::Length, T::eL>, t_BaseDim< T::Time, T::et>, t_BaseDim< T::Mass, T::eM>, t_BaseDim< T::Tempeture, T::eT>, t_BaseDim< T::Charge, T::eQ > >;
   EXPECT_EQ(t_List::TYPE::LENGTH, 5);
}

TEST(MetaList, SortWithUnit) {
   using T = t_Joule;
   using SystemOfUnits::t_BaseDim;

   using t_List = Meta::LIST5< t_BaseDim< t_Joule::Length, T::eL>, t_BaseDim< T::Time, T::et>, t_BaseDim< T::Mass, T::eM>, t_BaseDim< T::Tempeture, T::eT>, t_BaseDim< T::Charge, T::eQ > >::TYPE;

   //using myListA = Meta::LIST5< t_BaseDim< T::Length, T::eL>, t_Test<23>, t_Test<4>, t_Test<2>, t_Test<9> >::TYPE;

   using t_Sort = Meta::SORT< Meta::DIM_GT, t_List >::TYPE;
   constexpr int x = Meta::At< t_Sort, 0 >::RET::DIM;
   EXPECT_EQ( x , 2) << "Not sure why it would not use the enum DIM";
}

TEST(Dim, CharFromSingleString) {

   using T = t_Joule;
   using SystemOfUnits::helpers::t_SingleDim;
   EXPECT_EQ(T::et, -2);

   using t_List = Meta::LIST5< t_SingleDim< T::Length, T::eL>, t_SingleDim< T::Time, T::et>, t_SingleDim< T::Mass, T::eM>, t_SingleDim< T::Tempeture, T::eT>, t_SingleDim< T::Charge, T::eQ > >::TYPE;
   using t_Sorted = Meta::SORT< Meta::DIM_GT, t_List >::TYPE;

   //std::string const retStr = Meta::At< t_Sorted, 0 >::RET::c_str();
   EXPECT_EQ(std::string( Meta::At< t_Sorted, 0 >::RET::c_str() ), "[L]^2");
   EXPECT_EQ(std::string(Meta::At< t_Sorted, 1 >::RET::c_str()), "[M]");
   EXPECT_EQ( int(Meta::At< t_Sorted, 2 >::RET::DIM), 0);
   EXPECT_EQ(int(Meta::At< t_Sorted, 4 >::RET::DIM), -2);
   //EXPECT_STREQ( (Meta::At< t_Sorted, 4 >::RET::c_str()), "[T]^2");

}

TEST(Dim, DISABLED_FirstTest ) {  // disabled since it is not ready for use.
   std::string const str = SystemOfUnits::Dim(t_Joule() );

   EXPECT_EQ(str, std::string("[L]^2[M]/[T]^2")); // << "The return of Dim is: " << str;
}

TEST(MetaList, InverseOne) {
   using t_Unk = t_MakeType::MakeDim< -3, -2, -1, 0, 0 >::type;
   EXPECT_EQ("1/[M][T]^2[L]^3", SystemOfUnits::Dim(t_Unk()));
   //EXPECT_EQ("1/);
}


