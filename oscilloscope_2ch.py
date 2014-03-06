#! /usr/bin/python
'''
37.6Hz@115200 100 1 1
'''

import serial,sys,time

from macrobull.misc import serialChecker
from macrobull.dynamicPlot import dynamicFigure,defaultProcFunc,average, timeDiv

BUFFLEN=16
plotArgs=dict(alpha=0.5, linewidth=3.2, marker='o', markersize=4.4)#,color="green")
procFunc=[]#[lambda x,y:average(0.2,x,y), ]
buff=''


baud=9600
intv=50
cnt='4x'
sl=1

BLKSIZE = 4
MAXBUFF = 400
VLO=int(12e3)

dev=serialChecker(True,'USB','AMA')
ser = serial.Serial(dev, baud)
ser.flush()
ser.flushInput()
ser.flushOutput()
ser.close()


if len(sys.argv)>1:  baud = int(sys.argv[1])
if len(sys.argv)>2:  intv = int(sys.argv[2])
#if len(sys.argv)>3:  cnt = int(sys.argv[3]) % 10
if len(sys.argv)>3:  cnt = sys.argv[3]+'x'
if len(sys.argv)>4:  sl = float(sys.argv[4])

ser = serial.Serial(dev, baud, timeout=intv /1000)
print "baudrate={}".format(ser.getBaudrate())

ser.write(cnt);
flag=True
tStamp1=tStamp2=time.time()
to1=to2=0

def safeVal(s):
	global flag
	r=None
	try:
		r=int(s)
	except ValueError,e:
		flag=False
		pass
	return r

def adjustTS(t,to,ts):
	ot=t
	t+=(t<to)*VLO
	t+=(t<to)*VLO
	s=float(t-to)/VLO+ts
	return s,ot,s

def pause(event):
	global tStamp1, tStamp2
	if event.key=='i':
		df.pause=not(df.pause)
		tStamp1, tStamp2 = time.time()

def getData(fs):
	global buff,  flag
	global to1, to2, tStamp1, tStamp2
	w=ser.inWaiting()
	#print(w)
	if w>0:
		data=ser.read(w)
		buff+=data
		th1,th2=[],[]
		ch1,ch2=[],[]
		ts=te=0

		print time.time(),':'
		print len(buff)
		#while len(buff)>MAXBUFF : buff=buff[MAXBUFF:]

		while buff.find('S')>0:
			buff=buff[buff.find('S')-1:]
			pe=buff.find('E')
			if pe==-1: break
			if pe>len(buff)-6: break
			flag=True
			ts=safeVal(buff[2:8])
			te=safeVal(buff[pe+1:pe+7])
			if flag:
				tmpch=[]
				for i in range(8,pe-1,BLKSIZE):
					v=safeVal(buff[i+1:i+4])
					if v: tmpch.append(v *2.5/1024)
				if buff[0]=='A':
					ch1+=tmpch
					#print time.time(), (ts,to1,tStamp1), adjustTS(ts, to1, tStamp1)
					(ts,to1,tStamp1) = adjustTS(ts, to1, tStamp1)
					(te,to1,tStamp1) = adjustTS(te, to1, tStamp1)
					th1+=list(timeDiv(ts,te,len(tmpch)))
				if buff[0]=='B':
					ch2+=tmpch
					(ts, to2, tStamp2) =adjustTS(ts, to2, tStamp2)
					(te, to2, tStamp2) =adjustTS(te, to2, tStamp2)
					th2+=list(timeDiv(ts,te,len(tmpch)))

			buff=buff[pe+7:]

		#print len(data),data,buff
		#print th1, ch1
		#df.appendData([1], 'Ch1', 111, x=[time.time()], procFunc=procFunc, plotArgs=plotArgs)
		df.appendData(ch1, 'Ch1', 111, x=th1, procFunc=procFunc, plotArgs=plotArgs)
		df.appendData(ch2, 'Ch2', 111, x=th2, procFunc=procFunc, plotArgs=plotArgs)
		if df.check_subplot(111, "Channels"):
			df.subplots[111].set_ylabel("Voltage\V")

	ser.write(cnt);

df=dynamicFigure(updateInterval=intv,screenLen=sl)
df.keyHandler=pause
#df=dynamicFigure(updateInterval=1000,screenLen=60)
df.newData=getData


try:
	df.run()
except BaseException,e:
	print(e)

ser.close()
print('Exited')

