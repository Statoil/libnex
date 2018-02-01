import pandas
import datetime
import re
from six import StringIO



def _split_csvs(lines):
    r = re.compile(r'^(\s*[a-zA-Z]+\s*,)*\s*[a-zA-Z]+\s*$')
    csvs = []
    l = [lines[3].rstrip()]
    for line in lines[4:]:
        stripped = line.rstrip()
        if r.match(line):
            csvs.append('\n'.join(l))
            l = []
        l.append(stripped)
    if l:
        csvs.append('\n'.join(l))
    return csvs


def _csv_to_plt(dfs, start_date, date_format):
    instname = dfs.columns[0]
    split = [dfs.loc[:, [instname, 'DATE', col]] for col in dfs.columns[2:]]

    def date_to_time(date_str):
        date = datetime.datetime.strptime(date_str, date_format)
        return (date - start_date).days

    for df in split:
        df['classname'] = df.columns[0]
        df['varname'] = df.columns[2]
        df['timestep'] = 0
        df.rename(columns={'DATE': 'time', df.columns[0]: 'instancename',
                           df.columns[2]: 'value'}, inplace=True)
        df['time'] = df['time'].apply(date_to_time)

    plt = pandas.concat(split, ignore_index=True )
    return plt

_date_map = [
    ('YYYY', '%Y'),
    ('YY', '%y'),
    ('MM', '%m'),
    ('DD', '%d'),
]

def load_historical(filename, nx, ny, nz, unit_system=None):
    with open(filename) as f:
        lines = f.readlines()

    date_format = lines[0].split()[1]
    for d in _date_map:
        date_format = date_format.replace(*d)
    start_date_str = lines[1].split()[1]
    start_date = datetime.datetime.strptime(start_date_str, date_format)

    csvs = _split_csvs(lines)
    frames = [pandas.read_csv(StringIO(csv)) for csv in csvs]
    fragments = [_csv_to_plt(df, start_date, date_format) for df in frames]
    plt = pandas.concat(fragments, ignore_index=True)
    plt.start_date = start_date
    plt.nx = nx
    plt.ny = ny
    plt.nz = nz
    plt._unit_system = unit_system
    return plt
