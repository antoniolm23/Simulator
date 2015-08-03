#!/usr/bin/python
from subprocess import call
import itertools
import sys
#open the conf.txt file and write a new configuration in it
energyFactor = 1
advertiser = 1
timeslots = 23 #77 101
cells = [2, 5, 7, 11, 21] #30
ploss = [0.1, 0.2, 0.3]
###########SCHEDULES ARE COMPUTED OFFLINE#########################

#print "hello"
for i in cells:
	filename = "{0}timeslots/{1}cells.txt".format(timeslots, i)

	#copy the whole file into conf.txt, where the configuration will be read
	open("conf.txt", "w").writelines([l for l in open(filename).readlines()])
	c = "-C {0}" .format(i)
        for p in ploss:
	    z = "ts{0}cells{1}ploss{2}".format(timeslots, i, p)
	    erg = "-E {0}" .format(energyFactor)
            pcommand = "-p {0}".format(p)
	    #command = "./simulation -c 5 -l 5 -m 2 -e 2 -s " + z
	    #print command
	    #command = "../evaluateModel/main {0} {1} {2} {3}" .format(a, b, c, e)
	    #print command
	    parameters = "-s " + z
	    call(["./simulation", "-F conf.txt", "-T 1", "-a 1", "-P 1", erg, "-i 100000", c,pcommand, parameters])
	#call(["../evaluateModel/main",a, b, c, e, parameters])
	#print "hello";

