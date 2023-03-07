#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "07";
	static const char MONTH[] = "03";
	static const char YEAR[] = "2023";
	static const char UBUNTU_VERSION_STYLE[] =  "23.03";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 0;
	static const long MINOR  = 3;
	static const long BUILD  = 3335;
	static const long REVISION  = 17372;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 6242;
	#define RC_FILEVERSION 0,3,3335,17372
	#define RC_FILEVERSION_STRING "0, 3, 3335, 17372\0"
	static const char FULLVERSION_STRING [] = "0.3.3335.17372";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
