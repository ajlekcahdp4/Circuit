#!/usr/bin/env python3

import sys, math, re
import numpy as np

def scan_edge(line: str):
    nums = re.findall(r'\b\d\b\d\b\f', line)
    return {int(nums[0], int(nums[1]), float(nums[3]))}

def scan_edges(file):
    edges = np.zeros(0, dtype=tuple)
    for line in file:
        np.append(edges, scan_edge(line))
    return edges

def edge_cmp(edge1, edge2):
    return edge1[0] == edge2[0] and edge1[1] == edge2[1] and math.isclose(edge1[3], edge2[3], rel_tol=1e-6)

def edges_cmp(edges1, edges2):
    len1 = len(edges1)
    len2 = len(edges2)

    if len1 != len2:
        return False
    
    for i in range(len1):
        if edge_cmp(edges1[i], edges2[i]) == False:
            return False
    
    return True

def main():
    sys.
