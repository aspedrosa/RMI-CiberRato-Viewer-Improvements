/*
    This file is part of ciberRatoToolsSrc.

    Copyright (C) 2001-2011 Universidade de Aveiro

    ciberRatoToolsSrc is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    ciberRatoToolsSrc is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "crqdataview.h"
#include "ui_crqdataview.h"


#include<stdio.h>

#include "QtCore"
#include "QtGui"

CRQDataView::CRQDataView(CRReply *r, CRLab *l, QString skinFName, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CRQDataView)
{
    ui->setupUi(this);

#ifdef DEBUG
    cout << "CRQDataView::CRQDataView\n";
#endif

    lab = l;
    reply = r;
    status = 1;
    skin(skinFName);

    ui->label_Lab->setText(QString(lab->labName()));
    finalTime = reply->parameters->simTime;
    curTime = 0; // Current time
    nRobots = 5; // Number of robots by default

    ui->lcdNumber_Time->display(finalTime);
    connect( this, SIGNAL( timeRemaining(int) ), ui->lcdNumber_Time, SLOT( display(int) ) );

    // Robots processing
    if( lab->grid() != NULL )		// if grid exist
        nRobots = lab->grid()->howManyPositions();

    usedId = new int[nRobots];

    for ( int n = 0; n < nRobots; n++ )
        usedId[n] = 0;

    robots.resize( nRobots ); // robot is one vector of CRQRobotInfo

    adjustSize();
}

CRQDataView::~CRQDataView()
{

#ifdef DEBUG
    cout << "CRQDataView::~CRQDataView\n";
#endif

    if(usedId != NULL)
        delete usedId;

    for(int i = 0 ; i < 3 ; i++)
        if(robots[i] != NULL)
            delete robots[i];

    robots.clear();

    delete ui;
}

QTreeWidget* CRQDataView::getFilterTreeWidget() {
    return ui->treeWidget;
}

void CRQDataView::addItem(QString item){
    auto item_splitted = item.split(".");

    QTreeWidgetItem *topLevelItem = NULL;
    QTreeWidgetItem *subLevelItem = NULL;
    for(int i=0; i<item_splitted.length(); i++){
        if(i==0){
            auto item_list = ui->treeWidget->findItems(item_splitted[i], Qt::MatchExactly);
            if(item_list.length() > 0){
                topLevelItem = item_list[0];
            }
            if(topLevelItem == NULL){
                // Create new item (top level item)
                topLevelItem = new QTreeWidgetItem(ui->treeWidget);
                topLevelItem->setText(0,item_splitted[i]);
                topLevelItem->setCheckState(0, Qt::Checked);
                topLevelItem->setExpanded(true);
                ui->treeWidget->addTopLevelItem(topLevelItem);
            }
        }
        else{
            auto item_list = ui->treeWidget->findItems(item_splitted[i], Qt::MatchExactly | Qt::MatchRecursive);
            if(item_list.length() > 0){
                for(auto & j : item_list){
                    if(j->parent() == topLevelItem){
                        subLevelItem = j;
                        break;
                    }
                }
            }
            if(subLevelItem == NULL){
                subLevelItem = new QTreeWidgetItem(topLevelItem);
                subLevelItem->setText(0,item_splitted[i]);  // Set text for item
                subLevelItem->setCheckState(0, Qt::Checked);
                subLevelItem->setExpanded(true);
            }
            topLevelItem = subLevelItem;
            subLevelItem = NULL;
        }
    }
}

void CRQDataView::removeItem(QString item) {
    auto item_splitted = item.split(".");

    QTreeWidgetItem *topLevelItem = NULL;
    QTreeWidgetItem *subLevelItem = NULL;
    int i;
    for(i=0; i<item_splitted.length(); i++){
        if(i==0){
            auto item_list = ui->treeWidget->findItems(item_splitted[i], Qt::MatchExactly);
            topLevelItem = item_list[0];
        }
        else{
            auto item_list = ui->treeWidget->findItems(item_splitted[i], Qt::MatchExactly | Qt::MatchRecursive);
            if(item_list.length() > 0){
                for(auto & j : item_list){
                    if(j->parent() == topLevelItem){
                        subLevelItem = j;
                        break;
                    }
                }
            }
            topLevelItem = subLevelItem;
            subLevelItem = NULL;
        }
    }

    for(i=0; i<item_splitted.length(); i++){
        subLevelItem = topLevelItem;
        topLevelItem = subLevelItem->parent();
        if (topLevelItem == NULL || topLevelItem->childCount() > 1) {
            delete subLevelItem;
            break;
        }
    }
}

void CRQDataView::skin(QString skinFileName)
{
    FILE *fp = NULL;

    //cout << "CRQDataView::skin skin name = " << skinFileName << endl;

    fp = fopen(skinFileName.toAscii(), "r");
    if(fp == NULL)
    {
        cerr << "Unable to load file " << skinFileName.toStdString() << endl;
    }
    else
    {
        fscanf(fp, "%s", skinName);
        strcpy(backFile, "skins/");
        strcat(backFile, skinName);
        strcat(backFile, "/scorebg.png");

        strcpy(backFile2, "skins/");
        strcat(backFile2, skinName);
        strcat(backFile2, "/scorebg2.png");

        strcpy(clockImage, "skins/");
        strcat(clockImage, skinName);
        strcat(clockImage, "/clock.png");
    }

    fclose(fp);
}

void CRQDataView::deleteTreeWidget() {
   delete ui->treeWidget;
}

void CRQDataView::update( CRRobot *rob )
{
    if ( (rob->id() > 0) && (rob->id() < (nRobots + 1) ) )
    {
        if( usedId[ rob->id() - 1] == 0 )
        {
            CRQRobotInfo *robot = new CRQRobotInfo( rob, skinName, this);
            robots[rob->id() - 1] = robot; //Add new robot to vector
            usedId[rob->id() - 1] = 1;	   //Update the var used
            //robot->setGeometry( 279, (rob->id()-1) * 34 + 12, 715, 80 );
            ui->gridLayout_Robots->addWidget(robot, rob->id()-1, 0, Qt::AlignTop);
            curTime = rob->currentTime();
            emit timeRemaining(finalTime - rob->currentTime() );
            robot->show();
            robots[rob->id() - 1]->update( rob );
        }
        else
        {
            robots[rob->id() - 1]->update( rob );
            curTime = rob->currentTime();
            robStatus = rob->state();
            emit timeRemaining(finalTime - rob->currentTime());
        }

        if(curTime % 5 == 0)
            QWidget::update();
        }
}
