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
	static const QString CMD;
	static const QString CMD_SHOWS;
	static const QString CMD_SHOW;
	static const QString CMD_HISTORY;
	static const QString CMD_FUTURE;
	static const QString CMD_PING;
	static const QString CMD_GET_BANNER;
	static const QString CMD_GET_POSTER;
	static const QString QUERY_TVDBID;

	static const QString PATH_API;

	RESTClient();
	virtual ~RESTClient();

	// Returns class instance
	static RESTClient& instance();

	// for GET
	QUrl buildUri(const QList<QPair<QString, QString> > query);

public Q_SLOTS:

	void fetchData(const QString& cmd, const QList<QPair<QString, QString> >* data = 0);
	void testUrl(const QString& url);

Q_SIGNALS:

	void downloadComplete(const QString& cmd);
	void pong(const QString& result);

private:

	QNetworkAccessManager *mNetworkManager;

	// Extract the file name from the download url
	QString buildFilename(const QString& cmd, const QString& tvdbid = 0);

	// Get a filename from the request url
	QString getFilename(const QUrl& url, const QString& tvdbid = 0);

	// Get the REST command from the request url
	QString getCmd(const QUrl& url);

private Q_SLOTS:
	void slotRequestFinished(QNetworkReply *reply);

	void slotInformativeError(QNetworkReply::NetworkError error);
};

#endif /* RESTCLIENT_HPP_ */
