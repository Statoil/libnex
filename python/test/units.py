import unittest
import nex

class TestLoad(unittest.TestCase):

    def setUp(self):
        self.spe1 = 'test-data/SPE1.plt'

    def test_aosidjhoa(self):
        nex_var = ""
        value = 0
        measure = nex.units.measure(nex_var)
        ecl_var = nex.ecl.from_nex(nex_var)
        unit = nex.ecl.ecl_unit(measure)
        conversion = nex.ecl.unit_conversion(measure)
        print "{} {}".format(value * conversion, unit)