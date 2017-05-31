//
// Created by iroas on 17/05/2017.
//

#ifndef FINAL2_RASTERIZER_H
#define FINAL2_RASTERIZER_H

#include "objLists.h"

#define PI 3.14159265

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

int triangleDraw(vertexesProj *triangle, int c1, int c2, int c3, double* normal, pixels*** rasProt);
void bresenham(pixels*** raster1, vertexesProj point1, vertexesProj point2, int c1, int c2, int c3, double* normal);
void scanline(pixels*** raster1, int c1, int c2, int c3, double* normal);
void drawFace2Raster(pixels** rasProt, faces* fList, int fListSize, edges* eList, vertexesProj* vListProj, vertexes* vList,
                     unsigned char*** raster);

pixels** createRasProtX(int rows, int columns);
pixels* createRasProtY(int columns);
void cleanRasProt(pixels** rasProt, int columns, int rows);

#endif //FINAL2_RASTERIZER_H
