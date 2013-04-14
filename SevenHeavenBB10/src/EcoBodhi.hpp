// Tabbed pane project template
#ifndef ECOBODHI_HPP_
#define ECOBODHI_HPP_

#include <QObject>

namespace bb { namespace cascades { class Application; }}

/*!
 * @brief Application pane object
 *
 *Use this object to create and init app UI, to create context objects, to register the new meta types etc.
 */
class EcoBodhi : public QObject
{
    Q_OBJECT
public:
    EcoBodhi(bb::cascades::Application *app);
    virtual ~EcoBodhi() {}

Q_SIGNALS:
    void loadData(const QString& cmd);

private:
    void refreshData();
};

#endif /* ECOBODHI_HPP_ */
