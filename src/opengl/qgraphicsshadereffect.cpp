/***********************************************************************
*
* Copyright (c) 2012-2019 Barbara Geller
* Copyright (c) 2012-2019 Ansel Sermersheim
*
* Copyright (C) 2015 The Qt Company Ltd.
* Copyright (c) 2012-2016 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
*
* This file is part of CopperSpice.
*
* CopperSpice is free software. You can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* https://www.gnu.org/licenses/
*
***********************************************************************/

#include <qgraphicsshadereffect_p.h>

#ifndef QT_NO_GRAPHICSEFFECT

#include <qglshaderprogram.h>
#include <qglcustomshaderstage_p.h>

#define QGL_HAVE_CUSTOM_SHADERS 1

#include <qpainter.h>
#include <qgraphicsitem.h>
#include <qgraphicseffect_p.h>




static const char qglslDefaultImageFragmentShader[] = "\
    lowp vec4 customShader(lowp sampler2D imageTexture, highp vec2 textureCoords) { \
        return texture2D(imageTexture, textureCoords); \
    }\n";

#ifdef QGL_HAVE_CUSTOM_SHADERS

class QGLCustomShaderEffectStage : public QGLCustomShaderStage
{
 public:
   QGLCustomShaderEffectStage (QGraphicsShaderEffect *e, const QByteArray &source)
      : QGLCustomShaderStage(), effect(e) {
      setSource(source);
   }

   void setUniforms(QGLShaderProgram *program) override;

   QGraphicsShaderEffect *effect;
};

void QGLCustomShaderEffectStage::setUniforms(QGLShaderProgram *program)
{
   effect->setUniforms(program);
}

#endif

class QGraphicsShaderEffectPrivate : public QGraphicsEffectPrivate
{
   Q_DECLARE_PUBLIC(QGraphicsShaderEffect)

 public:
   QGraphicsShaderEffectPrivate()
      : pixelShaderFragment(qglslDefaultImageFragmentShader)
#ifdef QGL_HAVE_CUSTOM_SHADERS
      , customShaderStage(0)
#endif
   {
   }

   QByteArray pixelShaderFragment;
#ifdef QGL_HAVE_CUSTOM_SHADERS
   QGLCustomShaderEffectStage *customShaderStage;
#endif
};


QGraphicsShaderEffect::QGraphicsShaderEffect(QObject *parent)
   : QGraphicsEffect(*new QGraphicsShaderEffectPrivate(), parent)
{
}

QGraphicsShaderEffect::~QGraphicsShaderEffect()
{
#ifdef QGL_HAVE_CUSTOM_SHADERS
   Q_D(QGraphicsShaderEffect);
   delete d->customShaderStage;
#endif
}

/*#
    Returns the source code for the pixel shader fragment for
    this shader effect.  The default is a shader that copies
    its incoming pixmap directly to the output with no effect
    applied.

    \sa setPixelShaderFragment()
*/
QByteArray QGraphicsShaderEffect::pixelShaderFragment() const
{
   Q_D(const QGraphicsShaderEffect);
   return d->pixelShaderFragment;
}


void QGraphicsShaderEffect::setPixelShaderFragment(const QByteArray &code)
{
   Q_D(QGraphicsShaderEffect);
   if (d->pixelShaderFragment != code) {
      d->pixelShaderFragment = code;
#ifdef QGL_HAVE_CUSTOM_SHADERS
      delete d->customShaderStage;
      d->customShaderStage = 0;
#endif
   }
}

/*#
    \reimp
*/
void QGraphicsShaderEffect::draw(QPainter *painter)
{
   Q_D(QGraphicsShaderEffect);

#ifdef QGL_HAVE_CUSTOM_SHADERS
   // Set the custom shader on the paint engine.  The setOnPainter()
   // call may fail if the paint engine is not GL2.  In that case,
   // we fall through to drawing the pixmap normally.
   if (!d->customShaderStage) {
      d->customShaderStage = new QGLCustomShaderEffectStage
      (this, d->pixelShaderFragment);
   }
   bool usingShader = d->customShaderStage->setOnPainter(painter);

   QPoint offset;
   if (sourceIsPixmap()) {
      // No point in drawing in device coordinates (pixmap will be scaled anyways).
      const QPixmap pixmap = sourcePixmap(Qt::LogicalCoordinates, &offset);
      painter->drawPixmap(offset, pixmap);
   } else {
      // Draw pixmap in device coordinates to avoid pixmap scaling.
      const QPixmap pixmap = sourcePixmap(Qt::DeviceCoordinates, &offset);
      QTransform restoreTransform = painter->worldTransform();
      painter->setWorldTransform(QTransform());
      painter->drawPixmap(offset, pixmap);
      painter->setWorldTransform(restoreTransform);
   }

   // Remove the custom shader to return to normal painting operations.
   if (usingShader) {
      d->customShaderStage->removeFromPainter(painter);
   }
#else
   drawSource(painter);
#endif
}


void QGraphicsShaderEffect::setUniformsDirty()
{
#ifdef QGL_HAVE_CUSTOM_SHADERS
   Q_D(QGraphicsShaderEffect);
   if (d->customShaderStage) {
      d->customShaderStage->setUniformsDirty();
   }
#endif
}

/*#
    Sets custom uniform variables on the current GL context when
    \a program is about to be used by the paint engine.

    This function should be overridden if the shader set with
    setPixelShaderFragment() has additional parameters beyond
    those that the paint engine normally sets itself.

    \sa setUniformsDirty()
*/
void QGraphicsShaderEffect::setUniforms(QGLShaderProgram *program)
{
}



#endif
