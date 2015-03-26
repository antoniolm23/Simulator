#!/usr/bin/python
from subprocess import call
import itertools
import sys
#open the conf.txt file and write a new configuration in it

for occupied in range(8, 15):
	combo = itertools.combinations(range(15), occupied)
	for c in combo:
		
		out_file = open("conf.txt", 'w')
		#print "new combo"
		
		#split the chcombo entry
		ch = itertools.chain(c)
		z = ""
		for t in ch:
			#print t;
			#print "new element in chain"
			nc = t / 3;
			ns = t % 3;
			
			z += "S%i%i" % (ns, nc)
			
			#output of the form id\tnc\tns\n
			out_file.write("%i\t%i\t%i\n" % (0, nc, ns))
			#command = "./simulation -c 5 -l 5 -m 2 -s " + z
			#print command
		out_file.close()
		command = "./simulation -c 5 -l 5 -m 2 -s " + z
		#print command
		parameters = "-s " + z
		call(["./simulation", "-c 5", "-l 5", "-m 2", parameters])
		#print "hello";

