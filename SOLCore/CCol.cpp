#include "main.h"
#include "StdInc.h"

// COL buffer limts. default is 44000

static char colbuffer[120000]; // increase bytes :)

CMemory::InstallPatch<char*>(0x48A84D, colbuffer);
CMemory::InstallPatch<char*>(0x48A8CB, colbuffer);
CMemory::InstallPatch<char*>(0x48A987, colbuffer);
CMemory::InstallPatch<char*>(0x48AA1D, colbuffer);
CMemory::InstallPatch<char*>(0x48AA55, colbuffer);
CMemory::InstallPatch<char*>(0x48AAFC, colbuffer);
CMemory::InstallPatch<char*>(0x48AB31, colbuffer);
CMemory::InstallPatch<char*>(0x48AB61, colbuffer);