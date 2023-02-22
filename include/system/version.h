#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "22";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2023";
	static const char UBUNTU_VERSION_STYLE[] =  "23.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 0;
	static const long MINOR  = 3;
	static const long BUILD  = 2681;
	static const long REVISION  = 14878;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 4979;
	#define RC_FILEVERSION 0,3,2681,14878
	#define RC_FILEVERSION_STRING "0, 3, 2681, 14878\0"
	static const char FULLVERSION_STRING [] = "0.3.2681.14878";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
