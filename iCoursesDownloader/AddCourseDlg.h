#ifndef ADDCOURSEDLG_H
#define ADDCOURSEDLG_H

#include <QtWidgets/QDialog>
#include <QCloseEvent>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "ui_add_course.h"

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMessageBox>
#include <QDebug>

#include "datatypes.h"
#include "HttpManager.h"

class HTTPManager;
class Course;

class AddCourseDialog : public QDialog
{
    Q_OBJECT

public:
    enum ResultCode {
        Added,
        Failed
    };
    ResultCode execResult;
    AddCourseDialog(QWidget *parent = nullptr);
    Course *newCourse;//nullptr if failed
protected:
    void showEvent(QShowEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
protected slots:
    void btn_add();
    void onCourseIntroRequestFinished();
    void loadResources();
private:
    void inline loadingFinished();
    void onDataProcessedError(QString tips);
    void loadCourseLogo();
    void loadLogoFinished();
    void setDialogEnabled(bool e);
    Ui::AddCourseDialog ui;
    QNetworkReply *courseIntroductionReply;
    QNetworkReply *logoDownloadReply;
    QNetworkReply *resLoadingReply;

};

#endif // ADDCOURSEDLG_H
