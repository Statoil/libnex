from .pynex import load as _load
import pandas as pd


def load(filename):
    """Load a binary format nexus plot file into a pandas DataFrame

    Args:
        filename (str): Path to a binary format nexus plot file.
    :rtype: DataFrame

    """

    raw = _load(filename)
    df = pd.DataFrame([{
        'timestep'     : d.timestep,
        'time'         : d.time,
        'classname'    : d.classname,
        'instancename' : d.instancename,
        'varname'      : d.varname,
        'value'        : d.value,
    } for d in raw._data])
    return df
