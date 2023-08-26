#!/usr/bin/env python3
import random, sys
from lcapy import Circuit

# number of nodes  - 1 arg
# file to put test - 2 arg

def generate_test(number_of_nodes):
    edges = []
    for i in range(1, number_of_nodes - 1):
        for j in range(i + 1, number_of_nodes):
            if random.randint(0, 100) < 50:
                edges.append((i, j, random.uniform(10.0, 1000.0), random.uniform(-1000.0, 1000.0)))
    return edges

def print_test(edges, file):
    for edge in edges:
        file.write(str(edge[0]) + ' -- ' + str(edge[1]) + ', ' + str(edge[2]) + '; ' + str(edge[3]) + 'V\n')

def main():
    number_of_nodes = int(sys.argv[1]) 
    filename = sys.argv[2]
    edges = generate_test(number_of_nodes)
    file = open(filename, 'w')
    print_test(edges, file)
    file.close()

main()