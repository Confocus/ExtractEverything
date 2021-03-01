/*
The MIT License (MIT)

Copyright (c) 2020 Trail of Bits, Inc.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifdef _WIN32
#include <Windows.h>
#include <pe-parse/to_string.h>

namespace peparse {
std::string from_utf16(const UCharString &u) {
  std::string result;
  std::size_t size = WideCharToMultiByte(CP_UTF8,
                                         0,
                                         u.data(),
                                         static_cast<int>(u.size()),
                                         nullptr,
                                         0,
                                         nullptr,
                                         nullptr);

  if (size <= 0) {
    return result;
  }

  //result.reserve(size);
  char *name = new char[size + 1];
  WideCharToMultiByte(CP_UTF8,
                      0,
                      u.data(),
                      static_cast<int>(u.size()),
                      //&result[0],
					  name,
					  size,
                      nullptr,
                      nullptr);
  name[size] = 0;
  result = name;
  delete[]name;

  return result;
}
} // namespace peparse
#else
#include "to_string.h"

namespace peparse {
	std::string from_utf16(const UCharString &u) {

		std::string result;
		char *name = new char[u.size() + 1];
		if (static_cast<size_t>(-1) == wcstombs(name, reinterpret_cast<const wchar_t*>(u.data()), u.size() + 1))
		{
			delete[]name;
			return "";
		}

		name[u.size()] = 0;
		result = name;
		delete[]name;

		return result;
	}
} // namespace peparse
#endif
