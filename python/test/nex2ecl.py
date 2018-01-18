import datetime
import unittest
import os
import nex


class TestLoad(unittest.TestCase):

    def setUp(self):
        self.spe1 = 'test-data/SPE1_delayedWell.plt'

    def test_nex2ecl(self):
        plt = nex.load(self.spe1)
        ecl_sum = nex.nex._nex2ecl(plt, 'TEST_CASE')
        # self.assertTrue( ecl_sum_is_instance( ecl_sum ))


        QOP = list(plt.loc[
            (plt['classname'] == 'FIELD') &
            (plt['instancename'] == 'NETWORK') &
            (plt['varname'] == 'QOP')
            ]['value'].tolist())

        CGP = list(plt.loc[
               (plt['classname'] == 'WELL') &
               (plt['instancename'] == '3') &
               (plt['varname'] == 'CGP')
               ]['value'].tolist())


        self.assertTrue(ecl_sum.has_key('FOPR'))
        self.assertTrue(ecl_sum.has_key('WGPT:3'))

        FOPR = list(ecl_sum.get_values('FOPR'))
        WOPR3 = list(ecl_sum.get_values('WGPT:3'))

        self.assertEqual(QOP, FOPR)
        self.assertEqual(CGP, WOPR3[4:])


        start_date = ecl_sum.start_date
        self.assertEqual(start_date, datetime.date(1980,1,1))