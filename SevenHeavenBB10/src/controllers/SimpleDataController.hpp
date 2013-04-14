#ifndef SIMPLEDATACONTROLLER_HPP_
#define SIMPLEDATACONTROLLER_HPP_

#include <QObject>
#include <bb/cascades/ArrayDataModel>

class SimpleDataController : public bb::cascades::ArrayDataModel
{
    Q_OBJECT

public:
    explicit SimpleDataController(QObject* parent = 0);
    virtual ~SimpleDataController();

public Q_SLOTS:
	void load(const QString& file_name);

private:
    QVariantList m_data;
};
#endif /* SIMPLEDATACONTROLLER_HPP_ */
