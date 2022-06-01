#include "pybind11/pybind11.h"
#include "pybind11/stl.h"

#include "headers.hpp"

PYBIND11_MODULE( pyCDB_Reader, m ) 
{
    m.doc( ) = "CDB Reader"; // optional module docstring

    m.def( "vtuwriter", &vtuwriter);
  
}
