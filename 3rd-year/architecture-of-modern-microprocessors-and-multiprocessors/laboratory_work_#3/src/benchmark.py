import subprocess
from subprocess import Popen, PIPE
import signal
import numpy
import time


L = 1
max_L = 128
threshold = 1000
prime_numbers = [2,   3,   5,   7,  11,  13,  17,  19,  23,  29,  31,  37,
                 41,  43,  47,  53,  59,  61,  67,  71,  73,  79,  83,  89,  97,
                 101, 103, 107, 109, 113, 127, 131, 137]
results = []
stop = False


def stop_benchmark(signal, frame):
    global stop
    print("Benchmark is stopped!")
    stop = True


signal.signal(signal.SIGINT, stop_benchmark)

log = open("1st_step.log", "w")
branches = open("branches", "w")
branches.write("if(i % L == 0) a = 0;\n")
branches.close()

#first step
print("1st step start!\n")
while not stop and L <= max_L:

    L_file = open("declarations", "w")
    L_file.write("long long int a = 1;\n")
    L_file.write("int L = " + str(L) + ";")
    L_file.close()

    subprocess.call(["g++", "benchmark.cpp", "-lpapi"])

    with Popen(["./a.out"], stdout=PIPE) as test:
        try:
            mpr = float(test.stdout.read().decode("ascii"))
        except:
            continue
        log.write(str(L) + " " + str(mpr) + "\n")
        results.append(mpr)
    L = L + 1

log.close()

L_r = numpy.argmax(results)
while results[L_r] / results[L_r - 1] < threshold:
    L_r = L_r - 1

L = L_r
print("MAXIMUM LENGTH OF HISTORY: " + str(L))
print("MPR(MAX_LENGTH) = " + str(results[L - 1]) + " %")
print("MPR(MAX_LENGTH + 1) = " + str(results[L]) + " %")

# second step
print("\n2nd step start!\n")

L_file = open("declarations", "w")
L_file.write("long long int a = 1;\n")
L_file.write("int L = " + str(L_r) + ";")
L_file.close()

L = 2 * (L_r - 1)

branches = open("branches", "w")
for i in range(1, L + 1):
    branches.write("if(i < 0) a = 1;\n")
branches.write("if(i % L == 0) a = 0;\n")
branches.close()

subprocess.call(["g++", "benchmark.cpp", "-lpapi"])

mpr = 0.0
with Popen(["./a.out"], stdout=PIPE) as test:
    try:
        mpr = float(test.stdout.read().decode("ascii"))
    except:
        print("Step 2: something went wrong!")

print("MPR = " + str(mpr) + " %")

# third step
print("\n3rd step start!\n")
L = L_r
i = 0
while prime_numbers[i] < L:
    i = i + 1
L1 = prime_numbers[i - 2]
L2 = prime_numbers[i - 1]

L_file = open("declarations", "w")
L_file.write("int L1 = " + str(L1) + ";\n")
L_file.write("int L2 = " + str(L2) + ";\n")
L_file.write("int a, b, c;\n")
L_file.close()

branches = open("branches", "w")
branches.write("if(i % L1 == 0) a = 1; else a = 0;\n")
branches.write("if(i % L2 == 0) b = 1; else b = 0;\n")
branches.write("if(a * b == 1) c = 1;\n")
branches.close()

subprocess.call(["g++", "benchmark.cpp", "-lpapi"])

with Popen(["./a.out"], stdout=PIPE) as test:
    try:
        mpr = float(test.stdout.read().decode("ascii"))
    except:
        print("Step 3: something went wrong!")

print("MPR = " + str(mpr) + " %")

# fifth step
print("\n5th step start!\n")
L3 = L_r + 1

L_file = open("declarations", "w")
L_file.write("int L3 = " + str(L3) + ";\n")
L_file.write("int a;\n")
L_file.close()

branches = open("branches", "w")
branches.write("if(i % L3 == 0) a = 1;\n")
branches.close()

subprocess.call(["g++", "benchmark.cpp", "-lpapi"])

with Popen(["./a.out"], stdout=PIPE) as test:
    try:
        mpr = float(test.stdout.read().decode("ascii"))
    except:
        print("Step 5: something went wrong!")

print("MPR for 1 branch = " + str(mpr) + " %")

branches = open("branches", "w")
branches.write("if(i % L3 == 0) a = 1;\n")
branches.write("if(i % L3 == 0) a = 1;\n")
branches.close()

subprocess.call(["g++", "benchmark.cpp", "-lpapi"])

with Popen(["./a.out"], stdout=PIPE) as test:
    try:
        mpr = float(test.stdout.read().decode("ascii"))
    except:
        print("Step 5: something went wrong!")

print("MPR for 2 branches = " + str(mpr) + " %")

# delay
print("\nDefine a delay!\n")

L_file = open("declarations", "w")
L_file.write("int L = " + str(L_r) + ";")
L_file.close()

branches = open("branches", "w")
branches.write("if(i % " + str(L_r) + ") a = 1;\n")
branches.close()

subprocess.call(["g++", "delay.cpp", "-lpapi"])

mp_cycles = 0
with Popen(["./a.out"], stdout=PIPE) as test:
    try:
        mp_cycles = float(test.stdout.read().decode("ascii"))
    except:
        print("Delay: something went wrong!")

branches = open("branches", "w")
branches.write("if(i % " + str(L_r - 1) + ") a = 1;\n")
branches.close()

subprocess.call(["g++", "delay.cpp", "-lpapi"])

p_cycles = 0
with Popen(["./a.out"], stdout=PIPE) as test:
    try:
        p_cycles = float(test.stdout.read().decode("ascii"))
    except:
        print("Delay: something went wrong!")

print("Delay = " + str(int((mp_cycles - p_cycles) * (L_r - 1))) + " cycles")
