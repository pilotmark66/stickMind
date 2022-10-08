/**
 * File: perfect.h
 *
 * This is just here to communicate the prototype 
 * information for the functions in perfect.cpp that 
 * will be called from main.cpp
 */
#pragma once

long divisorSum(long n); //
bool isPerfect(long n);
void findPerfects(long stop);

long smarterSum(long n); //通过平方根快速确定完全数
bool isPerfectSmarter(long n);
void findPerfectsSmarter(long stop);

long findNthPerfectEuclid(long n); //通过欧几里得方法寻找完全数
