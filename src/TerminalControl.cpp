/*
 * TerminalControl.cpp
 *
 *  Created on: Dec 6, 2014
 *      Author: matt
 */

#include "TerminalControl.h"

#include <bb/cascades/Container>
#include <bb/cascades/Label>
#include <bb/cascades/ScrollView>
#include <bb/cascades/ScrollAnimation>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/TextStyle>

using namespace bb::cascades;

TerminalControl::TerminalControl(Container *parent)
    : CustomControl(parent)
{
    m_rootContainer = new Container();
    m_scrollView = new ScrollView();
    m_label = new Label();

    m_label->setMultiline(true);

    TextStyle *monospaceStyle = new TextStyle(SystemDefaults::TextStyles::bodyText());
    monospaceStyle->setFontFamily("\"DejaVu Sans Mono\", monospace");

    m_label->textStyle()->setBase(*monospaceStyle);

    m_scrollView->setContent(m_label);
    m_rootContainer->add(m_scrollView);

    setRoot(m_rootContainer);
}


TerminalControl::~TerminalControl()
{
    // TODO Auto-generated destructor stub
}

void TerminalControl::update(QString data)
{
    m_label->setText(data);

    // Dirty hack. Do better later.
    m_scrollView->scrollToPoint(0, 999999, ScrollAnimation::Smooth);
}
