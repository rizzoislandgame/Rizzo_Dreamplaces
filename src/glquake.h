/*
Copyright (C) 1996-1997 Id Software, Inc.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/

#ifndef GLQUAKE_H
#define GLQUAKE_H

// disable data conversion warnings

#ifdef _MSC_VER
#pragma warning(disable : 4244)     // LordHavoc: MSVC++ 4 x86, double/float
#pragma warning(disable : 4305)		// LordHavoc: MSVC++ 6 x86, double/float
#endif


//====================================================

// wgl uses APIENTRY
#ifndef APIENTRY
#define APIENTRY
#endif

// for platforms (wgl) that do not use GLAPIENTRY
#ifndef GLAPIENTRY
#define GLAPIENTRY APIENTRY
#endif

#ifndef GL_PROJECTION
typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef void GLvoid;
// 1-byte signed
typedef signed char GLbyte;
// 2-byte signed
typedef short GLshort;
// 4-byte signed
typedef int GLint;
// 1-byte unsigned
typedef unsigned char GLubyte;
// 2-byte unsigned
typedef unsigned short GLushort;
// 4-byte unsigned
typedef unsigned int GLuint;
// 4-byte signed
typedef int GLsizei;
// single precision float
typedef float GLfloat;
// single precision float in [0,1]
typedef float GLclampf;
// double precision float
typedef double GLdouble;
// double precision float in [0,1]
typedef double GLclampd;
#ifndef NO_GLEXTENSIONS
// int whose size is the same as a pointer (?)
typedef size_t GLintptrARB;
// int whose size is the same as a pointer (?)
typedef size_t GLsizeiptrARB;
#endif

#ifndef DREAMCAST
#define GL_MODELVIEW				0x1700
#define GL_PROJECTION				0x1701
#define GL_TEXTURE				0x1702
#else
#define GL_KOS_NEARZ_CLIPPING   0x0400
#define GL_MODELVIEW		1
#define GL_PROJECTION		2
#define GL_TEXTURE		3
#endif
#define GL_MATRIX_MODE				0x0BA0
#define GL_MODELVIEW_MATRIX			0x0BA6
#define GL_PROJECTION_MATRIX			0x0BA7
#define GL_TEXTURE_MATRIX			0x0BA8

#ifndef DREAMCAST
#define GL_DEPTH_TEST				0x0B71
#else
#define GL_DEPTH_TEST		0
#endif

#define GL_CULL_FACE				0x0B44

#define GL_BLEND				0x0BE2
#define GL_ALPHA_TEST			0x0BC0

#define GL_ZERO					0x0
#define GL_ONE					0x1
#define GL_SRC_COLOR				0x0300
#define GL_ONE_MINUS_SRC_COLOR			0x0301
#define GL_DST_COLOR				0x0306
#define GL_ONE_MINUS_DST_COLOR			0x0307
#define GL_SRC_ALPHA				0x0302
#define GL_ONE_MINUS_SRC_ALPHA			0x0303
#define GL_DST_ALPHA				0x0304
#define GL_ONE_MINUS_DST_ALPHA			0x0305
#define GL_SRC_ALPHA_SATURATE			0x0308
#define GL_CONSTANT_COLOR			0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR		0x8002
#define GL_CONSTANT_ALPHA			0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA		0x8004

#define GL_TEXTURE_ENV				0x2300
#define GL_TEXTURE_ENV_MODE			0x2200
#ifndef DREAMCAST
#define GL_TEXTURE_1D				0x0DE0
#define GL_TEXTURE_2D				0x0DE1
#else
#define GL_TEXTURE_1D		0x0001
#define GL_TEXTURE_2D		0x0001
#endif
#define GL_TEXTURE_WRAP_S			0x2802
#define GL_TEXTURE_WRAP_T			0x2803
#define GL_TEXTURE_WRAP_R			0x8072
#define GL_TEXTURE_BORDER_COLOR			0x1004
#define GL_TEXTURE_MAG_FILTER			0x2800
#define GL_TEXTURE_MIN_FILTER			0x2801
#define GL_PACK_ALIGNMENT			0x0D05
#define GL_UNPACK_ALIGNMENT			0x0CF5
#define GL_TEXTURE_BINDING_1D                   0x8068
#define GL_TEXTURE_BINDING_2D                   0x8069

#define GL_NEAREST				0x2600
#define GL_LINEAR				0x2601
#define GL_NEAREST_MIPMAP_NEAREST		0x2700
#define GL_NEAREST_MIPMAP_LINEAR		0x2702
#define GL_LINEAR_MIPMAP_NEAREST		0x2701
#define GL_LINEAR_MIPMAP_LINEAR			0x2703

#define GL_LINE					0x1B01
#define GL_FILL					0x1B02

#ifndef NO_GLEXTENSIONS
extern int gl_support_anisotropy;
extern int gl_max_anisotropy;
#endif
#define GL_TEXTURE_MAX_ANISOTROPY_EXT		0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT	0x84FF

#define GL_ADD					0x0104
#ifndef DREAMCAST
#define GL_DECAL				0x2101
#define GL_MODULATE				0x2100
#else
#define GL_DECAL		2
#define GL_MODULATE		1
#endif

#define GL_REPEAT				0x2901
#define GL_CLAMP				0x2900

#ifndef DREAMCAST
#define GL_POINTS				0x0000
#define GL_LINES				0x0001
#define GL_LINE_LOOP			0x0002
#define GL_LINE_STRIP			0x0003
#define GL_TRIANGLES			0x0004
#define GL_TRIANGLE_STRIP		0x0005
#define GL_TRIANGLE_FAN			0x0006
#define GL_QUADS				0x0007
#define GL_QUAD_STRIP			0x0008
#define GL_POLYGON				0x0009
#else
#define GL_POINTS               1
#define GL_LINES                2 
#define GL_LINE_LOOP            3
#define GL_LINE_STRIP           4
#define GL_TRIANGLES            5
#define GL_TRIANGLE_STRIP       6
#define GL_TRIANGLE_FAN         7
#define GL_QUADS                8
#define GL_QUAD_STRIP           9
#define GL_POLYGON              10
#endif

#define GL_FALSE				0x0
#define GL_TRUE					0x1

#define GL_BYTE					0x1400
#define GL_UNSIGNED_BYTE			0x1401
#define GL_SHORT				0x1402
#define GL_UNSIGNED_SHORT			0x1403
#define GL_INT					0x1404
#define GL_UNSIGNED_INT				0x1405
#define GL_FLOAT				0x1406
#define GL_DOUBLE				0x140A
#define GL_2_BYTES				0x1407
#define GL_3_BYTES				0x1408
#define GL_4_BYTES				0x1409

#define GL_VERTEX_ARRAY				0x8074
#define GL_NORMAL_ARRAY				0x8075
#define GL_COLOR_ARRAY				0x8076
//#define GL_INDEX_ARRAY				0x8077
#define GL_TEXTURE_COORD_ARRAY			0x8078
//#define GL_EDGE_FLAG_ARRAY			0x8079

#define GL_NONE					0
#define GL_FRONT_LEFT			0x0400
#define GL_FRONT_RIGHT			0x0401
#define GL_BACK_LEFT			0x0402
#define GL_BACK_RIGHT			0x0403
#define GL_FRONT				0x0404
#define GL_BACK					0x0405
#define GL_LEFT					0x0406
#define GL_RIGHT				0x0407
#define GL_FRONT_AND_BACK		0x0408
#define GL_AUX0					0x0409
#define GL_AUX1					0x040A
#define GL_AUX2					0x040B
#define GL_AUX3					0x040C

#define GL_VENDOR				0x1F00
#define GL_RENDERER				0x1F01
#define GL_VERSION				0x1F02
#define GL_EXTENSIONS				0x1F03

#define GL_NO_ERROR 				0x0
#define GL_INVALID_VALUE			0x0501
#define GL_INVALID_ENUM				0x0500
#define GL_INVALID_OPERATION			0x0502
#define GL_STACK_OVERFLOW			0x0503
#define GL_STACK_UNDERFLOW			0x0504
#define GL_OUT_OF_MEMORY			0x0505

#define GL_DITHER				0x0BD0
#define GL_RGB					0x1907
#define GL_RGBA					0x1908

#define GL_MAX_TEXTURE_SIZE			0x0D33

#define GL_NEVER				0x0200
#define GL_LESS					0x0201
#define GL_EQUAL				0x0202
#define GL_LEQUAL				0x0203
#define GL_GREATER				0x0204
#define GL_NOTEQUAL				0x0205
#define GL_GEQUAL				0x0206
#define GL_ALWAYS				0x0207
#ifndef DREAMCAST
#define GL_DEPTH_TEST				0x0B71
#else
#define GL_DEPTH_TEST			0
#endif

#define GL_RED_SCALE				0x0D14
#define GL_GREEN_SCALE				0x0D18
#define GL_BLUE_SCALE				0x0D1A
#define GL_ALPHA_SCALE				0x0D1C

#ifndef DREAMCAST
#define GL_DEPTH_BUFFER_BIT			0x00000100
#define GL_COLOR_BUFFER_BIT			0x00004000
#else
#define GL_DEPTH_BUFFER_BIT		0
#define GL_COLOR_BUFFER_BIT		0
#endif
#define GL_ACCUM_BUFFER_BIT			0x00000200
#define GL_STENCIL_BUFFER_BIT			0x00000400

#define GL_STENCIL_TEST				0x0B90
#define GL_KEEP					0x1E00
#ifndef DREAMCAST
#define GL_REPLACE				0x1E01
#else
#define GL_REPLACE			0
#endif
#define GL_INCR					0x1E02
#define GL_DECR					0x1E03

#define GL_POLYGON_OFFSET_FACTOR          0x8038
#define GL_POLYGON_OFFSET_UNITS           0x2A00
#define GL_POLYGON_OFFSET_POINT           0x2A01
#define GL_POLYGON_OFFSET_LINE            0x2A02
#define GL_POLYGON_OFFSET_FILL            0x8037

#define GL_POINT_SMOOTH                         0x0B10
#define GL_LINE_SMOOTH                          0x0B20
#define GL_POLYGON_SMOOTH                       0x0B41

#define GL_POLYGON_STIPPLE                0x0B42

#endif

extern int gl_max_texture_size;

// GL_ARB_multitexture

#ifndef NO_GLEXTENSIONS
extern int gl_textureunits;
#ifndef FIXED_OPENGL
extern void (GLAPIENTRY *qglMultiTexCoord1f) (GLenum, GLfloat);
extern void (GLAPIENTRY *qglMultiTexCoord2f) (GLenum, GLfloat, GLfloat);
extern void (GLAPIENTRY *qglMultiTexCoord3f) (GLenum, GLfloat, GLfloat, GLfloat);
extern void (GLAPIENTRY *qglMultiTexCoord4f) (GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
extern void (GLAPIENTRY *qglActiveTexture) (GLenum);
extern void (GLAPIENTRY *qglClientActiveTexture) (GLenum);
#else
#ifdef __cplusplus
extern "C" {
#endif
void glMultiTexCoord1f(GLenum, GLfloat);
#define qglMultiTexCoord1f(A,B) glMultiTexCoord1f(A,B)
void glMultiTexCoord2f(GLenum, GLfloat, GLfloat);
#define qglMultiTexCoord2f(A,B,C) glMultiTexCoord2f(A,B,C)
void glMultiTexCoord3f(GLenum, GLfloat, GLfloat, GLfloat);
#define qglMultiTexCoord3f(A,B,C,D) glMultiTexCoord3f(A,B,C,D)
void glMultiTexCoord4f(GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
#define qglMultiTexCoord4f(A,B,C,D,E) glMultiTexCoord4f(A,B,C,D,E)
void glActiveTexture(GLenum);
#define qglActiveTexture(A) glActiveTexture(A)
void glClientActiveTexture(GLenum);
#define qglClientActiveTexture(A) glClientActiveTexture(A)
#ifdef __cplusplus
}
#endif
#endif
#endif
#ifndef GL_ACTIVE_TEXTURE_ARB
#define GL_ACTIVE_TEXTURE_ARB			0x84E0
#define GL_CLIENT_ACTIVE_TEXTURE_ARB	0x84E1
#define GL_MAX_TEXTURE_UNITS_ARB		0x84E2
#define GL_TEXTURE0_ARB					0x84C0
#define GL_TEXTURE1_ARB					0x84C1
#define GL_TEXTURE2_ARB					0x84C2
#define GL_TEXTURE3_ARB					0x84C3
#define GL_TEXTURE4_ARB					0x84C4
#define GL_TEXTURE5_ARB					0x84C5
#define GL_TEXTURE6_ARB					0x84C6
#define GL_TEXTURE7_ARB					0x84C7
#define GL_TEXTURE8_ARB					0x84C8
#define GL_TEXTURE9_ARB					0x84C9
#define GL_TEXTURE10_ARB				0x84CA
#define GL_TEXTURE11_ARB				0x84CB
#define GL_TEXTURE12_ARB				0x84CC
#define GL_TEXTURE13_ARB				0x84CD
#define GL_TEXTURE14_ARB				0x84CE
#define GL_TEXTURE15_ARB				0x84CF
#define GL_TEXTURE16_ARB				0x84D0
#define GL_TEXTURE17_ARB				0x84D1
#define GL_TEXTURE18_ARB				0x84D2
#define GL_TEXTURE19_ARB				0x84D3
#define GL_TEXTURE20_ARB				0x84D4
#define GL_TEXTURE21_ARB				0x84D5
#define GL_TEXTURE22_ARB				0x84D6
#define GL_TEXTURE23_ARB				0x84D7
#define GL_TEXTURE24_ARB				0x84D8
#define GL_TEXTURE25_ARB				0x84D9
#define GL_TEXTURE26_ARB				0x84DA
#define GL_TEXTURE27_ARB				0x84DB
#define GL_TEXTURE28_ARB				0x84DC
#define GL_TEXTURE29_ARB				0x84DD
#define GL_TEXTURE30_ARB				0x84DE
#define GL_TEXTURE31_ARB				0x84DF
#endif

// GL_EXT_compiled_vertex_array
#ifndef NO_GLEXTENSIONS
extern int gl_supportslockarrays;
#endif
#ifndef FIXED_OPENGL
extern void (GLAPIENTRY *qglLockArraysEXT) (GLint first, GLint count);
extern void (GLAPIENTRY *qglUnlockArraysEXT) (void);
#else
#ifdef __cplusplus
extern "C" {
#endif
void glLockArraysEXT(GLint first, GLint count);
#define qglLockArraysEXT(A,B) glLockArraysEXT(A,B)
void glUnlockArraysEXT(void);
#define qglUnlockArraysEXT() glUnlockArraysEXT()
#ifdef __cplusplus
}
#endif
#endif

// GL_ARB_texture_env_combine
#ifndef NO_GLEXTENSIONS
extern int gl_combine_extension;
#endif
#ifndef GL_COMBINE_ARB
#define GL_COMBINE_ARB					0x8570
#define GL_COMBINE_RGB_ARB				0x8571
#define GL_COMBINE_ALPHA_ARB			0x8572
#define GL_SOURCE0_RGB_ARB				0x8580
#define GL_SOURCE1_RGB_ARB				0x8581
#define GL_SOURCE2_RGB_ARB				0x8582
#define GL_SOURCE0_ALPHA_ARB			0x8588
#define GL_SOURCE1_ALPHA_ARB			0x8589
#define GL_SOURCE2_ALPHA_ARB			0x858A
#define GL_OPERAND0_RGB_ARB				0x8590
#define GL_OPERAND1_RGB_ARB				0x8591
#define GL_OPERAND2_RGB_ARB				0x8592
#define GL_OPERAND0_ALPHA_ARB			0x8598
#define GL_OPERAND1_ALPHA_ARB			0x8599
#define GL_OPERAND2_ALPHA_ARB			0x859A
#define GL_RGB_SCALE_ARB				0x8573
#define GL_ADD_SIGNED_ARB				0x8574
#define GL_INTERPOLATE_ARB				0x8575
#define GL_SUBTRACT_ARB					0x84E7
#define GL_CONSTANT_ARB					0x8576
#define GL_PRIMARY_COLOR_ARB			0x8577
#define GL_PREVIOUS_ARB					0x8578
#endif

#ifndef GL_MAX_ELEMENTS_VERTICES
#define GL_MAX_ELEMENTS_VERTICES		0x80E8
#endif
#ifndef GL_MAX_ELEMENTS_INDICES
#define GL_MAX_ELEMENTS_INDICES			0x80E9
#endif


#ifndef NO_GLEXTENSIONS
extern int gl_texture3d;
extern int gl_max_3d_texture_size;
#endif
#ifndef GL_TEXTURE_3D
#define GL_PACK_SKIP_IMAGES			0x806B
#define GL_PACK_IMAGE_HEIGHT			0x806C
#define GL_UNPACK_SKIP_IMAGES			0x806D
#define GL_UNPACK_IMAGE_HEIGHT			0x806E
#define GL_TEXTURE_3D				0x806F
#define GL_PROXY_TEXTURE_3D			0x8070
#define GL_TEXTURE_DEPTH			0x8071
#define GL_TEXTURE_WRAP_R			0x8072
#define GL_MAX_3D_TEXTURE_SIZE			0x8073
#define GL_TEXTURE_BINDING_3D			0x806A
#ifndef FIXED_OPENGL
extern void (GLAPIENTRY *qglTexImage3D)(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void (GLAPIENTRY *qglTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
extern void (GLAPIENTRY *qglCopyTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
#else
#ifdef __cplusplus
extern "C" {
#endif
void glTexImage3D(GLenum target, GLint level, GLenum internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
#define qglTexImage3D(A,B,C,D,E,F,G,H,I,J) glTexImage3D(A,B,C,D,E,F,G,H,I,J)
void glTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const GLvoid *pixels);
#define qglTexSubImage3D(A,B,C,D,E,F,G,H,I,J,K) glTexSubImage3D(A,B,C,D,E,F,G,H,I,J,K)
void glCopyTexSubImage3D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLint x, GLint y, GLsizei width, GLsizei height);
#define qglCopyTexSubImage3D(A,B,C,D,E,F,G,H,I) glCopyTexSubImage3D(A,B,C,D,E,F,G,H,I)
#ifdef __cplusplus
}
#endif
#endif
#endif

#ifndef NO_GLEXTENSIONS
extern int gl_texturecubemap;
extern int gl_max_cube_map_texture_size;
#endif
#ifndef GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB
#define GL_NORMAL_MAP_ARB			    0x8511
#define GL_REFLECTION_MAP_ARB		    0x8512
#define GL_TEXTURE_CUBE_MAP_ARB		    0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP_ARB	    0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB     0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB     0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB     0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB     0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB     0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB     0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARB	    0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE_ARB		0x851C
#endif

#ifndef NO_GLEXTENSIONS
extern int gl_dot3arb;
#endif
#ifndef GL_DOT3_RGB_ARB
#define GL_DOT3_RGB_ARB				0x86AE
#define GL_DOT3_RGBA_ARB				0x86AF
#endif

/*
// uncomment if this is actually needed by anyone
extern int gl_dot3ext;
#ifndef GL_DOT3_RGB_EXT
#define GL_DOT3_RGB_EXT				0x8740
#define GL_DOT3_RGBA_EXT			0x8741
#endif
*/

#ifndef GL_SCISSOR_TEST
#ifndef DREAMCAST
#define GL_SCISSOR_TEST				0x0C11
#else
#define GL_SCISSOR_TEST				0x0008
#endif
#define GL_SCISSOR_BOX				0x0C10
#endif

// GL_SGIS_texture_edge_clamp or GL_EXT_texture_edge_clamp
#ifndef NO_GLEXTENSIONS
extern int gl_support_clamptoedge;
#endif
#ifndef GL_CLAMP_TO_EDGE
#define GL_CLAMP_TO_EDGE 0x812F
#endif

//GL_ATI_separate_stencil
#ifndef NO_GLEXTENSIONS
extern int gl_support_separatestencil;
#endif
#ifndef GL_STENCIL_BACK_FUNC
#define GL_STENCIL_BACK_FUNC              0x8800
#define GL_STENCIL_BACK_FAIL              0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#endif
#ifndef FIXED_OPENGL
extern void (GLAPIENTRY *qglStencilOpSeparate)(GLenum, GLenum, GLenum, GLenum);
extern void (GLAPIENTRY *qglStencilFuncSeparate)(GLenum, GLenum, GLint, GLuint);
#else
#ifdef __cplusplus
extern "C" {
#endif
void glStencilOpSeparate(GLenum, GLenum, GLenum, GLenum);
#define qglStencilOpSeparate(A,B,C,D) glStencilOpSeparate(A,B,C,D)
void glStencilFuncSeparate(GLenum, GLenum, GLint, GLuint);
#define qglStencilFuncSeparate(A,B,C,D) glStencilFuncSeparate(A,B,C,D)
#ifdef __cplusplus
}
#endif
#endif

//GL_EXT_stencil_two_side
#define GL_STENCIL_TEST_TWO_SIDE_EXT      0x8910
#define GL_ACTIVE_STENCIL_FACE_EXT        0x8911
#ifndef FIXED_OPENGL
extern void (GLAPIENTRY *qglActiveStencilFaceEXT)(GLenum);
#else
#ifdef __cplusplus
extern "C" {
#endif
void glActiveStencilFaceEXT(GLenum);
#define qglActiveStencilFaceEXT(A) glActiveStencilFaceEXT(A)
#ifdef __cplusplus
}
#endif
#endif
#ifndef NO_GLEXTENSIONS
extern int gl_support_stenciltwoside;
#endif
extern int gl_support_stenciltwoside;
extern int gl_support_stenciltwoside;

//GL_EXT_blend_minmax
#ifndef NO_GLEXTENSIONS
extern int gl_support_ext_blend_minmax;
#endif
#ifndef GL_FUNC_ADD_EXT
#define GL_FUNC_ADD_EXT                   0x8006 // also supplied by GL_EXT_blend_subtract
#define GL_MIN_EXT                        0x8007
#define GL_MAX_EXT                        0x8008
#define GL_BLEND_EQUATION_EXT             0x8009 // also supplied by GL_EXT_blend_subtract
#ifndef FIXED_OPENGL
extern void (GLAPIENTRY *qglBlendEquationEXT)(GLenum); // also supplied by GL_EXT_blend_subtract
#else
#ifdef __cplusplus
extern "C" {
#endif
void glBlendEquationEXT(GLenum);
#define qglBlendEquationEXT(A) glBlendEquationEXT(A)
#ifdef __cplusplus
}
#endif
#endif
#endif

//GL_EXT_blend_subtract
#ifndef NO_GLEXTENSIONS
extern int gl_support_ext_blend_subtract;
#endif
#ifndef GL_FUNC_SUBTRACT_EXT
#define GL_FUNC_SUBTRACT_EXT              0x800A
#define GL_FUNC_REVERSE_SUBTRACT_EXT      0x800B
#endif

//GL_ARB_texture_non_power_of_two
#ifndef NO_GLEXTENSIONS
extern int gl_support_arb_texture_non_power_of_two;
#endif

//GL_ARB_vertex_buffer_object
#ifndef NO_GLEXTENSIONS
extern int gl_support_arb_vertex_buffer_object;
#endif
#ifndef GL_ARRAY_BUFFER_ARB
#define GL_ARRAY_BUFFER_ARB               0x8892
#define GL_ELEMENT_ARRAY_BUFFER_ARB       0x8893
#define GL_ARRAY_BUFFER_BINDING_ARB       0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING_ARB 0x8895
#define GL_VERTEX_ARRAY_BUFFER_BINDING_ARB 0x8896
#define GL_NORMAL_ARRAY_BUFFER_BINDING_ARB 0x8897
#define GL_COLOR_ARRAY_BUFFER_BINDING_ARB 0x8898
#define GL_INDEX_ARRAY_BUFFER_BINDING_ARB 0x8899
#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING_ARB 0x889A
#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING_ARB 0x889B
#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING_ARB 0x889C
#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING_ARB 0x889D
#define GL_WEIGHT_ARRAY_BUFFER_BINDING_ARB 0x889E
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING_ARB 0x889F
#define GL_STREAM_DRAW_ARB                0x88E0
#define GL_STREAM_READ_ARB                0x88E1
#define GL_STREAM_COPY_ARB                0x88E2
#define GL_STATIC_DRAW_ARB                0x88E4
#define GL_STATIC_READ_ARB                0x88E5
#define GL_STATIC_COPY_ARB                0x88E6
#define GL_DYNAMIC_DRAW_ARB               0x88E8
#define GL_DYNAMIC_READ_ARB               0x88E9
#define GL_DYNAMIC_COPY_ARB               0x88EA
#define GL_READ_ONLY_ARB                  0x88B8
#define GL_WRITE_ONLY_ARB                 0x88B9
#define GL_READ_WRITE_ARB                 0x88BA
#define GL_BUFFER_SIZE_ARB                0x8764
#define GL_BUFFER_USAGE_ARB               0x8765
#define GL_BUFFER_ACCESS_ARB              0x88BB
#define GL_BUFFER_MAPPED_ARB              0x88BC
#define GL_BUFFER_MAP_POINTER_ARB         0x88BD
#endif
#ifndef NO_GLEXTENSIONS
#ifndef FIXED_OPENGL
extern void (GLAPIENTRY *qglBindBufferARB) (GLenum target, GLuint buffer);
extern void (GLAPIENTRY *qglDeleteBuffersARB) (GLsizei n, const GLuint *buffers);
extern void (GLAPIENTRY *qglGenBuffersARB) (GLsizei n, GLuint *buffers);
extern GLboolean (GLAPIENTRY *qglIsBufferARB) (GLuint buffer);
extern GLvoid* (GLAPIENTRY *qglMapBufferARB) (GLenum target, GLenum access);
extern GLboolean (GLAPIENTRY *qglUnmapBufferARB) (GLenum target);
extern void (GLAPIENTRY *qglBufferDataARB) (GLenum target, GLsizeiptrARB size, const GLvoid *data, GLenum usage);
extern void (GLAPIENTRY *qglBufferSubDataARB) (GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
#else
#ifdef __cplusplus
extern "C" {
#endif
void glBindBufferARB(GLenum target, GLuint buffer);
#define qglBindBufferARB(A,B) glBindBufferARB(A,B)
void glDeleteBuffersARB(GLsizei n, const GLuint *buffers);
#define qglDeleteBuffersARB(A,B) glDeleteBuffersARB(A,B)
void glGenBuffersARB(GLsizei n, GLuint *buffers);
#define qglGenBuffersARB(A,B) glGenBuffersARB(A,B)
GLboolean glIsBufferARB(GLuint buffer);
#define qglIsBufferARB(A) glIsBufferARB(A)
GLvoid* glMapBufferARB(GLenum target, GLenum access);
#define qglMapBufferARB(A,B) glMapBufferARB(A,B)
GLboolean glUnmapBufferARB(GLenum target);
#define qglUnmapBufferARB(A) glUnmapBufferARB(A)
void glBufferDataARB(GLenum target, GLsizeiptrARB size,const GLvoid *data, GLenum usage);
#define qglBufferDataARB(A,B,C,D) glBufferDataARB(A,B,C,D)
void glBufferSubDataARB(GLenum target, GLintptrARB offset, GLsizeiptrARB size, const GLvoid *data);
#define qglBufferSubDataARB(A,B,C,D) glBufferSubDataARB(A,B,C,D)
#ifdef __cplusplus
}
#endif
#endif
#endif

#ifndef FIXED_OPENGL
extern void (GLAPIENTRY *qglScissor)(GLint x, GLint y, GLsizei width, GLsizei height);

extern void (GLAPIENTRY *qglClearColor)(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);

extern void (GLAPIENTRY *qglClear)(GLbitfield mask);

extern void (GLAPIENTRY *qglAlphaFunc)(GLenum func, GLclampf ref);
extern void (GLAPIENTRY *qglBlendFunc)(GLenum sfactor, GLenum dfactor);
extern void (GLAPIENTRY *qglCullFace)(GLenum mode);

extern void (GLAPIENTRY *qglDrawBuffer)(GLenum mode);
extern void (GLAPIENTRY *qglReadBuffer)(GLenum mode);
extern void (GLAPIENTRY *qglEnable)(GLenum cap);
extern void (GLAPIENTRY *qglDisable)(GLenum cap);
extern GLboolean (GLAPIENTRY *qglIsEnabled)(GLenum cap);

extern void (GLAPIENTRY *qglEnableClientState)(GLenum cap);
extern void (GLAPIENTRY *qglDisableClientState)(GLenum cap);

extern void (GLAPIENTRY *qglGetBooleanv)(GLenum pname, GLboolean *params);
extern void (GLAPIENTRY *qglGetDoublev)(GLenum pname, GLdouble *params);
extern void (GLAPIENTRY *qglGetFloatv)(GLenum pname, GLfloat *params);
extern void (GLAPIENTRY *qglGetIntegerv)(GLenum pname, GLint *params);

extern GLenum (GLAPIENTRY *qglGetError)(void);
extern const GLubyte* (GLAPIENTRY *qglGetString)(GLenum name);
extern void (GLAPIENTRY *qglFinish)(void);
extern void (GLAPIENTRY *qglFlush)(void);

extern void (GLAPIENTRY *qglClearDepth)(GLclampd depth);
extern void (GLAPIENTRY *qglDepthFunc)(GLenum func);
extern void (GLAPIENTRY *qglDepthMask)(GLboolean flag);
extern void (GLAPIENTRY *qglDepthRange)(GLclampd near_val, GLclampd far_val);
extern void (GLAPIENTRY *qglColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
#else
#ifdef __cplusplus
extern "C" {
#endif
void glScissor(GLint x, GLint y, GLsizei width, GLsizei height);
#define qglScissor(A,B,C,D) glScissor(A,B,C,D)
void glClearColor(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
#define qglClearColor(A,B,C,D) glClearColor(A,B,C,D)
void glClear(GLbitfield mask);
#define qglClear(A) glClear(A)
void glAlphaFunc(GLenum func, GLclampf ref);
#define qglAlphaFunc(A,B) glAlphaFunc(A,B)
void glBlendFunc(GLenum sfactor, GLenum dfactor);
#define qglBlendFunc(A,B) glBlendFunc(A,B)
void glCullFace(GLenum mode);
#define qglCullFace(A) glCullFace(A)
void glDrawBuffer(GLenum mode);
#define qglDrawBuffer(A) glDrawBuffer(A)
void glReadBuffer(GLenum mode);
#define qglReadBuffer(A) glReadBuffer(A)
void glEnable(GLenum cap);
#define qglEnable(A) glEnable(A)
void glDisable(GLenum cap);
#define qglDisable(A) glDisable(A)
GLboolean glIsEnabled(GLenum cap);
#define qglIsEnabled(A) glIsEnabled(A)
void glEnableClientState(GLenum cap);
#define qglEnableClientState(A) glEnableClientState(A)
void glDisableClientState(GLenum cap);
#define qglDisableClientState(A) glDisableClientState(A)
void glGetBooleanv(GLenum pname, GLboolean *params);
#define qglGetBooleanv(A,B) glGetBooleanv(A,B)
void glGetDoublev(GLenum pname, GLdouble *params);
#define qglGetDoublev(A,B) glGetDoublev(A,B)
void glGetFloatv(GLenum pname, GLfloat *params);
#define qglGetFloatv(A,B) glGetFloatv(A,B)
void glGetIntegerv(GLenum pname, GLint *params);
#define qglGetIntegerv(A,B) glGetIntegerv(A,B)
GLenum glGetError(void);
#define qglGetError() glGetError()
const GLubyte* glGetString(GLenum name);
#define qglGetString(A) glGetString(A)
void glFinish(void);
#define qglFinish() glFinish()
void glFlush(void);
#define qglFlush() glFlush()
void glClearDepth(GLclampd depth);
#define qglClearDepth(A) glClearDepth(A)
void glDepthFunc(GLenum func);
#define qglDepthFunc(A) glDepthFunc(A)
void glDepthMask(GLboolean flag);
#define qglDepthMask(A) glDepthMask(A)
void glDepthRange(GLclampd near_val, GLclampd far_val);
#define qglDepthRange(A,B) glDepthRange(A,B)
void glColorMask(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
#define qglColorMask(A,B,C,D) glColorMask(A,B,C,D)
#ifdef __cplusplus
}
#endif
#endif

#ifndef FIXED_OPENGL
#ifndef NO_GLEXTENSIONS
extern void (GLAPIENTRY *qglDrawRangeElements)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
#endif
extern void (GLAPIENTRY *qglDrawElements)(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
extern void (GLAPIENTRY *qglVertexPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
extern void (GLAPIENTRY *qglNormalPointer)(GLenum type, GLsizei stride, const GLvoid *ptr);
extern void (GLAPIENTRY *qglColorPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
extern void (GLAPIENTRY *qglTexCoordPointer)(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
extern void (GLAPIENTRY *qglArrayElement)(GLint i);

extern void (GLAPIENTRY *qglColor4f)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
extern void (GLAPIENTRY *qglTexCoord1f)(GLfloat s);
extern void (GLAPIENTRY *qglTexCoord2f)(GLfloat s, GLfloat t);
extern void (GLAPIENTRY *qglTexCoord3f)(GLfloat s, GLfloat t, GLfloat r);
extern void (GLAPIENTRY *qglTexCoord4f)(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
extern void (GLAPIENTRY *qglVertex2f)(GLfloat x, GLfloat y);
extern void (GLAPIENTRY *qglVertex3f)(GLfloat x, GLfloat y, GLfloat z);
extern void (GLAPIENTRY *qglBegin)(GLenum mode);
extern void (GLAPIENTRY *qglEnd)(void);

extern void (GLAPIENTRY *qglMatrixMode)(GLenum mode);
extern void (GLAPIENTRY *qglOrtho)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);
extern void (GLAPIENTRY *qglFrustum)(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);
extern void (GLAPIENTRY *qglViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
extern void (GLAPIENTRY *qglPushMatrix)(void);
extern void (GLAPIENTRY *qglPopMatrix)(void);
extern void (GLAPIENTRY *qglLoadIdentity)(void);
extern void (GLAPIENTRY *qglLoadMatrixd)(const GLdouble *m);
extern void (GLAPIENTRY *qglLoadMatrixf)(const GLfloat *m);
extern void (GLAPIENTRY *qglMultMatrixd)(const GLdouble *m);
extern void (GLAPIENTRY *qglMultMatrixf)(const GLfloat *m);
extern void (GLAPIENTRY *qglRotated)(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
extern void (GLAPIENTRY *qglRotatef)(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
extern void (GLAPIENTRY *qglScaled)(GLdouble x, GLdouble y, GLdouble z);
extern void (GLAPIENTRY *qglScalef)(GLfloat x, GLfloat y, GLfloat z);
extern void (GLAPIENTRY *qglTranslated)(GLdouble x, GLdouble y, GLdouble z);
extern void (GLAPIENTRY *qglTranslatef)(GLfloat x, GLfloat y, GLfloat z);

extern void (GLAPIENTRY *qglReadPixels)(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);

extern void (GLAPIENTRY *qglStencilFunc)(GLenum func, GLint ref, GLuint mask);
extern void (GLAPIENTRY *qglStencilMask)(GLuint mask);
extern void (GLAPIENTRY *qglStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
extern void (GLAPIENTRY *qglClearStencil)(GLint s);

extern void (GLAPIENTRY *qglTexEnvf)(GLenum target, GLenum pname, GLfloat param);
extern void (GLAPIENTRY *qglTexEnvfv)(GLenum target, GLenum pname, const GLfloat *params);
extern void (GLAPIENTRY *qglTexEnvi)(GLenum target, GLenum pname, GLint param);
extern void (GLAPIENTRY *qglTexParameterf)(GLenum target, GLenum pname, GLfloat param);
extern void (GLAPIENTRY *qglTexParameterfv)(GLenum target, GLenum pname, GLfloat *params);
extern void (GLAPIENTRY *qglTexParameteri)(GLenum target, GLenum pname, GLint param);

extern void (GLAPIENTRY *qglGenTextures)(GLsizei n, GLuint *textures);
extern void (GLAPIENTRY *qglDeleteTextures)(GLsizei n, const GLuint *textures);
extern void (GLAPIENTRY *qglBindTexture)(GLenum target, GLuint texture);
extern GLboolean (GLAPIENTRY *qglIsTexture)(GLuint texture);
extern void (GLAPIENTRY *qglPixelStoref)(GLenum pname, GLfloat param);
extern void (GLAPIENTRY *qglPixelStorei)(GLenum pname, GLint param);

extern void (GLAPIENTRY *qglTexImage1D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void (GLAPIENTRY *qglTexImage2D)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
extern void (GLAPIENTRY *qglTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
extern void (GLAPIENTRY *qglTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
extern void (GLAPIENTRY *qglCopyTexImage1D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
extern void (GLAPIENTRY *qglCopyTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
extern void (GLAPIENTRY *qglCopyTexSubImage1D)(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
extern void (GLAPIENTRY *qglCopyTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);

extern void (GLAPIENTRY *qglPolygonOffset)(GLfloat factor, GLfloat units);
extern void (GLAPIENTRY *qglPolygonMode)(GLenum face, GLenum mode);

extern void (GLAPIENTRY *qglLineWidth)(GLfloat width);
extern void (GLAPIENTRY *qglPointSize)(GLfloat size);
extern void (GLAPIENTRY *qglPolygonStipple)(const GLubyte *mask);
#else
#ifdef __cplusplus
extern "C" {
#endif
#ifndef NO_GLEXTENSIONS
void glDrawRangeElements(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);
#define qglDrawRangeElements(A,B,C,D,E,F) glDrawRangeElements(A,B,C,D,E,F)
#endif
void glDrawElements(GLenum mode, GLsizei count, GLenum type, const GLvoid *indices);
#define qglDrawElements(A,B,C,D) glDrawElements(A,B,C,D)
void glVertexPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
#define qglVertexPointer(A,B,C,D) glVertexPointer(A,B,C,D)
void glNormalPointer(GLenum type, GLsizei stride, const GLvoid *ptr);
#define qglNormalPointer(A,B,C) glNormalPointer(A,B,C)
void glColorPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
#define qglColorPointer(A,B,C,D) glColorPointer(A,B,C,D)
void glTexCoordPointer(GLint size, GLenum type, GLsizei stride, const GLvoid *ptr);
#define qglTexCoordPointer(A,B,C,D) glTexCoordPointer(A,B,C,D)
void glArrayElement(GLint i);
#define qglArrayElement(A) glArrayElement(A)
void glColor4f(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
#define qglColor4f(A,B,C,D) glColor4f(A,B,C,D)
void glTexCoord1f(GLfloat s);
#define qglTexCoord1f(A) glTexCoord1f(A)
void glTexCoord2f(GLfloat s, GLfloat t);
#define qglTexCoord2f(A,B) glTexCoord2f(A,B)
void glTexCoord3f(GLfloat s, GLfloat t, GLfloat r);
#define qglTexCoord3f(A,B,C) glTexCoord3f(A,B,C)
void glTexCoord4f(GLfloat s, GLfloat t, GLfloat r, GLfloat q);
#define qglTexCoord4f(A,B,C,D) glTexCoord4f(A,B,C,D)
void glVertex2f(GLfloat x, GLfloat y);
#define qglVertex2f(A,B) glVertex2f(A,B)
void glVertex3f(GLfloat x, GLfloat y, GLfloat z);
#define qglVertex3f(A,B,C) glVertex3f(A,B,C)
void glBegin(GLenum mode);
#define qglBegin(A) glBegin(A)
void glEnd(void);
#define qglEnd() glEnd()
void glMatrixMode(GLenum mode);
#define qglMatrixMode(A) glMatrixMode(A)
void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);
#define qglOrtho(A,B,C,D,E,F) glOrtho(A,B,C,D,E,F)
void glFrustum(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near_val, GLdouble far_val);
#define qglFrustum(A,B,C,D,E,F) glFrustum(A,B,C,D,E,F)
void glViewport(GLint x, GLint y, GLsizei width, GLsizei height);
#define qglViewport(A,B,C,D) glViewport(A,B,C,D)
void glPushMatrix(void);
#define qglPushMatrix() glPushMatrix()
void glPopMatrix(void);
#define qglPopMatrix() glPopMatrix()
void glLoadIdentity(void);
#define qglLoadIdentity() glLoadIdentity()
void glLoadMatrixd(const GLdouble *m);
#define qglLoadMatrixd(A) glLoadMatrixd(A)
void glLoadMatrixf(const GLfloat *m);
#define qglLoadMatrixf(A) glLoadMatrixf(A)
void glMultMatrixd(const GLdouble *m);
#define qglMultMatrixd(A) glMultMatrixd(A)
void glMultMatrixf(const GLfloat *m);
#define qglMultMatrixf(A) glMultMatrixf(A)
void glRotated(GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
#define qglRotated(A,B,C,D) glRotated(A,B,C,D)
void glRotatef(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
#define qglRotatef(A,B,C,D) glRotatef(A,B,C,D)
void glScaled(GLdouble x, GLdouble y, GLdouble z);
#define qglScaled(A,B,C) glScaled(A,B,C)
void glScalef(GLfloat x, GLfloat y, GLfloat z);
#define qglScalef(A,B,C) glScalef(A,B,C)
void glTranslated(GLdouble x, GLdouble y, GLdouble z);
#define qglTranslated(A,B,C) glTranslated(A,B,C)
void glTranslatef(GLfloat x, GLfloat y, GLfloat z);
#define qglTranslatef(A,B,C) glTranslatef(A,B,C)
void glReadPixels(GLint x, GLint y, GLsizei width, GLsizei height, GLenum format, GLenum type, GLvoid *pixels);
#define qglReadPixels(A,B,C,D,E,F,G) glReadPixels(A,B,C,D,E,F,G)
void glStencilFunc(GLenum func, GLint ref, GLuint mask);
#define qglStencilFunc(A,B,C) glStencilFunc(A,B,C)
void glStencilMask(GLuint mask);
#define qglStencilMask(A) glStencilMask(A)
void glStencilOp(GLenum fail, GLenum zfail, GLenum zpass);
#define qglStencilOp(A,B,C) glStencilOp(A,B,C)
void glClearStencil(GLint s);
#define qglClearStencil(A) glClearStencil(A)
void glTexEnvf(GLenum target, GLenum pname, GLfloat param);
#define qglTexEnvf(A,B,C) glTexEnvf(A,B,C)
void glTexEnvfv(GLenum target, GLenum pname, const GLfloat *params);
#define qglTexEnvfv(A,B,C) glTexEnvfv(A,B,C)
void glTexEnvi(GLenum target, GLenum pname, GLint param);
#define qglTexEnvi(A,B,C) glTexEnvi(A,B,C)
void glTexParameterf(GLenum target, GLenum pname, GLfloat param);
#define qglTexParameterf(A,B,C) glTexParameterf(A,B,C)
void glTexParameterfv(GLenum target, GLenum pname, GLfloat *params);
#define qglTexParameterfv(A,B,C) glTexParameterfv(A,B,C)
void glTexParameteri(GLenum target, GLenum pname, GLint param);
#define qglTexParameteri(A,B,C) glTexParameteri(A,B,C)
void glGenTextures(GLsizei n, GLuint *textures);
#define qglGenTextures(A,B) glGenTextures(A,B)
void glDeleteTextures(GLsizei n, const GLuint *textures);
#define qglDeleteTextures(A,B) glDeleteTextures(A,B)
void glBindTexture(GLenum target, GLuint texture);
#define qglBindTexture(A,B) glBindTexture(A,B)
GLboolean glIsTexture(GLuint texture);
#define qglIsTexture(A) glIsTexture(A)
void glPixelStoref(GLenum pname, GLfloat param);
#define qglPixelStoref(A,B) glPixelStoref(A,B)
void glPixelStorei(GLenum pname, GLint param);
#define qglPixelStorei(A,B) glPixelStorei(A,B)
void glTexImage1D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
#define qglTexImage1D(A,B,C,D,E,F,G,H) glTexImage1D(A,B,C,D,E,F,G,H)
void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
#define qglTexImage2D(A,B,C,D,E,F,G,H,I) glTexImage2D(A,B,C,D,E,F,G,H,I)
void glTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLsizei width, GLenum format, GLenum type, const GLvoid *pixels);
#define qglTexSubImage1D(A,B,C,D,E,F,G) glTexSubImage1D(A,B,C,D,E,F,G)
void glTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const GLvoid *pixels);
#define qglTexSubImage2D(A,B,C,D,E,F,G,H,I) glTexSubImage2D(A,B,C,D,E,F,G,H,I)
void glCopyTexImage1D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLint border);
#define qglCopyTexImage1D(A,B,C,D,E,F,G) glCopyTexImage1D(A,B,C,D,E,F,G)
void glCopyTexImage2D(GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);
#define qglCopyTexImage2D(A,B,C,D,E,F,G,H) glCopyTexImage2D(A,B,C,D,E,F,G,H)
void glCopyTexSubImage1D(GLenum target, GLint level, GLint xoffset, GLint x, GLint y, GLsizei width);
#define glCopyTexSubImage1D(A,B,C,D,E,F) glCopyTexSubImage1D(A,B,C,D,E,F)
void glCopyTexSubImage2D(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint x, GLint y, GLsizei width, GLsizei height);
#define qglCopyTexSubImage2D(A,B,C,D,E,F,G,H) glCopyTexSubImage2D(A,B,C,D,E,F,G,H)
void glPolygonOffset(GLfloat factor, GLfloat units);
#define qglPolygonOffset(A,B) glPolygonOffset(A,B)
void glPolygonMode(GLenum face, GLenum mode);
#define qglPolygonMode(A,B) glPolygonMode(A,B)
void glLineWidth(GLfloat width);
#define qglLineWidth(A) glLineWidth(A)
void glPointSize(GLfloat size);
#define qglPointSize(A) glPointSize(A)
void glPolygonStipple(const GLubyte *mask);
#define qglPolygonStipple(A) glPolygonStipple(A)

#ifdef __cplusplus
}
#endif
#endif

// GL_ARB_shader_objects
#ifndef NO_GLEXTENSIONS
extern int gl_support_shader_objects;
#ifndef GL_PROGRAM_OBJECT_ARB
// 1-byte character string
typedef char GLcharARB;
// 4-byte integer handle to a shader object or program object
typedef int GLhandleARB;
#endif
extern void (GLAPIENTRY *qglDeleteObjectARB)(GLhandleARB obj);
extern GLhandleARB (GLAPIENTRY *qglGetHandleARB)(GLenum pname);
extern void (GLAPIENTRY *qglDetachObjectARB)(GLhandleARB containerObj, GLhandleARB attachedObj);
extern GLhandleARB (GLAPIENTRY *qglCreateShaderObjectARB)(GLenum shaderType);
extern void (GLAPIENTRY *qglShaderSourceARB)(GLhandleARB shaderObj, GLsizei count, const GLcharARB **string, const GLint *length);
extern void (GLAPIENTRY *qglCompileShaderARB)(GLhandleARB shaderObj);
extern GLhandleARB (GLAPIENTRY *qglCreateProgramObjectARB)(void);
extern void (GLAPIENTRY *qglAttachObjectARB)(GLhandleARB containerObj, GLhandleARB obj);
extern void (GLAPIENTRY *qglLinkProgramARB)(GLhandleARB programObj);
extern void (GLAPIENTRY *qglUseProgramObjectARB)(GLhandleARB programObj);
extern void (GLAPIENTRY *qglValidateProgramARB)(GLhandleARB programObj);
extern void (GLAPIENTRY *qglUniform1fARB)(GLint location, GLfloat v0);
extern void (GLAPIENTRY *qglUniform2fARB)(GLint location, GLfloat v0, GLfloat v1);
extern void (GLAPIENTRY *qglUniform3fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void (GLAPIENTRY *qglUniform4fARB)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void (GLAPIENTRY *qglUniform1iARB)(GLint location, GLint v0);
extern void (GLAPIENTRY *qglUniform2iARB)(GLint location, GLint v0, GLint v1);
extern void (GLAPIENTRY *qglUniform3iARB)(GLint location, GLint v0, GLint v1, GLint v2);
extern void (GLAPIENTRY *qglUniform4iARB)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void (GLAPIENTRY *qglUniform1fvARB)(GLint location, GLsizei count, const GLfloat *value);
extern void (GLAPIENTRY *qglUniform2fvARB)(GLint location, GLsizei count, const GLfloat *value);
extern void (GLAPIENTRY *qglUniform3fvARB)(GLint location, GLsizei count, const GLfloat *value);
extern void (GLAPIENTRY *qglUniform4fvARB)(GLint location, GLsizei count, const GLfloat *value);
extern void (GLAPIENTRY *qglUniform1ivARB)(GLint location, GLsizei count, const GLint *value);
extern void (GLAPIENTRY *qglUniform2ivARB)(GLint location, GLsizei count, const GLint *value);
extern void (GLAPIENTRY *qglUniform3ivARB)(GLint location, GLsizei count, const GLint *value);
extern void (GLAPIENTRY *qglUniform4ivARB)(GLint location, GLsizei count, const GLint *value);
extern void (GLAPIENTRY *qglUniformMatrix2fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GLAPIENTRY *qglUniformMatrix3fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GLAPIENTRY *qglUniformMatrix4fvARB)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GLAPIENTRY *qglGetObjectParameterfvARB)(GLhandleARB obj, GLenum pname, GLfloat *params);
extern void (GLAPIENTRY *qglGetObjectParameterivARB)(GLhandleARB obj, GLenum pname, GLint *params);
extern void (GLAPIENTRY *qglGetInfoLogARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *infoLog);
extern void (GLAPIENTRY *qglGetAttachedObjectsARB)(GLhandleARB containerObj, GLsizei maxCount, GLsizei *count, GLhandleARB *obj);
extern GLint (GLAPIENTRY *qglGetUniformLocationARB)(GLhandleARB programObj, const GLcharARB *name);
extern void (GLAPIENTRY *qglGetActiveUniformARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
extern void (GLAPIENTRY *qglGetUniformfvARB)(GLhandleARB programObj, GLint location, GLfloat *params);
extern void (GLAPIENTRY *qglGetUniformivARB)(GLhandleARB programObj, GLint location, GLint *params);
extern void (GLAPIENTRY *qglGetShaderSourceARB)(GLhandleARB obj, GLsizei maxLength, GLsizei *length, GLcharARB *source);
#endif
#ifndef GL_PROGRAM_OBJECT_ARB
#define GL_PROGRAM_OBJECT_ARB					0x8B40
#define GL_OBJECT_TYPE_ARB						0x8B4E
#define GL_OBJECT_SUBTYPE_ARB					0x8B4F
#define GL_OBJECT_DELETE_STATUS_ARB				0x8B80
#define GL_OBJECT_COMPILE_STATUS_ARB			0x8B81
#define GL_OBJECT_LINK_STATUS_ARB				0x8B82
#define GL_OBJECT_VALIDATE_STATUS_ARB			0x8B83
#define GL_OBJECT_INFO_LOG_LENGTH_ARB			0x8B84
#define GL_OBJECT_ATTACHED_OBJECTS_ARB			0x8B85
#define GL_OBJECT_ACTIVE_UNIFORMS_ARB			0x8B86
#define GL_OBJECT_ACTIVE_UNIFORM_MAX_LENGTH_ARB	0x8B87
#define GL_OBJECT_SHADER_SOURCE_LENGTH_ARB		0x8B88
#define GL_SHADER_OBJECT_ARB					0x8B48
#define GL_FLOAT								0x1406
#define GL_FLOAT_VEC2_ARB						0x8B50
#define GL_FLOAT_VEC3_ARB						0x8B51
#define GL_FLOAT_VEC4_ARB						0x8B52
#define GL_INT									0x1404
#define GL_INT_VEC2_ARB							0x8B53
#define GL_INT_VEC3_ARB							0x8B54
#define GL_INT_VEC4_ARB							0x8B55
#define GL_BOOL_ARB								0x8B56
#define GL_BOOL_VEC2_ARB						0x8B57
#define GL_BOOL_VEC3_ARB						0x8B58
#define GL_BOOL_VEC4_ARB						0x8B59
#define GL_FLOAT_MAT2_ARB						0x8B5A
#define GL_FLOAT_MAT3_ARB						0x8B5B
#define GL_FLOAT_MAT4_ARB						0x8B5C
#define GL_SAMPLER_1D_ARB						0x8B5D
#define GL_SAMPLER_2D_ARB						0x8B5E
#define GL_SAMPLER_3D_ARB						0x8B5F
#define GL_SAMPLER_CUBE_ARB						0x8B60
#define GL_SAMPLER_1D_SHADOW_ARB				0x8B61
#define GL_SAMPLER_2D_SHADOW_ARB				0x8B62
#define GL_SAMPLER_2D_RECT_ARB					0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW_ARB			0x8B64
#endif

// GL_ARB_vertex_shader
#ifndef NO_GLEXTENSIONS
extern int gl_support_vertex_shader;
//extern void (GLAPIENTRY *qglVertexAttrib1fARB)(GLuint index, GLfloat v0);
//extern void (GLAPIENTRY *qglVertexAttrib1sARB)(GLuint index, GLshort v0);
//extern void (GLAPIENTRY *qglVertexAttrib1dARB)(GLuint index, GLdouble v0);
//extern void (GLAPIENTRY *qglVertexAttrib2fARB)(GLuint index, GLfloat v0, GLfloat v1);
//extern void (GLAPIENTRY *qglVertexAttrib2sARB)(GLuint index, GLshort v0, GLshort v1);
//extern void (GLAPIENTRY *qglVertexAttrib2dARB)(GLuint index, GLdouble v0, GLdouble v1);
//extern void (GLAPIENTRY *qglVertexAttrib3fARB)(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2);
//extern void (GLAPIENTRY *qglVertexAttrib3sARB)(GLuint index, GLshort v0, GLshort v1, GLshort v2);
//extern void (GLAPIENTRY *qglVertexAttrib3dARB)(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2);
//extern void (GLAPIENTRY *qglVertexAttrib4fARB)(GLuint index, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
//extern void (GLAPIENTRY *qglVertexAttrib4sARB)(GLuint index, GLshort v0, GLshort v1, GLshort v2, GLshort v3);
//extern void (GLAPIENTRY *qglVertexAttrib4dARB)(GLuint index, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3);
//extern void (GLAPIENTRY *qglVertexAttrib4NubARB)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
//extern void (GLAPIENTRY *qglVertexAttrib1fvARB)(GLuint index, const GLfloat *v);
//extern void (GLAPIENTRY *qglVertexAttrib1svARB)(GLuint index, const GLshort *v);
//extern void (GLAPIENTRY *qglVertexAttrib1dvARB)(GLuint index, const GLdouble *v);
//extern void (GLAPIENTRY *qglVertexAttrib2fvARB)(GLuint index, const GLfloat *v);
//extern void (GLAPIENTRY *qglVertexAttrib2svARB)(GLuint index, const GLshort *v);
//extern void (GLAPIENTRY *qglVertexAttrib2dvARB)(GLuint index, const GLdouble *v);
//extern void (GLAPIENTRY *qglVertexAttrib3fvARB)(GLuint index, const GLfloat *v);
//extern void (GLAPIENTRY *qglVertexAttrib3svARB)(GLuint index, const GLshort *v);
//extern void (GLAPIENTRY *qglVertexAttrib3dvARB)(GLuint index, const GLdouble *v);
//extern void (GLAPIENTRY *qglVertexAttrib4fvARB)(GLuint index, const GLfloat *v);
//extern void (GLAPIENTRY *qglVertexAttrib4svARB)(GLuint index, const GLshort *v);
//extern void (GLAPIENTRY *qglVertexAttrib4dvARB)(GLuint index, const GLdouble *v);
//extern void (GLAPIENTRY *qglVertexAttrib4ivARB)(GLuint index, const GLint *v);
//extern void (GLAPIENTRY *qglVertexAttrib4bvARB)(GLuint index, const GLbyte *v);
//extern void (GLAPIENTRY *qglVertexAttrib4ubvARB)(GLuint index, const GLubyte *v);
//extern void (GLAPIENTRY *qglVertexAttrib4usvARB)(GLuint index, const GLushort *v);
//extern void (GLAPIENTRY *qglVertexAttrib4uivARB)(GLuint index, const GLuint *v);
//extern void (GLAPIENTRY *qglVertexAttrib4NbvARB)(GLuint index, const GLbyte *v);
//extern void (GLAPIENTRY *qglVertexAttrib4NsvARB)(GLuint index, const GLshort *v);
//extern void (GLAPIENTRY *qglVertexAttrib4NivARB)(GLuint index, const GLint *v);
//extern void (GLAPIENTRY *qglVertexAttrib4NubvARB)(GLuint index, const GLubyte *v);
//extern void (GLAPIENTRY *qglVertexAttrib4NusvARB)(GLuint index, const GLushort *v);
//extern void (GLAPIENTRY *qglVertexAttrib4NuivARB)(GLuint index, const GLuint *v);
extern void (GLAPIENTRY *qglVertexAttribPointerARB)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);
extern void (GLAPIENTRY *qglEnableVertexAttribArrayARB)(GLuint index);
extern void (GLAPIENTRY *qglDisableVertexAttribArrayARB)(GLuint index);
extern void (GLAPIENTRY *qglBindAttribLocationARB)(GLhandleARB programObj, GLuint index, const GLcharARB *name);
extern void (GLAPIENTRY *qglGetActiveAttribARB)(GLhandleARB programObj, GLuint index, GLsizei maxLength, GLsizei *length, GLint *size, GLenum *type, GLcharARB *name);
extern GLint (GLAPIENTRY *qglGetAttribLocationARB)(GLhandleARB programObj, const GLcharARB *name);
//extern void (GLAPIENTRY *qglGetVertexAttribdvARB)(GLuint index, GLenum pname, GLdouble *params);
//extern void (GLAPIENTRY *qglGetVertexAttribfvARB)(GLuint index, GLenum pname, GLfloat *params);
//extern void (GLAPIENTRY *qglGetVertexAttribivARB)(GLuint index, GLenum pname, GLint *params);
//extern void (GLAPIENTRY *qglGetVertexAttribPointervARB)(GLuint index, GLenum pname, GLvoid **pointer);
#endif
#ifndef GL_VERTEX_SHADER_ARB
#define GL_VERTEX_SHADER_ARB						0x8B31
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS_ARB		0x8B4A
#define GL_MAX_VARYING_FLOATS_ARB					0x8B4B
#define GL_MAX_VERTEX_ATTRIBS_ARB					0x8869
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB				0x8872
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS_ARB		0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS_ARB		0x8B4D
#define GL_MAX_TEXTURE_COORDS_ARB					0x8871
#define GL_VERTEX_PROGRAM_POINT_SIZE_ARB			0x8642
#define GL_VERTEX_PROGRAM_TWO_SIDE_ARB				0x8643
#define GL_OBJECT_ACTIVE_ATTRIBUTES_ARB				0x8B89
#define GL_OBJECT_ACTIVE_ATTRIBUTE_MAX_LENGTH_ARB	0x8B8A
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED_ARB			0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE_ARB				0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE_ARB			0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE_ARB				0x8625
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED_ARB		0x886A
#define GL_CURRENT_VERTEX_ATTRIB_ARB				0x8626
#define GL_VERTEX_ATTRIB_ARRAY_POINTER_ARB			0x8645
#define GL_FLOAT									0x1406
#define GL_FLOAT_VEC2_ARB							0x8B50
#define GL_FLOAT_VEC3_ARB							0x8B51
#define GL_FLOAT_VEC4_ARB							0x8B52
#define GL_FLOAT_MAT2_ARB							0x8B5A
#define GL_FLOAT_MAT3_ARB							0x8B5B
#define GL_FLOAT_MAT4_ARB							0x8B5C
#endif

// GL_ARB_fragment_shader
#ifndef NO_GLEXTENSIONS
extern int gl_support_fragment_shader;
#endif
#ifndef GL_FRAGMENT_SHADER_ARB
#define GL_FRAGMENT_SHADER_ARB						0x8B30
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS_ARB		0x8B49
#define GL_MAX_TEXTURE_COORDS_ARB					0x8871
#define GL_MAX_TEXTURE_IMAGE_UNITS_ARB				0x8872
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT_ARB		0x8B8B
#endif

// GL_ARB_shading_language_100
#ifndef NO_GLEXTENSIONS
extern int gl_support_shading_language_100;
#endif
#ifndef GL_SHADING_LANGUAGE_VERSION_ARB
#define GL_SHADING_LANGUAGE_VERSION_ARB				0x8B8C
#endif

// GL_EXT_bgr
#define GL_BGR					0x80E0

// GL_EXT_bgra
#define GL_BGRA					0x80E1

#ifndef NO_DEVELOPER
#define DEBUGGL
#endif


#ifdef DEBUGGL
#define CHECKGLERROR {if (gl_paranoid.integer){if (gl_printcheckerror.integer) Con_Printf("CHECKGLERROR at %s:%d\n", __FILE__, __LINE__);errornumber = qglGetError();if (errornumber) GL_PrintError(errornumber, __FILE__, __LINE__);}}
extern int errornumber;
void GL_PrintError(int errornumber, char *filename, int linenumber);
#else
#define CHECKGLERROR
#endif

#endif

