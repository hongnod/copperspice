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

#ifndef QSCRIPTABLE_P_H
#define QSCRIPTABLE_P_H



class QScriptable;

class QScriptablePrivate
{
   Q_DECLARE_PUBLIC(QScriptable)

 public:
   inline QScriptablePrivate()
      : engine(0) {
   }

   static inline QScriptablePrivate *get(QScriptable *q) {
      return q->d_func();
   }

   QScriptEngine *engine;

   QScriptable *q_ptr;
};



#endif
