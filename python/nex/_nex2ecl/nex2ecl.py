import textwrap
import warnings
from datetime import timedelta

try:
    from ecl.summary import EclSum
except ImportError:
    from ert.ecl import EclSum


class ConversionError(Exception):
    pass


__kw_nex2ecl = {
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


def __nex_class_to_ecl(ecl_values,
                       ecl_sum,
                       plt,
                       times,
                       classname,
                       prefix,
                       has_wgname=False,
                       has_num=False,
                       default_value=0.0):
    sub_plt = plt.loc[plt['classname'] == classname]
    for instancename in sub_plt.instancename.unique():
        inst = sub_plt.loc[sub_plt['instancename'] == instancename]
        for var in filter(__kw_nex2ecl.__contains__, inst.varname.unique()):
            node = ecl_sum.add_variable(
                '{}{}'.format(prefix, __kw_nex2ecl[var]),
                wgname=instancename if has_wgname else None,
                num=-1,
                unit=plt._unit_system.unit_str(var),
                default_value=default_value)

            conversion = plt._unit_system.conversion(var)
            iv = inst.loc[inst['varname'] == var]
            ecl_values.append((node.get_key1(), {
                time: iv[iv['time'] == time].value.iloc[0] * conversion
                for time in times if time in iv.time.values
            }))


def __interp_hist(start_date, sample_dates, hplt, field_name=None):
    ecl_sum = nex2ecl(hplt, 'HIST_CASE', field_name=field_name, warn=False)

    hist_dates = [ecl_sum.iget_date(i) for i in range(len(ecl_sum))]
    fst_date, snd_date = min(hist_dates), max(hist_dates)
    clamped_dates = [d for d in sample_dates if fst_date <= d <= snd_date]

    vectors = []
    for key in ecl_sum:
        interp_vector = ecl_sum.get_interp_vector(key, date_list=clamped_dates)
        node = ecl_sum.smspec_node(key)
        days = [(c - start_date).days for c in clamped_dates]
        v = zip(days, interp_vector)
        vectors.append((node, dict(v)))
    return vectors


def nex2ecl(
        plt, case, format=False, field_name=None, hist_data=None, warn=True):
    field_names = plt[plt['classname'] == 'FIELD'].instancename.unique()
    if len(field_names) > 1 and not field_name:
        raise ConversionError('More than one field in plot', 1)
    if field_name and len(field_names) > 0 and field_name not in field_names:
        raise ConversionError('{} not in plot'.format(field_name), 2)

    if warn and len(field_names) > 1:
        warnings.warn('Multifield model not supported by ecl')
    if len(field_names) > 0 and not field_name:
        field_name = field_names[0]

    ecl_sum = EclSum.writer(
        case,
        plt.start_date,
        plt.nx,
        plt.ny,
        plt.nz,
        fmt_output=format,
        unified=True,
        time_in_days=True,
        key_join_string=':'
    )

    times = plt.time.unique().tolist()

    ecl_values = []
    field_plt = plt.loc[(plt['classname'] == 'FIELD') &
                        (plt['instancename'] == field_name)]
    field_plt._unit_system = plt._unit_system
    __nex_class_to_ecl(
        ecl_values, ecl_sum, field_plt, times, 'FIELD', 'F')
    __nex_class_to_ecl(
        ecl_values, ecl_sum, plt, times, 'WELL', 'W', has_wgname=True)

    if hist_data is not None and not hist_data.empty:
        sample_dates = [plt.start_date + timedelta(days=t) for t in times]
        interp_vectors = __interp_hist(
            plt.start_date, sample_dates, hist_data, field_name=field_name)
        for n, v in interp_vectors:
            node = ecl_sum.add_variable(
                n.keyword + 'H',
                wgname=n.wgname,
                num=n.num,
                unit=n.unit)
            ecl_values.append((node.get_key1(), v))

    failed = [v for v in plt.varname.unique() if v not in __kw_nex2ecl]
    if warn and failed:
        msg = "could not convert nexus variables:\n    {}"
        failures = '\n    '.join(textwrap.wrap(
            ', '.join(map(str, failed)), 76))
        warnings.warn(msg.format(failures))

    timesteps = [(ecl_sum.add_t_step(i + 1, t), t)
                 for i, t in enumerate(times)]
    for ts, time in timesteps:
        for key, vector in ecl_values:
            if time in vector:
                ts[key] = vector[time]

    return ecl_sum
