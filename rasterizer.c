//
// Created by iroas on 17/05/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rasterizer.h"

// Esta función se encarga de alojar memoria dependiendo del tamaño filas del raster.
unsigned char*** createRasterX(int rows, int columns, int colors){
    unsigned char ***ras=(unsigned char***)malloc(sizeof(unsigned char**)*rows);
    int i;
    for(i=0;i<rows;i++){
        ras[i]= createRasterY(columns, colors);
    }
    return ras;
}

// Esta función se encarga de alojar memoria dependiendo del tamaño de columnas del raster.
unsigned char** createRasterY(int columns, int colors){
    unsigned char **ras=(unsigned char**)malloc(sizeof(unsigned char*)*columns);
    int i;
    for(i=0;i<columns;i++){
        ras[i]= createRasterP(colors);
    }
    return ras;
}

// Esta función se encarga de alojar memoria para el formato de color de cada pixel.
unsigned char* createRasterP(int colors){
    unsigned char *ras=(unsigned char*)malloc(sizeof(unsigned char)*colors);
    int i;
    for(i=0;i<colors;i++){
        ras[i]=0;
    }
    return ras;
}

//Esta función se encarga de imprimir los colores de los pixeles del raster en la terminal en formato RGB.
void printRasterRGB(unsigned char ***raster, int rows, int columns){
    printf("P3\n1920 1080\n255\n\n");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            printf("%i %i %i\n", raster[i][j][0], raster[i][j][1], raster[i][j][2]);
        }
    }
    return;
}

// Esta función se encarga de imprimir el raster directamente a un archivo con pixeles en formato RGB.
void printRaster2FileRGB(char* fileName, unsigned char ***raster, int rows, int columns){
    FILE* f;
    f=fopen(fileName, "w+");
    fprintf(f, "P3\n1920 1080\n255\n\n");
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            fprintf(f, "%i %i %i\n", raster[i][j][0], raster[i][j][1], raster[i][j][2]);
        }
    }
    fclose(f);
    return;
}

// Esta función se encarga de limpiar el raster (cambiando los pixeles a color negro en formato RGB).
void cleanRasterRGB(unsigned char ***raster, int columns, int rows){
    for (int i = 0; i < columns; ++i) {
        for (int j = 0; j < rows; ++j) {
            raster[i][j][0]=0;
            raster[i][j][1]=0;
            raster[i][j][2]=0;
        }
    }
    return;
}

// Esta función se encarga de dibujar un triangulo de la figura
pixels** triangleDraw(vertexesProj *triangle, int c1, int c2, int c3){
    pixels** rasProt=NULL;
    if(triangle[0].x<0 || triangle[1].x<0 || triangle[2].x<0 || triangle[0].y<0 || triangle[1].y<0 || triangle[2].y<0)
        return rasProt;
    if(triangle[0].x>=1920 || triangle[1].x>=1920 || triangle[2].x>=1920 || triangle[0].y>=1080 || triangle[1].y>=1080 || triangle[2].y>=1080)
        return rasProt;
    rasProt=createRasProtX(1080, 1920);
    bresenham(rasProt, triangle[0], triangle[1], c1, c2, c3);
    bresenham(rasProt, triangle[1], triangle[2], c1, c2, c3);
    bresenham(rasProt, triangle[2], triangle[0], c1, c2, c3);
    scanline(rasProt, c1, c2, c3);
    return rasProt;
}

// Esta función se encarga de hacer el cálculo de las líneas a dibujar junto con el zBuffer de cada pixel
void bresenham(pixels** raster1, vertexesProj point1, vertexesProj point2, int c1, int c2, int c3){
    int x1=point1.x, x2=point2.x, y1=point1.y, y2=point2.y;
    double dz, zB;
    zB=point1.zBuf<point2.zBuf ? point1.zBuf : point2.zBuf;
    dz=((point2.zBuf - point1.zBuf) / abs(point2.y - point1.y));
    int dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
    int dy = abs(y2-y1), sy = y1<y2 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;
    for(;;) {
        raster1[y1][x1].r = c1;
        raster1[y1][x1].g = c2;
        raster1[y1][x1].b = c3;
        raster1[y1][x1].zBuffer=zB;
        // raster1[y1][x1].normal=normal de cualuier punto;
        if (x1 == x2 && y1 == y2) break;
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dy) {
            err += dx;
            y1 += sy;
            zB+=dz;
        }
    }
}

// Esta función se encarga de darle superficie al triangulo
void scanline(pixels** raster1, int c1, int c2, int c3){
    int x1, x2, flag=0;
    double zB1, zB2, dz;
    for (int i = 0; i < 1080; ++i) {
        flag=0;
        for (int j = 0; j < 1920; ++j) {
            if(raster1[i][j].zBuffer>-100000){
                if(flag==0){
                    zB1=raster1[i][j].zBuffer;
                    x1=j;
                    ++flag;
                } else if(flag==1){
                    zB2=raster1[i][j].zBuffer;
                    x2=j;
                    dz=((zB2-zB1)/(x2-x1));
                    ++flag;
                    j=x1;
                } else if(flag==2){
                    j=1920;
                }
            } else if(flag==2){
                raster1[i][j].r=c1;
                raster1[i][j].g=c2;
                raster1[i][j].b=c3;
                zB1+=dz;
                raster1[i][j].zBuffer=zB1;
            }
        }
    }
}

// Esta función se encarga de decidir que caras se dibujan y cuales no a través de la normal de superficie
void drawFace2Raster(pixels** rasProt, faces* fList, int fListSize, edges* eList, vertexesProj* vListProj){
    vertexesProj auxVP[3];
    pixels** auxRasProt;
    for (int i = 0; i < fListSize; ++i) {
        auxVP[0]=vListProj[eList[fList[i].e1].v1];
        auxVP[1]=vListProj[eList[fList[i].e2].v1];
        auxVP[2]=vListProj[eList[fList[i].e3].v1];
        if(){
            auxRasProt= triangleDraw(auxVP, 38, 220, 226);
        }
    }
}

// Esta función crea un prototipo de raster que contiene los colores junto con el zBuffer y la normal de cada pixel
pixels** createRasProtX(int rows, int columns){
    pixels** rasProt=NULL;
    rasProt=(pixels**)malloc(sizeof(pixels*)*rows);
    for (int i = 0; i < rows; ++i) {
        rasProt[i]=createRasProtY(columns);
    }
    return rasProt;
}

// Esta función inicializa las variables de cada pixel
pixels* createRasProtY(int columns){
    pixels* rasProt=NULL;
    rasProt=(pixels*)malloc(sizeof(pixels)*columns);
    for (int i = 0; i < columns; ++i) {
        rasProt[i].r=0;
        rasProt[i].g=0;
        rasProt[i].b=0;
        rasProt[i].zBuffer=-100000;
        rasProt[i].normal[0]=0;
        rasProt[i].normal[1]=0;
        rasProt[i].normal[2]=0;
    }
    return rasProt;
}