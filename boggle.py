#!/usr/bin/env python
import re

board = []
available_letters = dict()

for line in open('board.txt'):
    letters = line[:-1]
    board.append(list(letters))
    for l in letters:
        available_letters[l] = 1

dictionary = dict()

for line in open('/usr/share/dict/words'):
    if "'" in line:
        continue
    if line[0] in 'ABCDEFGHIJKLMNOPQRSTUVWXYZ':
        continue
    if re.match(r'\W',line):
        continue
    word = line[:-1]
    if len(word) < 3:
        continue
    all_in = True
    for letter in word:
        if not available_letters.has_key(letter):
            all_in = False
    if all_in:
        dictionary[word] = 1

def rfind(left,found,board,row,col,trace):
    if len(left) == 0:
        return (found,trace)
    next_letter = left[0]
    left = left[1:]
    neighbors = [
        (row-1,col-1), # topleft
        (row-1,col  ), # top
        (row-1,col+1), # topright
        (row  ,col-1), # left
        (row  ,col+1), # right
        (row+1,col-1), # botleft
        (row+1,col  ), # bottom
        (row+1,col+1), # botright
        ]
    bsize  = len(board) - 1
    for (nrow,ncol) in neighbors:
        if nrow < 0 or nrow > bsize or ncol < 0 or ncol > bsize:
            continue
        if (nrow,ncol) in trace:
            continue
        if board[nrow][ncol] == next_letter:
            trace.append((row,col))
            (f,t) = rfind(left,[next_letter] + found,board,nrow,ncol,trace)
            if f is not None:
                return (f,t)
    # made it here without finding anything
    return (None,None)

found = dict()
for word in dictionary.keys():
    for i in range(4):
        for j in range(4):
            (f,t) = rfind(word,[],board,i,j,[])
            if f is not None:
                found[word] = 1

allwords  = found.keys()
allwords.sort(lambda a,b: cmp(len(a),len(b)))
allwords.reverse()
for w in allwords:
    print w
