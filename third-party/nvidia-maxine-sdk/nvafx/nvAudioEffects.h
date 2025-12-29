/*
Copyright (c) 2024, NVIDIA CORPORATION. All rights reserved.

NVIDIA CORPORATION and its licensors retain all intellectual property
and proprietary rights in and to this software, related documentation
and any modifications thereto. Any use, reproduction, disclosure or
distribution of this software and related documentation without an express
license agreement from NVIDIA CORPORATION is strictly prohibited.
*/

#ifndef __NVAUDIOEFFECTS_H__
#define __NVAUDIOEFFECTS_H__

#ifdef WIN32
#if defined NVAFX_API_EXPORT
#define NVAFX_API __declspec(dllexport)
#else
#define NVAFX_API __declspec(dllimport)
#endif
#else
// Exports are controlled by version map for Linux, hence this is not needed
#define NVAFX_API
#endif

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** API return values */
typedef enum {
  /** Success */
  NVAFX_STATUS_SUCCESS = 0,
  /** Failure */
  NVAFX_STATUS_FAILED = 1,
  /** Handle invalid */
  NVAFX_STATUS_INVALID_HANDLE = 2,
  /** Parameter value invalid */
  NVAFX_STATUS_INVALID_PARAM = 3,
  /** Parameter value immutable */
  NVAFX_STATUS_IMMUTABLE_PARAM = 4,
  /** Insufficient data to process */
  NVAFX_STATUS_INSUFFICIENT_DATA = 5,
  /** Effect not supported */
  NVAFX_STATUS_EFFECT_NOT_AVAILABLE = 6,
  /** Given buffer length too small to hold requested data */
  NVAFX_STATUS_OUTPUT_BUFFER_TOO_SMALL = 7,
  /** Model file could not be loaded */
  NVAFX_STATUS_MODEL_LOAD_FAILED = 8,
  /** Model is not loaded, it needs to be loaded for this operation */
  NVAFX_STATUS_MODEL_NOT_LOADED = 9,
  /** Selected model is incompatible */
  NVAFX_STATUS_INCOMPATIBLE_MODEL = 10,
  /** The selected GPU is not supported. The SDK requires Turing and above GPU with Tensor cores */
  NVAFX_STATUS_GPU_UNSUPPORTED = 11,
  /** No supported GPU found on the system */
  NVAFX_STATUS_NO_SUPPORTED_GPU_FOUND = 12,
  /** Current GPU is not the one selected */
  NVAFX_STATUS_WRONG_GPU = 13,
  /** Cuda operation failure */
  NVAFX_STATUS_CUDA_ERROR = 14,
  /** Invalid operation performed **/
  NVAFX_STATUS_INVALID_OPERATION = 15,
  /** CUDA runtime is less than supported version*/
  NVAFX_UNSUPPORTED_RUNTIME = 16,
  /** (32 bit SDK only) COM server was not registered, please see user manual for details */
  NVAFX_STATUS_32_SERVER_NOT_REGISTERED = 17,
  /** (32 bit SDK only) COM operation failed */
  NVAFX_STATUS_32_COM_ERROR = 18,
  /** Cuda Context Failure Error */
  NVAFX_STATUS_CUDA_CONTEXT_CREATION_FAILED = 19,
} NvAFX_Status;

/** Bool type (stdbool is available only with C99) */
#define NVAFX_TRUE 1
#define NVAFX_FALSE 0
typedef char NvAFX_Bool;

/** Logging level to enable, each level is inclusive of the level preceding it */
typedef enum LoggingSeverity_t {
  LOG_LEVEL_NONE = -1,
  LOG_LEVEL_FATAL = 0,
  LOG_LEVEL_ERROR = 1,
  LOG_LEVEL_WARNING = 2,
  LOG_LEVEL_INFO = 3,
} LoggingSeverity;

typedef enum LoggingTarget_t {
  // No logging.
  LOG_TARGET_NONE = -1,
  // Log to stderr.
  LOG_TARGET_STDERR = 0,
  // Log to specified file.
  LOG_TARGET_FILE = 1,
  // Log through invocation of a user-specified callback.
  LOG_TARGET_CALLBACK = 2,
} LoggingTarget;

inline const char* LogSeverityToString(LoggingSeverity severity) {
  switch (severity) {
    case LOG_LEVEL_FATAL:
      return "FATAL";
    case LOG_LEVEL_ERROR:
      return "ERROR";
    case LOG_LEVEL_WARNING:
      return "WARNING";
    case LOG_LEVEL_INFO:
      return "INFO";
    default:
      return "UNKNOWN";
  }
}

/** Function used for logging callback */
/// @param[in,out]  user_data   a pointer to data needed by the specific logger.
/// @param[in]      msg         a C-string to add to the log.
typedef void (*logging_cb_t)(void* user_data, const char* msg);

/** We use strings as effect selectors */
typedef const char* NvAFX_EffectSelector;

/** We use strings as parameter selectors. */
typedef const char* NvAFX_ParameterSelector;

/** Each effect instantiation is associated with an opaque handle. */
typedef void* NvAFX_Handle;

/** @brief Get a list of audio effects supported
 *
 * @param[out] num_effects Number of effects returned in effects array
 * @param[out] effects A list of effects returned by the API. This list is
 *                     statically allocated by the API implementation. Caller
 *                     does not need to allocate.
 *
 * @return Status values as enumerated in @ref NvAFX_Status
 */
NvAFX_Status NVAFX_API NvAFX_GetEffectList(int* num_effects, NvAFX_EffectSelector* effects[]);

/** @brief Create a new instance of an audio effect.
 *
 * @param[in] code   The selector code for the desired audio Effect.
 * @param[out] effect   A handle to the Audio Effect instantiation.
 *
 * @return Status values as enumerated in @ref NvAFX_Status
 */
NvAFX_Status NVAFX_API NvAFX_CreateEffect(NvAFX_EffectSelector code, NvAFX_Handle* effect);

/** @brief Create a new instance of an audio effect.
 *
 * @param[in] code   The selector code for the desired chained audio Effect.
 * @param[out] effect   A handle to the Audio Effect instantiation.
 *
 * @return Status values as enumerated in @ref NvAFX_Status
 */
NvAFX_Status NVAFX_API NvAFX_CreateChainedEffect(NvAFX_EffectSelector code, NvAFX_Handle* effect);

/** @brief Delete a previously instantiated audio Effect.
 *
 * @param[in]  effect A handle to the audio Effect to be deleted.
 *
 * @return Status values as enumerated in @ref NvAFX_Status
 */
NvAFX_Status NVAFX_API NvAFX_DestroyEffect(NvAFX_Handle effect);

/** Set the value of the selected parameter (unsigned int, char*)
 *
 * @param[in]  effect      The effect to configure.
 * @param[in]  param_name   The selector of the effect parameter to configure.
 * @param[in]  val         The value to be assigned to the selected effect parameter.
 *
 * @return Status values as enumerated in @ref NvAFX_Status
 */
NvAFX_Status NVAFX_API NvAFX_SetU32(NvAFX_Handle effect, NvAFX_ParameterSelector param_name, unsigned int val);
NvAFX_Status NVAFX_API NvAFX_SetU32List(NvAFX_Handle effect, NvAFX_ParameterSelector param_name, unsigned int* val,
                                        unsigned int size);
NvAFX_Status NVAFX_API NvAFX_SetString(NvAFX_Handle effect, NvAFX_ParameterSelector param_name, const char* val);
NvAFX_Status NVAFX_API NvAFX_SetStringList(NvAFX_Handle effect, NvAFX_ParameterSelector param_name, const char** val,
                                           unsigned int size);
NvAFX_Status NVAFX_API NvAFX_SetFloat(NvAFX_Handle effect, NvAFX_ParameterSelector param_name, float val);
NvAFX_Status NVAFX_API NvAFX_SetFloatList(NvAFX_Handle effect, NvAFX_ParameterSelector param_name, float* val,
                                          unsigned int size);

/** Get the value of the selected parameter (unsigned int, char*)
 *
 * @param[in]  effect      The effect handle.
 * @param[in]  param_name   The selector of the effect parameter to read.
 * @param[out]  val         Buffer in which the parameter value will be assigned.
 * @param[in]  max_length  The length in bytes of the buffer provided.
 *
 * @return Status values as enumerated in @ref NvAFX_Status
 */
NvAFX_Status NVAFX_API NvAFX_GetU32(NvAFX_Handle effect, NvAFX_ParameterSelector param_name, unsigned int* val);
NvAFX_Status NVAFX_API NvAFX_GetU32List(NvAFX_Handle effect, NvAFX_ParameterSelector param_name, unsigned int* val,
                                        unsigned int size);
NvAFX_Status NVAFX_API NvAFX_GetString(NvAFX_Handle effect, NvAFX_ParameterSelector param_name, char* val,
                                       int max_length);
NvAFX_Status NVAFX_API NvAFX_GetStringList(NvAFX_Handle effect, NvAFX_ParameterSelector param_name, char** val,
                                           int* max_length, unsigned int size);
NvAFX_Status NVAFX_API NvAFX_GetFloat(NvAFX_Handle effect, NvAFX_ParameterSelector param_name, float* val);
NvAFX_Status NVAFX_API NvAFX_GetFloatList(NvAFX_Handle effect, NvAFX_ParameterSelector param_name, float* val,
                                          unsigned int size);

/** Load the Effect based on the set params.
 *
 * @param[in]  effect     The effect object handle.
 *
 * @return Status values as enumerated in @ref NvAFX_Status
 */
NvAFX_Status NVAFX_API NvAFX_Load(NvAFX_Handle effect);

/** Get the devices supported by the model.
 *
 * @note This method must be called after setting model path.
 *
 * @param[in]      effect     The effect object handle.
 * @param[in,out]  num        The size of the input array. This value will be set by the function if call succeeds.
 * @param[in,out]  devices    Array of size num. The function will fill the array with CUDA device indices of devices
                              supported by the model, in descending order of preference (first = most preferred device)
 * @return Status values as enumerated in @ref NvAFX_Status
 */
NvAFX_Status NVAFX_API NvAFX_GetSupportedDevices(NvAFX_Handle effect, int* num, int* devices);

/** Process the input buffer as per the effect selected. e.g. denoising
 *
 * @note The input float data is expected to be standard 32-bit float type with values in range [-1.0, +1.0]
 *
 * @param[in]  effect               The effect handle.
 * @param[in]  input                Input float buffer array. It points to an array of buffers where each buffer holds
 *                                   audio data for a single channel. Array size should be same as number of
 *                                   input channels expected by the effect. Also ensure sampling rate is same as
 *                                   expected by the Effect.
 *                                   For e.g. for denoiser it should be equal to the value returned by NvAFX_GetU32()
 *                                   returned value for NVAFX_FIXED_PARAM_DENOISER_SAMPLE_RATE parameter.
 * @param[out]  output               Output float buffer array. The layout is same as input. It points to an an array of
 *                                   buffers where buffer has audio data corresponding to that channel. The buffers have
 *                                   to be preallocated by caller. Size of each buffer (i.e. channel) is same as that of
 *                                   input. However, number of channels may differ (can be queried by calling
 *                                   NvAFX_GetU32() with NVAFX_PARAM_NUM_OUTPUT_CHANNELS as parameter).
 * @param[in]  num_input_samples     The number of samples in the input buffer. After this call returns output
 *                                   can be determined by calling NvAFX_GetU32() with
 *                                   NVAFX_PARAM_NUM_OUTPUT_SAMPLES_PER_FRAME as parameter
 * @param[in]  num_input_channels    The number of channels in the input buffer. The @a input should point
 *                                   to @ num_input_channels number of buffers for input, which can be determined by
 *                                   calling NvAFX_GetU32() with NVAFX_PARAM_NUM_INPUT_CHANNELS as parameter.
 *
 * @return Status values as enumerated in @ref NvAFX_Status
 */
NvAFX_Status NVAFX_API NvAFX_Run(NvAFX_Handle effect, const float** input, float** output, unsigned num_input_samples,
                                 unsigned num_input_channels);

/** Reset effect state
 *
 * @note Allows the state of an effect to be reset. This operation will reset the state of selected in the next
 *       NvAFX_Run call
 *
 * @param[in]  effect        The effect handle.
 *
 * @return Status values as enumerated in @ref NvAFX_Status
 */
NvAFX_Status NVAFX_API NvAFX_Reset(NvAFX_Handle effect);

/** Initialize Logger
 *
 * @note Initializes Logger
 *
 * @param[in]  level         The logging level to enable.
 * @param[in]  target        Logging targets to write logs to, LoggingTarget_t can be OR'd
 * @param[in]  filename      The name of the file where to write logs.
 * @param[in]  cb            Callback to use if LOG_TARGET_CALLBACK is enabled.
 * @param[in]  userdata      Data passed back with log callback. Used only when LOG_TARGET_CALLBACK
 *                           is enabled.
 *
 * @return Status values as enumerated in @ref NvAFX_Status
 */
NvAFX_Status NVAFX_API NvAFX_InitializeLogger(LoggingSeverity level, LoggingTarget target, const char* filename,
                                              logging_cb_t cb, void* userdata);

/** Un-initializes Logger
 *
 * @note Un-initializes Logger
 *
 * @return Status values as enumerated in @ref NvAFX_Status
 */
NvAFX_Status NVAFX_API NvAFX_UninitializeLogger();

/** Effect selectors. @ref NvAFX_EffectSelector */

/** Denoiser Effect */
#define NVAFX_EFFECT_DENOISER "denoiser"
/** Dereverb Effect */
#define NVAFX_EFFECT_DEREVERB "dereverb"
/** Dereverb Denoiser Effect */
#define NVAFX_EFFECT_DEREVERB_DENOISER "dereverb_denoiser"
/** Acoustic Echo Cancellation Effect */
#define NVAFX_EFFECT_AEC "aec"
/** Super-resolution Effect */
#define NVAFX_EFFECT_SUPERRES "superres"

#define NVAFX_CHAINED_EFFECT_DENOISER_16k_SUPERRES_16k_TO_48k "denoiser16k_superres16kto48k"
#define NVAFX_CHAINED_EFFECT_DEREVERB_16k_SUPERRES_16k_TO_48k "dereverb16k_superres16kto48k"
#define NVAFX_CHAINED_EFFECT_DEREVERB_DENOISER_16k_SUPERRES_16k_TO_48k "dereverb_denoiser16k_superres16kto48k"
#define NVAFX_CHAINED_EFFECT_SUPERRES_8k_TO_16k_DENOISER_16k "superres8kto16k_denoiser16k"
#define NVAFX_CHAINED_EFFECT_SUPERRES_8k_TO_16k_DEREVERB_16k "superres8kto16k_dereverb16k"
#define NVAFX_CHAINED_EFFECT_SUPERRES_8k_TO_16k_DEREVERB_DENOISER_16k "superres8kto16k_dereverb_denoiser16k"

/** Parameter selectors */
/** Common Effect parameters. */
/** Number of audio streams in I/O (unsigned int). */
#define NVAFX_PARAM_NUM_STREAMS "num_streams"
/** To set if SDK should select the default GPU to run the effects in a Multi-GPU setup(unsigned int).
    Default value is 0. Please see user manual for details.*/
#define NVAFX_PARAM_USE_DEFAULT_GPU "use_default_gpu"
/** To be set to '1' if SDK user wants to create and manage own CUDA context. Other users can simply
    ignore this parameter. Once set to '1' this cannot be unset for that session (unsigned int) rw param
    Note: NVAFX_PARAM_USE_DEFAULT_GPU and NVAFX_PARAM_USER_CUDA_CONTEXT cannot be used at the same time */
#define NVAFX_PARAM_USER_CUDA_CONTEXT "user_cuda_context"
/** To be set to '1' if SDK user wants to disable cuda graphs. Other users can simply ignore this parameter.
Using Cuda Graphs helps to reduce the inference between GPU and CPU which makes operations quicker.*/
#define NVAFX_PARAM_DISABLE_CUDA_GRAPH "disable_cuda_graph"
/** To be set to '1' if SDK user wants to enable VAD */
#define NVAFX_PARAM_ENABLE_VAD "enable_vad"
/** Effect parameters. @ref NvAFX_ParameterSelector */
/** Model path (char*) */
#define NVAFX_PARAM_MODEL_PATH "model_path"
/** Input Sample rate (unsigned int). Currently supported sample rate(s): 48000, 16000, 8000 */
#define NVAFX_PARAM_INPUT_SAMPLE_RATE "input_sample_rate"
/** Output Sample rate (unsigned int). Currently supported sample rate(s): 48000, 16000 */
#define NVAFX_PARAM_OUTPUT_SAMPLE_RATE "output_sample_rate"
/** Number of input samples per frame (unsigned int). This is immutable parameter */
#define NVAFX_PARAM_NUM_INPUT_SAMPLES_PER_FRAME "num_input_samples_per_frame"
/** Number of output samples per frame (unsigned int). This is immutable parameter */
#define NVAFX_PARAM_NUM_OUTPUT_SAMPLES_PER_FRAME "num_output_samples_per_frame"
/** Number of input audio channels */
#define NVAFX_PARAM_NUM_INPUT_CHANNELS "num_input_channels"
/** Number of output audio channels */
#define NVAFX_PARAM_NUM_OUTPUT_CHANNELS "num_output_channels"
/** Effect intensity factor (float) */
#define NVAFX_PARAM_INTENSITY_RATIO "intensity_ratio"

/** Deprecated parameters */
#pragma deprecated(NVAFX_PARAM_DENOISER_MODEL_PATH)
#define NVAFX_PARAM_DENOISER_MODEL_PATH NVAFX_PARAM_MODEL_PATH
#pragma deprecated(NVAFX_PARAM_DENOISER_SAMPLE_RATE)
#define NVAFX_PARAM_DENOISER_SAMPLE_RATE NVAFX_PARAM_SAMPLE_RATE
#pragma deprecated(NVAFX_PARAM_DENOISER_NUM_SAMPLES_PER_FRAME)
#define NVAFX_PARAM_DENOISER_NUM_SAMPLES_PER_FRAME NVAFX_PARAM_NUM_SAMPLES_PER_FRAME
#pragma deprecated(NVAFX_PARAM_DENOISER_NUM_CHANNELS)
#define NVAFX_PARAM_DENOISER_NUM_CHANNELS NVAFX_PARAM_NUM_CHANNELS
#pragma deprecated(NVAFX_PARAM_DENOISER_INTENSITY_RATIO)
#define NVAFX_PARAM_DENOISER_INTENSITY_RATIO NVAFX_PARAM_INTENSITY_RATIO

/** Number of audio channels **/
#pragma deprecated(NVAFX_PARAM_NUM_CHANNELS)
#define NVAFX_PARAM_NUM_CHANNELS "num_channels"
/** Sample rate (unsigned int). Currently supported sample rate(s): 48000, 16000 */
#pragma deprecated(NVAFX_PARAM_SAMPLE_RATE)
#define NVAFX_PARAM_SAMPLE_RATE "sample_rate"
/** Number of samples per frame (unsigned int). This is immutable parameter */
#pragma deprecated(NVAFX_PARAM_NUM_SAMPLES_PER_FRAME)
#define NVAFX_PARAM_NUM_SAMPLES_PER_FRAME "num_samples_per_frame"

#if defined(__cplusplus)
}
#endif

#endif  // __NVAUDIOEFFECTS_H__
