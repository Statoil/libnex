import datetime
import unittest
import os
import tempfile
import shutil
import nex
import ecl.ecl as ecl


class TestLoad(unittest.TestCase):
    def setUp(self):
        self.plt = nex.load('test-data/SPE1.plt')
        self.plt_delayed = nex.load('test-data/SPE1_delayedWell.plt')
        self.work_area_path = tempfile.mkdtemp()
        self.cwd = os.getcwd()
        os.chdir(self.work_area_path)

    def tearDown(self):
        os.chdir(self.cwd)
        shutil.rmtree(self.work_area_path)

    def test_ecl_sum(self):
        ecl_sum = nex._nex2ecl(self.plt, 'ECL_CASE', format=False,
                               field_name='FIELD')
        self.assertTrue(isinstance(ecl_sum, ecl.ecl_sum.EclSum))

        start_date = ecl_sum.start_date
        self.assertEqual(start_date, datetime.date(1980, 1, 1))
        self.assertEqual(list(ecl_sum.wells()), ['1', '2'])
        self.assertEqual(ecl_sum.unit('FOPR'), 'SM3/DAY')
        self.assertEqual(len(ecl_sum.keys()), 53)
        self.assertEqual(len(ecl_sum), 10)

    def test_file_io(self):
        ecl_sum = nex._nex2ecl(self.plt, 'ECL_CASE', format=False,
                               field_name='FIELD')
        ecl_sum.fwrite()
        self.assertTrue(os.path.exists(
            os.path.join(os.getcwd(),'ECL_CASE.SMSPEC')))
        ecl_sum_reload = ecl.EclSum('ECL_CASE')
        self.assertEqual(len(ecl_sum), len(ecl_sum_reload))
        self.assertEqual(len(list(ecl_sum)), len(list(ecl_sum_reload)))
        self.assertEqual(list(ecl_sum.get_values('FOPR')),
                         list(ecl_sum_reload.get_values('FOPR')))

    def test_nex2ecl(self):
        ecl_sum = nex._nex2ecl(self.plt, 'ECL_CASE', format=False,
                               field_name='FIELD')

        QOP = self.plt.loc[
            (self.plt['classname'] == 'FIELD') &
            (self.plt['instancename'] == 'FIELD') &
            (self.plt['varname'] == 'QOP')]['value'].tolist()
        CGP = self.plt.loc[
            (self.plt['classname'] == 'WELL') &
            (self.plt['instancename'] == '2') &
            (self.plt['varname'] == 'CGP')]['value'].tolist()

        FOPR = list(ecl_sum.get_values('FOPR'))
        WGPT2 = list(ecl_sum.get_values('WGPT:2'))
        self.assertIn('FOPR', ecl_sum)
        self.assertIn('WGPT:2', ecl_sum)
        self.assertEqual(QOP, FOPR)
        self.assertEqual(CGP, WGPT2)

    # In nexus, instances aren't present in the dataset before the first
    # timestep they appear. In the dataset SPE1_delayedWell.plt, the well
    # instance "3" does not appear before timestep number 5. In ecl, however,
    # the well is present from the beginning. Because of this, the indices
    # where the data starts is different in the nex and ecl representations.
    def test_delayed_well(self):
        ecl_sum = nex._nex2ecl(self.plt_delayed, 'ECL_CASE', format=False,
                               field_name='FIELD')

        CGP = self.plt_delayed.loc[
            (self.plt_delayed['classname'] == 'WELL') &
            (self.plt_delayed['instancename'] == '3') &
            (self.plt_delayed['varname'] == 'CGP')]['value'].tolist()

        WOPR3 = list(ecl_sum.get_values('WGPT:3'))
        self.assertIn('WGPT:3', ecl_sum)
        self.assertEqual(CGP, WOPR3[4:])

    def test_multiple_fields(self):
        nex._nex2ecl(self.plt_delayed, 'ECL_CASE', format=False,
                     field_name='FIELD')
        nex._nex2ecl(self.plt_delayed, 'ECL_CASE', format=False,
                     field_name='NETWORK')
        with self.assertRaises(nex.ConversionError):
            nex._nex2ecl(self.plt_delayed, 'ECL_CASE', format=False,
                         field_name='INVALID')
