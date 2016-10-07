//
// File:	controlled_vars_limited_auto_init.cpp
// Object:	Test that the controlled_vars_limited_auto_init.h compiles
//              auto-initialize as expected.
//
// Copyright:	Copyright (c) 2011 Made to Order Software Corp.
//		All Rights Reserved.
//
// http://snapwebsites.org/
// contact@m2osw.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

// make sure we compile in debug mode otherwise we don't get the throws.
#ifndef CONTROLLED_VARS_DEBUG
#define CONTROLLED_VARS_DEBUG
#endif
#ifndef CONTROLLED_VARS_LIMITED
#define CONTROLLED_VARS_LIMITED
#endif
#include "controlled_vars_limited_auto_init.h"
#include "controlled_vars_limited_auto_enum_init.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>


typedef controlled_vars::limited_auto_init<int, 0, 100, 50> percent_t;
typedef controlled_vars::limited_auto_init<int, 0, 100, -50> bad_percent_t;

class Test
{
public:
	controlled_vars::flbool_t				f_false;
	controlled_vars::tlbool_t				f_true;
	percent_t						f_percent;
	//bad_percent_t						f_bad_percent;
};


int main(int /*argc*/, char * /*argv*/[])
{
	int e(0);
	Test t;

	if(t.f_false != false) {
		fprintf(stderr, "error: expected t.f_false to be false.\n");
		e = 1;
	}

	// This test fails when using 123 directly because the compiler
	// knows to convert the value to a bool! so we really have to
	// trick it to get a value other than just 0 or 1
// TODO: actually create a debug version! Right now _DEBUG does not get set
#ifdef _DEBUG
	// this works in debug but it gets optimized to the correct value
	// in release (i.e. the 123 becomes 1 and is accepted by the
	// assignment operator) so... I'm not too sure how to test this
	// in release mode at this point
	std::cerr << "info: test an invalid Boolean value (Debug version only)\n";
	char const boolean(123);
	bool bad_boolean;
	memcpy(&bad_boolean, &boolean, 1);
	try {
		// this must throw because the check uses a template<class L>
		// for the type instead of casting to bool first!
		t.f_false = bad_boolean;
		fprintf(stderr, "error: expected t.f_false to throw an exception on 123.\n");
		e = 1;
	}
	catch(const controlled_vars::controlled_vars_error&) {
		// it worked!
	}
#endif

	if(t.f_true != true) {
		fprintf(stderr, "error: expected t.f_true to be true.\n");
		e = 1;
	}

	if(t.f_percent != percent_t::DEFAULT_VALUE) {
		fprintf(stderr, "error: expected t.f_percent to be %d.\n", percent_t::DEFAULT_VALUE);
		e = 1;
	}

	try {
		t.f_percent = percent_t::MAX_BOUND + 1;
		fprintf(stderr, "error: expected t.f_percent to throw an exception on %d.\n", percent_t::MAX_BOUND + 1);
		e = 1;
	}
	catch(const controlled_vars::controlled_vars_error&) {
		// it worked!
	}

	try {
		t.f_percent = percent_t::MIN_BOUND - 1;
		fprintf(stderr, "error: expected t.f_percent to throw an exception on %d.\n", percent_t::MIN_BOUND - 1);
		e = 1;
	}
	catch(const controlled_vars::controlled_vars_error&) {
		// it worked!
	}

	exit(e);
}

