/***********************************************************************
*
* Copyright (c) 2012-2019 Barbara Geller
* Copyright (c) 2012-2019 Ansel Sermersheim
*
* Copyright (C) 2013 Klarälvdalens Datakonsult AB, a KDAB Group company
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

#include "qopenglfunctions_1_0.h"
#include "qopenglcontext.h"

QT_BEGIN_NAMESPACE

/*!
    \class QOpenGLFunctions_1_0
    \inmodule QtGui
    \since 5.1
    \wrapper
    \brief The QOpenGLFunctions_1_0 class provides all functions for OpenGL 1.0 specification.

    This class is a wrapper for functions from OpenGL 1.0 specification.
    See reference pages on \l {http://www.opengl.org/sdk/docs/}{opengl.org}
    for function documentation.

    \sa QAbstractOpenGLFunctions
*/

QOpenGLFunctions_1_0::QOpenGLFunctions_1_0()
 : QAbstractOpenGLFunctions()
 , d_1_0_Core(0)
 , d_1_0_Deprecated(0)
{
}

QOpenGLFunctions_1_0::~QOpenGLFunctions_1_0()
{
    if (d_1_0_Core && !d_1_0_Core->refs.deref()) {
        QAbstractOpenGLFunctionsPrivate::removeFunctionsBackend(d_1_0_Core->context, QOpenGLFunctions_1_0_CoreBackend::versionStatus());
        delete d_1_0_Core;
    }
    if (d_1_0_Deprecated && !d_1_0_Deprecated->refs.deref()) {
        QAbstractOpenGLFunctionsPrivate::removeFunctionsBackend(d_1_0_Deprecated->context, QOpenGLFunctions_1_0_DeprecatedBackend::versionStatus());
        delete d_1_0_Deprecated;
    }
}

bool QOpenGLFunctions_1_0::initializeOpenGLFunctions()
{
    if ( isInitialized() )
        return true;

    QOpenGLContext* context = QOpenGLContext::currentContext();

    // If owned by a context object make sure it is current.
    // Also check that current context is capable of resolving all needed functions
    if (((owningContext() && owningContext() == context) || !owningContext())
        && QOpenGLFunctions_1_0::isContextCompatible(context))
    {
        // Associate with private implementation, creating if necessary
        // Function pointers in the backends are resolved at creation time
        QOpenGLVersionFunctionsBackend* d = 0;
        d = QAbstractOpenGLFunctionsPrivate::functionsBackend(context, QOpenGLFunctions_1_0_CoreBackend::versionStatus());
        if (!d) {
            d = new QOpenGLFunctions_1_0_CoreBackend(context);
            QAbstractOpenGLFunctionsPrivate::insertFunctionsBackend(context, QOpenGLFunctions_1_0_CoreBackend::versionStatus(), d);
        }
        d_1_0_Core = static_cast<QOpenGLFunctions_1_0_CoreBackend*>(d);
        d->refs.ref();

        d = QAbstractOpenGLFunctionsPrivate::functionsBackend(context, QOpenGLFunctions_1_0_DeprecatedBackend::versionStatus());
        if (!d) {
            d = new QOpenGLFunctions_1_0_DeprecatedBackend(context);
            QAbstractOpenGLFunctionsPrivate::insertFunctionsBackend(context, QOpenGLFunctions_1_0_DeprecatedBackend::versionStatus(), d);
        }
        d_1_0_Deprecated = static_cast<QOpenGLFunctions_1_0_DeprecatedBackend*>(d);
        d->refs.ref();

        QAbstractOpenGLFunctions::initializeOpenGLFunctions();
    }
    return isInitialized();
}

bool QOpenGLFunctions_1_0::isContextCompatible(QOpenGLContext *context)
{
    Q_ASSERT(context);
    QSurfaceFormat f = context->format();
    const QPair<int, int> v = qMakePair(f.majorVersion(), f.minorVersion());
    if (v < qMakePair(1, 0))
        return false;

    if (f.profile() == QSurfaceFormat::CoreProfile)
        return false;

    return true;
}

QOpenGLVersionProfile QOpenGLFunctions_1_0::versionProfile()
{
    QOpenGLVersionProfile v;
    v.setVersion(1, 0);
    return v;
}

QT_END_NAMESPACE
