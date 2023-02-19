#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "18";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2023";
	static const char UBUNTU_VERSION_STYLE[] =  "23.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 0;
	static const long MINOR  = 2;
	static const long BUILD  = 2105;
	static const long REVISION  = 11640;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 3888;
	#define RC_FILEVERSION 0,2,2105,11640
	#define RC_FILEVERSION_STRING "0, 2, 2105, 11640\0"
	static const char FULLVERSION_STRING [] = "0.2.2105.11640";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
