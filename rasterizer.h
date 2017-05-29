//
// Created by iroas on 17/05/2017.
//

#ifndef FINAL2_RASTERIZER_H
#define FINAL2_RASTERIZER_H

#include "objLists.h"

typedef struct pix{
    unsigned char r,g,b;
    double zBuffer;
    double normal[3];
}pixels;

unsigned char*** createRasterX(int rows, int columns, int colors);
unsigned char** createRasterY(int columns, int colors);
unsigned char* createRasterP(int colors);

void printRasterRGB(unsigned char*** raster, int rows, int columns);
void printRaster2FileRGB(char* fileName, unsigned char*** raster, int rows, int columns);
void cleanRasterRGB(unsigned char*** raster, int columns, int rows);

pixels** triangleDraw(vertexesProj *triangle, int c1, int c2, int c3);
void bresenham(pixels** raster1, vertexesProj point1, vertexesProj point2, int c1, int c2, int c3);
void scanline(pixels** raster1, int c1, int c2, int c3);
void drawFace2Raster(pixels** rasProt, faces* fList, int fListSize, edges* eList, vertexesProj* vListProj);

pixels** createRasProtX(int rows, int columns);
pixels* createRasProtY(int columns);

#endif //FINAL2_RASTERIZER_H
