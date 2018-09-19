import datetime
from .pynex import load as _load
import pandas


def load(filename):
    """Load a binary format nexus plot file into a pandas DataFrame
    Args:
        filename (str): Path to a binary format nexus plot file.
    :rtype: DataFrame
    """

    raw = _load(filename)
    D = raw._data
    df = pandas.DataFrame()

    df['timestep'] = [d.timestep for d in D]
    df['time'] = [d.time for d in D]
    df['classname'] = [d.classname for d in D]
    df['instancename'] = [d.instancename for d in D]
    df['varname'] = [d.varname for d in D]
    df['value'] = [d.value for d in D]

    df.start_date = datetime.datetime(
        year=raw._header.year,
        month=raw._header.month,
        day=raw._header.day,
    )
    df.nx = raw._header.nx
    df.ny = raw._header.ny
    df.nz = raw._header.nz
    df._unit_system = raw._header.unit_system
    return df
