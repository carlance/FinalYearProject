#ifndef MAINWIDGET_H
#define MAINWIDGET_H
#include <QString>
#include <QComboBox>
#include <QWidget>
#include <QObject>
#include <string>


#include <pthread.h>
using namespace std;
//Message structure to send message to Scheduler Thread
typedef struct{
	int value;
	int offsetValue;
	std::string note;
}key;

extern  key key1, key2,key3,key4,key5,key6,key7,key8,key9,key10,key11,key12;
extern  key key13, key14,key15,key16,key17,key18,key19,key20,key21,key22,key23,key24;
extern pthread_mutex_t mutex;
extern int chanid2,status1;
extern pid_t pid1;


namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();
    
private slots:

    void on_toolButton_7_pressed();
    void on_toolButton_7_released();

    void on_toolButton_8_pressed();
    void on_toolButton_8_released();

    void on_toolButton_18_pressed();
    void on_toolButton_18_released();

    void on_toolButton_17_pressed();
    void on_toolButton_17_released();

    void on_toolButton_19_pressed();
    void on_toolButton_19_released();

    void on_toolButton_20_pressed();  
    void on_toolButton_20_released();

    void on_toolButton_5_pressed();
    void on_toolButton_5_released();

    void on_toolButton_21_pressed();
    void on_toolButton_21_released();

    void on_toolButton_22_pressed();
    void on_toolButton_22_released();

    void on_toolButton_1_released();
    void on_toolButton_1_pressed();

    void on_toolButton_23_pressed();
    void on_toolButton_23_released();

    void on_toolButton_24_pressed();
    void on_toolButton_24_released();

    void on_toolButton_25_pressed();
    void on_toolButton_25_released();

    void on_toolButton_26_pressed();
    void on_toolButton_26_released();

    void on_toolButton_32_pressed();
    void on_toolButton_32_released();

    void on_toolButton_30_pressed();
    void on_toolButton_30_released();

    void on_toolButton_14_pressed();
    void on_toolButton_14_released();

    void on_toolButton_28_pressed();
    void on_toolButton_28_released();

    void on_toolButton_27_pressed();
    void on_toolButton_27_released();

    void on_toolButton_33_pressed();
    void on_toolButton_33_released();

    void on_toolButton_15_pressed();
    void on_toolButton_15_released();

    void on_toolButton_31_pressed();
    void on_toolButton_31_released();

    void on_toolButton_13_pressed();
    void on_toolButton_13_released();

    void on_toolButton_29_pressed();
    void on_toolButton_29_released();

    void on_toolButton_34_pressed();
    void on_toolButton_34_released();

    void on_toolButton_4_pressed();

    void on_toolButton_3_pressed();

    void on_toolButton_9_pressed();
    void on_toolButton_9_released();

    void on_LoadSound_clicked();

    void on_LoadBeatBox_clicked();

    void on_dial_1_valueChanged();

private:
    Ui::MainWidget *ui;
};

#endif // MAINWIDGET_H

