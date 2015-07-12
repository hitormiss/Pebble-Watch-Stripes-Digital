#include <pebble.h>
#include <ctype.h>
#include "helper.h"	
	
///////////////////////////////

// HexStringToUInt borrowed from PebbleAuth https://github.com/JumpMaster/PebbleAuth.git
    unsigned int HexStringToUInt(char const* hexstring){
    unsigned int result = 0;
    char const *c = hexstring;
    unsigned char thisC;

    while( (thisC = *c) != 0 ){
        thisC = toupper(thisC);
        result <<= 4;

        if( isdigit(thisC))
                result += thisC - '0';
        else if(isxdigit(thisC))
                result += thisC - 'A' + 10;
        else{
                //APP_LOG(APP_LOG_LEVEL_DEBUG, "ERROR: Unrecognised hex character \"%c\" from \"%s\"", thisC, hexstring);
                return 0;
        }
        ++c;
    }
    return result;  
}
//and then you can do something like:
//GColor Textcolor = GColorfromHEX(HexStringToUInt("64ff46"));

///////////////////////////////	

char *itoa(int num)
{
  static char buff[20] = {};
  int i = 0, temp_num = num, length = 0;
  char *string = buff;
  
  if(num >= 0) { //See NOTE
    // count how many characters in the number
    while(temp_num) {
      temp_num /= 10;
      length++;
    }
    // assign the number to the buffer starting at the end of the 
    // number and going to the begining since we are doing the
    // integer to character conversion on the last number in the
    // sequence
    for(i = 0; i < length; i++) {
      buff[(length-1)-i] = '0' + (num % 10);
      num /= 10;
    }
    buff[i] = '\0'; // can't forget the null byte to properly end our string
  }
  else {
    return "Unsupported Number";
  }
  
  return string;
}

//////////////////////////