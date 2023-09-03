#!/usr/bin/env python3

import random, sys, math
import numpy as np

# number of nodes  - 1 arg
# file to put test - 2 arg

def need_add_edge():
    return random.randint(0, 100) < 50

def need_add_emf():
    return random.randint(0, 100) < 30

def generate_test(number_of_nodes):
    edges = []
    for i in range(number_of_nodes):
        for j in range(i + 1, number_of_nodes):
            if need_add_edge() == True:
                if need_add_emf() == True:
                    edges.append((i, j, random.uniform(10.0, 1000.0), random.uniform(-1000.0, 1000.0)))
                else:
                    edges.append((i, j, random.uniform(10.0, 1000.0), 0.0))
    return edges

def print_test(edges, file):
    for edge in edges:
        file.write(str(edge[0] + 1) + ' -- ' + str(edge[1] + 1) + ', ' + str(edge[2]) + '; ')
        if math.isclose(edge[3], 0.0, rel_tol=1e-6) != True:
            file.write(str(edge[3]) + 'V')
        file.write('\n')

def generate_answer(edges, number_of_edges, number_of_nodes):
    matrix_coef = np.zeros((number_of_nodes + number_of_edges, number_of_nodes + number_of_edges))
    i = 0
    for edge in edges:
        matrix_coef[edge[0]][i] = 1.0
        matrix_coef[edge[1]][i] = -1.0
        i += 1
    for k in range(number_of_edges + number_of_nodes):
        matrix_coef[number_of_nodes - 1][k] = 0
    matrix_coef[number_of_nodes - 1][number_of_edges] = 1.0
    i = 0
    for edge in edges:
        matrix_coef[number_of_nodes + i][i] = edge[2]
        matrix_coef[number_of_nodes + i][number_of_edges + edge[0]] = -1.0
        matrix_coef[number_of_nodes + i][number_of_edges + edge[1]] = 1.0
        i += 1

    free_coef = np.zeros((number_of_edges + number_of_nodes))
    for j in range(number_of_nodes):
        free_coef[j] = 0.0
    for j in range(number_of_edges):
        free_coef[number_of_nodes + j] = edges[j][3]
    
    if math.isclose(np.linalg.det(matrix_coef), 0.0) == True:
        return np.zeros(0)

    currents = np.linalg.solve(matrix_coef, free_coef)
    currents.resize(number_of_edges)
    return currents   

def calc_number_of_nodes(edges):
    max = 0
    for edge in edges:
        if max < edge[1]:
            max = edge[1]
    return max + 1

def print_answer(file_answer, edges, currents):
    if len(currents) == 0:
        return
    for i in range(len(edges)):
        file_answer.write(str(edges[i][0] + 1) + ' -- ' + str(edges[i][1] + 1) + ': ' + str(currents[i]) + ' A\n')

def main():
    number_of_nodes = int(sys.argv[1]) 
    filename = sys.argv[2]
    edges = generate_test(number_of_nodes)
    file = open(filename, 'w')
    print_test(edges, file)
    file.close()

    if len(sys.argv) == 4 and sys.argv[3] == '--ans':
        currents = generate_answer(edges, len(edges), calc_number_of_nodes(edges))
        filename_answer = filename + ".ans"
        file_answer = open(filename_answer, 'w')
        print_answer(file_answer, edges, currents)
        file_answer.close()

main()