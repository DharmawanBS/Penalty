// Two-Dimensional Sierpinski Gasket
// Generated using randomly selected vertices and bisection

#include "Angel.h"
#include <stdlib.h>
#include <vector>
#include <fstream>	// FILE
//----------------------------------------------------------------------------
std::vector<vec3> vertices;
std::vector<vec3> normals; // for shading
std::vector<vec2> UVs; //for texture

GLuint v_model,v_view,v_proj;

mat4 projection = Frustum(-0.2, 0.2, -0.2, 0.2, 0.2, 2.0);
mat4 view = Translate(0.0, 0.0, -0.5);
mat4 model = Translate(0.0, 0.0, 0.0);

bool loadObj(const char* filename);

void init( void )
{
    if(!loadObj("untitled.obj")) return;

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER,
                  8* vertices.size()*sizeof(GLfloat)
                  ,NULL, GL_STATIC_DRAW );

    glBufferSubData( GL_ARRAY_BUFFER,
                     0,
                     (3* vertices.size()*sizeof(GLfloat)),
                     vertices.data() );

    glBufferSubData( GL_ARRAY_BUFFER,
                     (3* vertices.size()*sizeof(GLfloat)),
                     (3* normals.size()*sizeof(GLfloat)),
                     normals.data() );

    glBufferSubData( GL_ARRAY_BUFFER,
                     6*vertices.size()*sizeof(GLfloat),
                     (2* UVs.size()*sizeof(GLfloat)),
                     UVs.data() );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader("vshader.glsl","fshader.glsl");
    glUseProgram( program );

    // set up vertex arrays
    GLuint Position = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( Position );
    glVertexAttribPointer( Position, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    GLuint Normal = glGetAttribLocation( program, "vNormal" );
    glEnableVertexAttribArray( Normal );
    glVertexAttribPointer( Normal, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(3*vertices.size()*sizeof(GLfloat)) );

    GLuint UV = glGetAttribLocation( program, "vUV" );
    glEnableVertexAttribArray( UV );
    glVertexAttribPointer( UV, 3, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(6*vertices.size()*sizeof(GLfloat)) );

    //transform
    v_model = glGetUniformLocation( program, "vModel" );
    v_view= glGetUniformLocation( program, "vView" );
    v_proj= glGetUniformLocation( program, "vProj" );



    //material - static ==========================================
    vec4 light_position( 0.0, 0.0, 2.0, 0.0 );
    vec4 light_ambient( 0.2, 0.2, 0.2, 1.0 );
    vec4 light_diffuse( 1.0, 1.0, 1.0, 1.0 );
    vec4 light_specular( 1.0, 1.0, 1.0, 1.0 );

    vec4 material_ambient( 1.0, 0.0, 1.0, 1.0 );
    vec4 material_diffuse( 1.0, 0.8, 0.0, 1.0 );
    vec4 material_specular( 1.0, 0.0, 1.0, 1.0 );
    float  material_shininess = 5.0;

    vec4 ambient_product = light_ambient * material_ambient;
    vec4 diffuse_product = light_diffuse * material_diffuse;
    vec4 specular_product = light_specular * material_specular;

    glUniform4fv( glGetUniformLocation(program, "AmbientProduct"),
                  1, ambient_product );
    glUniform4fv( glGetUniformLocation(program, "DiffuseProduct"),
                  1, diffuse_product );
    glUniform4fv( glGetUniformLocation(program, "SpecularProduct"),
                  1, specular_product );
    glUniform4fv( glGetUniformLocation(program, "LightPosition"),
                  1, light_position );
    glUniform1f( glGetUniformLocation(program, "Shininess"),
                 material_shininess );
    //=============================================================

    glEnable( GL_DEPTH_TEST );

    glClearColor( 0, 0, 0, 1.0 ); /* white background */
}

//----------------------------------------------------------------------------
void display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    model = Translate(0,0,0) *  Scale(.25,.25,.25);
    glUniformMatrix4fv( v_model, 1, GL_TRUE, model );
    glUniformMatrix4fv( v_view, 1, GL_TRUE, view );
    glUniformMatrix4fv( v_proj, 1, GL_TRUE,projection );

    glDrawArrays( GL_TRIANGLES, 0,vertices.size());

    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void keyboard( unsigned char key, int x, int y )
{
    switch( key )
    {
    case 033: // Escape Key
    case 'q':
    case 'Q':
        exit( EXIT_SUCCESS );
        break;
    }
}

//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{

}
//----------------------------------------------------------------------------
void
idle( void )
{
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize(720,720 );

    glutCreateWindow( "INI JUDUL" );

    glewInit();

    init();

    glutIdleFunc(idle);
    glutDisplayFunc( display );
    glutMouseFunc(mouse);
    glutKeyboardFunc( keyboard );

    glutMainLoop();
    return 0;
}


bool loadObj(const char* filename)
{
    FILE * file = fopen(filename, "r");
    if( file == NULL )
    {
        printf("failed to open the file !\n");
        return false;
    }
    std::vector<vec3> temp_vertices;
    std::vector<vec3> temp_normals;
    std::vector<vec2> temp_UVs;
    while( true )
    {
        char lineHeader[128];
        int res = fscanf(file, "%s", lineHeader);
        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        if ( strcmp( lineHeader, "v" ) == 0 )
        {
            vec3 v;
            fscanf(file, "%f %f %f\n", &v.x, &v.y, &v.z );
            temp_vertices.push_back(v);
        }
        else if ( strcmp( lineHeader, "vt" ) == 0 )
        {
            vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y );
            temp_UVs.push_back(uv);
        }
        else if ( strcmp( lineHeader, "vn" ) == 0 )
        {
            vec3 n;
            fscanf(file, "%f %f %f\n", &n.x, &n.y, &n.z );
            temp_normals.push_back(n);
        }
        else if ( strcmp( lineHeader, "f" ) == 0 )
        {
            unsigned int vIDX[3], uvIDX[3], nIDX[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
                         &vIDX[0], &uvIDX[0], &nIDX[0],
                         &vIDX[1], &uvIDX[1], &nIDX[1],
                         &vIDX[2], &uvIDX[2], &nIDX[2]);

            if(matches!=9)
            {
                printf("failed to read command !\n");
                return false;
            }
            //obj file index start at 1
            for(int a = 0; a<3 ; a++ )
            {
                vertices.push_back(temp_vertices[vIDX[a]-1]);
                normals.push_back(temp_normals[nIDX[a]-1]);
                UVs.push_back(temp_UVs[uvIDX[a]-1]);
            }
        }
    }
    //don't forget to close the file, clean your own mess!
    fclose(file);
    return true;
}
