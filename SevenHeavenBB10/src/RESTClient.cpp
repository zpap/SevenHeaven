#include "RESTClient.hpp"

#include <bb/data/JsonDataAccess>

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QByteArray>
#include <QThread>
#include <QDir>
#include <QtCore/QDebug>

#define TAG "RESTClient: "

using namespace bb::data;

/**
 * Qt conflates HTTP errors (layer 5) with network (layer 4 and below,
 * TCP/IP) errors.  We need to handle these separately from HTTP
 * errors.  Thankfully, the QNetworkReply::NetworkError enumeration
 * has "conveniently" placed all the HTTP errors above ordinal value
 * 100.
 */
int QNETWORKREPLY_ERROR_LAYER_5_THRESHOLD = 100;

const QString RESTClient::DATA_DIR 						= "/feeds/";
const QString RESTClient::BASE_URL 						= "http://eco-bodhi.herokuapp.com";
const QString RESTClient::CMD_FACTS 					= "/facts";
const QString RESTClient::CMD_TIPS 						= "/tips";
const QString RESTClient::CMD_CARBON_FOOTPRINTS 		= "/carbonFootprints";
const QString RESTClient::CMD_QUOTES					= "/quotes";
const QString RESTClient::FILENAME_FACTS 				= "facts.json";
const QString RESTClient::FILENAME_TIPS 				= "tips.json";
const QString RESTClient::FILENAME_CARBON_FOOTPRINTS 	= "carbon-footprints.json";
const QString RESTClient::FILENAME_QUOTES				= "quotes.json";

/* Constructor */
RESTClient::RESTClient()
	: mNetworkManager(0)
{
	qDebug() << TAG << "created!";

	// Create data/shows if it doesn't exist
	QDir dataDir(QDir::homePath() + DATA_DIR);
	if (!dataDir.exists()) {
		qDebug() << TAG << "Created directory: " << dataDir.absolutePath();
		dataDir.mkpath(dataDir.absolutePath());
	}
}

/* Destructor */
RESTClient::~RESTClient()
{
	qDebug() << "Deleting rest client instance";
	delete mNetworkManager;
}

QUrl RESTClient::buildUri(const QString& path) {
	QUrl result;
	result.setUrl(BASE_URL);
	result.setPath(path);

	qDebug() << "Build URI" << result;
	return result;
}

/*
 * As this is singleton class, this function creates (if needed)
 * and returns class intance
 * */
RESTClient& RESTClient::instance()
{
    static RESTClient client;
    return client;
}

/*
 * GET
 */
void RESTClient::fetchData(const QString& cmd) {
	qDebug() << TAG << "Fetching " << cmd;

	if (mNetworkManager == 0) {
		// Create it once
		mNetworkManager = new QNetworkAccessManager(this);
		Q_ASSERT(mNetworkManager != NULL);
		bool connectResult = QObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply *)),
								  SLOT(slotRequestFinished(QNetworkReply *)), Qt::DirectConnection);
		Q_ASSERT(connectResult);
	}

	QNetworkRequest request;
	request.setUrl(buildUri(cmd));
	mNetworkManager->get(request);

	qDebug() << "RestClient::fetchData() has successfully kicked off a request, on thread:" << QThread::currentThreadId();
}

QString RESTClient::buildFilename(const QString& cmd)
{
	if (cmd.compare(CMD_FACTS) == 0)
		return QDir::homePath() + DATA_DIR + FILENAME_FACTS;
	if (cmd.compare(CMD_TIPS) == 0)
		return QDir::homePath() + DATA_DIR + FILENAME_TIPS;
	if (cmd.compare(CMD_CARBON_FOOTPRINTS) == 0)
		return QDir::homePath() + DATA_DIR + FILENAME_CARBON_FOOTPRINTS;
	if (cmd.compare(CMD_QUOTES) == 0)
		return QDir::homePath() + DATA_DIR + FILENAME_QUOTES;

	// No match return tmp filename
	return QDir::homePath() + DATA_DIR + "tmp.log";
}

void RESTClient::slotRequestFinished(QNetworkReply* reply) {
	qDebug() << "FETCH FINISHED CALLBACK";

	QVariant statusCodeV = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
	if (reply->error() == QNetworkReply::NoError)
	{
		qDebug() << "fetch finished successfully.";

		QNetworkRequest request = reply->request();
		QUrl url = request.url();
		qDebug() << TAG << "REQUEST URL: " << url.toString();

		QString cmd = getCmd(url);

		QByteArray bytes = reply->readAll();
		qDebug() << TAG << "READ ALL BYTES SUCCESSFULLY";

		QString data(bytes);
		qDebug() << TAG << "CONVERT BYTES TO STRING";

		QString filename = getFilename(url);
		qDebug() << TAG << "SET FILENAME: " << filename;

		QFile file(QDir::homePath() + DATA_DIR + filename);
		qDebug() << TAG << file.fileName();
		if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			// OK, now we go ahead and load main application with new stuff
			//emit updatedFinished("slotDownloadRequestFinished(...): File could not be saved");
			qDebug() << "slotDownloadRequestFinished(...): File could not be saved";
			return;
		}

		QTextStream out(&file);
		out << data;
		file.close();

		emit downloadComplete(getCmd(url));
	} else {
		if(reply->error() < QNETWORKREPLY_ERROR_LAYER_5_THRESHOLD) {
			// TCP, IP, or otherwise network layer faults
			qDebug() << "fetch request failed, network error: ";
		} else {
			// HTTP faults
			qDebug() << "fetch request failed, HTTP error: '" + statusCodeV.toString() + "' occurred";
		}
		// we don't emit the signal.
	}
}

QString RESTClient::getFilename(const QUrl& url)
{
	QString cmd = getCmd(url);

	qDebug() << TAG << "CMD: " << cmd;
	if (cmd.compare(CMD_FACTS) == 0)
		return FILENAME_FACTS;
	if (cmd.compare(CMD_TIPS) == 0)
		return FILENAME_TIPS;
	if (cmd.compare(CMD_CARBON_FOOTPRINTS) == 0)
		return FILENAME_CARBON_FOOTPRINTS;
	if (cmd.compare(CMD_QUOTES) == 0)
		return FILENAME_QUOTES;

	// No match return
	return "tmp.log";
}

QString RESTClient::getCmd(const QUrl& url)
{
	QString cmd = url.path();
	return cmd;
}

void RESTClient::slotInformativeError(QNetworkReply::NetworkError error) {
	qDebug() << "A request encountered an error";
}
