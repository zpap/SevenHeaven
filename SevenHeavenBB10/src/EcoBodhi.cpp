// Tabbed pane project template
#include "EcoBodhi.hpp"
#include "RESTClient.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>

using namespace bb::cascades;

EcoBodhi::EcoBodhi(bb::cascades::Application *app)
: QObject(app)
{
    // create scene document from main.qml asset
    // set parent to created document to ensure it exists for the whole application lifetime
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);

    // create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();
    // set created root object as a scene
    app->setScene(root);

    // Update our data
    refreshData();
}

void EcoBodhi::refreshData()
{
    RESTClient::instance().fetchData(RESTClient::CMD_FACTS);
    RESTClient::instance().fetchData(RESTClient::CMD_TIPS);
    RESTClient::instance().fetchData(RESTClient::CMD_CARBON_FOOTPRINTS);
}
