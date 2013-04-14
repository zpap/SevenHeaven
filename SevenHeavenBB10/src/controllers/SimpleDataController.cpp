#include "SimpleDataController.hpp"
#include "RESTClient.hpp"
#include <bb/data/JsonDataAccess>

using namespace bb::data;

SimpleDataController::SimpleDataController(QObject* parent)
: bb::cascades::ArrayDataModel()
{
    setParent(parent);
    qDebug() << "Creating SimpleDataController object:" << this;
}

SimpleDataController::~SimpleDataController()
{
    qDebug() << "Destroying SimpleDataController object:" << this;
}

void SimpleDataController::load(const QString& file_name)
{
    // clear model
    clear();
    // clear not filtered data cache
    m_data.clear();
    {
        JsonDataAccess jda;
        QVariantMap root = jda.load(file_name).toMap();

        if (jda.hasError()) {
            DataAccessError error = jda.error();
            qDebug() << file_name << "JSON loading error: " << error.errorType() << ": " << error.errorMessage();
        }
        else {
            qDebug() << file_name << "JSON data loaded OK!";
            // when loaded, show all data (no filtering)
            m_data = root.value("data").toList();
            append(m_data);
        }
    }
}
