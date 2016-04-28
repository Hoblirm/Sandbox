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
char data[9] = {0};

void write(uint64_t v, char* buffer, int byte, int offset)
{
  if (!is_big_endian())
  {
    v = __builtin_bswap64(v);
  }

  printf("%02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx\n",*((char*)&v),*(((char*)&v)+1),*(((char*)&v)+2),*(((char*)&v)+3),*(((char*)&v)+4),*(((char*)&v)+5),*(((char*)&v)+6),*(((char*)&v)+7));
  print_binary((char*)&v,8);

  v >>= offset;
  printf("%02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx\n",*((char*)&v),*(((char*)&v)+1),*(((char*)&v)+2),*(((char*)&v)+3),*(((char*)&v)+4),*(((char*)&v)+5),*(((char*)&v)+6),*(((char*)&v)+7));
  print_binary((char*)&v,8); 
  *((uint64_t*)(buffer + byte)) |= v;

  printf("After or\n");
  print_binary(data,sizeof(data)); 
  uint64_t mask = 0xFF00000000000000;
  mask = mask << (8-offset);
  v |= mask;

  *((uint64_t*)(buffer + byte)) &= v;

/*
  char remainder = *(((char*)&v)+7) << (8-offset);
  printf("Remainder = %02hhx\n",remainder);
  *(buffer + byte + 8) |= remainder;

  char cmask = 0xFF;
  cmask = cmask >> offset;
  remainder |= cmask;
  *(buffer + byte + 8) &= remainder;*/
}

uint64_t read(char* buffer, int byte, int offset)
{
  return *((uint64_t*)(buffer + byte));
}


int main(int argc, char** argv) {
  

   if (!is_big_endian())
   {
     printf("LITTLE\n");
   }

   int a = 0x01020304; 
   printf("%02hhx %02hhx %02hhx %02hhx\n",*((char*)&a),*(((char*)&a)+1),*(((char*)&a)+2),*(((char*)&a)+3));
   a = __builtin_bswap32(a);
   printf("%02hhx %02hhx %02hhx %02hhx\n",*((char*)&a),*(((char*)&a)+1),*(((char*)&a)+2),*(((char*)&a)+3));


   int i=0;
   uint64_t b = 0x0102030405060708;
   uint64_t s = __builtin_bswap64(b);
   print_binary((char*)&s,8);
   //printf("%02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx\n",*((char*)&b),*(((char*)&b)+1),*(((char*)&b)+2),*(((char*)&b)+3),*(((char*)&b)+4),*(((char*)&b)+5),*(((char*)&b)+6),*(((char*)&b)+7));
   write(b,data,i,1);
   printf("%02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx\n",data[i],data[i+1],data[i+2],data[i+3],data[i+4],data[i+5],data[i+6],data[i+7],data[i+8]);
   print_binary(data, sizeof(data));
   b = read(data,i,0);
   printf("%02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx %02hhx\n",*((char*)&b),*(((char*)&b)+1),*(((char*)&b)+2),*(((char*)&b)+3),*(((char*)&b)+4),*(((char*)&b)+5),*(((char*)&b)+6),*(((char*)&b)+7));

   
   
   return 0;
}
