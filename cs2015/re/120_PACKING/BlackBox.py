import subprocess
import string
import sys

PIN = "/home/ameise/pin/pin"
INSCOUNT0 = "/home/ameise/pin/source/tools/ManualExamples/obj-ia32/inscount0.so"
TARGET = "./packing"


chars = string.printable
# chars = string.hexdigits


def run(argv1) :

    # pin -t inscount0 -- target "args"
    cmd = []
    cmd.append(PIN)
    cmd.append("-t")
    cmd.append(INSCOUNT0)
    cmd.append("--")
    cmd.append(TARGET)
    cmd.append('"' + argv1 + '"')

    cmd = ' '.join(cmd)

    #print cmd
    #sys.exit(1)

    proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    proc.communicate()

    fp = open('inscount.out', 'rb')
    raw = fp.read()
    fp.close()
    insns = raw.split('\n')[0].split(' ')[1]

    return int(insns)


keyctr = 0
key = ""


while keyctr < 32:
  print "brute forcing key[" + str(keyctr) + "] ",

  num0 = run(key + chars[0])
  num1 = run(key + chars[1])

  failnum = min([num0, num1])

  for i in range(len(chars)):
    sys.stdout.write(chars[i])
    num = run(key + chars[i])
    if num > failnum:
      key += chars[i]
      break

  print ""
  print "key: " + key
  keyctr += 1
