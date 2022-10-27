#pragma once

/* Needed for boggle.cpp */
#include "grid.h"
#include "lexicon.h"


using namespace std;

int scoreBoard(Grid<char>& board, Lexicon& lex);
void findWords(Grid<char>& board, Lexicon& lex, Grid<bool>& viaPath, int row, int col, string current, Set<string>& validWords);

/* Needed for voting.cpp */
#include "vector.h"

Vector<int> computePowerIndexes(Vector<int>& blocks);
