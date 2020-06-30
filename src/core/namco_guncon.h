#pragma once
#include "controller.h"
#include <memory>
#include <optional>
#include <string_view>

class NamcoGunCon final : public Controller
{
public:
  enum class Button : u8
  {
    Trigger = 0,
    A = 1,
    B = 2,
    Count
  };

  NamcoGunCon(System* system);
  ~NamcoGunCon() override;

  static std::unique_ptr<NamcoGunCon> Create(System* system);
  static std::optional<s32> StaticGetAxisCodeByName(std::string_view button_name);
  static std::optional<s32> StaticGetButtonCodeByName(std::string_view button_name);
  static AxisList StaticGetAxisNames();
  static ButtonList StaticGetButtonNames();
  static u32 StaticGetVibrationMotorCount();
  static SettingList StaticGetSettings();

  ControllerType GetType() const override;
  std::optional<s32> GetAxisCodeByName(std::string_view axis_name) const override;
  std::optional<s32> GetButtonCodeByName(std::string_view button_name) const override;

  void Reset() override;
  bool DoState(StateWrapper& sw) override;
  void LoadSettings(HostInterface* host_interface, const char* section) override;
  bool GetSoftwareCursor(const Common::RGBA8Image** image, float* image_scale) override;

  void SetAxisState(s32 axis_code, float value) override;
  void SetButtonState(s32 button_code, bool pressed) override;

  void ResetTransferState() override;
  bool Transfer(const u8 data_in, u8* data_out) override;

  void SetButtonState(Button button, bool pressed);

private:
  void UpdatePosition();

  enum class TransferState : u8
  {
    Idle,
    IDMSB,
    ButtonsLSB,
    ButtonsMSB,
    XLSB,
    XMSB,
    YLSB,
    YMSB
  };

  System* m_system;
  Common::RGBA8Image m_crosshair_image;
  std::string m_crosshair_image_path;
  float m_crosshair_image_scale = 1.0f;

  // buttons are active low
  u16 m_button_state = UINT16_C(0xFFFF);
  u16 m_position_x = 0;
  u16 m_position_y = 0;

  TransferState m_transfer_state = TransferState::Idle;
};
