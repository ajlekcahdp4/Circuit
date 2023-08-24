#!/usr/bin/env python3

from io import TextIOWrapper
from typing import List
import random, sys
from lcapy import Circuit

# number of nodes - 1 arg

class Edge:
    node1 = 0 
    node2 = 0
    res   = 0.0
    emf   = 0.0

def generate_test(number_of_nodes):
    edges = []
    for i in range(1, number_of_nodes):
        edges.append(i, ran)