import matplotlib.pyplot as plt
import sys
import platform
from cpuinfo import get_cpu_info

if len(sys.argv) < 2:
    raise Exception("usage: python3 build_plot.py compute.log")

log = open(sys.argv[1], "r")
lines = log.readlines()

x_axis = []
y_axis = []

for line in lines:
    data = line.split()
    x_axis.append(int(data[0]))
    y_axis.append(float(data[1]))

plt.plot(x_axis, y_axis)
plt.xlabel("Pattern length")
plt.ylabel("MPR, %")
plt.title("MPR-Pattern length dependency " + get_cpu_info()["brand"])

plt.show()
