import subprocess
import string
import sys



# USER VARIABLES
PIN = "absolute_path_to_pin"
INSCOUNT0 = "absolute_path_to_inscount0.so"
TARGET = "absolute_path_to_32-bit_binary"

# if you already know some of the key, put it here
KEY = ""

# if you know the key is a certain length, put it here
# otherwise, leave it as a relatively large number
KEYLEN = 256

# you can adjust this to something else like string.hexdigits
# or a custom character set
CHARS = string.printable



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

    proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    proc.communicate()

    fp = open('inscount.out', 'rb')
    raw = fp.read()
    fp.close()
    insns = raw.split('\n')[0].split(' ')[1]

    return int(insns)



print "================"
print "BlackBox RE Tool"
print "================"



keyctr = len(KEY)
key = KEY
chars = CHARS

while keyctr < KEYLEN:
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
