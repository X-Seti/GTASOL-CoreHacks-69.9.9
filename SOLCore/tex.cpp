/*
file:
TXD.cpp
desc :
Functions to load and uncompress
txd textures.
*/

#include "stdinc.h"
#include <stdio.h>
#include <math.h>
#include "tex.h"

/*---------------------------------------------
LoadDict

desc:

params:	TXD
Pointer to our struct where we
save all the data.

filename
Path to the txd.
-----------------------------------------------*/
void LoadDict(Dictionary* TXD, char* filename)
{
	TXD->TextureCount = 0;
	TXD->FileName = filename;
	TXD->Offset = 0;
	ReadDict(TXD);
}

/*---------------------------------------------
ReadDict

desc:		Reads all information from
txd given in filename.

params:	TXD
Pointer to our struct where we
save all the data.
-----------------------------------------------*/
void ReadDict(Dictionary* TXD)
{
	SectionHeader SectHeader, TexSectHeader;
	int EndPos, CurPos, EOT;
	int i, j, k;

	FILE* dict = fopen(TXD->FileName, "rb");

	fread(&SectHeader, sizeof(SectionHeader), 1, dict);

	EndPos = ftell(dict);
	EndPos += SectHeader.Size;

	CurPos = ftell(dict);
	i = 0;
	while (CurPos <= EndPos) {
		fread(&SectHeader, sizeof(SectionHeader), 1, dict);
		switch (SectHeader.Ident) {
		case 1:
			if (SectHeader.Size == 4) {
				fread(&TXD->TextureCount, 4, 1, dict);
				TXD->Textures = new Texture[TXD->TextureCount];
			}
			break;
		case 21:
			fread(&TexSectHeader, sizeof(SectionHeader), 1, dict);
			switch (TexSectHeader.Ident) {
			case 1:
				if (i == TXD->TextureCount) {
					TXD->TextureCount++;
					TXD->Textures = new Texture[TXD->TextureCount];
				}
				EOT = ftell(dict);
				EOT += TexSectHeader.Size;
				fread(&TXD->Textures[i].Header, sizeof(TXDTexHeader), 1, dict);
				TXD->Textures[i].Offset = ftell(dict);
				if ((TXD->Textures[i].Header.Compression == 0) && (TXD->Textures[i].Header.Bits == 8)) {
					TXD->Textures[i].Offset -= 4;
					fseek(dict, 1020, SEEK_CUR);
					fread(&TXD->Textures[i].Header.DataSize, 4, 1, dict);
					TXD->Textures[i].Header.DataSize += 1028;
				}
				TXD->Textures[i].MipMaps = new MipMap[TXD->Textures[i].Header.MipMaps - 1];
				fseek(dict, TXD->Textures[i].Offset + TXD->Textures[i].Header.DataSize, SEEK_SET);

				for (j = 0; j<TXD->Textures[i].Header.MipMaps - 2; j++) {
					fread(&TXD->Textures[i].MipMaps[j].Size, 4, 1, dict);
					TXD->Textures[i].MipMaps[j].Offset = ftell(dict);
					k = TXD->Textures[i].MipMaps[j].Size;
					if (TXD->Textures[i].Header.Compression == 3) k *= 2;
					TXD->Textures[i].MipMaps[j].Width = (int)((TXD->Textures[i].Header.Width / TXD->Textures[i].Header.Height) * sqrt(k / (TXD->Textures[i].Header.Width / TXD->Textures[i].Header.Height)));
					TXD->Textures[i].MipMaps[j].Height = (int)(sqrt(k / (TXD->Textures[i].Header.Width / TXD->Textures[i].Header.Height)));
					fseek(dict, TXD->Textures[i].MipMaps[j].Offset + TXD->Textures[i].MipMaps[j].Size, SEEK_SET);
				}

				fseek(dict, EOT, SEEK_SET);
				i++;
				break;
			default: fseek(dict, TexSectHeader.Size, SEEK_CUR); break;
			}
			break;
		default: fseek(dict, SectHeader.Size, SEEK_CUR); break;
		}
		CurPos = ftell(dict);
		CurPos += 12;
	}
	fclose(dict);
}

/*---------------------------------------------
GetTexture

desc:

params:	TXD
Pointer to our struct where we
saved all the data.

index
Index of texture inside of the
txd.

Decompressed
Pointer to a byte array where
we save the texture.
-----------------------------------------------*/
bool GetTexture(Dictionary* TXD, int Index, BYTE* Decompressed)
{
	BYTE* Compressed = (BYTE*)malloc(TXD->Textures[Index].Header.DataSize);

	FILE* f = fopen(TXD->FileName, "rb");
	fseek(f, TXD->Textures[Index].Offset, SEEK_SET);
	fread((void*)&Compressed[0], TXD->Textures[Index].Header.DataSize, 1, f);
	fclose(f);

	switch (TXD->Textures[Index].Header.Compression) {
	case 0:
		switch (TXD->Textures[Index].Header.Bits) {
		case 8: break;//Decompress8bit(Compressed, Decompressed, TXD->Textures[Index].Header.Width, TXD->Textures[Index].Header.Height);break;
		case 32: Decompress32bit(Compressed, Decompressed, TXD->Textures[Index].Header.Width, TXD->Textures[Index].Header.Height); break;
		}
		break;
	case 1: DecompressDXT1(Compressed, Decompressed, TXD->Textures[Index].Header.Width, TXD->Textures[Index].Header.Height); break;
	case 3: DecompressDXT3(Compressed, Decompressed, TXD->Textures[Index].Header.Width, TXD->Textures[Index].Header.Height); break;
	default: return false; break;
	}

	free(Compressed);
	return true;
}

/*---------------------------------------------
DecompressDXT1

desc:

params:
-----------------------------------------------*/
void DecompressDXT1(BYTE* Compressed, BYTE* Decompressed, int Width, int Height)
{
	int xblocks, yblocks, x, y;
	DXTBlock* pBlock;
	Color8888 Col0, Col1, Col2, Col3;
	int* pPos;

	xblocks = (int)(Width / 4);
	yblocks = (int)(Height / 4);

	for (y = 0; y<yblocks; y++)
	{
		pBlock = (DXTBlock*)((int)(Compressed)+y*xblocks * 8);
		for (x = 0; x<xblocks; x++)
		{
			GetBlockColours(pBlock, &Col0, &Col1, &Col2, &Col3);
			pPos = (int*)((int)(Decompressed)+(x * 16) + ((y * 4) * Width * 4));
			DecodeBlock(pPos, pBlock, Width, &Col0, &Col1, &Col2, &Col3);
			pBlock++;
		}
	}
}

/*---------------------------------------------
DecompressDXT3

desc:

params:
-----------------------------------------------*/
void DecompressDXT3(BYTE* Compressed, BYTE* Decompressed, int Width, int Height)
{
	int xblocks, yblocks, x, y;
	DXTBlock* pBlock;
	DXTAlphaBlock* pAlphaBlock;
	Color8888 Col0, Col1, Col2, Col3;
	int* pPos;
	int AlphaZero;

	xblocks = (int)(Width / 4);
	yblocks = (int)(Height / 4);

	Col0.a = 0;
	Col0.r = 0xFF;
	Col0.g = 0xFF;
	Col0.b = 0xFF;
	memcpy(&AlphaZero, &Col0, 4);

	for (y = 0; y<yblocks; y++)
	{
		pBlock = (DXTBlock*)((int)(Compressed)+(y*xblocks * 16));
		for (x = 0; x<yblocks; x++)
		{
			pAlphaBlock = (DXTAlphaBlock*)(pBlock);
			pBlock++;
			GetBlockColours(pBlock, &Col0, &Col1, &Col2, &Col3);
			pPos = (int*)((int)(Decompressed)+(x * 16) + ((y * 4) * Width * 4));
			DecodeBlock(pPos, pBlock, Width, &Col0, &Col1, &Col2, &Col3);
			DecodeAlphaBlock(pPos, pAlphaBlock, Width, AlphaZero);
			pBlock++;
		}
	}
}

/*---------------------------------------------
Decompress8bit

desc:

params:
-----------------------------------------------*/
void Decompress8bit(BYTE* Compressed, BYTE* Decompressed, int Width, int Height)
{
	/*RGBQuad Palette[256];
	BYTE* bRow;
	int x,y,pos;

	memcpy(&Palette[0],&Compressed[0],1024);
	pos=0;

	bRow = new BYTE [Width];
	for(y=0;y<Height-1;y++)
	{
	memcpy(&bRow[0],&Compressed[(y*Width)+1028],Width);
	for(x=0;x<Width-1;x++)
	{
	Decompressed[pos]=Palette[bRow[x]].res;
	pos++;
	Decompressed[pos]=Palette[bRow[x]].r;
	pos++;
	Decompressed[pos]=Palette[bRow[x]].g;
	pos++;
	Decompressed[pos]=Palette[bRow[x]].b;
	pos++;
	}
	}
	free(bRow);*/
}

/*---------------------------------------------
Decompress32bit

desc:

params:
-----------------------------------------------*/
void Decompress32bit(BYTE* Compressed, BYTE* Decompressed, int Width, int Height)
{
	int i;

	for (i = 0; i<Width*Height - 1; i++)
	{
		Decompressed[i * 4 + 2] = Compressed[(i * 4) + 2];
		Decompressed[(i * 4) + 1] = Compressed[(i * 4) + 1];
		Decompressed[(i * 4)] = Compressed[i * 4];
		Decompressed[(i * 4) + 3] = Compressed[(i * 4) + 3];
	}
}

/*---------------------------------------------
GetBlockColours

desc:

params:
-----------------------------------------------*/
void GetBlockColours(DXTBlock* Block, Color8888* Col0, Color8888* Col1, Color8888* Col2, Color8888* Col3)
{
	Color565to8888(Block->col0, Col0);
	Color565to8888(Block->col1, Col1);

	if (Block->col0 > Block->col1) {
		Col2->r = (BYTE)((int)((Col0->r * 2 + Col1->r) / 3));
		Col2->g = (BYTE)((int)((Col0->g * 2 + Col1->g) / 3));
		Col2->b = (BYTE)((int)((Col0->b * 2 + Col1->b) / 3));
		Col2->a = 0xFF;

		Col3->r = (BYTE)((int)((Col1->r * 2 + Col0->r) / 3));
		Col3->g = (BYTE)((int)((Col1->g * 2 + Col0->g) / 3));
		Col3->b = (BYTE)((int)((Col1->b * 2 + Col0->b) / 3));

		Col3->a = 0xFF;
	}
	else {
		Col2->r = (BYTE)((int)((Col0->r + Col1->r) / 2));
		Col2->g = (BYTE)((int)((Col0->g + Col1->g) / 2));
		Col2->b = (BYTE)((int)((Col0->b + Col1->b) / 2));
		Col2->a = 0xFF;

		Col3->r = 0xFF;
		Col3->g = 0x00;
		Col3->b = 0xFF;
		Col3->a = 0x00;
	}
}

/*---------------------------------------------
Color565to8888

desc:

params:
-----------------------------------------------*/
void Color565to8888(WORD Color565, Color8888* Color8888)
{
	Color8888->a = 0xFF;
	Color8888->r = (BYTE)((int)(((Color565 & 0x1F) * 255) / 31));
	Color8888->g = (BYTE)((int)((((Color565 >> 5) & 0x3F) * 255) / 63));
	Color8888->b = (BYTE)((int)(((Color565 >> 11) * 255) / 31));
}

/*---------------------------------------------
DecodeBlock

desc:

params:
-----------------------------------------------*/
void DecodeBlock(int* pPos, DXTBlock* pBlock, int Width, Color8888* Col0, Color8888* Col1, Color8888* Col2, Color8888* Col3)
{
	int masks[4];
	int shift[4];
	int bits, x, y;

	masks[0] = 3;
	masks[1] = 12;
	masks[2] = (3 << 4);
	masks[3] = (3 << 6);
	shift[0] = 0;
	shift[1] = 2;
	shift[2] = 4;
	shift[3] = 6;

	for (y = 0; y<4; y++)
	{
		for (x = 0; x<4; x++)
		{
			bits = (pBlock->row[y] & masks[x]);
			bits = (bits >> shift[x]);
			switch (bits)
			{
			case 0: memcpy(pPos, Col0, 4); break;
			case 1: memcpy(pPos, Col1, 4); break;
			case 2: memcpy(pPos, Col2, 4); break;
			case 3: memcpy(pPos, Col3, 4); break;
			}
			pPos++;
		}
		pPos += (Width - 4);
	}
}

/*---------------------------------------------
DecodeAlphaBlock

desc:

params:
-----------------------------------------------*/
void DecodeAlphaBlock(int *pPos, DXTAlphaBlock* pAlphaBlock, int Width, int AlphaZero)
{
	int x, y;
	int nCol;
	WORD wrd;
	Color8888 Col;

	Col.r = 0;
	Col.g = 0;
	Col.b = 0;

	for (y = 0; y<4; y++)
	{
		wrd = pAlphaBlock->row[y];
		for (x = 0; x<4; x++)
		{
			*pPos = (*pPos & AlphaZero);
			Col.a = (wrd & 0x000F);
			Col.a = (Col.a | (Col.a << 4));
			memcpy(&nCol, &Col, 4);
			*pPos = (*pPos | nCol);
			wrd = (wrd >> 4);
			pPos++;
		}
		pPos += (Width - 4);
	}
}
