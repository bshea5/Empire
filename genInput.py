# Generate random input file for testing Imperial Cities program

from random import randint

file = open("pyEmpire.txt", "w")
MYMAX = 100
count = 1

file.write(str(MYMAX) + "\n")

while (count < MYMAX):
    for x in range(0, count):
        randomVal = randint(0, 50)
        if (randomVal == 0):
            randomVal = 'x'
        file.write(str(randomVal))
        if (x != count - 1):
            file.write(" ")
    if (count != MYMAX - 1):
        file.write("\n")
    count = count + 1

file.close()