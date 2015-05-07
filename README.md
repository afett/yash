[![Build Status](https://travis-ci.org/afett/yash.svg?branch=master)](https://travis-ci.org/afett/yash)

yash yet another signal header

This header implements a signal class in C++.

Callbacks managed by signals can be disconnected to avoid further calls.
The header is implemented in C++03 using the variadic macro extension
provided by g++ and clang++ as well as tr1/functional for std::function
and tr1/memory for std::shared_ptr and std::weak_ptr.

The API is reentrant but not thread safe.

The code can be found at: https://github.com/afett/yash
