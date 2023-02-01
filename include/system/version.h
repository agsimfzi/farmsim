#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "01";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2023";
	static const char UBUNTU_VERSION_STYLE[] =  "23.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 0;
	static const long MINOR  = 1;
	static const long BUILD  = 628;
	static const long REVISION  = 3472;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1121;
	#define RC_FILEVERSION 0,1,628,3472
	#define RC_FILEVERSION_STRING "0, 1, 628, 3472\0"
	static const char FULLVERSION_STRING [] = "0.1.628.3472";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
