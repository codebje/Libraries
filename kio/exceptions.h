#pragma once
/*	Copyright  (c)	Günter Woigk 1999 - 2020
					mailto:kio@little-bat.de

	This file is free software.

	Permission to use, copy, modify, distribute, and sell this software
	and its documentation for any purpose is hereby granted without fee,
	provided that the above copyright notice appears in all copies and
	that both that copyright notice, this permission notice and the
	following disclaimer appear in supporting documentation.

	THIS SOFTWARE IS PROVIDED "AS IS", WITHOUT ANY WARRANTY, NOT EVEN THE
	IMPLIED WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE
	AND IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY DAMAGES
	ARISING FROM THE USE OF THIS SOFTWARE,
	TO THE EXTENT PERMITTED BY APPLICABLE LAW.


	custom exceptions
*/


#include "kio/kio.h"
#include <new>
#include <exception>
class FD;


/*
	class std::exception
	{
		public:			exception	()			noexcept{}
		virtual			~exception	()			noexcept;
		virtual cstr	what		() const	noexcept;
	};

hierarchy:
	std::exception					// #include <exception>		base class
		bad_alloc					// #include <new>			out of memory
		std::bad_typeid				// #include <typeinfo>		Falscher Objekttyp
		std::bad_cast				// #include <typeinfo>		Falscher Objekttyp bei Typumwandlung
		std::bad_exception			// #include <exception>		unexpected()

		AnyError;					//							base class for own errors: includes error code
			InternalError			// #include "kio/abort.h"	ABORT, TRAP, assert, IERR, TODO
			LimitError;			//							Array<T>: array too large
			DataError;				//							data parsers, e.g. for input data from file
			FileError;				// "unix/file_utilities.h"

		std::logic_error			// #include <stdexcept>		theoretisch vermeidbare Laufzeitfehler
			std::invalid_argument	// #include <stdexcept>		stdc++ lib ((general function argument error))
			std::length_error		// #include <stdexcept>		stdc++ lib ((data exceeding allowed size))
			std::out_of_range		// #include <stdexcept>		stdc++ lib ((e.g. index))
			std::domain_error		// #include <stdexcept>		stdc++ lib ((domain of math. function))

		std::runtime_error			// #include <stdexcept>		fehlerhafte Daten zur Laufzeit
			std::range_error		// #include <stdexcept>		arith. Bereichsüberschreitung
			std::overflow_error		// #include <stdexcept>		arith. Überlauf
			std::underflow_error	// #include <stdexcept>		arith. Unterlauf
*/


// ---------------------------------------------
//			AnyError
// ---------------------------------------------

class AnyError : public std::exception
{
protected:
	cstr	msg = nullptr;	// allocated
	int		err;			// errno
	int		dummy;			// padding

public:
	explicit AnyError (cstr msg, ...)	noexcept __printflike(2,3);
	AnyError (cstr msg, va_list va)		noexcept __printflike(2,0);
	explicit AnyError (int err)			noexcept :err(err){}
	AnyError (int err, cstr msg)		noexcept;
	~AnyError ()						noexcept override;

	cstr what () const					noexcept override;
	int error() const noexcept { return err; }

	AnyError (const AnyError&)			noexcept;
	AnyError (AnyError&&)				noexcept;
	AnyError& operator= (const AnyError&) = delete;
	AnyError& operator= (AnyError&&) = delete;
};



// ---------------------------------------------
//			InternalError
// ---------------------------------------------

class InternalError : public AnyError
{
public:
	InternalError (cstr file, uint line, int err=internalerror) noexcept;
	InternalError (cstr file, uint line, cstr msg) noexcept;
};



// ---------------------------------------------
//			LimitError
// ---------------------------------------------

class LimitError : public AnyError
{
public:
	template<typename T>
	LimitError (cstr where, T sz, T max) noexcept : LimitError(where,long(sz),long(max)){}
	LimitError (cstr where, long sz, long max)	noexcept;
};



// ---------------------------------------------
//			DataError
// ---------------------------------------------

class DataError : public AnyError
{
public:
	 DataError ()						noexcept : AnyError(dataerror){}
	 DataError (int error)				noexcept : AnyError(error){}
	 DataError (int error, cstr text)	noexcept : AnyError(error,text){}
	 DataError (cstr msg, ...)			noexcept __printflike(2,3);
};



// ---------------------------------------------
//			FileError
// ---------------------------------------------

class FileError : public AnyError
{
public:
	cstr filepath;
	int  fd;

public:
	FileError (cstr path, int error)				noexcept;
	FileError (cstr path, int error, cstr msg)	noexcept;
	FileError (int fd, cstr path, int error)		noexcept;
	FileError (int fd, cstr path, int error, cstr msg) noexcept;
	FileError (const FD&, int error)				noexcept;
	FileError (const FD&, int error, cstr msg)	noexcept;

	FileError (FileError const&)					noexcept;
	FileError (FileError&&)						noexcept;

	FileError& operator= (FileError const&)	= delete;
	FileError& operator= (FileError&&)		= delete;
	~FileError ()								noexcept override;

	cstr what() const							noexcept override;
};
































