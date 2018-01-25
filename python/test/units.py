import unittest
import nex

class TestLoad(unittest.TestCase):

    def setUp(self):
        self.spe1 = 'test-data/SPE1.plt'

    def test_units(self):
        plt = nex.load(self.spe1)
        unit1 = plt._unit_system.unit_str('QOP')
        unit2 = plt._unit_system.unit_str('CPI')
        unit3 = plt._unit_system.unit_str('BHP')

        val1_conv = plt._unit_system.conversion('QOP')
        val2_conv = plt._unit_system.conversion('CPI')
        val3_conv = plt._unit_system.conversion('BHP')

        self.assertEqual(unit1, 'SM3/DAY')
        self.assertEqual(unit2, 'KG/SM3')
        self.assertEqual(unit3, 'BARS')

        self.assertEqual(val1_conv, 1.0)
        self.assertEqual(val2_conv, 1.0)
        self.assertEqual(val3_conv, 1.0)


if __name__ == '__main__':
    unittest.main()
