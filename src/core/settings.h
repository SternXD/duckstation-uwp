#pragma once
#include "common/log.h"
#include "types.h"
#include <array>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

class SettingsInterface
{
public:
  virtual void Clear() = 0;

  virtual int GetIntValue(const char* section, const char* key, int default_value = 0) = 0;
  virtual float GetFloatValue(const char* section, const char* key, float default_value = 0.0f) = 0;
  virtual bool GetBoolValue(const char* section, const char* key, bool default_value = false) = 0;
  virtual std::string GetStringValue(const char* section, const char* key, const char* default_value = "") = 0;

  virtual void SetIntValue(const char* section, const char* key, int value) = 0;
  virtual void SetFloatValue(const char* section, const char* key, float value) = 0;
  virtual void SetBoolValue(const char* section, const char* key, bool value) = 0;
  virtual void SetStringValue(const char* section, const char* key, const char* value) = 0;

  virtual std::vector<std::string> GetStringList(const char* section, const char* key) = 0;
  virtual void SetStringList(const char* section, const char* key, const std::vector<std::string_view>& items) = 0;
  virtual bool RemoveFromStringList(const char* section, const char* key, const char* item) = 0;
  virtual bool AddToStringList(const char* section, const char* key, const char* item) = 0;

  virtual void DeleteValue(const char* section, const char* key) = 0;
};

struct SettingInfo
{
  enum class Type
  {
    Boolean,
    Integer,
    Float,
    String,
    Path,
  };

  Type type;
  const char* key;
  const char* visible_name;
  const char* description;
  const char* default_value;
  const char* min_value;
  const char* max_value;
  const char* step_value;

  const char* StringDefaultValue() const;
  bool BooleanDefaultValue() const;
  s32 IntegerDefaultValue() const;
  s32 IntegerMinValue() const;
  s32 IntegerMaxValue() const;
  s32 IntegerStepValue() const;
  float FloatDefaultValue() const;
  float FloatMinValue() const;
  float FloatMaxValue() const;
  float FloatStepValue() const;
};

struct Settings
{
  Settings();

  ConsoleRegion region = ConsoleRegion::Auto;

  CPUExecutionMode cpu_execution_mode = CPUExecutionMode::Interpreter;

  float emulation_speed = 1.0f;
  bool speed_limiter_enabled = true;
  bool increase_timer_resolution = true;
  bool start_paused = false;
  bool start_fullscreen = false;
  bool save_state_on_exit = true;
  bool confim_power_off = true;
  bool load_memory_cards_from_save_states = false;

  GPURenderer gpu_renderer = GPURenderer::Software;
  std::string gpu_adapter;
  u32 gpu_resolution_scale = 1;
  bool gpu_use_debug_device = false;
  bool gpu_true_color = true;
  bool gpu_scaled_dithering = false;
  bool gpu_texture_filtering = false;
  bool gpu_disable_interlacing = false;
  bool gpu_force_ntsc_timings = false;
  DisplayCropMode display_crop_mode = DisplayCropMode::None;
  DisplayAspectRatio display_aspect_ratio = DisplayAspectRatio::R4_3;
  bool display_linear_filtering = true;
  bool display_integer_scaling = false;
  bool display_show_osd_messages = false;
  bool display_show_fps = false;
  bool display_show_vps = false;
  bool display_show_speed = false;
  bool video_sync_enabled = true;

  bool cdrom_read_thread = true;
  bool cdrom_region_check = true;

  AudioBackend audio_backend = AudioBackend::Cubeb;
  s32 audio_output_volume = 100;
  u32 audio_buffer_size = 2048;
  bool audio_output_muted = false;
  bool audio_sync_enabled = true;
  bool audio_dump_on_boot = true;

  // timing hacks section
  TickCount dma_max_slice_ticks = 1000;
  TickCount dma_halt_ticks = 100;
  u32 gpu_fifo_size = 128;
  TickCount gpu_max_run_ahead = 128;

  struct DebugSettings
  {
    bool show_vram = false;
    bool dump_cpu_to_vram_copies = false;
    bool dump_vram_to_cpu_copies = false;

    // Mutable because the imgui window can close itself.
    mutable bool show_gpu_state = false;
    mutable bool show_cdrom_state = false;
    mutable bool show_spu_state = false;
    mutable bool show_timers_state = false;
    mutable bool show_mdec_state = false;
  } debugging;

  // TODO: Controllers, memory cards, etc.

  std::string bios_path;
  bool bios_patch_tty_enable = false;
  bool bios_patch_fast_boot = false;

  std::array<ControllerType, NUM_CONTROLLER_AND_CARD_PORTS> controller_types{};
  std::array<MemoryCardType, NUM_CONTROLLER_AND_CARD_PORTS> memory_card_types{};
  std::array<std::string, NUM_CONTROLLER_AND_CARD_PORTS> memory_card_paths{};

  LOGLEVEL log_level = LOGLEVEL_INFO;
  std::string log_filter;
  bool log_to_console = false;
  bool log_to_debug = false;
  bool log_to_window = false;
  bool log_to_file = false;

  bool HasAnyPerGameMemoryCards() const;

  enum : u32
  {
    DEFAULT_DMA_MAX_SLICE_TICKS = 1000,
    DEFAULT_DMA_HALT_TICKS = 100,
    DEFAULT_GPU_FIFO_SIZE = 16,
    DEFAULT_GPU_MAX_RUN_AHEAD = 128
  };

  void Load(SettingsInterface& si);
  void Save(SettingsInterface& si) const;

  static std::optional<LOGLEVEL> ParseLogLevelName(const char* str);
  static const char* GetLogLevelName(LOGLEVEL level);
  static const char* GetLogLevelDisplayName(LOGLEVEL level);

  static std::optional<ConsoleRegion> ParseConsoleRegionName(const char* str);
  static const char* GetConsoleRegionName(ConsoleRegion region);
  static const char* GetConsoleRegionDisplayName(ConsoleRegion region);

  static std::optional<DiscRegion> ParseDiscRegionName(const char* str);
  static const char* GetDiscRegionName(DiscRegion region);
  static const char* GetDiscRegionDisplayName(DiscRegion region);

  static std::optional<CPUExecutionMode> ParseCPUExecutionMode(const char* str);
  static const char* GetCPUExecutionModeName(CPUExecutionMode mode);
  static const char* GetCPUExecutionModeDisplayName(CPUExecutionMode mode);

  static std::optional<GPURenderer> ParseRendererName(const char* str);
  static const char* GetRendererName(GPURenderer renderer);
  static const char* GetRendererDisplayName(GPURenderer renderer);

  static std::optional<DisplayCropMode> ParseDisplayCropMode(const char* str);
  static const char* GetDisplayCropModeName(DisplayCropMode crop_mode);
  static const char* GetDisplayCropModeDisplayName(DisplayCropMode crop_mode);

  static std::optional<DisplayAspectRatio> ParseDisplayAspectRatio(const char* str);
  static const char* GetDisplayAspectRatioName(DisplayAspectRatio ar);
  static float GetDisplayAspectRatioValue(DisplayAspectRatio ar);

  static std::optional<AudioBackend> ParseAudioBackend(const char* str);
  static const char* GetAudioBackendName(AudioBackend backend);
  static const char* GetAudioBackendDisplayName(AudioBackend backend);

  static std::optional<ControllerType> ParseControllerTypeName(const char* str);
  static const char* GetControllerTypeName(ControllerType type);
  static const char* GetControllerTypeDisplayName(ControllerType type);

  static std::optional<MemoryCardType> ParseMemoryCardTypeName(const char* str);
  static const char* GetMemoryCardTypeName(MemoryCardType type);
  static const char* GetMemoryCardTypeDisplayName(MemoryCardType type);

  // Default to D3D11 on Windows as it's more performant and at this point, less buggy.
#ifdef WIN32
  static constexpr GPURenderer DEFAULT_GPU_RENDERER = GPURenderer::HardwareD3D11;
#else
  static constexpr GPURenderer DEFAULT_GPU_RENDERER = GPURenderer::HardwareOpenGL;
#endif
  static constexpr ConsoleRegion DEFAULT_CONSOLE_REGION = ConsoleRegion::NTSC_U;
  static constexpr CPUExecutionMode DEFAULT_CPU_EXECUTION_MODE = CPUExecutionMode::Recompiler;
  static constexpr AudioBackend DEFAULT_AUDIO_BACKEND = AudioBackend::Cubeb;
  static constexpr DisplayCropMode DEFAULT_DISPLAY_CROP_MODE = DisplayCropMode::Overscan;
  static constexpr DisplayAspectRatio DEFAULT_DISPLAY_ASPECT_RATIO = DisplayAspectRatio::R4_3;
  static constexpr ControllerType DEFAULT_CONTROLLER_1_TYPE = ControllerType::DigitalController;
  static constexpr ControllerType DEFAULT_CONTROLLER_2_TYPE = ControllerType::None;
  static constexpr MemoryCardType DEFAULT_MEMORY_CARD_1_TYPE = MemoryCardType::Shared;
  static constexpr MemoryCardType DEFAULT_MEMORY_CARD_2_TYPE = MemoryCardType::None;
  static constexpr LOGLEVEL DEFAULT_LOG_LEVEL = LOGLEVEL_INFO;
};
