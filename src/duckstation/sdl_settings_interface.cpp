#include "sdl_settings_interface.h"
#include "YBaseLib/Log.h"
Log_SetChannel(SDLSettingsInterface);

SDLSettingsInterface::SDLSettingsInterface(const char* filename) : m_filename(filename), m_ini(true)
{
  SI_Error err = m_ini.LoadFile(filename);
  if (err != SI_OK)
    Log_WarningPrintf("Settings could not be loaded from '%s', defaults will be used.", filename);
}

SDLSettingsInterface::~SDLSettingsInterface()
{
  SI_Error err = m_ini.SaveFile(m_filename.c_str(), false);
  if (err != SI_OK)
    Log_WarningPrintf("Failed to save settings to '%s'.", m_filename.c_str());
}

int SDLSettingsInterface::GetIntValue(const char* section, const char* key, int default_value /*= 0*/)
{
  return static_cast<int>(m_ini.GetLongValue(section, key, default_value));
}

float SDLSettingsInterface::GetFloatValue(const char* section, const char* key, float default_value /*= 0.0f*/)
{
  return static_cast<float>(m_ini.GetDoubleValue(section, key, default_value));
}

bool SDLSettingsInterface::GetBoolValue(const char* section, const char* key, bool default_value /*= false*/)
{
  return m_ini.GetBoolValue(section, key, default_value);
}

std::string SDLSettingsInterface::GetStringValue(const char* section, const char* key,
                                                 const char* default_value /*= ""*/)
{
  return m_ini.GetValue(section, key, default_value);
}

void SDLSettingsInterface::SetIntValue(const char* section, const char* key, int value)
{
  m_ini.SetLongValue(section, key, static_cast<long>(value));
}

void SDLSettingsInterface::SetFloatValue(const char* section, const char* key, float value)
{
  m_ini.SetDoubleValue(section, key, static_cast<double>(value));
}

void SDLSettingsInterface::SetBoolValue(const char* section, const char* key, bool value)
{
  m_ini.SetBoolValue(section, key, value);
}

void SDLSettingsInterface::SetStringValue(const char* section, const char* key, const char* value)
{
  m_ini.SetValue(section, key, value);
}

void SDLSettingsInterface::DeleteValue(const char* section, const char* key)
{
  m_ini.DeleteValue(section, key, nullptr);
}
