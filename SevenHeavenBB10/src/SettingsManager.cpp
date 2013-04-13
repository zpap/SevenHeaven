#include "SettingsManager.hpp"

#include <QtCore/QSettings>
#include <QtCore/QDebug>

#define TAG "SettingsManager: "

static const QString ORGANIZATION = "BROCKOLI";
static const QString APPLICATION_NAME = "MEDIAJERK";

const QString SettingsManager::DATA_DIR = "/shows/";
const QString SettingsManager::SHOW_DETAILS_DIR = "details/";
const QString SettingsManager::FILE_SHOWS = "shows.json";
const QString SettingsManager::FILE_HISTORY = "history.json";
const QString SettingsManager::FILE_FUTURE = "future.json";
const QString SettingsManager::FILE_PONG = "pong.json";

const QString SettingsManager::SICKBEARD_URL = "SICKBEARD_URL";
const QString SettingsManager::SICKBEARD_API_KEY = "SICKBEARD_API_KEY";
const QString SettingsManager::SICKBEARD_PORT = "SICKBEARD_PORT";

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

bool SettingsManager::validSettings()
{
	QSettings registrationSettings(ORGANIZATION, APPLICATION_NAME);

	if (registrationSettings.contains(SICKBEARD_URL) && registrationSettings.contains(SICKBEARD_PORT) &&
			registrationSettings.contains(SICKBEARD_API_KEY)) {
		return true;
	}

	return false;
}
