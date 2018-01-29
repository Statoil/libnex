/*
   Copyright (C) 2017 Statoil ASA, Norway.

   The file 'load.cpp' is part of ERT - Ensemble based Reservoir Tool.

   ERT is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   ERT is distributed in the hope that it will be useful, but WITHOUT ANY
   WARRANTY; without even the implied warranty of MERCHANTABILITY or
   FITNESS FOR A PARTICULAR PURPOSE.

   See the GNU General Public License at <http://www.gnu.org/licenses/gpl.html>
   for more details.
*/

#include <catch/catch.hpp>

#include <ert/util/test_util.hpp>
#include <nex/private/util.hpp>

TEST_CASE( "load", "[load]") {

    GIVEN( "an invalid header" ) {
        std::stringstream stream( "xxxxINVALID_HEADER" );
        CHECK_THROWS_AS( nex::load(stream), nex::bad_header );
        stream.str( "xxx" );
        CHECK_THROWS_AS( nex::load(stream), nex::bad_header );
    }

    GIVEN( "a valid header" ) {
        std::stringstream stream( "xxxxPLOT  BIN   " );
        CHECK_THROWS_AS( nex::load(stream), nex::unexpected_eof );
    }
}
