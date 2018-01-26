import unittest
import nex
import pandas
from ecl.ecl import EclSum
from ecl.test import TestAreaContext


class TestHist(unittest.TestCase):
    def setUp(self):
        pass

    def test_load_historical(self):
        hist_data = nex.load_historical('...')
        print(hist_data)

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
