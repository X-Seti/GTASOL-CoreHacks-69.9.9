/*
	file:
		TXD.h
	desc:
		Functions to load and uncompress
		txd textures.
*/

#ifndef TXD_H
#define TXD_H

#include <windows.h>

struct TXDTexHeader {
	int Always8;
	int Unknown;
	char Name[31];
	char AlphaName[31];
	int Flags;
	int Alpha;
	WORD Width;
	WORD Height;
	BYTE Bits;
	BYTE MipMaps;
	BYTE Always4;
	BYTE Compression;
	int DataSize;
};

struct MipMap {
	int Offset;
	int Size;
	int Width;
	int Height;
};

struct Texture {
	int Offset;
	TXDTexHeader Header;
	MipMap* MipMaps;
};

struct Dictionary {
	char* FileName;
	int Offset;
	int TextureCount;
	Texture* Textures;
	char* Name;
};

struct SectionHeader {
	int Ident;
	int Size;
	int Version;
};

struct DXTBlock {
	WORD col0;
	WORD col1;
	BYTE row[4];
};

struct Color8888 {
	BYTE r,g,b,a;
};

struct Color565 {
    BYTE r,g,b;
};
 
struct DXTAlphaBlock {
	WORD row[4];
};

struct RGBQuad {
	BYTE r,g,b,res;
};

void LoadDict(Dictionary* TXD,char* filename);
void ReadDict(Dictionary* TXD);
bool GetTexture(Dictionary* TXD, int index, BYTE* Decompressed);
void DecompressDXT1(BYTE* Compressed, BYTE* Decompressed, int Width, int Height);
void DecompressDXT3(BYTE* Compressed, BYTE* Decompressed, int Width, int Height);
void Decompress8bit(BYTE* Compressed, BYTE* Decompressed, int Width, int Height);
void Decompress32bit(BYTE* Compressed, BYTE* Decompressed, int Width, int Height);
void GetBlockColours(DXTBlock* Block, Color8888* Col0, Color8888* Col1, Color8888* Col2, Color8888* Col3);
void Color565to8888(WORD Color565, Color8888* Color8888);
void DecodeBlock(int* pPos, DXTBlock* pBlock, int Width, Color8888* Col0, Color8888* Col1, Color8888* Col2, Color8888* Col3);
void DecodeAlphaBlock(int *pPos, DXTAlphaBlock* pAlphaBlock, int Width, int AlphaZero);

#endif