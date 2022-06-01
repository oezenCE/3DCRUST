#pragma once

#include <iostream>
#include <Windows.h>
#include <string>

//basic test 
void print1(int x);

//data collector function
void vtuwriter(char* fileName, int loadcase, char* cdb_dll, const std::string& vtu11writer_path);