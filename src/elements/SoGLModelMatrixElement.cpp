/**************************************************************************\
 *
 *  Copyright (C) 1998-2000 by Systems in Motion.  All rights reserved.
 *
 *  This file is part of the Coin library.
 *
 *  This file may be distributed under the terms of the Q Public License
 *  as defined by Troll Tech AS of Norway and appearing in the file
 *  LICENSE.QPL included in the packaging of this file.
 *
 *  If you want to use Coin in applications not covered by licenses
 *  compatible with the QPL, you can contact SIM to aquire a
 *  Professional Edition license for Coin.
 *
 *  Systems in Motion AS, Prof. Brochs gate 6, N-7030 Trondheim, NORWAY
 *  http://www.sim.no/ sales@sim.no Voice: +47 22114160 Fax: +47 67172912
 *
\**************************************************************************/

/*!
  \class SoGLModelMatrixElement Inventor/elements/SoGLModelMatrixElement.h
  \brief The SoGLModelMatrixElement class is yet to be documented.

  FIXME: write doc.
*/

#include <Inventor/elements/SoGLModelMatrixElement.h>

#include <Inventor/elements/SoViewingMatrixElement.h>
#include <Inventor/SbName.h>
#include <Inventor/SbRotation.h>
#include <Inventor/SbVec3f.h>
#include <Inventor/errors/SoDebugError.h>

#ifdef _WIN32
#include <windows.h>
#endif // !_WIN32

#include <GL/gl.h>

SO_ELEMENT_SOURCE(SoGLModelMatrixElement);

/*!
  This static method initializes static data for the
  SoGLModelMatrixElement class.
*/

void
SoGLModelMatrixElement::initClass(void)
{
  SO_ELEMENT_INIT_CLASS(SoGLModelMatrixElement, inherited);
}

/*!
  A constructor.  Can't be used directly.

  \sa void * SoGLModelMatrixElement::createInstance(void)
*/

SoGLModelMatrixElement::SoGLModelMatrixElement(void)
  : state(NULL), viewEltNodeId( 0 )
{
    setTypeId(SoGLModelMatrixElement::classTypeId);
    setStackIndex(SoGLModelMatrixElement::classStackIndex);
}

/*!
  The destructor.
*/

SoGLModelMatrixElement::~SoGLModelMatrixElement(void)
{
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::init(SoState * state)
{
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLModelMatrixElement::init", "");
#endif // 0
  this->state = state;
  inherited::init(state);
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::push(SoState * state)
{
  glPushMatrix();
  inherited::push(state);
  SoGLModelMatrixElement *elem = (SoGLModelMatrixElement*)
    this->next;
  elem->state = state;
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::pop(SoState * state,
                            const SoElement * prevTopElement)
{
#if 0 // too much debug output.. 981021 mortene.
  SoDebugError::postInfo("SoGLModelMatrixElement::pop", "");
#endif // 0
  inherited::pop(state, prevTopElement);
  glPopMatrix();
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::makeEltIdentity()
{
  const SbMatrix &mat = SoViewingMatrixElement::get(this->state);
  glLoadMatrixf(mat[0]);
  inherited::makeEltIdentity();
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::setElt(const SbMatrix &matrix)
{
  inherited::setElt(matrix);
  const SbMatrix &mat = SoViewingMatrixElement::get(this->state);
  glLoadMatrixf(mat[0]);
  glMultMatrixf(matrix[0]);
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::multElt(const SbMatrix &matrix)
{
#if 0 // debug
  SbMatrix m = this->modelMatrix;
  SoDebugError::post("SoGLModelMatrixElement::multElt",
                     "\n\tmodelmatrix before:"
                     "\n\t\t[ %f %f %f %f"
                     "\n\t\t  %f %f %f %f"
                     "\n\t\t  %f %f %f %f"
                     "\n\t\t  %f %f %f %f ]",
                     m[0][0], m[0][1], m[0][2], m[0][3],
                     m[1][0], m[1][1], m[1][2], m[1][3],
                     m[2][0], m[2][1], m[2][2], m[2][3],
                     m[3][0], m[3][1], m[3][2], m[3][3]);

  SoDebugError::post("SoGLModelMatrixElement::multElt",
                     "\n\tmultiply with:"
                     "\n\t\t[ %f %f %f %f"
                     "\n\t\t  %f %f %f %f"
                     "\n\t\t  %f %f %f %f"
                     "\n\t\t  %f %f %f %f ]",
                     matrix[0][0], matrix[0][1], matrix[0][2], matrix[0][3],
                     matrix[1][0], matrix[1][1], matrix[1][2], matrix[1][3],
                     matrix[2][0], matrix[2][1], matrix[2][2], matrix[2][3],
                     matrix[3][0], matrix[3][1], matrix[3][2], matrix[3][3]);
#endif // debug

  glMultMatrixf(matrix[0]);
  inherited::multElt(matrix);

#if 0 // debug
  m = this->modelMatrix;
  SoDebugError::post("SoGLModelMatrixElement::multElt",
                     "\n\tmodelmatrix after:"
                     "\n\t\t[ %f %f %f %f"
                     "\n\t\t  %f %f %f %f"
                     "\n\t\t  %f %f %f %f"
                     "\n\t\t  %f %f %f %f ]",
                     m[0][0], m[0][1], m[0][2], m[0][3],
                     m[1][0], m[1][1], m[1][2], m[1][3],
                     m[2][0], m[2][1], m[2][2], m[2][3],
                     m[3][0], m[3][1], m[3][2], m[3][3]);
#endif // debug
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::translateEltBy(const SbVec3f &translation)
{
  glTranslatef(translation[0], translation[1], translation[2]);
  inherited::translateEltBy(translation);
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::rotateEltBy(const SbRotation &rotation)
{
  SbVec3f axis;
  float angle;
  rotation.getValue(axis, angle);
  glRotatef(angle*180.0f/float(M_PI), axis[0], axis[1], axis[2]);
  inherited::rotateEltBy(rotation);
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::scaleEltBy(const SbVec3f &scaleFactor)
{
  glScalef(scaleFactor[0], scaleFactor[1], scaleFactor[2]);
  inherited::scaleEltBy(scaleFactor);
}

//! FIXME: write doc.

SbMatrix
SoGLModelMatrixElement::pushMatrixElt()
{
  SbMatrix matrix;
  glGetFloatv(GL_MODELVIEW_MATRIX, matrix[0]);
  return matrix;
}

//! FIXME: write doc.

void
SoGLModelMatrixElement::popMatrixElt(const SbMatrix &matrix)
{
  glLoadMatrixf(matrix[0]);
}
