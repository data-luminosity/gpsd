

import os
import glob

import re

p = re.compile(ur'APP ID: (\d*)')
elapsedregex=re.compile(ur'time elapsed:(\d*)')

path="test_r64"

sum0=0
tally0=0
sum50=0
tally50=0
sum99=0
tally99=0

def tally(filename,appid):
    global sum0, sum50, sum99, tally0, tally50, tally99
    with open(infile, 'r') as f:
        for line in f:
            if re.match(elapsedregex,line):
                matchgroup=re.search(elapsedregex,line)
                elapsedtime=int(matchgroup.group(1))
                #print(elapsedtime,appid)
                if elapsedtime>0:
                    if 0==appid:
                        sum0=sum0+elapsedtime
                        tally0=tally0+1
                    elif 50==appid:
                        sum50=sum50+elapsedtime
                        tally50=tally50+1
                    elif 99==appid:
                        sum99=sum99+elapsedtime
                        tally99=tally99+1

for infile in glob.glob(os.path.join(path,"*txt")):
    #print(infile)
    with open(infile, 'r') as f:
        first_line = f.readline()
        #print(first_line)
        matchgroup=re.search(p, first_line)
        appid=matchgroup.group(1)
        #print(appid)
        tally(infile,int(appid))


print(sum0,sum0/tally0)
print(sum50,sum50/tally50)
print(sum99,sum99/tally99)
