#include "AddCourseDlg.h"
#include "CoursesWnd.h"
#include "DataManager.h"
#include "CourseDetailsWnd.h"

QString strGetIntroductionURL("http://mobile.icourses.cn/hep-mobile/sword/app/share/intro/getIntroduction");

int loadIndex = 0;
QStringList resUrl;
QStringList resSavingPaths;
QStringList resFriendlyName;
#define RES_FILE_COUNT 4

AddCourseDialog::AddCourseDialog(QWidget *parent) : QDialog(parent),
													courseIntroductionReply(nullptr),
													logoDownloadReply(nullptr),
													resLoadingReply(nullptr){
	ui.setupUi(this);

	resUrl/*<<<<<<<<<<<<<<<*/<<"http://mobile.icourses.cn/hep-mobile/sword/app/share/detail/getCharacters"
	/*<<<<<<<<<<<<<<<<<<<<<*/<<"http://mobile.icourses.cn/hep-mobile/sword/app/share/detail/getExcercises"
	/*<<<<<<<<<<<<<<<<<<<<<*/<<"http://mobile.icourses.cn/hep-mobile/sword/app/share/detail/getExam"
	/*<<<<<<<<<<<<<<<<<<<<<*/<<"http://mobile.icourses.cn/hep-mobile/sword/app/share/detail/getOtherRes";
	resSavingPaths/*<<<<<<<*/<<"CoursesData\\%1_charpters.json"
	/*<<<<<<<<<<<<<<<<<<<<<*/<<"CoursesData\\%1_exercises.json"
	/*<<<<<<<<<<<<<<<<<<<<<*/<<"CoursesData\\%1_testPapers.json"
	/*<<<<<<<<<<<<<<<<<<<<<*/<<"CoursesData\\%1_otherRes.json";
	resFriendlyName/*<<<<<<*/<<"course charpters"
	/*<<<<<<<<<<<<<<<<<<<<<*/<<"exercises"
	/*<<<<<<<<<<<<<<<<<<<<<*/<<"test papers"
	/*<<<<<<<<<<<<<<<<<<<<<*/<<"other resources";

	//setAttribute(Qt::WA_ShowModal, true);//属性设置
	//setWindowModality(Qt::ApplicationModal);//设置阻塞类型
}

void AddCourseDialog::onDataProcessedError(QString tips){
	ui.tv_status->setText(tips);
	QMessageBox::warning(this, "Failed.", tips, QMessageBox::Ok);
	setDialogEnabled(true);
	execResult = ResultCode::Failed;
}


void AddCourseDialog::btn_add(){
	if(!ui.edit_IDorURL->text().trimmed().isEmpty()){
		setDialogEnabled(false);
		loadIndex = 0;

		QNetworkRequest request(strGetIntroductionURL);
		request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");
		QString postDataStr = QString("courseId=%1").arg(ui.edit_IDorURL->text().toInt());
		
		courseIntroductionReply = HTTPManager::post(request, postDataStr.toLatin1());
		connect(
			courseIntroductionReply, &QNetworkReply::finished,
			this, &AddCourseDialog::onCourseIntroRequestFinished);
		ui.tv_status->setText("Loading introduction info...");
	}
}

void AddCourseDialog::onCourseIntroRequestFinished(){
	if (courseIntroductionReply->error() != QNetworkReply::NoError){
		onDataProcessedError("Network error.");
		return;
	}

	QByteArray replyData = courseIntroductionReply->readAll();
	//qDebug()<<replyData.data()<<endl;

	QJsonParseError jsonErr;
	QJsonDocument jsonDoc = QJsonDocument::fromJson(replyData.data(), &jsonErr);

	if(jsonDoc.isNull()){
		onDataProcessedError("JSON callback is empty.");		
		return;
	}

	if(jsonErr.error != QJsonParseError::NoError)	{
		onDataProcessedError("QJsonParseError parse error.");
		return;
	}
	if(!jsonDoc.isObject())	{
		onDataProcessedError("JSON format error.");
		return;
	}

	QJsonObject rootObj = jsonDoc.object();
	if(!rootObj.contains("success")){
		onDataProcessedError("JSON format error.");
		return;
	}

	if(!rootObj.value("success").toBool()){
		if(rootObj.contains("message")){
			onDataProcessedError(rootObj.value("message").toString());
		}else{
			onDataProcessedError("UnKnown error occured.");
		}
		return;
	}

	if(!rootObj.contains("data")||!rootObj.value("data").isObject()){
		onDataProcessedError("JSON format error.");
		return;
	}

	QJsonObject data = rootObj.value("data").toObject();
	if(!data.value("base").isObject()||!data.value("school").isObject()||!data.value("mainTeacher").isArray()){
		onDataProcessedError("JSON format error.");
		return;
	}

	QJsonObject base = data.value("base").toObject();
	QJsonObject school = data.value("school").toObject();
	QJsonArray mainTeacherObj = data.value("mainTeacher").toArray();
	if(mainTeacherObj.isEmpty()){
		onDataProcessedError("JSON format error.");
		return;
	}
	QJsonObject mainTeacher = mainTeacherObj.at(0).toObject();

	newCourse = new Course();
	
	if(school.contains("schoolName")){
		newCourse->school = school.value("schoolName").toString();
	}
	if(base.contains("id")){
		newCourse->id = base.value("id").toString().toInt();
	}
	if(base.contains("title")){
		newCourse->title = base.value("title").toString();
	}
	if(base.contains("fullPicUrl")){
		newCourse->logoPath = base.value("fullPicUrl").toString();
	}
	if(mainTeacher.contains("realname")){
		newCourse->teacher = mainTeacher.value("realname").toString();
	}

	loadCourseLogo();
}

void inline AddCourseDialog::loadCourseLogo(){
	ui.tv_status->setText("Loading course logo...");
	logoDownloadReply  = HTTPManager::get(newCourse->logoPath);
	QObject::connect(logoDownloadReply, &QNetworkReply::finished,this, &AddCourseDialog::loadLogoFinished);
}


void AddCourseDialog::loadLogoFinished(){
	if (courseIntroductionReply->error() != QNetworkReply::NoError){
		onDataProcessedError("Load logo failed.");
		return;
	}
	QString imgSavePath = QString("CourseLogos\\LOGO_%1.jpg").arg(newCourse->id);//TODO: replace to the actual type

	if(!StorageManager::saveFile(logoDownloadReply, imgSavePath)){
		onDataProcessedError("Save logo file failed.");
		return;
	}
	newCourse->logoLocalPath = imgSavePath;
	
	Q_ASSERT(resLoadingReply == nullptr);
	loadResources();
}

void AddCourseDialog::loadResources(){

	ui.tv_status->setText(QString("Loading %1...").arg(resFriendlyName.at(loadIndex)));

	if(resLoadingReply != nullptr)	{
		if (resLoadingReply->error() != QNetworkReply::NoError){
			onDataProcessedError("Res loaded failed.");
			return;
		}

		QString resSavingPath = QString(resSavingPaths.at(loadIndex)).arg(newCourse->id);

		newCourse->resLocalPaths[loadIndex] = resSavingPath;

		if(!StorageManager::saveFile(resLoadingReply, resSavingPath)){
			onDataProcessedError(QString("Save %1 failed.").arg(resFriendlyName.at(loadIndex)));
			return;
		}
		loadIndex++;

		delete resLoadingReply;
		resLoadingReply = nullptr;
		if(loadIndex >= RES_FILE_COUNT){
			loadingFinished();
			return;
		}
	}
	
	QNetworkRequest request(resUrl.at(loadIndex));
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded;charset=utf-8");
	QString postDataStr = QString("subjectType=1&courseId=%1").arg(newCourse->id);
		
	resLoadingReply = HTTPManager::post(request, postDataStr.toLatin1());
	connect(
		resLoadingReply, &QNetworkReply::finished,
		this, &AddCourseDialog::loadResources);
}

void AddCourseDialog::loadingFinished(){
	execResult = ResultCode::Added;
	close();
}

void AddCourseDialog::showEvent(QShowEvent* event)
{
	setDialogEnabled(true);
	ui.edit_IDorURL->clear();
	ui.tv_status->clear();
	execResult = ResultCode::Failed;
	newCourse = nullptr;
}

void AddCourseDialog::closeEvent(QCloseEvent* event)
{
	qDebug()<<QString("%1").arg(result());
}

void AddCourseDialog::setDialogEnabled(bool e){
	if(e){
		ui.btn_add->setText("Add");
	}else{
		ui.btn_add->setText("Wait...");
	}
	setEnabled(e);
}
