
//headers
#include "Angel.h"
#include<GL/gl.h>
#include<GL/glut.h>
#include<stdio.h>
//globals
GLuint objek;
float gawangrot;
char ch='1';
//other functions and main
//wavefront .obj loader code begins

void PerspectiveGL(GLdouble fovY, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    const GLdouble pi=3.14159265;
    GLdouble fW,fH;

    fH =tan (fovY / 360 *pi)*zNear;
    fW =fH * aspect;
    glFrustum (-fW,fW,-fH,fH,zNear,zFar);
}

void loadObj(char *fname)
{
    FILE *fp;
    int read;
    int read2;
    GLfloat x, y, z;
    GLint vertex[3], uv[3], normal[3];
    char ch;
    objek=glGenLists(1);
    fp=fopen(fname,"r");
    if (!fp)
    {
        printf("can't open file %s\n", fname);
        exit(1);
    }
    glPointSize(2.0);
    glNewList(objek, GL_COMPILE);
    {
        glPushMatrix();
        glBegin(GL_POINTS);
        while(!(feof(fp)))
        {
            read=fscanf(fp,"%c %f %f %f",&ch,&x,&y,&z);
            if(read==4&&ch=='v')
            {
               glVertex3f(x,y,z);
            }
            if(read==4&&ch=='vn')
            {
               glNormal3f(x,y,z);
            }
            if(read==4&&ch=='vt')
            {
               glTexCoord3f(x,y,z);
            }
            read2=fscanf(fp,"%c %d/%d/%d %d/%d/%d %d/%d/%d",&ch,&vertex[0],&uv[0],&normal[0],&vertex[1],&uv[1],&normal[1],&vertex[2],&uv[2],&normal[2]);
            if(ch=='f'){
                glVertex3f(vertex[0],vertex[1],vertex[2]);
                glNormal3f(normal[0],normal[1],normal[2]);
            }
            //if(read==4&&ch=='vn')
            //{
            //   glVertex3f(x,y,z);
            //}
        }
        glEnd();
    }
    glPopMatrix();
    glEndList();
    fclose(fp);
}
//wavefront .obj loader code ends here
void reshape(int w,int h)
{
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    PerspectiveGL (60, (GLfloat)w / (GLfloat)h, 0.1, 1000.0);
    //glOrtho(-25,25,-2,2,0.1,100);
    glMatrixMode(GL_MODELVIEW);
}
void drawGawang()
{
    glPushMatrix();
    glTranslatef(0,0,-105);
    glColor3f(0,0,1);
    glScalef(0.1,0.1,0.1);
    glRotatef(gawangrot,0,1,0);
    glCallList(objek);
    glPopMatrix();
    gawangrot=gawangrot+0.01;
    if(gawangrot>360)gawangrot=gawangrot-360;
}

void display(void)
{
    glClearColor (1.0,1.0,1.0,1.0);
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    drawGawang();
    glutSwapBuffers(); //swap the buffers
}
int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(800,450);
    glutInitWindowPosition(20,20);
    glutCreateWindow("Penalti");
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutIdleFunc(display);
    loadObj("Gawang.obj");
    glutMainLoop();
    return 0;
}
