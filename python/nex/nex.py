import datetime
from .pynex import load as _load
import ecl.ecl as ecl
import pandas


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
    df.grid_dimensions = (raw._header.nx, raw._header.ny, raw._header.nz)
    df._unit_system = raw._header.unit_system
    return df

def _nex2ecl(plt, format=True, field='FIELD', case='NEW_CASE'):

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
        "PAVH": "PR"}

    inv_kw_nex2ecl = {v: k for k, v in kw_nex2ecl.iteritems()}

    start_time = plt.start_date
    nx = plt.grid_dimensions[0]
    ny = plt.grid_dimensions[1]
    nz = plt.grid_dimensions[2]

    # Create EclSum Obj
    ecl_sum = ecl.EclSum.writer(
        case,
        start_time,
        nx,
        ny,
        nz,
        fmt_output=format,
        unified=True,
        time_in_days=True,
        key_join_string=':'
    )


    # Add variables/create nodes
    for var in plt[plt['classname'] == 'FIELD'].varname.unique():
        if var in kw_nex2ecl:
            ecl_unit = plt._unit_system.unit_str(var)
            ecl_var = 'F' + kw_nex2ecl[var]
            ecl_sum.add_variable(ecl_var, wgname=None, num=-1, unit=ecl_unit, default_value=0.0)

    for var in plt[plt['classname'] == 'WELL'].varname.unique():
        for inst in plt[(plt['classname'] == 'WELL') & (plt['varname'] == var) ].instancename.unique():
            if var in kw_nex2ecl:
                ecl_unit = plt._unit_system.unit_str(var)
                ecl_var = 'W' + kw_nex2ecl[var]
                ecl_sum.add_variable(ecl_var, wgname=inst, num=-1, unit=ecl_unit, default_value=0.0)

    # Create ecl timesteps
    tsteps = [ecl_sum.add_t_step(plt.timestep.unique()[idx], sim_days)
              for idx, sim_days in enumerate(plt.time.unique())]

    # Add data
    for tstep in tsteps:
        for key in ecl_sum:
            #Add FIELD
            if (key[0] == 'F'):
                tstep[key] = plt[(plt['classname'] == 'FIELD') &
                                 (plt['varname'] == inv_kw_nex2ecl[key[1:]]) &
                                 (plt['timestep'] == tstep.get_report())&
                                 (plt['instancename'] == 'FIELD')].value.tolist()[0]
            #Add WELL
            elif (key[0] == 'W'):
                tstep[key] = plt[(plt['classname'] == 'WELL') &
                                 (plt['varname'] == inv_kw_nex2ecl[key[1:-2]]) &
                                 (plt['timestep'] == tstep.get_report()) &
                                 (plt['instancename'] == key[-1])].value.tolist()[0]
            else:
                pass

    return ecl_sum
