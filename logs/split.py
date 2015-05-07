#!/usr/bin/python
import itertools
import sys
import datetime
import math

def getTime(s):
	l = s.split(':')
	if len(l) == 2:
		return int(l[0]) * 60000 + int(float(l[1])*1000)
	if(len(l) == 3):
		return long(3600000 + int(l[1]) * 60000 + int(float(l[2])*1000))

def getTimeslot(s):
	l = s.split('.')
	return int(l[1], 16)
	
output = open("3adv/S00S11S23.txt", "a")
#output1 = open("statTS.txt", "a")
with open("S00S11S23.txt") as formula:
	for linef in formula:
		linef = linef[:-1]
		str2 = linef.split('\t')
		#if("ASNs") in linef:
			#ts_send = getTimeslot(str2[2])
		#if("ASNr") in linef:
			#ts_rec = getTimeslot(str2[2])
			#ts_diff = ts_rec - ts_send_def
			#ts_send_def = 0
			#print "{0} {1}" .format("timeslot_difference", ts_diff)
			#output1.write(repr(ts_diff))
			#output1.write('\n')
		if "ON" in linef:
			x = getTime(str2[0])
			#ts_send_def = ts_send
		if "OFF" in linef:
			z = getTime(str2[0])
			y = z - x			
			#y = float(y)
			#print "{0}: {1}" .format(z, x)
			#x = y / 15.01092
			#print "{0} {1}" .format(x, y)
			#x = round(x)
			#x = int(x + 1)
			#Skymote perform 128 clock ticks per second
			#print y
			output.write(repr(y))
			output.write('\n')
output.close()	
#output1.close()
