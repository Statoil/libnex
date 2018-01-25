import unittest
import nex
import pandas
from ecl.test import TestAreaContext
from ecl.ecl import EclSum


class TestHistorical(unittest.TestCase):
    def test_load_historical(self):
        plt = nex.load_historical('test-data/historical.csv', 1, 1, 1)
        A08 = plt.loc[
            (plt['classname'] == 'WELL') &
            (plt['instancename'] == 'A-08') &
            (plt['varname'] == 'COP')
            ]['value'].tolist()
        self.assertEqual(plt.classname.unique().tolist(), ['WELL', 'FIELD'])
        self.assertEqual(A08, [0, 1, 3, 4, 5, 4, 3, 4, 6, 7])

    def test_convert_historical(self):
        plt = nex.load('test-data/SPE1.plt')
        hplt = pandas.DataFrame({
            'timestep': [0] * 5,
            'time': [400, 800, 1200, 1600, 2000],
            'classname': ['WELL'] * 5,
            'instancename': ['1'] * 5,
            'varname': ['QOP'] * 5,
            'value': [100, 200, 300, 400, 500],
        })
        hplt.start_date = plt.start_date
        hplt.nx = plt.nx
        hplt.ny = plt.ny
        hplt.nz = plt.nz
        hplt._unit_system = plt._unit_system
        with TestAreaContext('nex_test_convert_historical'):
            ecl_sum = nex._nex2ecl.nex2ecl(plt, 'SPE1', field_name='FIELD',
                                           hist_data=hplt)
            ecl_sum.fwrite()
            loaded = EclSum('SPE1')
            woprh = [n.value for n in list(loaded['WOPRH:1'])]
            qop = hplt.value.tolist()
            self.assertEqual(woprh[1:5], qop[1:])

if __name__ == '__main__':
    unittest.main()
