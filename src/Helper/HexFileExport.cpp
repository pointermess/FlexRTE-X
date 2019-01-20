#include "HexFileExport.h"
using namespace FlexRTE::DriverInterface;
void HexFileExporter::ExportArrayToFile(char * arrayPtr, unsigned int arrayLength,const char * path)
{
    FileStream * fs = new FileStream(path);

    if (fs->IsOpen())
    {
        // -------- 00 01 02 03 04 05 06 07 08 09 0a 0
        fs->PrintF("-------- 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\n");

        char itemCounter = 0;
        int lineCounter = 0;
        for (int arrayIndex = 0; arrayIndex < arrayLength; arrayIndex++)
        {
            if (itemCounter == 0)
            {
                fs->PrintF("%07x0 ", lineCounter);
            }

            fs->PrintF("%02x ", (unsigned char)arrayPtr[arrayIndex]);

            itemCounter += 1;
            if (itemCounter == 16)
            {
                fs->PrintF("\n", lineCounter);
                itemCounter = 0;
                lineCounter += 1;
            }
        }

        fs->Close();
    }
}


void HexFileExporter::ExportArrayToBinaryFile(char * arrayPtr, unsigned int arrayLength, const char * path)
{
    FileStream * fs = new FileStream(path);

    if (fs->IsOpen())
    {
        for (int arrayIndex = 0; arrayIndex < arrayLength; arrayIndex++)
        {
            int s = (int)arrayPtr[arrayIndex];
            unsigned int mask = 1 << ((sizeof(int) << 3) - 1);
            while (mask) {
                fs->PrintF("%d", (s&mask ? 1 : 0));
                mask >>= 1;
            }
        }

        fs->Close();
    }
}
