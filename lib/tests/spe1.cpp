/*
   Copyright (C) 2017 Statoil ASA, Norway.

   The file 'spe1.cpp' is part of ERT - Ensemble based Reservoir Tool.

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
#include <algorithm>

#include <catch/catch.hpp>

#include <nex/private/util.hpp>

using namespace nex;

namespace {
    const std::vector< std::string > spe1_classnames {{
        "WELL", "NODE", "CONN", "REGION", "FIELD", "CONNLIST"
    }};
    const std::array< int, 9 > spe1_vars_in_class {{
        56, 52, 4, 43, 69, 58, 20, 25, 25
    }};
    const std::array< int, 10 > spe1_timesteps {{
        33, 46, 59, 72, 85, 89, 91, 92, 94, 97
    }};
    const std::array< std::vector< std::string >, 9 > spe1_varnames {{
        std::vector< std::string > {{
            "COP" , "CGP" , "CWP" , "CGI" , "CWI" , "QOP" , "QGP" , "QWP" ,
            "QGI" , "QWI" , "BHP" , "WPH" , "WKH" , "WPAV", "THP" , "COWP",
            "QOWP", "GOR" , "WCUT", "WOR" , "QGLG", "CGLG", "DRDN", "DRMX",
            "CROP", "CRGP", "CRWP", "CROI", "CRGI", "CRWI", "ROP" , "RGP" ,
            "RWP" , "ROI" , "RGI" , "RWI" , "ONTM", "ALQ" , "API" , "QCDP",
            "CCDP", "YCDP", "ACTV", "STAT", "Q1P" , "Q1I" , "C1P" , "C1I" ,
            "X1P" , "Y1P" , "Q2P" , "Q2I" , "C2P" , "C2I" , "X2P" , "Y2P"
        }},
        std::vector< std::string > {{
            "PNOD", "PDAT", "TNOD", "ACTV"
        }},
        std::vector< std::string > {{
            "QGAS", "QOIL", "QWTR", "CGAS", "COIL", "CWTR", "CBFG", "CBFO",
            "CBFW", "QGIS", "QOIS", "QWIS", "P_IN", "POUT", "T_IN", "TOUT",
            "ACTV", "STAT", "CSTR", "ITRG", "ONTM", "ALQ" , "SETM", "SETA",
            "POWM", "POWA", "SPDM", "SPDA", "API" , "DELP", "QTOT", "GVF" ,
            "EFF" , "POSN", "WCUT", "GOR" , "WOR" , "Q1"  , "Q2"  , "X1"  ,
            "X2"  , "Y1"  , "Y2"
        }},
        std::vector< std::string > {{
            "COP" , "CGP" , "CWP" , "COI" , "CGI" , "CWI" , "PAVT", "PAVH",
            "OIP" , "GIP" , "WIP" , "QOP" , "QGP" , "QWP" , "QOI" , "QGI" ,
            "QWI" , "OIN" , "GIN" , "WIN" , "SO"  , "SG"  , "SW"  , "OREC",
            "FGIP", "CIP" , "PAVE", "PAVD", "ROIP", "RGIP", "RWIP", "MRO" ,
            "MRG" , "MRW" , "NFLX", "PV"  , "HCPV", "TAVT", "TAVH", "CROP",
            "CRGP", "CRWP", "CROI", "CRGI", "CRWI", "ROP" , "RGP" , "RWP" ,
            "ROI" , "RGI" , "RWI" , "QCDP", "CCDP", "YCDP", "API" , "GOR" ,
            "WCUT", "WOR" , "Z1"  , "Z2"  , "MC1" , "MC2" , "MC3" , "C1P" ,
            "C2P" , "C3P" , "C1I" , "C2I" , "C3I"
        }},
        std::vector< std::string > {{
            "COP" , "CGP" , "CWP" , "CGI" , "CWI" , "QOP" , "QGP" , "QWP" ,
            "QGI" , "QWI" , "COWP", "QOWP", "GOR" , "OREC", "GREC", "PAVT",
            "PAVH", "QGLG", "CGLG", "WCUT", "NFLX", "CROP", "CRGP", "CRWP",
            "CROI", "CRGI", "CRWI", "ROP" , "RGP" , "RWP" , "ROI" , "RGI" ,
            "RWI" , "OIP" , "GIP" , "WIP" , "QCDP", "CCDP", "YCDP", "WLLS",
            "PRDW", "GLFW", "WINJ", "GINJ", "ACTW", "API" , "Q1P" , "Q1I" ,
            "C1P" , "C1I" , "X1P" , "Y1P" , "Q2P" , "Q2I" , "C2P" , "C2I" ,
            "X2P" , "Y2P"
        }},
        std::vector< std::string > {{
            "QOP", "QGP", "QWP", "QOI", "QGI", "QWI" , "COP", "CGP",
            "CWP", "COI", "CGI", "CWI", "API", "WCUT", "GOR", "WOR",
            "Q1P", "Q1I", "Q2P", "Q2I"
        }}
    }};

    const auto spe1 = load("test-data/SPE1.plt");
}

SCENARIO( "spe1", "[spe1]") { WHEN( "loading nexus plot file" ) {

    THEN( "header is correct" ) {
        CHECK( spe1.header.num_classes  == 9 );
        CHECK( spe1.header.day          == 1 );
        CHECK( spe1.header.month        == 1 );
        CHECK( spe1.header.year         == 1980 );
        CHECK( spe1.header.nx           == 1 );
        CHECK( spe1.header.ny           == 1 );
        CHECK( spe1.header.nz           == 1 );
        CHECK( spe1.header.ncomp        == 2 );
    }

    THEN( "classes are correct" ) {
        auto classes = unique( spe1, get::classname_str );
        for (const auto& c : classes) {
            auto f = std::find( spe1_classnames.begin(), spe1_classnames.end(), c );
            CHECK( f != spe1_classnames.end() );
        }
    }

    THEN( "timesteps are correct" ) {
        auto data = spe1.data;
        std::vector< int32_t > timesteps;
        std::transform( data.begin(), data.end(), std::back_inserter( timesteps ),
                        get::timestep );
        std::sort( timesteps.begin(), timesteps.end() );
        timesteps.erase( std::unique( timesteps.begin(), timesteps.end() ),
                                      timesteps.end() );

        for (size_t i = 0; i < timesteps.size(); i++) {
            CHECK( timesteps[i] == spe1_timesteps[i] );
        }
    }

    THEN( "well comulative gas injected is correct" ) {
        std::vector< float > cgi_all_timesteps {{
            6.222797E+08, 6.291119E+08, 6.291336E+08, 6.291336E+08, 6.291336E+08,
            6.291336E+08, 6.291336E+08, 6.291336E+08, 6.291336E+08, 6.291336E+08
        }};

        auto data = spe1.data;
        auto pred_well_1_cgi = [](const NexusData& d) {
            return is::classname("WELL")(d)
                && is::instancename("1")(d)
                && is::varname("CGI")(d);
        };
        std::vector< NexusData > well_1_cgi;
        std::copy_if( data.begin(), data.end(), std::back_inserter( well_1_cgi ),
                      pred_well_1_cgi );

        CHECK( well_1_cgi.size() == cgi_all_timesteps.size() );
        for (size_t i = 0; i < cgi_all_timesteps.size(); i++) {
            CHECK( get::varname_str(well_1_cgi[i]) == std::string { "CGI" } );
            CHECK( well_1_cgi[i].value == Approx(cgi_all_timesteps[i]) );
        }
    }

    THEN( "all region rgs00002 ts97 variables are correct" ) {
        std::vector< float > rgs00002_ts97_values {{
            4.011786E+06, 1.171299E+09, 2.301113E-01, 0.000000E+00, 6.291336E+08, 0.000000E+00,
            3.071745E+02, 3.076096E+02, 4.116595E+04, 9.679420E+06, 1.007316E+04, 7.339244E+02,
            3.030684E+05, 6.215015E-05, 0.000000E+00, 0.000000E+00, 0.000000E+00, 0.000000E+00,
            3.354057E-08, 0.000000E+00, 7.575931E-01, 1.221395E-01, 1.202674E-01, 8.951913E+00,
            1.980881E+06, 1.869358E+02, 3.075959E+02, 3.080320E+02, 6.521049E+04, 1.051327E+04,
            1.035212E+04, 5.231004E+04, 9.115127E+03, 8.709983E+01, 0.000000E+00, 8.607588E+04,
            7.572375E+04, 0.000000E+00, 0.000000E+00, 6.233132E+06, 3.475308E+06, 2.375755E-01,
            0.000000E+00, 1.436540E+06, 0.000000E+00, 1.103374E+03, 1.443378E+03, 6.422060E-05,
            0.000000E+00, 0.000000E+00, 0.000000E+00, 1.428432E-02, 4.753889E+01, 7.038322E-05,
            3.776984E+01, 4.129422E+02, 8.468194E-08, 8.468194E-08, 7.952135E-01, 2.047865E-01,
            3.483044E+10, 8.969672E+09, 1.003638E+10, 3.405106E+09, 1.084520E+09, 2.292876E+02,
            0.000000E+00, 5.836104E+08, 0.000000E+00
        }};

        auto data = spe1.data;

        auto pred_rgs00002_ts97 = [](const NexusData& d){
            return is::instancename( "RGS00002" )(d)
                && is::timestep( 97 )(d);
        };
        std::vector< NexusData > rgs00002_ts97;
        std::copy_if( data.begin(), data.end(), std::back_inserter( rgs00002_ts97 ),
                      pred_rgs00002_ts97 );

        // Test the test
        CHECK( rgs00002_ts97_values.size() == spe1_varnames[3].size() );
        CHECK( rgs00002_ts97_values.size() == rgs00002_ts97.size() );

        for (size_t i = 0; i < rgs00002_ts97.size(); i++) {
            CHECK( get::classname_str(rgs00002_ts97[i]) == std::string { "REGION" } );
            CHECK( get::varname_str(rgs00002_ts97[i]) == spe1_varnames[3][i] );
            CHECK( rgs00002_ts97[i].value == Approx(rgs00002_ts97_values[i]) );
        }
    }

    THEN( "test spe1 class varnames are correct" ) {
        auto data = spe1.data;

        for (size_t i = 0; i < spe1_classnames.size(); i++) {
            auto vn = varnames( spe1, spe1_classnames[i] );
            auto spe1_varnames_sorted = spe1_varnames[i];
            std::sort( spe1_varnames_sorted.begin(), spe1_varnames_sorted.end() );

            CHECK( vn.size() == spe1_varnames_sorted.size() );
            for (size_t k = 0; k < vn.size(); k++) {
                CHECK( vn[k] == spe1_varnames_sorted[k] );
            }
        }
    }
}}
