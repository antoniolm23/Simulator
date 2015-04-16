#!/usr/bin/python
import itertools
import sys

output = open("out.txt", "a")
with open("stat4.txt") as formula:
	for linef in formula:
		linef = linef[:-1]
		with open("sim4.txt") as sim:
			for lines in sim:
				lines = lines[:-1]
				str1 = lines.split('\t')
				str2 = linef.split('\t')
				z = str1;
				x = str2;
				names = str1[0].split('S')
				namef = str2[0].split('S')
				
				#now order both lists
				names.sort()
				namef.sort()
				
				#print "{0}\t{1}\n".format(names, namef)
				
				if cmp(names, namef) == 0:
					string = "{0}\t{1}\t{2}\n".format(z[0], z[1], x[1])
					print string
					output.write(string)
					break
				

