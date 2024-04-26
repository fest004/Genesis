#include <cstdio>
#include "src/genesis.hpp"

int main()
{
  printf("Genesis is born \n");

  Genesis genesis;
  genesis.init();
  genesis.run();
}
