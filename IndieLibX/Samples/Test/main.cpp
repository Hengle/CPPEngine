#include <STX/STX.h>
#include <STX/Matrix.h>

int ApplicationLogic()
{
	LOG_FNLN;
	LOG_PRINT("sizeof(half)=%d\n", sizeof(half));
	LOG_PRINT("sizeof(unsigned short)=%d\n", sizeof(unsigned short));
	LOG_PRINT("sizeof(int)=%d\n", sizeof(int));
	LOG_PRINT("sizeof(uint)=%d\n", sizeof(uint));
	LOG_PRINT("sizeof(WORD)=%d\n", sizeof(WORD));

	std::string f=std::string("/");
	std::string file=stx_convertpath(f);

	LOG_PRINT("iosDocumentsFolder(/)=%s\n", file.c_str());
	return 0;
}

