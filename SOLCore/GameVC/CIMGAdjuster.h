
//Fastman92 Limit Adjuster
#include <Windows.h>
#include <string>
//#include <iostream>

#define patch(a,b) _patch(a,b,sizeof(b))

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)

void InitInstance(HANDLE hModule);
void ExitInstance();

class SALimitAdjuster
{
public:
	
	struct tCDimageName
	{
		char name[64];
	};

	// Array of CDIMAGE handles
	static HANDLE* CDIMAGES_HANDLES;

	// Array of CDIMAGE names
	static tCDimageName* CDIMAGE_NAMES;

	// Clears memory
	static void SALimitAdjuster::ReleaseMemory();

	// IMG : max number of IMG archives
	static void SALimitAdjuster::SetMaxNumberOfIMGarchives(int iIMGarchives);
};