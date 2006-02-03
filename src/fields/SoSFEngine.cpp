/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2005 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org/> for more information.
 *
 *  Systems in Motion, Postboks 1283, Pirsenteret, 7462 Trondheim, NORWAY.
 *  <URL:http://www.sim.no/>.
 *
\**************************************************************************/

// Important note: this sourcefile was in full generated by the
// Coin/scripts/templant script from the code in SFNodeEnginePath.tpl.

///////////////////////////////////////////////////////////////////////////


//$ BEGIN TEMPLATE SFNodeEnginePath(ENGINE, Engine, engine)

/*!
  \class SoSFEngine SoSFEngine.h Inventor/fields/SoSFEngine.h
  \brief The SoSFEngine class is a container for a single engine.
  \ingroup fields

  This field container stores a pointer to a Coin engine. It takes care
  of the necessary functionality for handling copy, import and export
  operations.

  Note that the engine pointer stored in a field instance of this type
  may be a \c NULL pointer.

  \sa SoEngine, SoMFEngine

*/

// Type-specific define to be able to do #ifdef tests on type.  (Note:
// used to check the header file wrapper define, but that doesn't work
// with --enable-compact build.)
#define COIN_INTERNAL_SOSFENGINE

#include <Inventor/fields/SoSFEngine.h>
#include <Inventor/fields/SoSubFieldP.h>

#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/errors/SoReadError.h>
#include <Inventor/nodes/SoNode.h>
#include <Inventor/engines/SoEngine.h>
#include <Inventor/SoOutput.h>
#if COIN_DEBUG
#include <Inventor/errors/SoDebugError.h>
#endif // COIN_DEBUG


// Can't use SO_SFIELD_SOURCE() because we need to modify setValue()
// to ref and unref the passed engine.
SO_SFIELD_REQUIRED_SOURCE(SoSFEngine);


// Override from parent class.
void
SoSFEngine::initClass(void)
{
  SO_SFIELD_INTERNAL_INIT_CLASS(SoSFEngine);
}

// (Declarations hidden in SO_[S|M]FIELD_HEADER macro in header file,
// so don't use Doxygen commenting.)
#ifndef DOXYGEN_SKIP_THIS

/* Constructor, sets initial engine pointer to a \c NULL pointer. */
SoSFEngine::SoSFEngine(void)
{
  this->value = NULL;
#ifdef COIN_INTERNAL_SOSFPATH
  this->head = NULL;
#endif // COIN_INTERNAL_SOSFPATH
}

/* Destructor, dereferences the current engine pointer if necessary. */
SoSFEngine::~SoSFEngine(void)
{
  this->enableNotify(FALSE);
  this->setValue(NULL);
}

#endif // DOXYGEN_SKIP_THIS


// No need to document readValue() and writeValue() here, as the
// necessary information is provided by the documentation of the
// parent classes.
#ifndef DOXYGEN_SKIP_THIS

// Store the \a newval engine pointer in this field. If \a newval is not
// \c NULL, will add 1 to the reference count of the engine.
void
SoSFEngine::setValue(SoEngine * newval)
{
  // Don't use getValue() to find oldptr, since this might trigger a
  // recursive evaluation call if the field is connected.
  SoEngine * oldptr = this->value;
  if (oldptr == newval) return;

  if (oldptr) {
#ifdef COIN_INTERNAL_SOSFPATH
    SoNode * h = oldptr->getHead();
    // The path should be audited by us at all times. So don't use
    // SoSFPath to wrap SoTempPath or SoLightPath, for instance.
    assert(h==this->head && "Path head changed without notification!");
    if (h) {
      h->removeAuditor(this, SoNotRec::FIELD);
      h->unref();
    }
#endif // COIN_INTERNAL_SOSFPATH
    oldptr->removeAuditor(this, SoNotRec::FIELD);
    oldptr->unref();
  }

  if (newval) {
    newval->addAuditor(this, SoNotRec::FIELD);
    newval->ref();
#ifdef COIN_INTERNAL_SOSFPATH
    this->head = newval->getHead();
    if (this->head) {
      this->head->addAuditor(this, SoNotRec::FIELD);
      this->head->ref();
    }
#endif // COIN_INTERNAL_SOSFPATH
  }

  this->value = newval;
  this->valueChanged();
}

// Compares to see if the \a field points to the same engine as this
// field does, and returns \c TRUE if this is the case.
//
// Be aware that this method does \e not check for engine/subgraph
// equality if the pointers are not the same, so \c FALSE is returned
// even though the contents of the engine/subgraph are equal.
SbBool
SoSFEngine::operator==(const SoSFEngine & field) const
{
  return (this->getValue() == field.getValue());
}

// Import engine.
SbBool
SoSFEngine::readValue(SoInput * in)
{
  SoBase * baseptr;
  if (!SoBase::read(in, baseptr, SoEngine::getClassTypeId())) return FALSE;

  if (in->eof()) {
    SoReadError::post(in, "Premature end of file");
    return FALSE;
  }
  if (!baseptr) {
    SoReadError::post(in, "Unable to read value for SoSFEngine");
    return FALSE;
  }

  this->setValue((SoEngine *)baseptr);
  return TRUE;
}

// Export engine.
void
SoSFEngine::writeValue(SoOutput * out) const
{
  // NB: This code is common for SoSFNode, SoSFPath and SoSFEngine.
  // That's why we check the base type before writing.
  SoBase * base = this->getValue();
  if (base) {
    if (base->isOfType(SoNode::getClassTypeId())) {
      ((SoNode*)base)->writeInstance(out);
    }
    else if (base->isOfType(SoPath::getClassTypeId())) {
      SoWriteAction wa(out);
      wa.continueToApply((SoPath *)base);
    }
    else if (base->isOfType(SoEngine::getClassTypeId())) {
      ((SoEngine *)base)->writeInstance(out);
    }
    else {
      assert(0 && "strange internal error");
    }
  }
  else {
    // This actually works for both ASCII and binary formats.
    out->write("NULL");
  }
}

#endif // DOXYGEN_SKIP_THIS


// Overridden from parent to propagate write reference counting to
// engine.
void
SoSFEngine::countWriteRefs(SoOutput * out) const
{
  inherited::countWriteRefs(out);

  SoBase * base = this->getValue();
  if (base == NULL) return;

  // NB: This code is common for SoSFNode, SoSFPath and SoSFEngine.
  // That's why we check the base type before writing/counting

  if (base->isOfType(SoNode::getClassTypeId())) {
    ((SoNode*)base)->writeInstance(out);
  }
  else if (base->isOfType(SoEngine::getClassTypeId())) {
    ((SoEngine*)base)->addWriteReference(out);
  }
  else if (base->isOfType(SoPath::getClassTypeId())) {
    SoWriteAction wa(out);
    wa.continueToApply((SoPath*)base);
  }
}

// Override from parent to update our engine pointer
// reference. This is necessary so we do the Right Thing with regard
// to the copyconnections flag.
//
// Note that we have to unplug auditing and the reference counter
// addition we made during the copy process.
//
// For reference for future debugging sessions, copying of this field
// goes like this:
//
//    - copyFrom() is called (typically from SoFieldData::overlay())
//    - copyFrom() calls operator=()
//    - operator=() calls setValue()
//    - we have a local copy (ie not from SoSubField.h) of setValue()
//      that sets up auditing and references the item
//
// <mortene@sim.no>
void
SoSFEngine::fixCopy(SbBool copyconnections)
{
  SoEngine * n = this->getValue();
  if (!n) return;

#if COIN_DEBUG
  n->assertAlive();
#endif // COIN_DEBUG

  // The setValue() call below will automatically de-audit and un-ref
  // the old pointer-value reference we have, *before* re-inserting a
  // copy.

#if defined(COIN_INTERNAL_SOSFNODE) || defined(COIN_INTERNAL_SOSFENGINE)
  SoFieldContainer * fc = SoFieldContainer::findCopy(n, copyconnections);
#if COIN_DEBUG
  if (fc) fc->assertAlive();
#endif // COIN_DEBUG
  if (fc) this->setValue((SoEngine *)fc);
#endif // COIN_INTERNAL_SOSFNODE || COIN_INTERNAL_SOSFENGINE

#ifdef COIN_INTERNAL_SOSFPATH
  this->setValue(n->copy());
#endif // COIN_INTERNAL_SOSFPATH
}

// Override from SoField to check engine pointer.
SbBool
SoSFEngine::referencesCopy(void) const
{
  if (inherited::referencesCopy()) return TRUE;

  SoBase * n = this->getValue();
  if (!n) return FALSE;

  if (n->isOfType(SoNode::getClassTypeId()) ||
      n->isOfType(SoEngine::getClassTypeId())) {
    if (SoFieldContainer::checkCopy((SoFieldContainer *)n)) return TRUE;
  }
  else if (n->isOfType(SoPath::getClassTypeId())) {
    SoPath * p = (SoPath *)n;
    if (p->getHead() == NULL) return FALSE;
    if (SoFieldContainer::checkCopy(p->getHead())) return TRUE;
  }
  else {
    assert(0 && "strange internal error");
  }

  return FALSE;
}

// Kill the type-specific define.
#undef COIN_INTERNAL_SOSFENGINE
//$ END TEMPLATE SFNodeEnginePath
