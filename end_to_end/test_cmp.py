#!/usr/bin/env python3

import sys, math, re, os
import numpy as np

def scan_edge(line: str):
    # print('scan_edge')
    nums = re.findall(r'[-+]?(?:(?:\d*\.\d+)|(?:\d+\.?))(?:[Ee][+-]?\d+)?', line)
    return (int(nums[0]), int(nums[1]), float(nums[2]))

def scan_edges(file):
    edges = []
    for line in file:
        edges.append(scan_edge(line))
    return edges

def edge_cmp(edge1, edge2):
    return edge1[0] == edge2[0] and edge1[1] == edge2[1] and math.isclose(edge1[2], edge2[2], rel_tol=1e-4, abs_tol=1e-8)

def edges_cmp(result, answer, test_name):
    len1 = len(result)
    len2 = len(answer)

    # print(len1)

    if len1 != len2:
        print(test_name + ': result and answer has different number of edges:')
        print('\tin result: ' + str(len1))
        print('\tin answer: ' + str(len2))
        return 0
    
    for i in range(len1):
        if not edge_cmp(result[i], answer[i]):
            print(test_name + ': Failed:')
            print('\tresult edge: ' + str(result[i][0]) + ' -- ' + str(result[i][1]) + ': ' + str(result[i][2]) + ' A')
            print('\tanswer edge: ' + str(answer[i][0]) + ' -- ' + str(answer[i][1]) + ': ' + str(answer[i][2]) + ' A')
            return 0
    print(test_name + ': Passed')
    return 1

def main():
    nmumber_of_tests = len(sys.argv) - 2
    build_dir = sys.argv[1]
    passed = 0

    for i in range(nmumber_of_tests):
        test_name = sys.argv[i + 2]

        os.system('./' + build_dir + './task/currents < ' + test_name + ' > tmp')
        file_result = open('tmp', 'r')
        result = scan_edges(file_result)
        file_result.close()
        os.system('rm tmp')

        file_answer = open(test_name + '.ans', 'r')
        answer = scan_edges(file_answer)
        file_answer.close()

        passed += edges_cmp(result, answer, test_name)

    print('Passed ' + str(passed) + ' out of ' + str(nmumber_of_tests))

main()