#include "RESTClient.hpp"
#include "SettingsManager.hpp"

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

const QString RESTClient::CMD = "cmd";
const QString RESTClient::CMD_SHOWS = "shows";
const QString RESTClient::CMD_SHOW = "show";
const QString RESTClient::CMD_HISTORY = "history";
const QString RESTClient::CMD_FUTURE = "future";
const QString RESTClient::CMD_PING = "sb.ping";
const QString RESTClient::CMD_GET_BANNER = "show.getbanner";
const QString RESTClient::CMD_GET_POSTER = "show.getposter";
const QString RESTClient::QUERY_TVDBID = "tvdbid";

const QString RESTClient::PATH_API = "api/";

/* Constructor */
RESTClient::RESTClient()
	: mNetworkManager(0)
{
	qDebug() << TAG << "created!";

	// Create data/shows if it doesn't exist
	QDir dataDir(QDir::homePath() + SettingsManager::DATA_DIR);
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

QUrl RESTClient::buildUri(const QList<QPair<QString, QString> > query) {
	QUrl result;
	result.setUrl(SettingsManager::instance().getSettingValue(SettingsManager::SICKBEARD_URL, ""));
    result.setPort(SettingsManager::instance().getSettingValue(SettingsManager::SICKBEARD_PORT, "").toInt());
    result.setPath(PATH_API + SettingsManager::instance().getSettingValue(SettingsManager::SICKBEARD_API_KEY, "") + "/");
    result.setQueryItems(query);

	qDebug() << "Build URI" << result;
	return result;
}

/*
 * As this is single tone class, this function creates (if needed)
 * and returns class intance
 * */
RESTClient& RESTClient::instance()
{
    static RESTClient client;
    return client;
}

void RESTClient::testUrl(const QString& url) {
	qDebug() << TAG << "Testing url: " << url;

	if (mNetworkManager == 0) {
		// Create it once
		mNetworkManager = new QNetworkAccessManager(this);
		Q_ASSERT(mNetworkManager != NULL);
		bool connectResult = QObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply *)),
								  SLOT(slotRequestFinished(QNetworkReply *)), Qt::DirectConnection);
		Q_ASSERT(connectResult);
	}

	QNetworkRequest request;
	request.setUrl(QUrl(url));
	mNetworkManager->get(request);

	qDebug() << TAG << "RestClient::fetchData() has successfully kicked off a request, on thread:" << QThread::currentThreadId();
}

/*
 * GET
 */
void RESTClient::fetchData(const QString& cmd, const QList<QPair<QString, QString> >* data) {
	qDebug() << TAG << "Fetching " << cmd;

	if (mNetworkManager == 0) {
		// Create it once
		mNetworkManager = new QNetworkAccessManager(this);
		Q_ASSERT(mNetworkManager != NULL);
		bool connectResult = QObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply *)),
								  SLOT(slotRequestFinished(QNetworkReply *)), Qt::DirectConnection);
		Q_ASSERT(connectResult);
	}

	QList<QPair<QString, QString> > query;
	QPair<QString, QString> pair(CMD, cmd);
	query.append(pair);

	// Add any passed in key/value pairs to our query
	if (data != 0) {
		query.append(*data);
	}

	QNetworkRequest request;
	request.setUrl(buildUri(query));
	mNetworkManager->get(request);

	qDebug() << "RestClient::fetchData() has successfully kicked off a request, on thread:" << QThread::currentThreadId();
}

QString RESTClient::buildFilename(const QString& cmd, const QString& tvdbid)
{
	if (cmd.compare(CMD_SHOWS) == 0)
		return QDir::homePath() + SettingsManager::DATA_DIR + SettingsManager::FILE_SHOWS;
	if (cmd.compare(CMD_HISTORY) == 0)
		return QDir::homePath() + SettingsManager::DATA_DIR + SettingsManager::FILE_HISTORY;
	if (cmd.compare(CMD_FUTURE) == 0)
		return QDir::homePath() + SettingsManager::DATA_DIR + SettingsManager::FILE_FUTURE;
	if (cmd.compare(CMD_SHOW) == 0)
		return QDir::homePath() + SettingsManager::DATA_DIR + SettingsManager::SHOW_DETAILS_DIR + tvdbid + ".json";

	// No match return tmp filename
	return QDir::homePath() + SettingsManager::DATA_DIR + "tmp.log";
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

		if (cmd.compare(CMD_PING) == 0) {  // Handle ping response
			JsonDataAccess jda;
			QVariantMap map = jda.loadFromBuffer(bytes).toMap();
			if (map.value("result").toString().compare("success") == 0) {
				emit pong("pong");
			} else {
				emit pong("api");
			}
		} else { // Handle download file
			QString data(bytes);
			qDebug() << TAG << "CONVERT BYTES TO STRING";

			QString filename = getFilename(url);
			qDebug() << TAG << "SET FILENAME: " << filename;

			QFile file(QDir::homePath() + SettingsManager::DATA_DIR + filename);
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
		}
	} else {
		if(reply->error() < QNETWORKREPLY_ERROR_LAYER_5_THRESHOLD) {
			// TCP, IP, or otherwise network layer faults
			qDebug() << "fetch request failed, network error: ";
			emit pong("network");
		} else {
			// HTTP faults
			qDebug() << "fetch request failed, HTTP error: '" + statusCodeV.toString() + "' occurred";
			emit pong("http");
		}
		// we don't emit the signal.
	}
}

QString RESTClient::getFilename(const QUrl& url, const QString& tvdbid)
{
	QString cmd = url.queryItemValue(CMD);

	qDebug() << TAG << "CMD: " << cmd;
	if (cmd.compare(CMD_SHOWS) == 0)
		return SettingsManager::FILE_SHOWS;
	if (cmd.compare(CMD_HISTORY) == 0)
		return SettingsManager::FILE_HISTORY;
	if (cmd.compare(CMD_FUTURE) == 0)
		return SettingsManager::FILE_FUTURE;
	if (cmd.compare(CMD_PING) == 0)
		return SettingsManager::FILE_PONG;
	if (cmd.compare(CMD_SHOW) == 0) {
		QString tvdbid = url.queryItemValue(QUERY_TVDBID);
		if (!tvdbid.isEmpty() & !tvdbid.isNull()) {
			return SettingsManager::SHOW_DETAILS_DIR + tvdbid + ".json";
		}
	}

	// No match return
	return "tmp.log";
}

QString RESTClient::getCmd(const QUrl& url)
{
	QString cmd = url.queryItemValue(CMD);
	return cmd;
}

void RESTClient::slotInformativeError(QNetworkReply::NetworkError error) {
	qDebug() << "A request encountered an error";
}
