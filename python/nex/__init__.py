from __future__ import absolute_import

from .nex import load
from .historical import load_historical
import nex._nex2ecl as _nex2ecl

__version__ = '0.0.3'
__author__ = 'Software Innovation Bergen, Statoil ASA'

__copyright__ = 'Copyright 2017, Statoil ASA'
__license__ = 'GNU General Public License, version 3 or any later version'

__credits__ = __author__
__maintainer__ = __author__
__email__ = 'fg_gpl@statoil.com'
__status__ = 'Production'

__all__ = ['load', 'load_historical']
