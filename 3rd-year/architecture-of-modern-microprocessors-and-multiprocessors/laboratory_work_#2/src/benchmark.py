import subprocess
from subprocess import Popen, PIPE
import signal
import matplotlib.pyplot as plt
import platform

nops_amount = 1
max_nops_amount = 500
results = []
stop = False


def stop_benchmark(signal, frame):
    global stop
    print("Benchmark is stopped!")
    stop = True


signal.signal(signal.SIGINT, stop_benchmark)
log = open("compute.log", "w")
while not stop and nops_amount <= max_nops_amount:

    nops = open("nops", "w")
    for i in range(nops_amount):
        nops.write("asm(\"nop\");\n")
    nops.close()
    subprocess.call(["gcc", "main.c", "-O0"])

    with Popen(["./a.out"], stdout=PIPE) as test:
        try:
            cycles = int(test.stdout.read().decode("ascii"))
        except:
            continue
        print("nops command amount: " + str(nops_amount) + ", clocks: " + str(cycles))
        log.write(str(nops_amount) + " " + str(cycles) + "\n")
        results.append(cycles)
    nops_amount = nops_amount + 1

log.close()

plt.plot(range(nops_amount)[1:], results)
plt.xlabel("nops")
plt.ylabel("cycles")
plt.title("nops-cycles dependency " + platform.machine())
plt.show()