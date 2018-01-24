#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cctype>
#include <algorithm>

#include <nex/private/nexus_plot.hpp>

namespace py = pybind11;

namespace {

std::string strip( const std::string& str ) {
    using namespace std;
    auto s = str;
    auto p = [](unsigned char c){ return isspace(c); };
    s.erase( remove_if( begin(s), end(s), p ), end(s) );
    return s;
}


/*
 * NexusData
 */

int32_t get_timestep( const nex::NexusData& nd ) {
    return nd.timestep;
}

float get_time( const nex::NexusData& nd ) {
    return nd.time;
}

// int32_t get_max_perfs( const nex::NexusData& nd ) {
//     return nd.max_perfs;
// }

std::string get_classname( const nex::NexusData& nd ) {
    std::string str( nd.classname.begin(), nd.classname.end() );
    return strip( str );
}

std::string get_instancename( const nex::NexusData& nd ) {
    std::string str( nd.instancename.begin(), nd.instancename.end() );
    return strip( str );
}

std::string get_varname( const nex::NexusData& nd ) {
    std::string str( nd.varname.begin(), nd.varname.end() );
    return strip( str );
}

float get_value( const nex::NexusData& nd ) {
    return nd.value;
}


/*
 * NexusHeader
 */

int get_nx( const nex::NexusHeader& nh ) {
    return nh.nx;
}

int get_ny( const nex::NexusHeader& nh ) {
    return nh.ny;
}

int get_nz( const nex::NexusHeader& nh ) {
    return nh.nz;
}

int get_year( const nex::NexusHeader& nh ) {
    return nh.year;
}

int get_month( const nex::NexusHeader& nh ) {
    return nh.month;
}

int get_day( const nex::NexusHeader& nh ) {
    return nh.day;
}

const nex::UnitSystem& get_unit_system( const nex::NexusHeader& nh ) {
    return nh.unit_system;
}

std::string unit_str( const nex::UnitSystem& us, const std::string& s ) {
    return us.unit_str( s );
}

float conversion( const nex::UnitSystem& us, const std::string& s ) {
    return us.conversion( s );
}


/*
 * NexusPlot
 */
std::vector< nex::NexusData > get_data(const nex::NexusPlot& plt) {
    return plt.data;
};

nex::NexusHeader get_header(const nex::NexusPlot& plt) {
    return plt.header;
};

}

PYBIND11_MODULE(pynex, m) {

    py::class_< nex::UnitSystem >(m, "UnitSystem")
        .def_property_readonly("name", &nex::UnitSystem::name)
        .def("conversion", &conversion)
        .def("unit_str",   &unit_str)
        ;

    py::class_< nex::NexusHeader >(m, "NexusHeader")
        .def_property_readonly("nx",          &get_nx)
        .def_property_readonly("ny",          &get_ny)
        .def_property_readonly("nz",          &get_nz)
        .def_property_readonly("unit_system", &get_unit_system)
        .def_property_readonly("year",        &get_year)
        .def_property_readonly("month",       &get_month)
        .def_property_readonly("day",         &get_day)
        ;

    py::class_< nex::NexusData >(m, "NexusData")
        .def_property_readonly("timestep",     &get_timestep)
        .def_property_readonly("time",         &get_time)
        .def_property_readonly("classname",    &get_classname)
        .def_property_readonly("instancename", &get_instancename)
        .def_property_readonly("varname",      &get_varname)
        .def_property_readonly("value",        &get_value)
        ;

    py::class_< nex::NexusPlot >(m, "NexusPlot")
        .def_property_readonly("_data", &get_data)
        .def_property_readonly("_header", &get_header)
        ;

    m.def("load", &nex::load);

}
