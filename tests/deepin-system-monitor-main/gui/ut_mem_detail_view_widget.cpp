/*
* Copyright (C) 2019 ~ 2021 Uniontech Software Technology Co.,Ltd.
*
* Author:     yukuan  <yukuan@uniontech.com>
*
* Maintainer: yukuan  <yukuan@uniontech.com>
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

//Self
#include "mem_detail_view_widget.h"

//gtest
#include "stub.h"
#include <gtest/gtest.h>
#include "addr_pri.h"

//Qt
#include <QSignalSpy>

/***************************************STUB begin*********************************************/

/***************************************STUB end**********************************************/

class UT_MemDetailViewWidget : public ::testing::Test
{
public:
    UT_MemDetailViewWidget() : m_tester(nullptr) {}

public:
    virtual void SetUp()
    {
        static QWidget widget;
        m_tester = new MemDetailViewWidget(&widget);
    }

    virtual void TearDown()
    {
        delete m_tester;
    }

protected:
    MemDetailViewWidget *m_tester;
};

TEST_F(UT_MemDetailViewWidget, initTest)
{

}

TEST_F(UT_MemDetailViewWidget, test_detailFontChanged_01)
{
    QFont font;
    m_tester->detailFontChanged(font);
}


TEST_F(UT_MemDetailViewWidget, test_onModelUpdate_01)
{
    m_tester->onModelUpdate();
}
