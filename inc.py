#!/cygdrive/c/Python310/Python.exe

import re
import sys
import os

argLen = len(sys.argv)
if argLen >= 2:
    inpFile = sys.argv[1]
else:
    sys.exit()

if argLen >= 3:
    outFile = sys.argv[2]
else:
    sys.exit()

def cVarName(var):
    cVar = ""
    last = False
    for j in range(len(var)):
        ch = var[j]
        if ch.isupper():
            if last:
                cVar += "_"
            last = False
        else:
            last = True
        cVar += ch.upper()
    return cVar

baseName = os.path.basename(inpFile)
name = baseName.split(".")[0]
inc = cVarName(name) + "_INC"
    
inp = open(inpFile, 'r')
fOut = open(outFile, 'wb')

def fWrite(txt):
    fOut.write(txt.encode())

out = False
for l in inp:
    if not out:
        if re.search(r".*?// <-", l):
            out = True
            fWrite("#if !defined(" + inc + ")\t// <-\n")
            fWrite("#define " + inc + "\n")
    else:
        if re.search(r"#define *INCLUDE", l):
            continue
        if re.search(r".*?// ->", l):
            out = False
            fWrite("#endif  /* " + inc + " */\t// ->\n")
            break
        fWrite(l)
inp.close()
fOut.close()
