#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <string>

bool is_big_endian()
{
  union {
    int i;
    char c[4];
  } data;

  data.i = 1;
  return (data.c[0]!=1);
}

void print_binary(char* buffer,int size)
{
for (int i=0;i<size;++i)
{
  for (int z=128;z>0;z>>=1)
  {
      ((buffer[i] & z) == z) ? printf("1") : printf("0");
      if (z==16) printf(" ");
  }
  printf(" | ");
}
printf("\n");
}

int main(int argc, char** argv) {
   uint64_t b = 0x0102030405060708;
   
   if (!is_big_endian()) b = __builtin_bswap64(b);
   
   print_binary((char*)&b,8);
   printf("%02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx\n",*((char*)&b),*(((char*)&b)+1),*(((char*)&b)+2),*(((char*)&b)+3),*(((char*)&b)+4),*(((char*)&b)+5),*(((char*)&b)+6),*(((char*)&b)+7));
  
   b = b >> 1ULL;

   printf("After >> 1\n");
   print_binary((char*)&b,8);
   printf("%02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx\n",*((char*)&b),*(((char*)&b)+1),*(((char*)&b)+2),*(((char*)&b)+3),*(((char*)&b)+4),*(((char*)&b)+5),*(((char*)&b)+6),*(((char*)&b)+7));
 
   return 0;
}
