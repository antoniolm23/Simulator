#!/usr/bin/python
from subprocess import call
import itertools
import sys
#open the conf.txt file and write a new configuration in it

for occupied in range(3, 4):
	combo = itertools.combinations(range(15), occupied)
	for c in combo:
		
		out_file = open("conf.txt", 'w')
		#print "new combo"
		
		#split the chcombo entry
		ch = itertools.chain(c)
		z = ""
		i = 0
		for t in ch:
			#print t;
			#print "new element in chain"
			nc = t / 3;
			ns = t % 3;
			z += "S%i%i" % (ns, nc)
			#output of the form id\tnc\tns\n
			out_file.write("%i\t%i\t%i\n" % (i, nc, ns))
			i += 1
			#command = "./simulation -c 5 -l 5 -m 2 -s " + z
			#print command
		out_file.close()
		command = "./simulation -c 5 -l 5 -m 4 -p 0.3 -s " + z
		#print command
		parameters = "-s " + z
		call(["./simulation", "-c 5", "-l 5", "-m 4", "-p 0.3", "-i 100", parameters])
		#print "hello";

