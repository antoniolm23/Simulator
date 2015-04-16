#!/usr/bin/python
import itertools
import sys

s_outputfile1 = open("ploss01S.txt", "w")
s_outputfile2 = open("ploss02S.txt", "w")
s_outputfile3 = open("ploss03S.txt", "w")
f_outputfile1 = open("ploss01F.txt", "w")
f_outputfile2 = open("ploss02F.txt", "w")
f_outputfile3 = open("ploss03F.txt", "w")
with open("../4channel/7adv/sim.txt") as sim3:
	for linef in sim3:
		str1 = linef.split('\t')
		if str1[0] == 'ploss01':
			s_outputfile1.write("4 {0}" .format(str1[1]))
		if str1[0] == 'ploss02':
			s_outputfile2.write("4 {0}" .format(str1[1]))
		if str1[0] == 'ploss03':
			s_outputfile3.write("4 {0}" .format(str1[1]))
with open("../8channel/7adv/sim.txt") as sim3:
	for linef in sim3:
		str1 = linef.split('\t')
		if str1[0] == 'ploss01':
			s_outputfile1.write("8 {0}" .format(str1[1]))
		if str1[0] == 'ploss02':
			s_outputfile2.write("8 {0}" .format(str1[1]))
		if str1[0] == 'ploss03':
			s_outputfile3.write("8 {0}" .format(str1[1]))
with open("../16channels/7adv/sim.txt") as sim3:
	for linef in sim3:
		str1 = linef.split('\t')
		if str1[0] == 'ploss01':
			s_outputfile1.write("16 {0}" .format(str1[1]))
		if str1[0] == 'ploss02':
			s_outputfile2.write("16 {0}" .format(str1[1]))
		if str1[0] == 'ploss03':
			s_outputfile3.write("16 {0}" .format(str1[1]))
with open("../4channel/7adv/formula.txt") as form:
	for linef in form:
		str1 = linef.split('\t')
		if str1[0] == 'Fploss0.1:':
			f_outputfile1.write("4 {0}" .format(str1[1]))
		if str1[0] == 'Fploss0.2:':
			f_outputfile2.write("4 {0}" .format(str1[1]))
		if str1[0] == 'Fploss0.3:':
			f_outputfile3.write("4 {0}" .format(str1[1]))
with open("../8channel/7adv/formula.txt") as form:
	for linef in form:
		str1 = linef.split('\t')
		if str1[0] == 'Fploss0.1:':
			f_outputfile1.write("8 {0}" .format(str1[1]))
		if str1[0] == 'Fploss0.2:':
			f_outputfile2.write("8 {0}" .format(str1[1]))
		if str1[0] == 'Fploss0.3:':
			f_outputfile3.write("8 {0}" .format(str1[1]))
with open("../16channels/7adv/formula.txt") as form:
	for linef in form:
		str1 = linef.split('\t')
		if str1[0] == 'Fploss0.1:':
			f_outputfile1.write("16 {0}" .format(str1[1]))
		if str1[0] == 'Fploss0.2:':
			f_outputfile2.write("16 {0}" .format(str1[1]))
		if str1[0] == 'Fploss0.3:':
			f_outputfile3.write("16 {0}" .format(str1[1]))
s_outputfile1.close()
s_outputfile2.close()
s_outputfile3.close()
f_outputfile1.close()
f_outputfile2.close()
f_outputfile3.close()

