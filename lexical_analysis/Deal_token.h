#ifndef DEAL_TOKEN_H
#define DEAL_TOKEN_H

#include <string>
using namespace std;

bool is_letter(char C);
bool is_digit(char C);

void create_reserved_table();

bool is_reserved(string token);
bool is_defined(string token);
void table_insert(string token);
float SToF(string token);
int SToI(string token);
void cat();

#endif // !DEAL_IDTIFIERS_H
