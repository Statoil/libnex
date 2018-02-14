import datetime
import unittest
import os
import nex
from nex._nex2ecl import nex2ecl, ConversionError

try:
    from ecl.summary import EclSum
except ImportError:
    from ert.ecl import EclSum

from ecl.util.test import TestAreaContext


class TestNex2Ecl(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.plt = nex.load('test-data/SPE1.plt')
        cls.plt_delayed = nex.load('test-data/SPE1_delayedWell.plt')

    def test_ecl_sum(self):
        ecl_sum = nex2ecl(self.plt, 'ECL_CASE', format=False,
                          field_name='FIELD')
        self.assertTrue(isinstance(ecl_sum, EclSum))

        start_date = ecl_sum.start_date
        self.assertEqual(start_date, datetime.date(1980, 1, 1))
        self.assertEqual(list(ecl_sum.wells()), ['1', '2'])
        self.assertEqual(ecl_sum.unit('FOPR'), 'SM3/DAY')
        self.assertEqual(len(ecl_sum.keys()), 53)
        self.assertEqual(len(ecl_sum), 10)

    def test_file_io(self):
        with TestAreaContext('test_file_io'):
            ecl_sum = nex2ecl(
                self.plt, 'ECL_CASE', format=False, field_name='FIELD')
            ecl_sum.fwrite()
            self.assertTrue(os.path.exists(
                os.path.join(os.getcwd(), 'ECL_CASE.SMSPEC')))

            ecl_sum_loaded = EclSum('ECL_CASE')
            self.assertEqual(len(ecl_sum), len(ecl_sum_loaded))
            self.assertIn('WGPT:2', ecl_sum_loaded)

            WGPT2 = list(ecl_sum_loaded.get_values('WGPT:2'))
            CGP = self.plt.loc[
                (self.plt['classname'] == 'WELL') &
                (self.plt['instancename'] == '2') &
                (self.plt['varname'] == 'CGP')]['value'].tolist()
            self.assertEqual(WGPT2, CGP)

            dates_loaded = ecl_sum_loaded.dates
            dates_plt = [self.plt.start_date + datetime.timedelta(days=days)
                         for days in self.plt.time.unique()]
            self.assertEqual(dates_loaded, dates_plt)

    def test_nex2ecl(self):
        ecl_sum = nex2ecl(self.plt, 'ECL_CASE', format=False,
                          field_name='FIELD')
        QOP = self.plt.loc[
            (self.plt['classname'] == 'FIELD') &
            (self.plt['instancename'] == 'FIELD') &
            (self.plt['varname'] == 'QOP')]['value'].tolist()
        FOPR = list(ecl_sum.get_values('FOPR'))
        self.assertEqual(QOP, FOPR)

    # In nexus, instances aren't present in the dataset before the first
    # timestep they appear. In the dataset SPE1_delayedself.plt, the well
    # instance "3" does not appear before timestep number 5. In ecl, however,
    # the well is present from the beginning. Because of this, the indices
    # where the data starts is different in the nex and ecl representations.
    def test_delayed_well(self):
        ecl_sum = nex2ecl(self.plt_delayed, 'ECL_CASE', format=False,
                          field_name='FIELD')
        CGP = self.plt_delayed.loc[
            (self.plt_delayed['classname'] == 'WELL') &
            (self.plt_delayed['instancename'] == '3') &
            (self.plt_delayed['varname'] == 'CGP')]['value'].tolist()
        WOPR3 = list(ecl_sum.get_values('WGPT:3'))
        self.assertEqual(CGP, WOPR3[4:])

    def test_multiple_fields(self):
        nex2ecl(self.plt_delayed, 'ECL_CASE', format=False,
                field_name='FIELD')
        nex2ecl(self.plt_delayed, 'ECL_CASE', format=False,
                field_name='NETWORK')
        with self.assertRaises(ConversionError):
            nex2ecl(self.plt_delayed, 'ECL_CASE', format=False,
                    field_name='INVALID')
        with self.assertRaises(ConversionError):
            nex2ecl(self.plt_delayed, 'ECL_CASE', format=False)


if __name__ == '__main__':
    unittest.main()

