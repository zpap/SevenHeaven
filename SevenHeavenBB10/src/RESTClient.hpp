/*
 * RESTClient.hpp
 *
 */

#ifndef RESTCLIENT_HPP_
#define RESTCLIENT_HPP_

#include <QObject>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include <QtCore/QFile>
#include <QUrl>
#include <QMap>
#include <QString>

class RESTClient : public QObject
{
	Q_OBJECT
public:
	static const QString DATA_DIR;

	static const QString BASE_URL;

	static const QString CMD_FACTS;
	static const QString CMD_TIPS;
	static const QString CMD_CARBON_FOOTPRINTS;
	static const QString CMD_QUOTES;

	static const QString FILENAME_FACTS;
	static const QString FILENAME_TIPS;
	static const QString FILENAME_CARBON_FOOTPRINTS;
	static const QString FILENAME_QUOTES;

	RESTClient();
	virtual ~RESTClient();

	// Returns class instance
	static RESTClient& instance();

	// for GET
	QUrl buildUri(const QString& path);

public Q_SLOTS:

	void fetchData(const QString& cmd);

Q_SIGNALS:

	void downloadComplete(const QString& cmd);

private:

	QNetworkAccessManager *mNetworkManager;

	// Extract the file name from the download url
	QString buildFilename(const QString& cmd);

	// Get a filename from the request url
	QString getFilename(const QUrl& url);

	// Get the REST command from the request url
	QString getCmd(const QUrl& url);

private Q_SLOTS:
	void slotRequestFinished(QNetworkReply *reply);

	void slotInformativeError(QNetworkReply::NetworkError error);
};

#endif /* RESTCLIENT_HPP_ */
