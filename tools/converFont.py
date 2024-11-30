
chars = ""
with open('font.bdf', 'r') as file:
    onChar=False
    for line in file:
        if not onChar:
            if line=="BITMAP\n":
                onChar=True
        else:
            if line=="ENDCHAR\n":
                onChar=False
            else:
                chars += (line[:2])
    file.close()

print(chars)

with open('font.bin', 'wb') as file:
    file.write(bytes.fromhex(chars))