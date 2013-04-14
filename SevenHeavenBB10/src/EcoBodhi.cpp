// Tabbed pane project template
#include "EcoBodhi.hpp"
#include "RESTClient.hpp"
#include "controllers/SimpleDataController.hpp"
#include "controllers/QuotesController.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/data/DataSource>

using namespace bb::cascades;

EcoBodhi::EcoBodhi(bb::cascades::Application *app)
: QObject(app)
{
    // We want to use DataSource in QML
    bb::data::DataSource::registerQmlTypes();

    // register type SimpleDataController to enable its instantiation in QML
    qmlRegisterType<SimpleDataController>("eco.bodhi", 1, 0, "SimpleDataController");

    // register type QuotesController to enable it's instantiation in QML
    qmlRegisterType<QuotesController>("eco.bodhi", 1, 0, "QuotesController");

    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("ecoBodhi", this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);

    // Update our data
    refreshData();
}

void EcoBodhi::refreshData()
{
	bool res = QObject::connect(&RESTClient::instance(), SIGNAL(downloadComplete(const QString&)),
								SIGNAL(loadData(const QString&)));
	Q_ASSERT(res);

    RESTClient::instance().fetchData(RESTClient::CMD_FACTS);
    RESTClient::instance().fetchData(RESTClient::CMD_TIPS);
    RESTClient::instance().fetchData(RESTClient::CMD_CARBON_FOOTPRINTS);
    RESTClient::instance().fetchData(RESTClient::CMD_QUOTES);
}
