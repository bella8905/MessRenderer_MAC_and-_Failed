/////////////////////////////////////////////////////////////////
//
//  Practice_11_19_14 - main
//
//  Implementation of Anton's OpenGL tutorial
//  p1 - a simple start demo. 
//      gl program basic setups: error checks / info print
//      sd / sp load, compile and link
//      antTweakbar simple integration
//
//  Copyright (c) 2014 Bella Q
//  
/////////////////////////////////////////////////////////////////

#include "Utl_Include.h"

#include <gl/glew.h>
#define GLFW_DLL  // force it to use dynamic library, think this is fine for compling in vs
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <streambuf>

using namespace std;

static const char* VS_FILE = "../common/shaders/simple.vert";
static const char* FS_FILE = "../common/shaders/simple.frag";

int gWinWidth = 640;
int gWinHeight = 480;


// read in txt file as std strings
bool _readFileToStr( const char* t_file, string& t_fileContentStr  ) {
    ifstream in( t_file );
    if( !in || in.bad() ) {
        LogError<<"read shader file <<"<<t_file<<">> failed"<<LogEndl;
        in.close();
        in.clear();
        return false;
    }

    t_fileContentStr = string( ( istreambuf_iterator<char>( in ) ), istreambuf_iterator<char>() );

    in.close();
    in.clear();

    return true;
}

void _glfwErrorCallback( int t_error, const char* t_desc ) {
    LogError<<"GLFW ERROR: code "<<t_error<<" msg: "<<t_desc<<LogEndl;
}

void _glfwWindowSizeCallback( GLFWwindow* t_win, int t_width, int t_height ) {
    gWinWidth = t_width;
    gWinHeight = t_height;

}




void _logGLParams() {
    enum ParamValType { SINGLE_INT, VEC_INT, SINGLE_BOOL, };
    
    struct glPrama {
        GLenum _param;
        string _paramStr;
        ParamValType _valType;
    } glParams[] = {
        // https://www.opengl.org/sdk/docs/man4/
        { GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS,  "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS",  SINGLE_INT },
        { GL_MAX_CUBE_MAP_TEXTURE_SIZE,         "GL_MAX_CUBE_MAP_TEXTURE_SIZE",         SINGLE_INT },
        // max count of buffer, that rendering in same time.
        // for more advanced effects where we want to split the output from our rendering into different images
        { GL_MAX_DRAW_BUFFERS,                  "GL_MAX_DRAW_BUFFERS",                  SINGLE_INT },
        { GL_MAX_FRAGMENT_UNIFORM_COMPONENTS,   "GL_MAX_FRAGMENT_UNIFORM_COMPONENTS",   SINGLE_INT },
        { GL_MAX_TEXTURE_IMAGE_UNITS,           "GL_MAX_TEXTURE_IMAGE_UNITS",           SINGLE_INT },
        { GL_MAX_TEXTURE_SIZE,                  "GL_MAX_TEXTURE_SIZE",                  SINGLE_INT },
        // Varying floats are those sent from the vertex shader to the fragment shaders. Usually
        { GL_MAX_VARYING_FLOATS,                "GL_MAX_VARYING_FLOATS",                SINGLE_INT },
        { GL_MAX_VERTEX_ATTRIBS,                "GL_MAX_VERTEX_ATTRIBS",                SINGLE_INT },
        { GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS,    "GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS",    SINGLE_INT },
        { GL_MAX_VERTEX_UNIFORM_COMPONENTS,     "GL_MAX_VERTEX_UNIFORM_COMPONENTS",     SINGLE_INT },
        { GL_MAX_VIEWPORT_DIMS,                 "GL_MAX_VIEWPORT_DIMS",                 VEC_INT },
        { GL_STEREO,                            "GL_STEREO",                            SINGLE_BOOL },
    };

    us count = sizeof( glParams ) / sizeof( glPrama );
    LogMsg<<"GL Context Params: "<<LogEndl;
    for( us i = 0 ; i < count; ++i  ) {
        LogMsg<<glParams[ i ]._paramStr<<": ";
        switch( glParams[ i ]._valType )
        {
        case SINGLE_INT:
            {
                int v = 0;
                glGetIntegerv( glParams[ i ]._param, &v );
                LogMsg<<v;
            } break;
        case VEC_INT:
            {
                int v[2];
                glGetIntegerv( glParams[ i ]._param, v );
                LogMsg<<v[0]<<" "<<v[1];
            } break;
        case SINGLE_BOOL:
            {
                unsigned char s = 0;
                glGetBooleanv( glParams[ i ]._param, &s );
                LogMsg<<(us)s;
            } break;
        default: break;
        }
        LogMsg<<LogEndl;
    }
    LogMsg<<LogEndl;
}

void _updateFPSCounter( GLFWwindow* t_window ) {
    if( t_window == 0  ) return;
    static double prevSeconds = glfwGetTime();
    static int frameCount = 0;
    double curSeconds = glfwGetTime();
    double elapsedSeconds = curSeconds - prevSeconds;
    if( elapsedSeconds > 0.25 ) {
        prevSeconds = curSeconds;
        double fps = ( double )frameCount / elapsedSeconds;
        ostringstream ss;
        ss<<"frame rate: "<<fps;
        string fpsStr = ss.str();
        glfwSetWindowTitle( t_window, fpsStr.c_str() );

        frameCount = 0;
    }

    frameCount++;
}

///////////////////////////////////////////////
// Shader / Shader Program Error Checking
string _glTypeToString( GLenum t_type ) {
    switch( t_type ) {
    case GL_BOOL: return "bool";
    case GL_INT: return "int";
    case GL_FLOAT: return "float";
    case GL_FLOAT_VEC2: return "vec2";
    case GL_FLOAT_VEC3: return "vec3";
    case GL_FLOAT_VEC4: return "vec4";
    case GL_FLOAT_MAT2: return "mat2";
    case GL_FLOAT_MAT3: return "mat3";
    case GL_FLOAT_MAT4: return "mat4";
    case GL_SAMPLER_2D: return "sampler2D";
    case GL_SAMPLER_3D: return "sampler3D";
    case GL_SAMPLER_CUBE: return "samplerCube";
    case GL_SAMPLER_2D_SHADOW: return "sampler2DShadow";
    default: break;
    }
    return "other";
}

// print errors in shader compilation
void _printShaderInfoLog( const GLuint& t_shaderIndex ) {
    int maxLength = 2048;
    int actualLength = 0;
    char log[2048];
    glGetShaderInfoLog( t_shaderIndex, maxLength, &actualLength, log );
    LogMsg<<"shader info log for GL index "<<t_shaderIndex<<LogEndl<<log<<LogEndl<<LogEndl;
}

// print errors in shader linking
void _printSPInfoLog( const GLuint& t_spIndex) {
    int maxLength = 2048;
    int actualLength = 0;
    char log[2048];
    glGetProgramInfoLog (t_spIndex, maxLength, &actualLength, log);
    LogMsg<<"program info log for GL index "<<t_spIndex<<LogEndl<<log<<LogEndl<<LogEndl;
}


bool _checkShaderCompileStatus( const GLuint& t_shaderIndex ) {
    int param = -1;
    glGetShaderiv( t_shaderIndex, GL_COMPILE_STATUS, &param );
    if( GL_TRUE != param ) {
        LogError<<"GL shader index "<<t_shaderIndex<<" did not compile"<<LogEndl;
        _printShaderInfoLog( t_shaderIndex );
        return false;
    }
    return true;
}

bool _checkSPLinkingStatus( const GLuint& t_spIndex ) {
    int param = -1;
    glGetProgramiv( t_spIndex, GL_LINK_STATUS, &param );
    if( GL_TRUE != param ) {
        LogError<<"GL shader program index "<<t_spIndex<<" did not link successfully"<<LogEndl;
        _printSPInfoLog( t_spIndex );
        return false;
    }
    return true;
}

bool _validateSP( const GLuint& t_spIndex ) {
    int param = -1;
    glValidateProgram( t_spIndex );
    glGetProgramiv( t_spIndex, GL_VALIDATE_STATUS, &param );
    LogMsg<<"program "<<t_spIndex<<" GL_VALIDATE_STATUS = " << param<<LogEndl;
    if (GL_TRUE != param) {
        _printSPInfoLog( t_spIndex );
        return false;
    }
    return true;
}

// print absolutely everything about a shader program - only useful if you get really
// stuck wondering why a shader isn't working properly 
void _printSPInfo( const GLuint& sp) {
    int params = -1;
    int i;

    LogMsg<<"--------------------shader program "<<sp<<" info--------------------"<<LogEndl;
    glGetProgramiv( sp, GL_LINK_STATUS, &params);
    LogMsg<<"GL_LINK_STATUS = "<<params<<LogEndl;

    glGetProgramiv( sp, GL_ATTACHED_SHADERS, &params);
    LogMsg<<"GL_ATTACHED_SHADERS = "<<params<<LogEndl;


    glGetProgramiv( sp, GL_ACTIVE_ATTRIBUTES, &params);
    LogMsg<<"GL_ACTIVE_ATTRIBUTES = "<<params<<LogEndl;

    for( i = 0; i < params; ++i) {
        char name[64];
        int maxLength = 64;
        int actualLength = 0;
        int size = 0;
        GLenum type;
        glGetActiveAttrib( sp, i, maxLength, &actualLength, &size, &type, name);
        if( size > 1) {
            int j;
            for( j = 0; j < size; ++j) {
                ostringstream ss;
                int location;
                ss<<name<<"["<<j<<"]";
                string longName = ss.str();
                location = glGetAttribLocation(sp, longName.c_str() );
                LogMsg<<i<<"i) type: "<<_glTypeToString( type )<<", name: "<<longName<<", location: "<<location<<LogEndl;
            }
        } else {
            int location = glGetAttribLocation( sp, name );
            LogMsg<<i<<") type: "<<_glTypeToString( type )<<", name: "<<name<<", location: "<<location<<LogEndl;
        }
    }

    glGetProgramiv( sp, GL_ACTIVE_UNIFORMS, &params);
    LogMsg<<"GL_ACTIVE_UNIFORMS = "<<params<<LogEndl;
    for( i = 0; i < params; ++i) {
        char name[64];
        int maxLength = 64;
        int actualLength = 0;
        int size = 0;
        GLenum type;
        glGetActiveUniform( sp, i, maxLength, &actualLength, &size, &type, name);
        if( size > 1) {
            int j;
            for( j = 0; j < size; ++j) {
                ostringstream ss;
                int location;
                ss<<name<<"["<<j<<"]";
                string longName = ss.str();
                location = glGetUniformLocation( sp, longName.c_str() );
                LogMsg<<i<<"i) type: "<<_glTypeToString( type )<<", name: "<<longName<<", location: "<<location<<LogEndl;
            }
        } else {
            int location = glGetUniformLocation( sp, name);
            LogMsg<<i<<") type: "<<_glTypeToString( type )<<", name: "<<name<<", location: "<<location<<LogEndl;
        }
    }

    _printSPInfoLog( sp );
}

GLint _createShader( const char* t_shaderFile, GLenum t_shaderType ) {
    string shaderContent;
    if( !_readFileToStr( t_shaderFile, shaderContent ) ) return -1;

    const GLchar* p = 0;

    GLuint shaderIndex = glCreateShader( t_shaderType );
    p = (const GLchar*)shaderContent.c_str();
    glShaderSource( shaderIndex, 1, &p, NULL );
    glCompileShader( shaderIndex );
    if( !_checkShaderCompileStatus( shaderIndex ) )  return -1;

    return shaderIndex;

}

GLint _createShaderProgram( const GLint& t_vs, const GLint& t_fs ) {
    GLuint sp = glCreateProgram();
    glAttachShader( sp, t_vs );
    glAttachShader( sp, t_fs );
    glLinkProgram ( sp );
    if( !_checkSPLinkingStatus( sp ) ) return -1;
    assert( _validateSP( sp ) );

    // delete shaders here to free memory
    // https://www.opengl.org/discussion_boards/showthread.php/182721-Does-glDeleteProgram-free-shaders
    glDetachShader( sp, t_vs );
    glDetachShader( sp, t_fs ); 
    glDeleteShader( t_vs );
    glDeleteShader( t_fs );

    return sp;
}

// bind shaders by vertex shader file and fragment shader fie
GLuint _createShaderProgramByShaderFiles( const char* t_vsFile, const char* t_fsFile ) {
    GLint vs = _createShader( t_vsFile, GL_VERTEX_SHADER );
    GLint fs = _createShader( t_fsFile, GL_FRAGMENT_SHADER );
    assert(  vs >= 0 );
    assert(  fs >= 0 );
   
    GLint sp = _createShaderProgram( vs, fs );
    assert( sp >= 0 );

    return (GLuint)sp;
}


// Shader / Shader Program Error Checking
///////////////////////////////////////////////



int main()
{
    glfwSetErrorCallback( _glfwErrorCallback );

    if( !glfwInit() ) {
        LogError<< "could not start GLFW" << LogEndl;
        return 1;
    }

#ifdef __APPLE__
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 2 );
#else
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 0 );
    
#endif
    // disable all of the functionality from previous versions of OpenGL that has been marked as removal in the future
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    // mark functions deprecation
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );

    //glfwWindowHint( GLFW_SAMPLES, 4 );

    GLFWwindow* window = glfwCreateWindow( gWinWidth, gWinHeight, "Hello Triangle", NULL, NULL );
    if( !window ) {
        LogError<< "could not open GLFW window" <<LogEndl;
        glfwTerminate();
        return 1;
    }

    // glfwSetWindowSizeCallback( window, _glfwWindowSizeCallback );
    glfwMakeContextCurrent( window );
    // this should come before anything gl does
    // we should have some kind of context first

    // GLEW obtains information on the supported extensions from the graphics driver. 
    // Experimental or pre-release drivers, however, might not report every available extension through the standard mechanism, 
    // in which case GLEW will report it unsupported. To circumvent this situation, 
    // the glewExperimental global switch can be turned on by setting it to GL_TRUE before calling glewInit(), 
    // which ensures that all extensions with valid entry points will be exposed.
    glewExperimental = GL_TRUE;
    glewInit();

    // log version info
    const GLubyte* renderer = glGetString( GL_RENDERER );
    const GLubyte* version = glGetString( GL_VERSION );
    LogMsg<< "Renderer: " << renderer<<LogEndl;
    LogMsg<< "OpenGL version supported "<< version<<LogEndl<<LogEndl;

    // log monitors info
    int count;
    GLFWmonitor** monitors = glfwGetMonitors( &count );
    LogMsg<<"Monitors connected:"<<LogEndl;
    for( int i = 0; i < count; ++i ) {
        GLFWmonitor* mon = monitors[ i ];
        LogMsg<<i<<": "<<glfwGetMonitorName( mon );
        const GLFWvidmode* vmode = glfwGetVideoMode( mon );
        LogMsg<<", width: "<<vmode->width<<", height: "<<vmode->height<<"."<<LogEndl;
    }
    LogMsg<<LogEndl;

    // log context info
    _logGLParams();


    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LESS );


    // geos
    GLfloat points[] = {
        0.0f, 0.5f, 0.0f, 
        0.5f, -0.5f, 0.0f, 
        -0.5f, -0.5f, 0.0f
    };

    GLuint vbo = 0;
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, sizeof( points ), points, GL_STATIC_DRAW );

    GLuint vao = 0; 
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glEnableVertexAttribArray( 0 );
    // attribute is the older name for vertex shader 'in' variables
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );


    GLuint sp = _createShaderProgramByShaderFiles( VS_FILE, FS_FILE );
    glUseProgram( sp );

    GLint colorLoc = glGetUniformLocation( (GLuint)sp, "inputColor" );
    assert( colorLoc >= 0 );
    vec4 vertexColor( 1.0f, 0.0f, 0.0f, 1.0f );


    // cull back face
    glEnable( GL_CULL_FACE ); 
    glCullFace( GL_BACK );
    glFrontFace( GL_CW );

    // _printSPInfo( sp );

    // update and draw!
    while ( !glfwWindowShouldClose( window ) ) {
        _updateFPSCounter( window );

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        // SUCK APPLE
        // glViewport( 0, 0, gWinWidth, gWinHeight );
        glUseProgram( sp );
        glUniform4fv( colorLoc, 1, glm::value_ptr( vertexColor ) );
        glBindVertexArray( vao );

        glDrawArrays( GL_TRIANGLES, 0, 3 );
// 
//         // split screen
//         glViewport( 0, 0, winWidth * 0.5f, winHeight * 0.5f );
//         glUseProgram( shader_programme );
//         glBindVertexArray( vao );
//         glDrawArrays( GL_TRIANGLES, 0, 3 );


        glfwSwapBuffers( window );

        // events
        glfwPollEvents();
        if( GLFW_PRESS == glfwGetKey( window, GLFW_KEY_ESCAPE ) ) { 
            glfwSetWindowShouldClose( window, 1 ); 
        }

    }


    glfwTerminate();

    return 1;
}