
#include "samplewidget.h"
#include "global.h"

SampleWidget::SampleWidget(QWidget *parent)
    : QWidget(parent) {}

SampleWidget::~SampleWidget() {
}

void SampleWidget::SetBaseConfiguration(QPushButton *button, QStorageInfo dDisk, QStorageInfo sDisk, int butId) {
	this->resize(200, 200);
	this->setWindowTitle(button->text());

	sampleButton = button;
    buttonId = butId;
	downloadDisk = dDisk;
	searchDisk   = sDisk;

	QVBoxLayout *layout       = new QVBoxLayout(this);
	QPushButton *renameButton = new QPushButton("Rename");
	connect(renameButton, &QPushButton::clicked, this, &SampleWidget::RenameButtonClicked);
	renameButton->setStyleSheet("QPushButton { border-radius: 9px;  border: 2px solid black; background-color: " + ButtonColor + ";} \
    QPushButton:hover { background-color: orange; } \
    QPushButton:pressed { background-color: silver; }");
	layout->addWidget(renameButton);

	QPushButton *addFileButton = new QPushButton("Add File");
	connect(addFileButton, &QPushButton::clicked, this, &SampleWidget::AddFileButtonClicked);
	addFileButton->setStyleSheet("QPushButton { border-radius: 9px;  border: 2px solid black; background-color: " + ButtonColor + ";} \
    QPushButton:hover { background-color: orange; } \
    QPushButton:pressed { background-color: silver; }");

	layout->addWidget(addFileButton);

	SearchSuitableMP3Files();

	listWidgetMain = new QListWidget();
	foreach (QFileInfo fileInfo, files) {
		QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
		listWidgetMain->addItem(item);
	}
	layout->addWidget(listWidgetMain);
}

void SampleWidget::SearchSuitableMP3Files() {
	QDir dir(downloadDisk.rootPath());
	QStringList filters;
	filters << sampleButton->text() + ".mp3";
	files = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
}

void SampleWidget::RenameButtonClicked() {
	QInputDialog inputDialog;
	QString text = QInputDialog::getText(this, tr("Rename Preset"), tr("New name(Without .mp3):"), QLineEdit::Normal);
	if (!text.isEmpty()) {
        QSettings settings("config.ini", QSettings::IniFormat);
        settings.setValue(QString("button%1").arg(buttonId), text);
		sampleButton->setText(text);
		foreach (QFileInfo fileInfo, files) {
			QString newFileName = fileInfo.absolutePath() + "/" + text + "." + fileInfo.suffix();
			QFile::rename(fileInfo.absoluteFilePath(), newFileName);
		}
	}
}

void SampleWidget::AddFileButtonClicked() {
	QDialog *dialog = new QDialog(this);

	dialog->setWindowTitle("Add File");
	dialog->resize(150, 150);

	listWidgetChild        = new QListWidget(dialog);
	QPushButton *addButton = new QPushButton(dialog);

	addButton->setText("Add");
	connect(addButton, &QPushButton::clicked, this, &SampleWidget::AddButtonClicked);
	addButton->setGeometry(25, 5, 100, 30);
	listWidgetChild->setGeometry(0, 40, 150, 150);

	QDir dir(searchDisk.rootPath());
	QStringList filters;
	filters << "*.mp3";
	files = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

	foreach (QFileInfo fileInfo, files) {
		QListWidgetItem *item = new QListWidgetItem(fileInfo.fileName());
		listWidgetChild->addItem(item);
	}
	dialog->exec();
}

void SampleWidget::AddButtonClicked() {
	if (listWidgetChild->count() > 0) {
		QListWidgetItem *item = listWidgetChild->currentItem();
		foreach (QFileInfo fileInfo, files) {
			if (item->text() == fileInfo.fileName()) {

				QString sourceFilePath = fileInfo.absolutePath() + "/" + fileInfo.fileName();
				QString targetFilePath = downloadDisk.rootPath() + sampleButton->text() + ".mp3";
				QFile::copy(sourceFilePath, targetFilePath);
			}
		}
	}
}
