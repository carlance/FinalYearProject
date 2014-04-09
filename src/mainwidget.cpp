#include "mainwidget.h"
#include "ui_mainwidget.h"
#include <pthread.h>
#include <bps/audiomixer.h>
#include <mm/renderer.h>
#include<mqueue.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <QMessageBox>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <string>
using namespace std;
float volume;
bool volumechange;

char path1[] = {"app/native/assets/pianoc.wav"};
char path2[] = {"app/native/assets/pianoc.wav"};
char path3[] = {"app/native/assets/testloop.wav"};

char* WAV_RELATIVE_PATH = path1;
char* WAV_RELATIVE_PATH_2 = path2;
char* WAV_RELATIVE_PATH_3 = path3;

bool changepath;

//setup key data structure
key key1, key2,key3,key4,key5,key6,key7,key8,key9,key10,key11,key12;
key key13, key14,key15,key16,key17,key18,key19,key20,key21,key22,key23,key24,key25;
pthread_mutex_t mutex;

key KeyMsg;

//declare variable for message queue
char* replybuff1[50];
int sbyte1 = sizeof(KeyMsg);
int rbyte1 = sizeof(replybuff1);
pid_t pid1=0;
int chanid2;
int status1;
int coid1;

MainWidget::MainWidget(QWidget *parent) :QWidget(parent), ui(new Ui::MainWidget)
{
    ui->setupUi(this);
    audiomixer_get_output_level(AUDIOMIXER_OUTPUT_SPEAKER, &volume);
    ui->dial_1->setValue(volume);
    key1.note="c",key2.note="c#", key3.note="d", key4.note="d#", key5.note="e", key6.note="f", key7.note="f#", key8.note="g";
	key9.note="g#", key10.note="a", key11.note="a#", key12.note="b", key13.note="hc", key14.note="hc#", key15.note="hd", key16.note="hd#";
	key17.note="he", key18.note="hf", key19.note="hf#", key20.note="hg", key21.note="hg#", key22.note="ha", key23.note="ha#", key24.note="hb" , key25.note="hcc";

	/*key1.offsetValue=0,key2.offsetValue=1, key3.offsetValue=2, key4.offsetValue=3, key5.offsetValue=4, key6.offsetValue=5, key7.offsetValue=6, key8.offsetValue=7;
	key9.offsetValue=8, key10.offsetValue=9, key11.offsetValue=10, key12.offsetValue=11, key13.offsetValue=0, key14.offsetValue=1, key15.offsetValue=2, key16.offsetValue=3;
	key17.offsetValue=4, key18.offsetValue=5, key19.offsetValue=6, key20.offsetValue=7, key21.offsetValue=8, key22.offsetValue=9, key23.offsetValue=10, key24.offsetValue=11;*/

	key1.offsetValue=0,key2.offsetValue=1, key3.offsetValue=2, key4.offsetValue=3, key5.offsetValue=4, key6.offsetValue=5, key7.offsetValue=6, key8.offsetValue=7;
	key9.offsetValue=8, key10.offsetValue=9, key11.offsetValue=10, key12.offsetValue=11, key13.offsetValue=0, key14.offsetValue=1, key15.offsetValue=2, key16.offsetValue=3;
	key17.offsetValue=4, key18.offsetValue=5, key19.offsetValue=6, key20.offsetValue=7, key21.offsetValue=8, key22.offsetValue=9, key23.offsetValue=10, key24.offsetValue=11, key25.offsetValue=12;
	chanid2 = ChannelCreate(NULL);
	coid1 = ConnectAttach( 0 ,pid1, chanid2, 0 , NULL);
}

MainWidget::~MainWidget()
{
    delete ui;
}

void MainWidget::on_toolButton_7_pressed()
{   ui->toolButton_7->setIcon(QIcon("app/native/assets/guipicR.png")); //Set Icon for Button when Press
	ui->toolButton_7->setAutoRaise(false); //create a border for button
	pthread_mutex_lock(&mutex); //acquire resource
	KeyMsg = key4; //set message to key
	KeyMsg.value=1; //set key value to pressed
	//send message to Schuduler
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex); //release resource

	/* This is thesame for all other events*/
}

void MainWidget::on_toolButton_7_released()
{   ui->toolButton_7->setIcon(QIcon("app/native/assets/guipic2.png"));
	ui->toolButton_7->setAutoRaise(false);
	pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_5_pressed()
{
    ui->toolButton_5->setIcon(QIcon("app/native/assets/guipicR.png"));
    ui->toolButton_5->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key19;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_5_released()
{
    ui->toolButton_5->setIcon(QIcon("app/native/assets/guipic2.png"));
    ui->toolButton_5->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg.value=2;
    status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_8_pressed()
{
    ui->toolButton_8->setIcon(QIcon("app/native/assets/guipicR.png"));
    ui->toolButton_8->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key2;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);

}

void MainWidget::on_toolButton_8_released()
{
    ui->toolButton_8->setIcon(QIcon("app/native/assets/guipic2.png"));
    ui->toolButton_8->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_18_pressed()
{
	ui->toolButton_18->setIcon(QIcon("app/native/assets/frwhite.png"));
	ui->toolButton_18->setAutoRaise(false);
	pthread_mutex_lock(&mutex);
    KeyMsg = key1;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_18_released()
{
    ui->toolButton_18->setIcon(QIcon("app/native/assets/fpwhite.png"));
    ui->toolButton_18->setAutoRaise(false);
   pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}


void MainWidget::on_toolButton_21_pressed()
{
     ui->toolButton_21->setIcon(QIcon("app/native/assets/frwhite.png"));
     ui->toolButton_21->setAutoRaise(false);
     pthread_mutex_lock(&mutex);
    KeyMsg = key3;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_21_released()
{
    ui->toolButton_21->setIcon(QIcon("app/native/assets/fpwhite.png"));
    ui->toolButton_21->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_22_pressed()
{
      ui->toolButton_22->setIcon(QIcon("app/native/assets/frwhite.png"));
      ui->toolButton_22->setAutoRaise(false);
     pthread_mutex_lock(&mutex);
    KeyMsg = key5;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_22_released()
{
     ui->toolButton_22->setIcon(QIcon("app/native/assets/fpwhite.png"));
     ui->toolButton_22->setAutoRaise(false);
     pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_20_pressed()
{
	 ui->toolButton_20->setIcon(QIcon("app/native/assets/guipicR.png"));
	 ui->toolButton_20->setAutoRaise(false);
	 pthread_mutex_lock(&mutex);
    KeyMsg = key14;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_20_released()
{
	  ui->toolButton_20->setIcon(QIcon("app/native/assets/guipic2.png"));
	  ui->toolButton_20->setAutoRaise(false);
	  pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_17_pressed()
{
  ui->toolButton_17->setIcon(QIcon("app/native/assets/guipicR.png"));
  ui->toolButton_17->setAutoRaise(false);
  pthread_mutex_lock(&mutex);
    KeyMsg = key9;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_17_released()
{
  ui->toolButton_17->setIcon(QIcon("app/native/assets/guipic2.png"));
  ui->toolButton_17->setAutoRaise(false);
  pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_19_pressed()
{
    ui->toolButton_19->setIcon(QIcon("app/native/assets/guipicR.png"));
    ui->toolButton_19->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key7;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_19_released()
{
    ui->toolButton_19->setIcon(QIcon("app/native/assets/guipic2.png"));
    ui->toolButton_19->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_1_pressed()
{
     ui->toolButton_1->setIcon(QIcon("app/native/assets/guipicR.png"));
     ui->toolButton_1->setAutoRaise(false);
     pthread_mutex_lock(&mutex);
    KeyMsg = key11;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_1_released()
{
    ui->toolButton_1->setIcon(QIcon("app/native/assets/guipic2.png"));
    ui->toolButton_1->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_23_pressed()
{
    ui->toolButton_23->setIcon(QIcon("app/native/assets/frwhite.png"));
    ui->toolButton_23->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key6;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_23_released()
{
    ui->toolButton_23->setIcon(QIcon("app/native/assets/fpwhite.png"));
    ui->toolButton_23->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_24_pressed()
{
    ui->toolButton_24->setIcon(QIcon("app/native/assets/frwhite.png"));
    ui->toolButton_24->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key8;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_24_released()
{
   ui->toolButton_24->setIcon(QIcon("app/native/assets/fpwhite.png"));
   ui->toolButton_24->setAutoRaise(false);
   pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_25_pressed()
{
     ui->toolButton_25->setIcon(QIcon("app/native/assets/frwhite.png"));
     ui->toolButton_25->setAutoRaise(false);
     pthread_mutex_lock(&mutex);
    KeyMsg = key10;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_25_released()
{
     ui->toolButton_25->setIcon(QIcon("app/native/assets/fpwhite.png"));
     ui->toolButton_25->setAutoRaise(false);
     pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_26_pressed()
{
     ui->toolButton_26->setIcon(QIcon("app/native/assets/frwhite.png"));
     ui->toolButton_26->setAutoRaise(false);
     pthread_mutex_lock(&mutex);
    KeyMsg = key12;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_26_released()
{
     ui->toolButton_26->setIcon(QIcon("app/native/assets/fpwhite.png"));
     ui->toolButton_26->setAutoRaise(false);
     pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_32_pressed()
{
    ui->toolButton_32->setIcon(QIcon("app/native/assets/frwhite.png"));
    ui->toolButton_32->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key13;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_32_released()
{
    ui->toolButton_32->setIcon(QIcon("app/native/assets/fpwhite.png"));
    ui->toolButton_32->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_30_pressed()
{
    ui->toolButton_30->setIcon(QIcon("app/native/assets/frwhite.png"));
    ui->toolButton_30->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key15;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_30_released()
{
    ui->toolButton_30->setIcon(QIcon("app/native/assets/fpwhite.png"));
    ui->toolButton_30->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_14_pressed()
{
    ui->toolButton_14->setIcon(QIcon("app/native/assets/guipicR.png"));
    ui->toolButton_14->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key16;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_14_released()
{
    ui->toolButton_14->setIcon(QIcon("app/native/assets/guipic2.png"));
    ui->toolButton_14->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_28_pressed()
{
    ui->toolButton_28->setIcon(QIcon("app/native/assets/frwhite.png"));
    ui->toolButton_28->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key17;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_28_released()
{
    ui->toolButton_28->setIcon(QIcon("app/native/assets/fpwhite.png"));
    ui->toolButton_28->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_27_pressed()
{
    ui->toolButton_27->setIcon(QIcon("app/native/assets/frwhite.png"));
    ui->toolButton_27->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key18;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_27_released()
{
    ui->toolButton_27->setIcon(QIcon("app/native/assets/fpwhite.png"));
    ui->toolButton_27->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_33_pressed()
{
    ui->toolButton_33->setIcon(QIcon("app/native/assets/frwhite.png"));
    ui->toolButton_33->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key20;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_33_released()
{
    ui->toolButton_33->setIcon(QIcon("app/native/assets/fpwhite.png"));
    ui->toolButton_33->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_15_pressed()
{
    ui->toolButton_15->setIcon(QIcon("app/native/assets/guipicR.png"));
    ui->toolButton_15->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key21;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_15_released()
{
    ui->toolButton_15->setIcon(QIcon("app/native/assets/guipic2.png"));
    ui->toolButton_15->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_31_pressed()
{
    ui->toolButton_31->setIcon(QIcon("app/native/assets/frwhite.png"));
    ui->toolButton_31->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key22;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_31_released()
{
    ui->toolButton_31->setIcon(QIcon("app/native/assets/fpwhite.png"));
    ui->toolButton_31->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_13_pressed()
{
    ui->toolButton_13->setIcon(QIcon("app/native/assets/guipicR.png"));
    ui->toolButton_13->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key23;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_13_released()
{
    ui->toolButton_13->setIcon(QIcon("app/native/assets/guipic2.png"));
    ui->toolButton_13->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_29_pressed()
{
    ui->toolButton_29->setIcon(QIcon("app/native/assets/frwhite.png"));
    ui->toolButton_29->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key24;
    KeyMsg.value=1;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_29_released()
{
    ui->toolButton_29->setIcon(QIcon("app/native/assets/fpwhite.png"));
    ui->toolButton_29->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
	pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_34_pressed()
{
    ui->toolButton_34->setIcon(QIcon("app/native/assets/frwhite.png"));
    ui->toolButton_34->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
    KeyMsg = key25;
   	KeyMsg.value=1;
   	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
   	pthread_mutex_unlock(&mutex);
    
}

void MainWidget::on_toolButton_34_released()
{
    ui->toolButton_34->setIcon(QIcon("app/native/assets/fpwhite.png"));
    ui->toolButton_34->setAutoRaise(false);
    pthread_mutex_lock(&mutex);
	KeyMsg.value=2;
   	status1 = MsgSend(coid1, (const char*) &KeyMsg, sbyte1, replybuff1, rbyte1);
    pthread_mutex_unlock(&mutex);
}

void MainWidget::on_toolButton_4_pressed()
{
    ui->toolButton_4->setIcon(QIcon("app/native/assets/playicon2.png"));
    ui->toolButton_3->setIcon(QIcon("app/native/assets/stopicon1.png"));
}

void MainWidget::on_toolButton_3_pressed()
{
    ui->toolButton_3->setIcon(QIcon("app/native/assets/stopicon1.png"));
    ui->toolButton_4->setIcon(QIcon("app/native/assets/playicon1.png"));
    ui->toolButton_9->setIcon(QIcon("app/native/assets/recordicon.png"));
}

void MainWidget::on_toolButton_9_pressed()
{
    ui->toolButton_9->setIcon(QIcon("app/native/assets/recordicon2.png"));
    ui->toolButton_3->setIcon(QIcon("app/native/assets/stopicon1.png"));
    ui->toolButton_9->setAutoRaise(false);
}

void MainWidget::on_toolButton_9_released()
{
	ui->toolButton_9->setIcon(QIcon("app/native/assets/recordicon1.png"));
	ui->toolButton_9->setAutoRaise(false);

}

void MainWidget::on_LoadSound_clicked()
{

    if (ui->comboBox->currentText() == "Piano")
    { strcpy(path1, "app/native/assets/pianoc.wav");
       WAV_RELATIVE_PATH = path1;
       changepath = true;
    }
    else if (ui->comboBox->currentText() == "Guitar")
    { strcpy(path1,"app/native/assets/guitar.wav");
      WAV_RELATIVE_PATH = path1;
      changepath = true;
    }
    else if (ui->comboBox->currentText() == "Violin")
    { strcpy(path1,"app/native/assets/violinc.wav");
      WAV_RELATIVE_PATH = path1;
      changepath = true;
    }
    else if (ui->comboBox->currentText() == "Synth")
    { strcpy(path1,"app/native/assets/C4loop.wav");
      WAV_RELATIVE_PATH = path1;
      changepath = true;
    }

    if (ui->comboBox_2->currentText() == "Piano")
    { strcpy(path2,"app/native/assets/pianoc.wav");
      WAV_RELATIVE_PATH_2 = path2;
      changepath = true;
    }
    else if (ui->comboBox_2->currentText() == "Guitar")
    { strcpy(path2,"app/native/assets/guitar.wav");
      WAV_RELATIVE_PATH_2 = path2;
      changepath = true;
    }
    else if (ui->comboBox_2->currentText() == "Violin")
    { strcpy(path2,"app/native/assets/violinc.wav");
      WAV_RELATIVE_PATH_2 = path2;
      changepath = true;
    }
    else if (ui->comboBox_2->currentText() == "Synth")
    { strcpy(path2,"app/native/assets/C4loop.wav");
      WAV_RELATIVE_PATH_2 = path2;
      changepath = true;
    }

    QMessageBox::information(this,"path description", path1);
    QMessageBox::information(this,"path description", path2);
}

void MainWidget::on_LoadBeatBox_clicked()
{
	int y3;

    if (ui->comboBox_3->currentText() == "Beatbox1")
    { strcpy(path3, "app/native/beatbox1.wav");
      //y3 = snprintf(WAV_RELATIVE_PATH_3, PATH_MAX, "%s",path3);
    }
    else if (ui->comboBox_3->currentText() == "Beatbox2")
    { strcpy(path3, "app/native/beatbox2.wav");
      y3 = snprintf(WAV_RELATIVE_PATH_3, PATH_MAX, "%s",path3);
    }
    else if (ui->comboBox_3->currentText() == "Beatbox3")
    { strcpy(path3, "app/native/beatbox3.wav");
      y3 = snprintf(WAV_RELATIVE_PATH_3, PATH_MAX, "%s",path3);
    }
    else if (ui->comboBox_3->currentText() == "Beatbox4")
    { strcpy(path3, "app/native/beatbox4.wav");
      y3 = snprintf(WAV_RELATIVE_PATH_3, PATH_MAX, "%s",path3);
    }
    else if (ui->comboBox_3->currentText() == "Beatbox5")
    { strcpy(path3,"app/native/beatbox5.wav");
      y3 = snprintf(WAV_RELATIVE_PATH_3, PATH_MAX, "%s",path3);
    }

    QMessageBox::information(this,"path description", path3);
}

void MainWidget::on_dial_1_valueChanged()
{
	volume = ui->dial_1->value();
    ui->progressBar->setValue(ui->dial_1->value());
    audiomixer_set_output_level(AUDIOMIXER_OUTPUT_SPEAKER, volume);
    volumechange = true;
}



