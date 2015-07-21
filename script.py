#!/usr/bin/python
from subprocess import call
import itertools
import sys
#open the conf.txt file and write a new configuration in it
e = "-e 4"
t0 = 0
t1 = 0
t2 = 0
#tN = 15

###########SCHEDULES ARE COMPUTED OFFLINE#########################
#nodeID, channel offset, timeslot
adv10cells7 = "%i\t%i\t%i\n%i\t%i\t%i\t%i\t%i\t%i\n%i\t%i\t%i\n%i\t%i\t%i\n%i\t%i\t%i\n%i\t%i\t%i\n" %(0, 0, 0, 0, 11, 7, 0, 7, 13, 0, 2, 20, 0, 14, 3, 0, 9, 10, 0, 5, 16) #352.217 with energy = 4
adv10cells5 = "%i\t%i\t%i\n%i\t%i\t%i\n%i\t%i\t%i\n%i\t%i\t%i\n%i\t%i\t%i\n" %(0, 0, 0, 0, 6, 5, 0, 13, 9, 0, 3, 14, 0, 10, 18)

energyFactors = [0.25, 0.2, 0.9]
advertisers = [10]
for energy in energyFactors:
        erg = "-E {0}".format(energy)
	#print "hello"
	for i in range(0, 2):
		
		out_file = open("conf.txt", 'w')
		#print "new combo"
		if(i == 1):
			out_file.write(adv10cells7)
			z = "S{0}{1}".format("ADV10Cell7E", energy)
                        c = "-C 7"
		if(i == 0):
			out_file.write(adv10cells5)
			z = "S{0}{1}".format("ADV10Cell5E", energy)
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
		call(["./simulation","-f 1", "-F conf.txt", "-T 1", "-a 10", "-P 1", erg, "-i 1000000", c, parameters])
		#call(["../evaluateModel/main",a, b, c, e, parameters])
		#print "hello";

