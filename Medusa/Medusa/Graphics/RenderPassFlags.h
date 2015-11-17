// Copyright (c) 2015 fjz13. All rights reserved.
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file.
#pragma once
//[IGNORE_PRE_DECLARE_FILE]
#include "MedusaPreDeclares.h"
#include "Core/String/StringRef.h"
#include "Core/String/StringParser.h"
#include "Core/String/HeapString.h"
MEDUSA_BEGIN;
//STRONG_FLAGS_1(RenderPassFlags, SuppressWorldViewProjectMatrixUniform);
class RenderPassFlags	//[IGNORE_PRE_DECLARE] 
{
 template<int TValue,const StringRef& TName()>
 class RenderPassFlagsField	//[IGNORE_PRE_DECLARE]
{
friend class RenderPassFlags;
public:
 RenderPassFlagsField()
{
}
 bool operator==( RenderPassFlags rhs ) const
{
return TValue == rhs.mState;
}
 bool operator!=( RenderPassFlags rhs ) const
{
return TValue != rhs.mState;
}
 bool operator<( RenderPassFlags rhs ) const
{
return TValue < rhs.mState;
}
 bool operator>( RenderPassFlags rhs ) const
{
return TValue > rhs.mState;
}
 bool operator<=( RenderPassFlags rhs ) const
{
return TValue <= rhs.mState;
}
 bool operator>=( RenderPassFlags rhs ) const
{
return TValue >= rhs.mState;
}
 template<int TValue2,const StringRef& TName2()>
 bool operator==( RenderPassFlagsField<TValue2,TName2> rhs ) const
{
return TValue == TValue2;
}
 template<int TValue2,const StringRef& TName2()>
 bool operator!=( RenderPassFlagsField<TValue2,TName2> rhs ) const
{
return TValue != TValue2;
}
 template<int TValue2,const StringRef& TName2()>
 bool operator<( RenderPassFlagsField<TValue2,TName2> rhs ) const
{
return TValue < TValue2;
}
 template<int TValue2,const StringRef& TName2()>
 bool operator>( RenderPassFlagsField<TValue2,TName2> rhs ) const
{
return TValue > TValue2;
}
 template<int TValue2,const StringRef& TName2()>
 bool operator<=( RenderPassFlagsField<TValue2,TName2> rhs ) const
{
return TValue <= TValue2;
}
 template<int TValue2,const StringRef& TName2()>
 bool operator>=( RenderPassFlagsField<TValue2,TName2> rhs ) const
{
return TValue >= TValue2;
}
 RenderPassFlags operator|( RenderPassFlags rhs ) const
{
return RenderPassFlags( TValue | rhs.mState );
}
 RenderPassFlags operator&( RenderPassFlags rhs ) const
{
return RenderPassFlags( TValue & rhs.mState );
}
 RenderPassFlags operator^( RenderPassFlags rhs ) const
{
return RenderPassFlags( TValue ^ rhs.mState );
}
 RenderPassFlags operator~() const
{
return RenderPassFlags(~TValue);
}
 template<int TValue2,const StringRef& TName2()>
 RenderPassFlags operator|( RenderPassFlagsField<TValue2,TName2> rhs ) const
{
return RenderPassFlags(TValue | TValue2);
}
 template<int TValue2,const StringRef& TName2()>
 RenderPassFlags operator&( RenderPassFlagsField<TValue2,TName2> rhs ) const
{
return RenderPassFlags(TValue & TValue2);
}
 template<int TValue2,const StringRef& TName2()>
 RenderPassFlags operator^( RenderPassFlagsField<TValue2,TName2> rhs ) const
{
return RenderPassFlags(TValue ^ TValue2);
}
 static constexpr int IntValue=TValue;
 constexpr int ToInt()const
{
return TValue;
}
 StringRef ToString()const
{
return TName();
};
};
 protected:
int mState;
 public:
 const static bool IsEnum = true;
 RenderPassFlags( const RenderPassFlags& rhs ) : mState( rhs.mState ) 
{
}
 RenderPassFlags operator=( RenderPassFlags rhs )
{
mState = rhs.mState;
return *this;
}
 template<int TValue,const StringRef& TName()>
 RenderPassFlags(RenderPassFlagsField<TValue,TName> rhs ) : mState( TValue ) 
{
}
 template<int TValue,const StringRef& TName()>
 RenderPassFlags operator=( RenderPassFlagsField<TValue,TName> rhs )
{
mState = TValue;
return *this;
}
 bool operator==( RenderPassFlags rhs ) const
{
return mState == rhs.mState;
}
 bool operator!=( RenderPassFlags rhs ) const
{
return mState != rhs.mState;
}
 bool operator<( RenderPassFlags rhs ) const
{
return mState < rhs.mState;
}
 bool operator>( RenderPassFlags rhs ) const
{
return mState > rhs.mState;
}
 bool operator<=( RenderPassFlags rhs ) const
{
return mState <= rhs.mState;
}
 bool operator>=( RenderPassFlags rhs ) const
{
return mState >= rhs.mState;
}
 template<int TValue,const StringRef& TName()>
 bool operator==( RenderPassFlagsField<TValue,TName> rhs ) const
{
return mState == TValue;
}
 template<int TValue,const StringRef& TName()>
 bool operator!=( RenderPassFlagsField<TValue,TName> rhs ) const
{
return mState != TValue;
}
 template<int TValue,const StringRef& TName()>
 bool operator<( RenderPassFlagsField<TValue,TName> rhs ) const
{
return mState < TValue;
}
 template<int TValue,const StringRef& TName()>
 bool operator>( RenderPassFlagsField<TValue,TName> rhs ) const
{
return mState > TValue;
}
 template<int TValue,const StringRef& TName()>
 bool operator<=( RenderPassFlagsField<TValue,TName> rhs ) const
{
return mState <= TValue;
}
 template<int TValue,const StringRef& TName()>
 bool operator>=( RenderPassFlagsField<TValue,TName> rhs ) const
{
return mState >= TValue;
}
 RenderPassFlags operator|( RenderPassFlags rhs ) const
{
return RenderPassFlags( mState | rhs.mState );
}
 RenderPassFlags operator&( RenderPassFlags rhs ) const
{
return RenderPassFlags( mState & rhs.mState );
}
 RenderPassFlags operator^( RenderPassFlags rhs ) const
{
return RenderPassFlags( mState ^ rhs.mState );
}
 RenderPassFlags operator~() const
{
return RenderPassFlags( ~mState );
}
 template<int TValue,const StringRef& TName()>
 RenderPassFlags operator|( RenderPassFlagsField<TValue,TName> rhs ) const
{
return RenderPassFlags( mState | TValue );
}
 template<int TValue,const StringRef& TName()>
 RenderPassFlags operator&( RenderPassFlagsField<TValue,TName> rhs ) const
{
return RenderPassFlags( mState & TValue );
}
 template<int TValue,const StringRef& TName()>
 RenderPassFlags operator^( RenderPassFlagsField<TValue,TName> rhs ) const
{
return RenderPassFlags( mState ^ TValue );
}
 RenderPassFlags& operator|=( RenderPassFlags rhs )
{
mState |= rhs.mState;
return *this;
}
 RenderPassFlags& operator&=( RenderPassFlags rhs )
{
mState &= rhs.mState;
return *this;
}
 RenderPassFlags& operator^=( RenderPassFlags rhs )
{
mState ^= rhs.mState;
return *this;
}
 template<int TValue,const StringRef& TName()>
 RenderPassFlags operator|=( RenderPassFlagsField<TValue,TName> rhs )
{
mState |= TValue;
return RenderPassFlags( *this );
}
 template<int TValue,const StringRef& TName()>
 RenderPassFlags operator&=( RenderPassFlagsField<TValue,TName> rhs )
{
mState &= TValue;
return RenderPassFlags( *this );
}
 template<int TValue,const StringRef& TName()>
 RenderPassFlags operator^=( RenderPassFlagsField<TValue,TName> rhs )
{
mState ^= TValue;
return RenderPassFlags( *this );
}
 RenderPassFlags operator<<( unsigned int s )
{
return RenderPassFlags( mState << s );
}
 RenderPassFlags operator>>( unsigned int s )
{
return RenderPassFlags( mState >> s );
}
 RenderPassFlags& operator<<=( unsigned int s )
{
mState <<= s;
return *this;
}
 RenderPassFlags& operator>>=( unsigned int s )
{
mState >>= s;
return *this;
}
 int ToInt()const
{
return mState;
}
 bool IsZero()const
{
return mState==0;
}
 bool HasValue()const
{
return mState!=0;
}
 bool Has(RenderPassFlags rhs)const
{
return (mState&rhs.ToInt())==rhs.ToInt();
}
 void Set(RenderPassFlags rhs)
{
mState|=rhs.ToInt();
}
 void Remove(RenderPassFlags rhs)
{
mState&=~rhs.ToInt();
}
 void SetIf(RenderPassFlags rhs,bool isSet)
{
if(isSet)
{
mState|=rhs.ToInt();
}
}
 void RemoveIf(RenderPassFlags rhs,bool isSet)
{
if(isSet)
{
mState&=~rhs.ToInt();
}
}
 void SetOrRemoveIf(RenderPassFlags rhs,bool isSet)
{
if(isSet)
{
mState|=rhs.ToInt();
}
else
{
mState&=~rhs.ToInt();
}
}
 template<int TValue,const StringRef& TName()>
 bool Has(RenderPassFlagsField<TValue,TName> field)const
{
return (mState&TValue)==TValue;
}
 template<int TValue,const StringRef& TName()>
 void Set(RenderPassFlagsField<TValue,TName> field)
{
mState|=TValue;
}
 template<int TValue,const StringRef& TName()>
 void Remove(RenderPassFlagsField<TValue,TName> field)
{
mState&=~TValue;
}
 template<int TValue,const StringRef& TName()>
 void SetOrRemoveIf(RenderPassFlagsField<TValue,TName> field,bool isSet)
{
if(isSet)
{
mState|=TValue;
}
else
{
mState&=~TValue;
}
}
 bool ForceHasFlag(int flag)const
{
return (mState&flag)==flag;
}
 void ForceSetOrRemoveFlagIf(int flag,bool isSet)
{
if(isSet)
{
mState|=flag;
}
else
{
mState&=~flag;
}
}
 void ForceSetFlag(int flag)
{
mState|=flag;
}
 void ForceRemoveFlag(int flag)
{
mState&=~flag;
}
 void ForceSet(int state)
{
mState=state;
}
 int* ForceGetPointer()
{
return &mState;
}
 int& ForceGetReference()
{
return mState;
}
 unsigned int ToUInt()const
{
return (unsigned int)mState;
}
 explicit operator unsigned int()const
{
return (unsigned int)mState;
}
 explicit operator int()const
{
return (int)mState;
}
 intp HashCode()const
{
return mState;
};
 public:
explicit RenderPassFlags(int state=0):mState(state)
{
}
 private:
static const StringRef& OnNameNone()
{
return NoneName;
}
static const StringRef& OnNameAll()
{
return AllName;
}
static const StringRef& OnName0()
{
return Names[0];
}
public:
HeapString ToString()const
{
if (mState == None.IntValue) 
{
 return NoneName;
 
}
if (mState == All.IntValue) 
{
 return AllName;
 
}
HeapString result;
for(uint i=0;i<Size;++i)
{
if ((mState & Values[i]) == Values[i]) 
{
 if (!result.IsEmpty()) 
{
 result += '|';
 
}
result += Names[i];
 
}
}
return result;
}
static bool IsDefined(int val)
{
for(uint i=0;i<Size;++i)
{
if(Values[i]==val) return true;
}
return false;
}
static bool HasField(const StringRef& name,bool ignoreCase=false)
{
for(uint i=0;i<Size;++i)
{
if(Names[i].Compare(name,ignoreCase)==0) return true;
}
return false;
}
static RenderPassFlags Parse(int val)
{
if(IsDefined(val))
{
return RenderPassFlags(val);
}
MEDUSA_ASSERT_FAILED("Invalid flags val.");
return RenderPassFlags(val);
}
static bool TryParse(int val,RenderPassFlags& outFlags)
{
if(IsDefined(val))
{
outFlags.ForceSet(val);
return true;
}
return false;
}
static RenderPassFlags Parse(const StringRef& name,bool ignoreCase=false)
{
if (name.Compare(NoneName, ignoreCase) == 0) 
{
 return None;
 
}
 if (name.Compare(AllName, ignoreCase) == 0) 
{
 return All;
 
}
RenderPassFlags result;
bool isSucceed=true;
uint j=0;
List<HeapString> names;
bool splitResult=StringParser::Split<char>(name,"|",names);
if(!splitResult)
{
MEDUSA_ASSERT_FAILED("Invalid flags names");
}
for(auto i = (names).GetEnumerator();i.MoveNext();)
{
const HeapString& str=*i;
if (isSucceed)
{
for(j=0;j<Size;++j)
{
if (str.Compare(Names[j],ignoreCase)==0)
{
result.ForceSetFlag(Values[j]);
break;
}
}
if(j==Size)
{
isSucceed=false;
}
}
}
if(!isSucceed)
{
MEDUSA_ASSERT_FAILED("Invalid flags names");
}
return result;
}
static bool TryParse(RenderPassFlags& outFlags,const StringRef& name,bool ignoreCase=false)
{
if (name.Compare(NoneName, ignoreCase) == 0) 
{
 outFlags = None;
 return true;
 
}
 if (name.Compare(AllName, ignoreCase) == 0) 
{
 outFlags = All;
 return true;
 
}
bool isSucceed=true;
uint j=0;
List<HeapString> names;
if ((StringParser::Split<char>(name,"|",names))==false)
{
return false;
};
for(auto i = (names).GetEnumerator();i.MoveNext();)
{
const HeapString& str=*i;
if (isSucceed)
{
for(j=0;j<Size;++j)
{
if (str.Compare(Names[j],ignoreCase)==0)
{
outFlags.ForceSetFlag(Values[j]);
break;
}
}
if(j==Size)
{
isSucceed=false;
}
}
}
return isSucceed;
}
static constexpr unsigned int Size=1;
static const StringRef NoneName;
static const StringRef AllName;
static const StringRef Names[Size];
static const int Values[Size];
static const RenderPassFlagsField<0,RenderPassFlags::OnNameNone> None;
bool IsNone()const 
{
 return mState == None.IntValue;
 
}
static const RenderPassFlagsField<1,RenderPassFlags::OnNameAll> All;
bool IsAll()const 
{
 return mState == All.IntValue;
 
}
static const RenderPassFlagsField<1,RenderPassFlags::OnName0> SuppressWorldViewProjectMatrixUniform;
bool IsSuppressWorldViewProjectMatrixUniform()const 
{
 return (mState&SuppressWorldViewProjectMatrixUniform.IntValue) == SuppressWorldViewProjectMatrixUniform.IntValue;
 
}
};
MEDUSA_WEAK_MULTIPLE_DEFINE const StringRef RenderPassFlags::NoneName = "None";
MEDUSA_WEAK_MULTIPLE_DEFINE const StringRef RenderPassFlags::AllName = "All";
MEDUSA_WEAK_MULTIPLE_DEFINE const StringRef RenderPassFlags::Names[Size]=
{
"SuppressWorldViewProjectMatrixUniform"
};
MEDUSA_WEAK_MULTIPLE_DEFINE const int RenderPassFlags::Values[Size]=
{
1
};
MEDUSA_WEAK_MULTIPLE_DEFINE const RenderPassFlags::RenderPassFlagsField<0,RenderPassFlags::OnNameNone> RenderPassFlags::None;
MEDUSA_WEAK_MULTIPLE_DEFINE const RenderPassFlags::RenderPassFlagsField<1,RenderPassFlags::OnNameAll> RenderPassFlags::All;
 MEDUSA_WEAK_MULTIPLE_DEFINE const RenderPassFlags::RenderPassFlagsField<1,RenderPassFlags::OnName0> RenderPassFlags::SuppressWorldViewProjectMatrixUniform;
MEDUSA_END;
