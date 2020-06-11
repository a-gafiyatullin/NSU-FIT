import matplotlib.pyplot as plt
import pandas as pd
import sys

if len(sys.argv) < 3:
    raise Exception("usage: python3 build_plot.py compute.log max_bandwidth")

log = open(sys.argv[1], "r")
lines = log.readlines()

thread_axis = list(range(1, int(lines[0]) + 1))
read_axis =  list(map(float, lines[2].split(' ')))
write_axis = list(map(float, lines[1].split(' ')))
copy_axis =  list(map(float, lines[3].split(' ')))
max_bandwidth = [float(sys.argv[2])] * int(lines[0])

df=pd.DataFrame({'Threads': thread_axis, 'READ': read_axis, 'WRITE': write_axis, 'COPY': copy_axis,
    'SYSTEM MAX MEMORY BANDWIDTH' : max_bandwidth})

plt.xlabel("Threads")
plt.ylabel("GB/s")
plt.plot('Threads', 'WRITE', data=df, color='red', linewidth=2)
plt.plot('Threads', 'READ',data=df, color='green',linewidth=2)
plt.plot('Threads', 'COPY', data=df, color='blue', linewidth=2)
plt.plot('Threads', 'SYSTEM MAX MEMORY BANDWIDTH', data=df, color='black', linewidth=2)
plt.legend()

plt.show()