import random
import os

zahl = 0
zahlenarray = []

def report():
    statfork = os.fork()
    if statfork == 0:
        stat()
    else:
        os.wait()
        print(stat())

def stat():
    logfork = os.fork()
    if logfork == 0:
        log()
    else:
        os.wait()
        mittelwert = sum(conv()) / len(conv())
        return mittelwert

def log():
    convfork = os.fork()
    if convfork == 0:
        conv()
    else:
        os.wait()
        dateizahlen = open('zahlen.txt', 'w')
        dateizahlen.truncate(0)
        dateizahlen.write(str(conv()))
        dateizahlen.close()

def conv():
    for i in range(10):
        zahl = random.randint(0, 1000)
        zahlenarray.append(zahl)
    return zahlenarray

report()
