# MySSL 

A user-friedly interface to the OpenSSL library
=======================================================

Overview
--------

The MySSL library is a simple interface to OpenSSL library to permits 
user-friendly writing of Servers and Clients using OpenSSL.

Copyright © 2019 Aldo Abate <aldo.abate99@gmail.com>


Notes
-----

- This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
  MA 02110-1301, USA.

- The free software library "OpenSSL" is distributed under a "dual licensed" 
  system: under the OpenSSL License and the SSLeay License. The OpenSSL License
  is Apache License 1.0 and SSLeay License bears some similarity to a 4-clause 
  BSD License. Both licenses apply.

- The library "OpenSSL" reference version is 1.0.2g


Installation
------------

This is currently a beta version, so uses a very simple Makefile and to generate 
and test the SSL library you need to do some manual steps:

1. go to the src directory and generate the library with "make clean && make". 
   This generates a shared-library (libmyssl.so) and copies it to the tests/lib 
   directory. The header file myssl.h is also copied to the tests/include 
   directory.
2. position yourself in the tests directory and generate the two test programs 
   (sslserver and sslclient) with "make clean && make".
3. see the "Testing" paragraph of this file to see how to execute a simple test 
   of the MySSL library using two terminals.
4. note that in this beta version the library is not installed automatically, so 
   to perform the test you can install the libmyssl.so in the system or you can 
   do a temporary installation in both two terminals in this way:

   export LD_LIBRARY_PATH="~/path-of-myssl-package/tests/lib"

The next version of the MySSL library shall use Autotools to generate and install
automatically and making the package portable to many Unix-like systems.


Documentation
-------------

TODO


Testing
-------

Two tests are provided in *tests/server* and *tests/client* directories, you 
can freely edit the source code to fit your needs (it's Free Software :).

The two test programs are a SSL Server and a SSL Client that use the MYSSL 
library and, in addition to testing the library, they can serve as an example
of how to write code using the MySSL library. 

The two test directories provide basic (and working) examples of the SSL 
certificates used by the MySSL library to work.

For a quick test of MySSL library, you can run the following programs in two 
shells open in the directories tests/server and tests/client:

1. ./sslserver 8888
2. ./sslclient 127.0.0.1 8888
