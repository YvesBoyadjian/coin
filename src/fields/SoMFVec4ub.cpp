/**************************************************************************\
 * Copyright (c) Kongsberg Oil & Gas Technologies AS
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 * 
 * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 
 * Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 * 
 * Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
\**************************************************************************/

/*!
  \class SoMFVec4ub SoMFVec4ub.h Inventor/fields/SoMFVec4ub.h
  \brief The SoMFVec4ub class is a container for SbVec4ub vectors.

  \ingroup coin_fields

  This field is used where nodes, engines or other field containers
  need to store an array of vectors with four elements.

  This field supports application data sharing through a
  setValuesPointer() method. See SoMField documentation for
  information on how to use this function.

  \sa SbVec4ub, SoSFVec4ub
  \COIN_CLASS_EXTENSION
  \since Coin 2.5
*/

// *************************************************************************

#include <Inventor/fields/SoMFVec4ub.h>

#include <cassert>

#include <Inventor/SoInput.h>
#include <Inventor/errors/SoDebugError.h>

#include "fields/SoSubFieldP.h"
#include "fields/shared.h"

// *************************************************************************

SO_MFIELD_SOURCE(SoMFVec4ub, SbVec4ub, SbVec4ub);

SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec4ub, SbVec4ub, SbVec4ub);
SO_MFIELD_SETVALUESPOINTER_SOURCE(SoMFVec4ub, SbVec4ub, uint8_t);

// *************************************************************************

/*!
  \copydetails SoField::initClass(void)
*/
void
SoMFVec4ub::initClass(void)
{
  SO_MFIELD_INTERNAL_INIT_CLASS(SoMFVec4ub);
}

// *************************************************************************

// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

SbBool
SoMFVec4ub::read1Value(SoInput * in, int idx)
{
  assert(idx < this->maxNum);
  return
    in->readByte(this->values[idx][0]) &&
    in->readByte(this->values[idx][1]) &&
    in->readByte(this->values[idx][2]) &&
    in->readByte(this->values[idx][3]);
}

void
SoMFVec4ub::write1Value(SoOutput * out, int idx) const
{
  sosfvec4ub_write_value(out, (*this)[idx]);
}

#endif // DOXYGEN_SKIP_THIS

// *************************************************************************

/*!
  Set \a num vector array elements from \a xyzw, starting at index
  \a start.
*/
void
SoMFVec4ub::setValues(int start, int numarg, const uint8_t xyzw[][4])
{
  if(start+numarg > this->maxNum) this->allocValues(start+numarg);
  else if(start+numarg > this->num) this->num = start+numarg;

  for(int i=0; i < numarg; i++) this->values[i+start].setValue(xyzw[i]);
  this->valueChanged();
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec4ub::set1Value(int idx, uint8_t x, uint8_t y, uint8_t z, uint8_t w)
{
  this->set1Value(idx, SbVec4ub(x, y, z, w));
}

/*!
  Set the vector at \a idx.
*/
void
SoMFVec4ub::set1Value(int idx, const uint8_t xyzw[4])
{
  this->set1Value(idx, SbVec4ub(xyzw));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec4ub::setValue(uint8_t x, uint8_t y, uint8_t z, uint8_t w)
{
  this->setValue(SbVec4ub(x,y,z,w));
}

/*!
  Set this field to contain a single vector with the given
  element values.
*/
void
SoMFVec4ub::setValue(const uint8_t xyzw[4])
{
  if (xyzw == NULL) this->setNum(0);
  else this->setValue(SbVec4ub(xyzw));
}

// *************************************************************************

#ifdef COIN_TEST_SUITE

BOOST_AUTO_TEST_CASE(initialized)
{
  SoMFVec4ub field;
  BOOST_CHECK_MESSAGE(field.getTypeId() != SoType::badType(),
                      "missing class initialization");
  BOOST_CHECK_EQUAL(field.getNum(), 0);
}

BOOST_AUTO_TEST_CASE(textinput)
{
  SbBool ok;
  SoMFVec4ub field;
  ok = field.set("[]");
  BOOST_CHECK_EQUAL(ok, TRUE);
  BOOST_CHECK_EQUAL(field.getNum(), 0);
  ok = field.set("1 2 3 4");
  BOOST_CHECK_EQUAL(ok, TRUE);
  BOOST_CHECK_EQUAL(field.getNum(), 1);
  ok = field.set("[1 2 3 4]");
  BOOST_CHECK_EQUAL(ok, TRUE);
  BOOST_CHECK_EQUAL(field.getNum(), 1);
  ok = field.set("[1 2 3 4 1 2 3 4]");
  BOOST_CHECK_EQUAL(ok, TRUE);
  BOOST_CHECK_EQUAL(field.getNum(), 2);
  BOOST_CHECK_EQUAL(field[0], field[1]);
  ok = field.set("[1 2 3 4, 1 2 3 4,]");
  BOOST_CHECK_EQUAL(ok, TRUE);
  BOOST_CHECK_EQUAL(field.getNum(), 2);
  BOOST_CHECK_EQUAL(field[0], field[1]);
}

#endif // COIN_TEST_SUITE
