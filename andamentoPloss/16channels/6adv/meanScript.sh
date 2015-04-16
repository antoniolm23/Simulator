#!/bin/sh
cat ploss01.txt | awk 'BEGIN{somma = 0; conta = 0} {somma += $2; conta++;} END{print "ploss01\011"somma/conta}' > sim.txt
cat ploss02.txt | awk 'BEGIN{somma = 0; conta = 0} {somma += $2; conta++} END{print "ploss02\011"somma/conta}' >> sim.txt
cat ploss03.txt | awk 'BEGIN{somma = 0; conta = 0} {somma += $2; conta++} END{print "ploss03\011"somma/conta}' >> sim.txt
