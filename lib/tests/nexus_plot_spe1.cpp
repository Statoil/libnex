/*
   Copyright (C) 2017 Statoil ASA, Norway.

   The file 'nexus_plot_load.cpp' is part of ERT - Ensemble based Reservoir Tool.

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
#include <array>
#include <fstream>
#include <iostream>
#include <vector>


#include <ert/util/test_util.hpp>
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
}

void test_spe1_header(const NexusPlot& spe1) {
    test_assert_int_equal(spe1.header.num_classes, 9);
    test_assert_int_equal(spe1.header.day,         1);
    test_assert_int_equal(spe1.header.month,       1);
    test_assert_int_equal(spe1.header.year,        1980);
    test_assert_int_equal(spe1.header.nx,          1);
    test_assert_int_equal(spe1.header.ny,          1);
    test_assert_int_equal(spe1.header.nz,          1);
    test_assert_int_equal(spe1.header.ncomp,       2);
}

void test_spe1_classes(const NexusPlot& spe1) {
    auto classes = unique( spe1, get::classname_str );
    for (const auto& c : classes) {
        auto f = std::find( spe1_classnames.begin(), spe1_classnames.end(), c );
        test_assert_true( f != spe1_classnames.end() );
    }
}

void test_spe1_timesteps(const NexusPlot& spe1) {
    auto data = spe1.data;
    std::vector< int32_t > timesteps;
    std::transform(data.begin(), data.end(), std::back_inserter( timesteps ),
                   get::timestep);
    std::sort( timesteps.begin(), timesteps.end() );
    timesteps.erase( std::unique( timesteps.begin(), timesteps.end() ),
                timesteps.end() );

    for (size_t i = 0; i < timesteps.size(); i++) {
        test_assert_int_equal( timesteps[i], spe1_timesteps[i] );
    }
}

void test_spe1_well_comulative_gas_injected(const NexusPlot& spe1) {
    std::vector< float > cgi_all_timesteps {{
        6.222797E+08F, 6.291119E+08F, 6.291336E+08F, 6.291336E+08F,
        6.291336E+08F, 6.291336E+08F, 6.291336E+08F, 6.291336E+08F,
        6.291336E+08F, 6.291336E+08F
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

    test_assert_int_equal( well_1_cgi.size(), cgi_all_timesteps.size() );
    for (size_t i = 0; i < cgi_all_timesteps.size(); i++) {
        test_assert_std_string_equal( get::varname_str(well_1_cgi[i]),
                                      std::string { "CGI" } );
        test_assert_float_equal( well_1_cgi[i].value, cgi_all_timesteps[i] );
    }
}

void test_spe1_region_rgs00002_ts97_all_variables(const NexusPlot& spe1) {
    std::vector< float > rgs00002_ts97_values {{
        4.011786E+06F, 1.171299E+09F, 2.301113E-01F, 0.000000E+00F,
        6.291336E+08F, 0.000000E+00F, 3.071745E+02F, 3.076096E+02F,
        4.116595E+04F, 9.679420E+06F, 1.007316E+04F, 7.339244E+02F,
        3.030684E+05F, 6.215015E-05F, 0.000000E+00F, 0.000000E+00F,
        0.000000E+00F, 0.000000E+00F, 3.354057E-08F, 0.000000E+00F,
        7.575931E-01F, 1.221395E-01F, 1.202674E-01F, 8.951913E+00F,
        1.980881E+06F, 1.869358E+02F, 3.075959E+02F, 3.080320E+02F,
        6.521049E+04F, 1.051327E+04F, 1.035212E+04F, 5.231004E+04F,
        9.115127E+03F, 8.709983E+01F, 0.000000E+00F, 8.607588E+04F,
        7.572375E+04F, 0.000000E+00F, 0.000000E+00F, 6.233132E+06F,
        3.475308E+06F, 2.375755E-01F, 0.000000E+00F, 1.436540E+06F,
        0.000000E+00F, 1.103374E+03F, 1.443378E+03F, 6.422060E-05F,
        0.000000E+00F, 0.000000E+00F, 0.000000E+00F, 1.428432E-02F,
        4.753889E+01F, 7.038322E-05F, 3.776984E+01F, 4.129422E+02F,
        8.468194E-08F, 8.468194E-08F, 7.952135E-01F, 2.047865E-01F,
        3.483044E+10F, 8.969672E+09F, 1.003638E+10F, 3.405106E+09F,
        1.084520E+09F, 2.292876E+02F, 0.000000E+00F, 5.836104E+08F,
        0.000000E+00F
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
    test_assert_int_equal( rgs00002_ts97_values.size(),
                           spe1_varnames[3].size() );
    test_assert_int_equal( rgs00002_ts97_values.size(),
                           rgs00002_ts97.size() );
    for (size_t i = 0; i < rgs00002_ts97.size(); i++) {
        test_assert_std_string_equal( get::classname_str(rgs00002_ts97[i]),
                                      std::string { "REGION" } );
        test_assert_std_string_equal( get::varname_str(rgs00002_ts97[i]),
                                      spe1_varnames[3][i] );
        test_assert_float_equal( rgs00002_ts97[i].value,
                                 rgs00002_ts97_values[i] );

    }
}

void test_spe1_class_varnames(const NexusPlot& plt) {
    auto data = plt.data;

    for (size_t i = 0; i < spe1_classnames.size(); i++) {
        auto vn = varnames( plt, spe1_classnames[i] );
        auto spe1_varnames_sorted = spe1_varnames[i];
        std::sort( spe1_varnames_sorted.begin(), spe1_varnames_sorted.end() );

        test_assert_int_equal( vn.size(),
                               spe1_varnames_sorted.size() );
        for (size_t k = 0; k < vn.size(); k++) {
            test_assert_std_string_equal( vn[k],
                                          spe1_varnames_sorted[k] );
        }
    }
}


int main(int argc, char* argv[]) {
    const auto spe1 = load("data/SPE1.plt");

    test_spe1_header(spe1);
    test_spe1_classes(spe1);
    test_spe1_timesteps(spe1);
    test_spe1_well_comulative_gas_injected(spe1);
    test_spe1_region_rgs00002_ts97_all_variables(spe1);
    test_spe1_class_varnames(spe1);
    return 0;
}
