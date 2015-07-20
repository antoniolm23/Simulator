#!/usr/bin/python
from subprocess import call
import itertools
import sys
#open the conf.txt file and write a new configuration in it
e = "-e 4"
t0 = 0
t1 = 0
t2 = 0
tN = 15

###########SCHEDULES ARE COMPUTED OFFLINE#########################
#nodeID, channel offset, timeslot
adv5cells3 = "%i\t%i\t%i\n%i\t%i\t%i\t%i\t%i\t%i\n" %(0, 0, 0, 0, 5, 8, 0, 11, 15) #352.217 with energy = 4
adv5cells5 = "%i\t%i\t%i\n%i\t%i\t%i\n%i\t%i\t%i\n%i\t%i\t%i\n%i\t%i\t%i\n" %(0, 0, 0, 0, 6, 5, 0, 13, 9, 0, 3, 14, 0, 10, 18)

advertisers = [7]
for occupied in advertisers:
	#print "hello"
	for i in range(0, 2):
		
		out_file = open("conf.txt", 'w')
		#print "new combo"
		if(i == 0):
			out_file.write(adv5cells3)
			z = "S{0}".format("S00S85S1511E4A7")
                        c = "-C 3"
		if(i == 1):
			out_file.write(adv5cells5)
			z = "S{0}".format("S00S56S913S143S1810E4A7")
                        c = "-C 5"
		#split the chcombo entry
		#ch = itertools.chain(c)
			#output of the form id\tnc\tns\n
			#command = "./simulation -c 5 -l 5 -m 2 -s " + z
			#print command
		#a = "-a {0}".format(a)
		#b = "-b {0}".format(t2 - t1)
		#c = "-c {0}".format(tN - t2)
		i = i+1
		out_file.close()
		#command = "./simulation -c 5 -l 5 -m 2 -e 2 -s " + z
		#print command
		#command = "../evaluateModel/main {0} {1} {2} {3}" .format(a, b, c, e)
		#print command
		parameters = "-s " + z
		call(["./simulation","-f 1", "-F conf.txt", "-T 1", "-a 7", "-P 1", "-E 4", "-i 1000000", c, parameters])
		#call(["../evaluateModel/main",a, b, c, e, parameters])
		#print "hello";

