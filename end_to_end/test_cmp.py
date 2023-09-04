#!/usr/bin/env python3

import sys, math, re
sys.path.append('test_runner')
import test_runner

# build directory - 1 arg
# name of tests   - other args

def scan_edge(line: str):
    nums = re.findall(r'[-+]?(?:(?:\d*\.\d+)|(?:\d+\.?))(?:[Ee][+-]?\d+)?', line)
    return (int(nums[0]), int(nums[1]), float(nums[2]))

def scan_edges(file):
    edges = []
    for line in file:
        edges.append(scan_edge(line))
    return edges

def edge_cmp(edge1, edge2):
    return edge1[0] == edge2[0] and edge1[1] == edge2[1] and math.isclose(edge1[2], edge2[2], rel_tol=1e-4, abs_tol=1e-8)

def edges_cmp(file_res, file_ans):
    result = scan_edges(file_res)
    answer = scan_edges(file_ans)
    len1 = len(result)
    len2 = len(answer)

    if len1 != len2:
        output = '\tresult and answer has different number of edges:\n'
        output += '\tin result: ' + str(len1) + '\n'
        output += '\tin answer: ' + str(len2) + '\n'
        return (0, output)
    
    for i in range(len1):
        if not edge_cmp(result[i], answer[i]):
            output = '\tresult edge: ' + str(result[i][0]) + ' -- ' + str(result[i][1]) + ': ' + str(result[i][2]) + ' A\n'
            output += '\tanswer edge: ' + str(answer[i][0]) + ' -- ' + str(answer[i][1]) + ': ' + str(answer[i][2]) + ' A\n'
            return (0, output)
    
    return (1, '')

def main():
    test_names = sys.argv.copy()
    test_names.pop(0)
    test_names.pop(0)
    test_runner.run_tests(sys.argv[1] + '/task/currents', test_names, edges_cmp)
    
main()