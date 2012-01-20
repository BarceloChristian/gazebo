/****************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 **
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of a Qt Solutions component.
 **
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 **
 ****************************************************************************/


#ifndef QTGROUPBOXPROPERTYBROWSER_H
#define QTGROUPBOXPROPERTYBROWSER_H

#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QGroupBox>

#include "qtpropertybrowser.h"

#if QT_VERSION >= 0x040400
QT_BEGIN_NAMESPACE
#endif

class QtGroupBoxPropertyBrowserPrivate;


class QT_QTPROPERTYBROWSER_EXPORT QtGroupBoxPropertyBrowser
: public QtAbstractPropertyBrowser
{
  Q_OBJECT
  public:

    explicit QtGroupBoxPropertyBrowser(QWidget *parent = 0);
    ~QtGroupBoxPropertyBrowser();

  protected:
    virtual void itemInserted(QtBrowserItem *item, QtBrowserItem *afterItem);
    virtual void itemRemoved(QtBrowserItem *item);
    virtual void itemChanged(QtBrowserItem *item);

  private:

    QtGroupBoxPropertyBrowserPrivate *d_ptr;
    Q_DECLARE_PRIVATE(QtGroupBoxPropertyBrowser)
      Q_DISABLE_COPY(QtGroupBoxPropertyBrowser)
      Q_PRIVATE_SLOT(d_func(), void slotUpdate())
      Q_PRIVATE_SLOT(d_func(), void slotEditorDestroyed())
};

class QtGroupBoxPropertyBrowserPrivate
{
  QtGroupBoxPropertyBrowser *q_ptr;
  Q_DECLARE_PUBLIC(QtGroupBoxPropertyBrowser)
  public:
    QtGroupBoxPropertyBrowserPrivate() : q_ptr(NULL), m_mainLayout(NULL) {}

    void init(QWidget *parent);

    void propertyInserted(QtBrowserItem *index, QtBrowserItem *afterIndex);
    void propertyRemoved(QtBrowserItem *index);
    void propertyChanged(QtBrowserItem *index);
    QWidget *createEditor(QtProperty *property, QWidget *parent) const
    { return q_ptr->createEditor(property, parent); }

    void slotEditorDestroyed();
    void slotUpdate();

    struct WidgetItem
    {
      WidgetItem() : widget(0), label(0), widgetLabel(0),
      groupBox(0), layout(0), line(0), parent(0)
      { }
      QWidget *widget;  // can be null
      QLabel *label;
      QLabel *widgetLabel;
      QGroupBox *groupBox;
      QGridLayout *layout;
      QFrame *line;
      WidgetItem *parent;
      QList<WidgetItem *> children;
    };
  private:
    void updateLater();
    void updateItem(WidgetItem *item);
    void insertRow(QGridLayout *layout, int row) const;
    void removeRow(QGridLayout *layout, int row) const;

    bool hasHeader(WidgetItem *item) const;

    QMap<QtBrowserItem *, WidgetItem *> m_indexToItem;
    QMap<WidgetItem *, QtBrowserItem *> m_itemToIndex;
    QMap<QWidget *, WidgetItem *> m_widgetToItem;
    QGridLayout *m_mainLayout;
    QList<WidgetItem *> m_children;
    QList<WidgetItem *> m_recreateQueue;
};

#if QT_VERSION >= 0x040400
QT_END_NAMESPACE
#endif

#endif
