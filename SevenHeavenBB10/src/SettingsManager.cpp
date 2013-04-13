#include "SettingsManager.hpp"

#include <QtCore/QSettings>
#include <QtCore/QDebug>

#define TAG "SettingsManager: "

static const QString ORGANIZATION = "MACADAMIAN";
static const QString APPLICATION_NAME = "ECO_BOHDI";

const QString SettingsManager::DATA_DIR = "/feeds/";

const QString SettingsManager::FEED_URL = "FEED_URL";

SettingsManager::SettingsManager()
{
	qDebug() << TAG << "created";
}

SettingsManager::~SettingsManager()
{
	qDebug() << TAG << "destrutor";
}

/*
 * As this is singleton class, this function creates (if needed)
 * and returns class instance
 * */
SettingsManager& SettingsManager::instance()
{
    static SettingsManager instance;
    return instance;
}

/*
 * Returns given setting in settings file
 * */
QString SettingsManager::getSettingValue(const QString &strSettingName, const QString &strDefaultValue)
{
	QSettings registrationSettings(ORGANIZATION, APPLICATION_NAME);

	if (registrationSettings.value(strSettingName).isNull()) {
		return strDefaultValue;
	}

	return registrationSettings.value(strSettingName).toString();
}

/*
 * Sets given setting to given value in settings file
 * */
void SettingsManager::updateSettingValue(const QString &strSettingName, const QString &value)
{
	QSettings settings(ORGANIZATION, APPLICATION_NAME);
	settings.setValue(strSettingName, QVariant(value));
	settings.sync();
}
