//
// Created by iroas on 17/05/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rasterizer.h"

double amb[3]={11, 25, 25};
double abc[3]={.004, .008, .0009};
double radial[3]={250, 350, 40};
double radialRGB[3]={18, 226, 223};

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
int triangleDraw(vertexesProj *triangle, int c1, int c2, int c3, pixels*** rasProt){
    // Si la cara rebasa el margen del raster, no se dibuja
    if(triangle[0].x<0 || triangle[1].x<0 || triangle[2].x<0 || triangle[0].y<0 || triangle[1].y<0 || triangle[2].y<0)
        return -1;
    if(triangle[0].x>=1920 || triangle[1].x>=1920 || triangle[2].x>=1920 || triangle[0].y>=1080 || triangle[1].y>=1080 || triangle[2].y>=1080)
        return -1;
    // Se dibuja cada una de las aristas a través de Bresenham
    bresenham(rasProt, triangle[0], triangle[1], c1, c2, c3);
    bresenham(rasProt, triangle[1], triangle[2], c1, c2, c3);
    bresenham(rasProt, triangle[2], triangle[0], c1, c2, c3);
    // Se escanea el triángulo dibujado a través de Bresenham y se aplica Scanline para rellenar la cara
    scanline(rasProt);
    return 0;
}

// Esta función se encarga de hacer el cálculo de las líneas a dibujar junto con el zBuffer de cada pixel
void bresenham(pixels*** raster1, vertexesProj point1, vertexesProj point2, int c1, int c2, int c3){
    int x1=point1.x, x2=point2.x, y1=point1.y, y2=point2.y;
    double dz, zB, dxN, dyN, dzN, distance, attenuation, dotProd;
    double totColor[3];
    double radVector[3], length;
    double normal[3];
    normal[0]=point1.normal[0];
    normal[1]=point1.normal[1];
    normal[2]=point1.normal[2];
    dxN=((point2.normal[0]-point1.normal[0])/abs(point2.x-point1.x));
    dyN=((point2.normal[1]-point1.normal[1])/abs(point2.y-point1.y));
    dzN=((point2.normal[2]-point1.normal[2])/abs(point2.y-point1.y));
    zB=point1.zBuf;
    dz=((point2.zBuf - point1.zBuf) / abs(point2.y - point1.y));
    int dx = abs(x2-x1), sx = x1<x2 ? 1 : -1;
    int dy = abs(y2-y1), sy = y1<y2 ? 1 : -1;
    int err = (dx>dy ? dx : -dy)/2, e2;
    for(;;) {
        // Cálculo del vector desde el punto emisor de luz hacia el pixel
        radVector[0]=(x1-radial[0]);
        radVector[1]=(y1-radial[1]);
        radVector[2]=(zB-radial[2]);
        // Cálculo del tamaño del vector
        length=sqrt(pow(radVector[0],2)+pow(radVector[1],2)+pow(radVector[2],2));
        // Normalización del vector
        radVector[0]/=length;
        radVector[1]/=length;
        radVector[2]/=length;
        // Producto punto entre el vector normal y el vector radial
        dotProd=((radVector[0]*normal[0])+(radVector[1]*normal[1])+(radVector[2]*normal[2]));
        // Cálculo del factor de atenuación
        distance=sqrt(pow(x1-radial[0], 2)+pow(y1-radial[1], 2)+pow(zB-radial[2], 2));
        attenuation=(1/((abc[0]*pow(distance, 2)+(abc[1]*distance)+abc[2])));
        // Suma de las luces
        totColor[0]=c1 + (amb[0]*ka) + (kd*attenuation*radialRGB[0]*dotProd);
        totColor[1]=c2 + (amb[1]*ka) + (kd*attenuation*radialRGB[1]*dotProd);
        totColor[2]=c3 + (amb[2]*ka) + (kd*attenuation*radialRGB[2]*dotProd);
        printf("%lf\t%lf\t%d\t%lf\t%lf\n", distance, attenuation, c3, totColor[2], dotProd);
        //printf("%d\t%d\t%lf\n", x1, y1, zB);
        raster1[0][y1][x1].r = (unsigned char) (floor(totColor[0] > 255 ? 255 : totColor[0]));
        raster1[0][y1][x1].g = (unsigned char) (floor(totColor[1] > 255 ? 255 : totColor[1]));
        raster1[0][y1][x1].b = (unsigned char) (floor(totColor[2] > 255 ? 255 : totColor[2]));
        raster1[0][y1][x1].zBuffer=zB;
        raster1[0][y1][x1].normal[0]=normal[0];
        raster1[0][y1][x1].normal[1]=normal[1];
        raster1[0][y1][x1].normal[2]=normal[2];
        // raster1[y1][x1].normal=normal de cualquier punto;
        if (x1 == x2 && y1 == y2) break;
        e2 = err;
        if (e2 > -dx) {
            err -= dy;
            x1 += sx;
            normal[0]+=dxN;
        }
        if (e2 < dy) {
            err += dx;
            y1 += sy;
            zB+=dz;
            normal[1]+=dyN;
            normal[2]+=dzN;
        }
    }
}

// Esta función se encarga de darle superficie al triangulo
void scanline(pixels*** raster1){
    int x1, x2, flag=0;
    double zB1, zB2, dz, r1, r2, dr, g1, g2, dg, b1, b2, db;
    double normal[0];
    for (int i = 0; i < 1080; ++i) {
        flag=0;
        for (int j = 0; j < 1920; ++j) {
            if(raster1[0][i][j].zBuffer>-100000){
                if(raster1[0][i][j+1].zBuffer==-100000){
                    if(flag==0){
                        zB1=raster1[0][i][j].zBuffer;
                        r1=(double) raster1[0][i][j].r;
                        g1=(double) raster1[0][i][j].g;
                        b1=(double) raster1[0][i][j].b;
                        x1=j;
                        ++flag;
                    } else if(flag==1){
                        zB2=raster1[0][i][j].zBuffer;
                        r2=(double) raster1[0][i][j].r;
                        g2=(double) raster1[0][i][j].g;
                        b2=(double) raster1[0][i][j].b;
                        x2=j;
                        dz=((zB2-zB1)/(x2-x1));
                        dr=((r2-r1)/(x2-x1));
                        dg=((g2-g1)/(x2-x1));
                        db=((b2-b1)/(x2-x1));
                        ++flag;
                        j=x1;
                    } else if(flag==2){
                        j=1920;
                    }
                }
            } else if(flag==2){
                r1+=dr;
                g1+=dg;
                b1+=db;
                raster1[0][i][j].r= (unsigned char) (r1 > 255 ? 255 : r1);
                raster1[0][i][j].g= (unsigned char) (g1 > 255 ? 255 : g1);
                raster1[0][i][j].b= (unsigned char) (b1 > 255 ? 255 : b1);
                // Interpolación del zBuffer
                zB1+=dz;
                raster1[0][i][j].zBuffer=zB1;
                raster1[0][i][j].normal[0]=normal[0];
                raster1[0][i][j].normal[1]=normal[1];
                raster1[0][i][j].normal[2]=normal[2];
            }
        }
    }
}

// Esta función se encarga de decidir que caras se dibujan y cuales no a través de la normal de superficie
void drawFace2Raster(pixels** rasProt, faces* fList, int fListSize, edges* eList, vertexesProj* vListProj, vertexes* vList,
                     unsigned char*** raster){
    vertexesProj auxVP[3];
    vertexes auxV[3];
    double normal[3];
    pixels** auxRasProt=NULL;
    auxRasProt=createRasProtX(1080, 1920);;
    double angle;
    for (int i = 0; i < fListSize; ++i) {
        // Vertices de la cara
        auxV[0]=vList[eList[fList[i].e1].v1];
        auxV[1]=vList[eList[fList[i].e2].v1];
        auxV[2]=vList[eList[fList[i].e3].v1];
        // Calculo de la normal de la cara
        normal[0]=(((auxV[1].y-auxV[0].y)*(auxV[2].z-auxV[1].z))-((auxV[1].z-auxV[0].z)*(auxV[2].y-auxV[1].y)));
        normal[1]=(((auxV[1].z-auxV[0].z)*(auxV[2].x-auxV[1].x))-((auxV[1].x-auxV[0].x)*(auxV[2].z-auxV[1].z)));
        normal[2]=(((auxV[1].x-auxV[0].x)*(auxV[2].y-auxV[1].y))-((auxV[1].y-auxV[0].y)*(auxV[2].x-auxV[1].x)));
        // Calculo del ángulo de la normal de la cara contra la normal de la cámara
        angle=((normal[2]*-1)/((sqrt((normal[0]*normal[0])+(normal[1]*normal[1])+(normal[2]*normal[2])))));
        angle=acos(angle)*(180/PI);
        // Selección de las caras que son visibles respecto a la normal
        if(angle>=90 && angle<=180){
            // Limpieza del auxiliar de raster para no volver a alojar memoria
            cleanRasProt(auxRasProt, 1080, 1920);
            auxVP[0]=vListProj[eList[fList[i].e1].v1];
            auxVP[1]=vListProj[eList[fList[i].e2].v1];
            auxVP[2]=vListProj[eList[fList[i].e3].v1];
            if(triangleDraw(auxVP, 21, 37, 38, &auxRasProt)==0){
                for (int j = 0; j < 1080; ++j) {
                    for (int k = 0; k < 1920; ++k) {
                        // Aplicación del zBuffer para colocar los triangulos en el rasterProt
                        if(auxRasProt[j][k].zBuffer>rasProt[j][k].zBuffer){
                            rasProt[j][k].r=auxRasProt[j][k].r;
                            rasProt[j][k].g=auxRasProt[j][k].g;
                            rasProt[j][k].b=auxRasProt[j][k].b;
                            rasProt[j][k].zBuffer=auxRasProt[j][k].zBuffer;
                            rasProt[j][k].normal[0]=auxRasProt[j][k].normal[0];
                            rasProt[j][k].normal[1]=auxRasProt[j][k].normal[1];
                            rasProt[j][k].normal[2]=auxRasProt[j][k].normal[2];
                        }
                    }
                }
            }
        }
    }
    // Al terminar de dibujar todas las caras en el rasterProt, se procede a colocar el modelo en el raster original
    for (int l = 0; l < 1080; ++l) {
        for (int i = 0; i < 1920; ++i) {
            raster[l][i][0]=rasProt[l][i].r;
            raster[l][i][1]=rasProt[l][i].g;
            raster[l][i][2]=rasProt[l][i].b;
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

// Esta función limpia el raster prototipo
void cleanRasProt(pixels** rasProt, int columns, int rows){
    for (int i = 0; i < columns; ++i) {
        for (int j = 0; j < rows; ++j) {
            rasProt[i][j].r=0;
            rasProt[i][j].g=0;
            rasProt[i][j].b=0;
            rasProt[i][j].zBuffer=-100000;
            rasProt[i][j].normal[0]=0;
            rasProt[i][j].normal[1]=0;
            rasProt[i][j].normal[2]=0;
        }
    }
}