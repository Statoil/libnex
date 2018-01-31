import datetime
import textwrap
import warnings
from .pynex import load as _load
import pandas
try:
    from ecl.summary import EclSum
except ImportError:
    from ert.ecl import EclSum


def load(filename):
    """Load a binary format nexus plot file into a pandas DataFrame

    Args:
        filename (str): Path to a binary format nexus plot file.
    :rtype: DataFrame

    """

    raw = _load(filename)
    df = pandas.DataFrame([{
        'timestep': d.timestep,
        'time': d.time,
        'classname': d.classname,
        'instancename': d.instancename,
        'varname': d.varname,
        'value': d.value,
    } for d in raw._data])
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


class ConversionError(Exception):
    pass


def _nex2ecl(plt, case, format=True, field_name=None):
    field_names = plt[plt['classname'] == 'FIELD'].instancename.unique()
    if len(field_names) > 1 and not field_name:
        raise ConversionError('More than one field in plot', 1)
    if len(field_names) > 0 and field_name not in field_names:
        raise ConversionError('{} not in plot'.format(field_name), 2)

    if len(field_names) > 1:
        warnings.warn('Multifield model not supported by ecl')
    if len(field_names) > 0 and not field_name:
        field_name = field_names[0]

    kw_nex2ecl = {
        "QOP": "OPR",
        "QWP": "WPR",
        "QGP": "GPR",
        "GOR": "GOR",
        "WCUT": "WCT",
        "COP": "OPT",
        "CWP": "WPT",
        "CGP": "GPT",
        "QWI": "WIR",
        "QGI": "GIR",
        "CWI": "WIT",
        "CGI": "GIT",
        "QPP": "CPR",
        "CPP": "CPC",
        "COWP": "LPT",
        "QOWP": "LPR",
        "GOR": "GOR",
        "PRDW": "MWPT",
        "CCPP": "CPT",
        "CCPI": "CIT",
        "QPI": "CIR",
        "CPI": "CIC",
        "BHP": "BHP",
        "THP": "THP",
        "WPAV": "BP",
        "OIP": "OIP",
        "WIP": "WIP",
        "GIP": "GIP",
        "PAVH": "PR",
    }

    start_time = plt.start_date
    ecl_sum = EclSum.writer(
        case,
        start_time,
        plt.nx,
        plt.ny,
        plt.nz,
        fmt_output=format,
        unified=True,
        time_in_days=True,
        key_join_string=':'
    )

    times = plt.time.unique().tolist()
    ecl_values = {}

    field = plt.loc[(plt['classname'] == 'FIELD') & (plt['instancename'] == field_name)]
    for var in filter(kw_nex2ecl.__contains__, field.varname.unique()):
        unit = plt._unit_system.unit_str(var)
        conversion = plt._unit_system.conversion(var)
        node = ecl_sum.add_variable('F{}'.format(kw_nex2ecl[var]),
                                    wgname=None,
                                    num=-1,
                                    unit=unit,
                                    default_value=0.0)
        fv = field.loc[field['varname'] == var]
        ecl_values[node.get_key1()] = {
            time: fv[fv['time'] == time].value.iloc[0] * conversion
            for time in times if time in fv.time.values
        }

    # Well
    wells = plt.loc[plt['classname'] == 'WELL']
    for well_name in wells.instancename.unique():
        inst = wells.loc[wells['instancename'] == well_name]
        for var in filter(kw_nex2ecl.__contains__, inst.varname.unique()):
            unit = plt._unit_system.unit_str(var)
            conversion = plt._unit_system.conversion(var)
            node = ecl_sum.add_variable('W{}'.format(kw_nex2ecl[var]),
                                        wgname=well_name,
                                        num=-1,
                                        unit=unit,
                                        default_value=0.0)
            iv = inst.loc[inst['varname'] == var]
            ecl_values[node.get_key1()] = {
                time: iv[iv['time'] == time].value.iloc[0] * conversion
                for time in times if time in iv.time.values
            }

    failed = filter(lambda v: v not in kw_nex2ecl,
                    plt.varname.unique().tolist())
    if failed:
        msg = "could not convert nexus variables:\n    {}"
        failures = '\n    '.join(textwrap.wrap(
            ', '.join(map(str, failed)), 76))
        warnings.warn(msg.format(failures), RuntimeWarning)

    timesteps = [(ecl_sum.add_t_step(i + 1, t), t)
                 for i, t in enumerate(times)]
    for ts, time in timesteps:
        for key, value in ecl_values.items():
            if time in value:
                ts[key] = value[time]

    return ecl_sum
