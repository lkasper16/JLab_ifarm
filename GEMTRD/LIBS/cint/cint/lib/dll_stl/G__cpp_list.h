/********************************************************************
* cint/cint/lib/dll_stl/G__cpp_list.h
* CAUTION: DON'T CHANGE THIS FILE. THIS FILE IS AUTOMATICALLY GENERATED
*          FROM HEADER FILES LISTED IN G__setup_cpp_environmentXXX().
*          CHANGE THOSE HEADER FILES AND REGENERATE THIS FILE.
********************************************************************/
#ifdef __CINT__
#error cint/cint/lib/dll_stl/G__cpp_list.h/C is only for compilation. Abort cint.
#endif
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#define G__ANSIHEADER
#define G__DICTIONARY
#define G__PRIVATE_GVALUE
#include "G__ci.h"
#include "FastAllocString.h"
extern "C" {
extern void G__cpp_setup_tagtable();
extern void G__cpp_setup_inheritance();
extern void G__cpp_setup_typetable();
extern void G__cpp_setup_memvar();
extern void G__cpp_setup_global();
extern void G__cpp_setup_memfunc();
extern void G__cpp_setup_func();
extern void G__set_cpp_environment();
}


#include "cint/cint/lib/dll_stl/lst.h"
#include <algorithm>
namespace std { }
using namespace std;

#ifndef G__MEMFUNCBODY
#endif

extern G__linked_taginfo G__LN_string;
extern G__linked_taginfo G__LN_allocatorlEintgR;
extern G__linked_taginfo G__LN_listlEintcOallocatorlEintgRsPgR;
extern G__linked_taginfo G__LN_listlEintcOallocatorlEintgRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_listlEintcOallocatorlEintgRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__LN_allocatorlElonggR;
extern G__linked_taginfo G__LN_listlElongcOallocatorlElonggRsPgR;
extern G__linked_taginfo G__LN_listlElongcOallocatorlElonggRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_listlElongcOallocatorlElonggRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__LN_allocatorlEfloatgR;
extern G__linked_taginfo G__LN_listlEfloatcOallocatorlEfloatgRsPgR;
extern G__linked_taginfo G__LN_listlEfloatcOallocatorlEfloatgRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_listlEfloatcOallocatorlEfloatgRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__LN_allocatorlEdoublegR;
extern G__linked_taginfo G__LN_listlEdoublecOallocatorlEdoublegRsPgR;
extern G__linked_taginfo G__LN_listlEdoublecOallocatorlEdoublegRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_listlEdoublecOallocatorlEdoublegRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__LN_allocatorlEvoidmUgR;
extern G__linked_taginfo G__LN_listlEvoidmUcOallocatorlEvoidmUgRsPgR;
extern G__linked_taginfo G__LN_listlEvoidmUcOallocatorlEvoidmUgRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_listlEvoidmUcOallocatorlEvoidmUgRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__LN_allocatorlEcharmUgR;
extern G__linked_taginfo G__LN_listlEcharmUcOallocatorlEcharmUgRsPgR;
extern G__linked_taginfo G__LN_listlEcharmUcOallocatorlEcharmUgRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_listlEcharmUcOallocatorlEcharmUgRsPgRcLcLreverse_iterator;
extern G__linked_taginfo G__LN_allocatorlEstringgR;
extern G__linked_taginfo G__LN_listlEstringcOallocatorlEstringgRsPgR;
extern G__linked_taginfo G__LN_listlEstringcOallocatorlEstringgRsPgRcLcLiterator;
extern G__linked_taginfo G__LN_listlEstringcOallocatorlEstringgRsPgRcLcLreverse_iterator;

/* STUB derived class for protected member access */
typedef allocator<int> G__allocatorlEintgR;
typedef list<int,allocator<int> > G__listlEintcOallocatorlEintgRsPgR;
typedef list<int,allocator<int> >::iterator G__listlEintcOallocatorlEintgRsPgRcLcLiterator;
typedef list<int,allocator<int> >::reverse_iterator G__listlEintcOallocatorlEintgRsPgRcLcLreverse_iterator;
typedef allocator<long> G__allocatorlElonggR;
typedef list<long,allocator<long> > G__listlElongcOallocatorlElonggRsPgR;
typedef list<long,allocator<long> >::iterator G__listlElongcOallocatorlElonggRsPgRcLcLiterator;
typedef list<long,allocator<long> >::reverse_iterator G__listlElongcOallocatorlElonggRsPgRcLcLreverse_iterator;
typedef allocator<float> G__allocatorlEfloatgR;
typedef list<float,allocator<float> > G__listlEfloatcOallocatorlEfloatgRsPgR;
typedef list<float,allocator<float> >::iterator G__listlEfloatcOallocatorlEfloatgRsPgRcLcLiterator;
typedef list<float,allocator<float> >::reverse_iterator G__listlEfloatcOallocatorlEfloatgRsPgRcLcLreverse_iterator;
typedef allocator<double> G__allocatorlEdoublegR;
typedef list<double,allocator<double> > G__listlEdoublecOallocatorlEdoublegRsPgR;
typedef list<double,allocator<double> >::iterator G__listlEdoublecOallocatorlEdoublegRsPgRcLcLiterator;
typedef list<double,allocator<double> >::reverse_iterator G__listlEdoublecOallocatorlEdoublegRsPgRcLcLreverse_iterator;
typedef allocator<void*> G__allocatorlEvoidmUgR;
typedef list<void*,allocator<void*> > G__listlEvoidmUcOallocatorlEvoidmUgRsPgR;
typedef list<void*,allocator<void*> >::iterator G__listlEvoidmUcOallocatorlEvoidmUgRsPgRcLcLiterator;
typedef list<void*,allocator<void*> >::reverse_iterator G__listlEvoidmUcOallocatorlEvoidmUgRsPgRcLcLreverse_iterator;
typedef allocator<char*> G__allocatorlEcharmUgR;
typedef list<char*,allocator<char*> > G__listlEcharmUcOallocatorlEcharmUgRsPgR;
typedef list<char*,allocator<char*> >::iterator G__listlEcharmUcOallocatorlEcharmUgRsPgRcLcLiterator;
typedef list<char*,allocator<char*> >::reverse_iterator G__listlEcharmUcOallocatorlEcharmUgRsPgRcLcLreverse_iterator;
typedef allocator<string> G__allocatorlEstringgR;
typedef list<string,allocator<string> > G__listlEstringcOallocatorlEstringgRsPgR;
typedef list<string,allocator<string> >::iterator G__listlEstringcOallocatorlEstringgRsPgRcLcLiterator;
typedef list<string,allocator<string> >::reverse_iterator G__listlEstringcOallocatorlEstringgRsPgRcLcLreverse_iterator;
