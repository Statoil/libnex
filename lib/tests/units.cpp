/*
   Copyright (C) 2017 Statoil ASA, Norway.

   The file 'units.cpp' is part of ERT - Ensemble based Reservoir Tool.

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
#include <iostream>
#include <random>

#include <catch/catch.hpp>

#include <nex/private/util.hpp>

namespace {

void test_single_conversion( const nex::UnitSystem& u,
                             nex::UnitSystem::Measure measure,
                             float n,
                             float expected_conversion ) {
    float c = n * u.conversion( measure );
    CHECK( n == Approx(c / expected_conversion) );
}

}

TEST_CASE( "unit conversions", "[units]") {
    const nex::UnitSystem u( nex::UnitSystem::UnitType::metric_bars );

    SECTION( "metric bars units" ) {
        auto compressibility             = u.unit_str(nex::UnitSystem::Measure::compressibility);
        auto concentration               = u.unit_str(nex::UnitSystem::Measure::concentration);
        auto density                     = u.unit_str(nex::UnitSystem::Measure::density);
        auto formation_volume_factor_gas = u.unit_str(nex::UnitSystem::Measure::formation_volume_factor_gas);
        auto formation_volume_factor_oil = u.unit_str(nex::UnitSystem::Measure::formation_volume_factor_oil);
        auto fraction                    = u.unit_str(nex::UnitSystem::Measure::fraction);
        auto gas_liquid_ratio            = u.unit_str(nex::UnitSystem::Measure::gas_liquid_ratio);
        auto identity                    = u.unit_str(nex::UnitSystem::Measure::identity);
        auto length                      = u.unit_str(nex::UnitSystem::Measure::length);
        auto moles                       = u.unit_str(nex::UnitSystem::Measure::moles);
        auto permeability                = u.unit_str(nex::UnitSystem::Measure::permeability);
        auto pressure                    = u.unit_str(nex::UnitSystem::Measure::pressure);
        auto pressure_absolute           = u.unit_str(nex::UnitSystem::Measure::pressure_absolute);
        auto reservoir_rates             = u.unit_str(nex::UnitSystem::Measure::reservoir_rates);
        auto reservoir_volumes           = u.unit_str(nex::UnitSystem::Measure::reservoir_volumes);
        auto surface_rates_gas           = u.unit_str(nex::UnitSystem::Measure::surface_rates_gas);
        auto surface_rates_liquid        = u.unit_str(nex::UnitSystem::Measure::surface_rates_liquid);
        auto surface_volumes_gas         = u.unit_str(nex::UnitSystem::Measure::surface_volumes_gas);
        auto surface_volumes_liquid      = u.unit_str(nex::UnitSystem::Measure::surface_volumes_liquid);
        auto temperature                 = u.unit_str(nex::UnitSystem::Measure::temperature);
        auto time                        = u.unit_str(nex::UnitSystem::Measure::time);
        auto viscosity                   = u.unit_str(nex::UnitSystem::Measure::viscosity);
        auto volume                      = u.unit_str(nex::UnitSystem::Measure::volume);
        auto water_cut                   = u.unit_str(nex::UnitSystem::Measure::water_cut);

        CHECK( compressibility             == "BARS-1");
        CHECK( concentration               == "KG/SM3");
        CHECK( density                     == "KG/M3");
        CHECK( formation_volume_factor_gas == "RM3/SM3");
        CHECK( formation_volume_factor_oil == "RM3/SM3");
        CHECK( fraction                    == "");
        CHECK( gas_liquid_ratio            == "SM3/SM3");
        CHECK( identity                    == "");
        CHECK( length                      == "M");
        CHECK( moles                       == "KG-M");
        CHECK( permeability                == "MD");
        CHECK( pressure                    == "BARS");
        CHECK( pressure_absolute           == "BARSA");
        CHECK( reservoir_rates             == "RM3/DAY");
        CHECK( reservoir_volumes           == "RM3");
        CHECK( surface_rates_gas           == "SM3/DAY");
        CHECK( surface_rates_liquid        == "SM3/DAY");
        CHECK( surface_volumes_gas         == "SM3");
        CHECK( surface_volumes_liquid      == "SM3");
        CHECK( temperature                 == "C");
        CHECK( time                        == "DAY");
        CHECK( viscosity                   == "CP");
        CHECK( volume                      == "M3");
        CHECK( water_cut                   == "SM3/SM3");
    }

    SECTION( "metric conversions" ) {
        std::default_random_engine gen;
        std::uniform_real_distribution<float> dist( -1e20, 1e20 );

        test_single_conversion( u, nex::UnitSystem::Measure::compressibility,             dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::density,                     dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::formation_volume_factor_gas, dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::formation_volume_factor_oil, dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::fraction,                    dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::gas_liquid_ratio,            dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::identity,                    dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::length,                      dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::moles,                       dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::permeability,                dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::pressure,                    dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::pressure_absolute,           dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::reservoir_rates,             dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::reservoir_volumes,           dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::surface_rates_gas,           dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::surface_rates_liquid,        dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::surface_volumes_gas,         dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::surface_volumes_liquid,      dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::temperature,                 dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::time,                        dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::viscosity,                   dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::volume,                      dist(gen), 1.0f    );
        test_single_conversion( u, nex::UnitSystem::Measure::water_cut,                   dist(gen), 1.0f    );

    }
}
