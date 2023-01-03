import matplotlib.pyplot as plt
import sys
import platform

if len(sys.argv) < 2:
    raise Exception("usage: python3 build_plot.py compute.log")

log = open(sys.argv[1], "r")
lines = log.readlines()

x_axis = []
y_axis = []

for line in lines:
    data = line.split()
    x_axis.append(int(data[0]))
    y_axis.append(int(data[1]))

plt.plot(x_axis, y_axis)
plt.xlabel("nops")
plt.ylabel("cycles")
plt.title("nops-cycles dependency " + platform.machine())
plt.show()