#!/usr/bin/python3

import numpy as np
import random
import math
import copy
import sys

C = 1.0

class Node:
    def __init__(self, data, turn = 'max', move_to_here = None, is_leaf = True):
        self.state = np.copy(data)
        self.is_leaf = is_leaf
        self.move_to_here = copy.copy(move_to_here)
        self.turn = turn
        self.children = []
        
        self.w = 0.0
        self.n = 1
        self.c = C
        
    def UCB1(self, n):
        score = (15.0*(self.w/self.n) + self.c*(math.sqrt(math.log(n))/self.n))/15.0
        #print(score)
        return score

    def score(self, n):
        return self.UCB1(n)

def generate_random_state(num_cols, num_rows):
    state = np.zeros( (num_cols, num_rows) )
    for c in range(state.shape[0]):
        for r in range(state.shape[1]):
            randval = np.random.uniform(0.0, 3.0)
            if randval <= 1:
                state[c][r] = -1
            elif randval <= 2:
                state[c][r] = 0
            elif randval <= 3:
                state[c][r] = 1
    return state

def generate_successors(state, move):
    num_cols = state.shape[0]
    num_rows = state.shape[1]

    successors = []

    if move == 'max':
        piece_type = 1
    if move == 'min':
        piece_type = -1

    for c in range(num_cols):
        for r in range(num_rows):
            if state[c][r] == 0:
                new_state = np.copy(state)
                new_state[c][r] = piece_type
                successors.append(new_state)
    return successors

def generate_successor_moves(state, move):
    num_cols = state.shape[0]
    num_rows = state.shape[1]

    moves = []

    if move == 'max':
        piece_type = 1
    if move == 'min':
        piece_type = -1

    for c in range(num_cols):
        for r in range(num_rows):
            if state[c][r] == 0:
                moves.append((c,r))
    return moves

def is_valid_pos(col, row, num_cols, num_rows):
    if col < 0:
        return False
    if col >= num_cols:
        return False
    if row < 0:
        return False
    if row >= num_rows:
        return False

    return True

def check_dir(state, direction, k, move_to_here):
    num_cols = state.shape[0]
    num_rows = state.shape[1]

    col = move_to_here[0]
    row = move_to_here[1]

    piece_type = state[col][row]

    num = 0

    for i in range(1, k):
        newcol = col + i*direction[0]
        newrow = row + i*direction[1]
        if not is_valid_pos(newcol, newrow, num_cols, num_rows):
            break
        if state[newcol][newrow] != piece_type:
            break
        num += 1

    return num
        

def terminal_test(state, k, move_to_here):
    dirs = [ (0,1), (1,1), (1,0), (-1,1), (0,-1), (-1,-1), (-1,0), (1,-1) ]
    dirdict = dict()

    for direction in dirs:
        dirdict[direction] = check_dir(state, direction, k, move_to_here)
    
    if dirdict[(0,1)] + dirdict[(0,-1)] + 1 == k:
        return True
    if dirdict[(1,1)] + dirdict[(-1,-1)] + 1 == k:
        return True
    if dirdict[(1,0)] + dirdict[(-1,0)] + 1 == k:
        return True
    if dirdict[(-1,1)] + dirdict[(1,-1)] + 1 == k:
        return True

    return False


def random_playout(state, k, turn, move_to_here = None):
    if move_to_here != None and terminal_test(state, k, move_to_here):
        #print(move_to_here)
        #print(state)
        if turn == 'max':
            return 0 #min won
        elif turn == 'min':
            return 1 #max won
    
    successors = generate_successor_moves(state, turn)
    if len(successors) == 0:
        #print(state)
        return 0.5 #draw

    index = random.randint(0, len(successors)-1)

    if turn == 'max':
        piece_type = 1
        next_turn = 'min'
    else:
        piece_type = -1
        next_turn = 'max'

    move = successors[index]
    new_state = np.copy(state)
    new_state[move[0]][move[1]] = piece_type

    return random_playout(new_state, k, next_turn, move)

def MCTS_r(node, k, net = None, depth = 1):
    print(depth)
    node.n += 1
    if node.is_leaf:
        node.w += random_playout(node.state, k, node.turn, node.move_to_here)
        node.n += 1
        moves = generate_successor_moves(node.state, node.turn)

        next_turn = 'min' if node.turn == 'max' else 'max'
        for move in moves:
            new_state = np.copy(node.state)
            node.children.append( Node(new_state, next_turn, copy.copy(move)) )
        node.is_leaf = False
        return node.w

    best_child = max(node.children, key = lambda child : child.score(node.n))
    score = MCTS_r(best_child, k, net, depth+1)
    node.w += score
    if net != None:
        unrolled = np.reshape(node.state, node.state.shape[0]*node.state.shape[1])
        #print(node.w, node.n)
#        net.online_train(unrolled, (node.w/depth)/node.n)
#        net.feedforward(unrolled, (node.w/depth)/node.n)
    return score
    
    

def MCTS(num_cols, num_rows, k, num_updates=1000, net = None):
    initial_state = np.zeros( (num_cols, num_rows) )
    initial_node = Node(initial_state)

    for i in range(num_updates):
        MCTS_r(initial_node, k, net)


class Net:
    def __init__(self, schematic, prev = None):
        if prev == None:
            self.bs = []
            self.Ws = []

            for layer_size in schematic[1:]:
                self.bs.append( np.random.randn(layer_size) )
            for i in range(1, len(schematic)):
                self.Ws.append( np.random.randn(schematic[i], schematic[i-1]) )
        else:
            self.Ws = prev[0]
            self.bs = prev[1]
        self.LR = 0.5
    
    
    def feedforward(self, x, y=None):
        a = np.copy(x)
    
        for i in range(len(self.bs)):
            z = np.dot(self.Ws[i], a) + self.bs[i]
            a = self.sigmoid(z)

        if y != None:
            print( self.cost(a, y) )
        return a

    def sigmoid(self, z):
        return 1.0/(1.0 + np.exp(-z))

    def sigmoid_prime(self, z):
        return self.sigmoid(z)*(1.0 - self.sigmoid(z))

#   def train(self, xs, ys):
#       aes = []
#       for x in xs:
#           a = np.copy(x)
#           for i in range(len(self.bs)):
#               z = np.dot(self.Ws[i], a) + self.bs[i]
#               a = self.sigmoid(z)
#           error_L = a*z   
#           aes.append(np.copy(a))

    def online_train(self, x, y):
        a = np.copy(x)
        aes = [ np.copy(a) ]
        zs = [None]
        for i in range(len(self.bs)):
            z = np.dot(self.Ws[i], a) + self.bs[i]
            a = self.sigmoid(z)
            zs.append(np.copy(z))
            aes.append(np.copy(a))
        
        self.backprop(zs, aes, y)

    def backprop(self, zs, aes, y):
        errors = [ None for bias in self.bs ]
        errors.append(None)

        assert len(errors) == len(aes)

        L = len(errors)
        #errors[L-1] = aes[L-1]*y
        errors[L-1] = self.cost_der(aes[L-1], y)*self.sigmoid_prime(zs[L-1])
    
        
        
        delta_Ws = [None for error in errors]
        delta_bs = [None for error in errors]
        for layer in range(L-1, 0, -1):
            delta_Ws[layer] = np.outer(errors[layer], aes[layer-1])
#           print("Layer", layer)
#           print("delta_Ws =", delta_Ws[layer].shape)
#           print("errors =", errors[layer].shape)
            delta_bs[layer] = errors[layer]

            if layer > 1:
                errors[layer-1] = np.dot(self.Ws[layer-1].T, errors[layer]) * self.sigmoid_prime(zs[layer-1])

        for layer in range(1, len(delta_Ws)):
            #print(delta_Ws[layer])
            self.Ws[layer-1] -= self.LR*delta_Ws[layer]
            self.bs[layer-1] -= self.LR*delta_bs[layer]
        
    def cost(self, a, y):
        return 0.5*(a-y)**2.0

    def cost_der(self, a, y):
        return a - y
            
        
#state = generate_random_state(8, 8)
#unrolled = np.reshape(state, state.shape[0]*state.shape[1])
#test = Net([64, 100, 1])
#
#for i in range(10000):
#   test.online_train(unrolled, 0.6)
#   print(test.feedforward(unrolled))







