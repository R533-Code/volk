/**
 * volk
 *
 * Copyright (C) 2018-2025, by Arseny Kapoulkine (arseny.kapoulkine@gmail.com)
 * Report bugs and download new versions at https://github.com/zeux/volk
 *
 * This library is distributed under the MIT License. See notice at the end of this file.
 */
/* clang-format off */
#ifndef VOLK_H_
#define VOLK_H_

#ifndef VKAPI
# if defined(VOLK_VKAPI_EXPORT)
#  if defined(_WIN32) || defined(__CYGWIN__)
#   if defined(VOLK_VKAPI_EXPORT_BUILD)
#    if defined(__GNUC__)
#     define VKAPI __attribute__ ((dllexport)) extern
#    else
#     define VKAPI __declspec(dllexport) extern
#    endif
#   else
#    if defined(__GNUC__)
#     define VKAPI __attribute__ ((dllimport)) extern
#    else
#     define VKAPI __declspec(dllimport) extern
#    endif
#   endif
#  elif defined(__GNUC__) && defined(VOLK_VKAPI_EXPORT_BUILD)
#   define VKAPI __attribute__ ((visibility ("default"))) extern
#  else
#   define VKAPI extern
#  endif
# else
#  define VKAPI extern
# endif
#endif

#if defined(VOLK_NAMESPACE) && !defined(__cplusplus)
#error VOLK_NAMESPACE is only supported in C++
#endif

#if defined(VULKAN_H_) && !defined(VK_NO_PROTOTYPES)
#	error To use volk, you need to define VK_NO_PROTOTYPES before including vulkan.h
#endif

/* VOLK_GENERATE_VERSION_DEFINE */
#define VOLK_HEADER_VERSION 335
/* VOLK_GENERATE_VERSION_DEFINE */

#ifndef VK_NO_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif

#ifndef VULKAN_H_
#	ifdef VOLK_VULKAN_H_PATH
#		include VOLK_VULKAN_H_PATH
#	else /* Platform headers included below */
#		include <vulkan/vk_platform.h>
#		include <vulkan/vulkan_core.h>
#	endif
#endif

#ifdef __cplusplus
#ifdef VOLK_NAMESPACE
namespace volk {
#else
extern "C" {
#endif
#endif

struct VolkInstanceTable;
struct VolkDeviceTable;

/**
 * Initialize library by loading Vulkan loader; call this function before creating the Vulkan instance.
 *
 * Returns VK_SUCCESS on success and VK_ERROR_INITIALIZATION_FAILED otherwise.
 */
VkResult volkInitialize(void);

/**
 * Initialize library by providing a custom handler to load global symbols.
 *
 * This function can be used instead of volkInitialize.
 * The handler function pointer will be asked to load global Vulkan symbols which require no instance
 * (such as vkCreateInstance, vkEnumerateInstance* and vkEnumerateInstanceVersion if available).
 */
void volkInitializeCustom(PFN_vkGetInstanceProcAddr handler);

/**
 * Finalize library by unloading Vulkan loader and resetting global symbols to NULL.
 *
 * This function does not need to be called on process exit (as loader will be unloaded automatically) or if volkInitialize failed.
 * In general this function is optional to call but may be useful in rare cases eg if volk needs to be reinitialized multiple times.
 */
void volkFinalize(void);

/**
 * Get Vulkan instance version supported by the Vulkan loader, or 0 if Vulkan isn't supported
 *
 * Returns 0 if volkInitialize wasn't called or failed.
 */
uint32_t volkGetInstanceVersion(void);

/**
 * Load global function pointers using application-created VkInstance; call this function after creating the Vulkan instance.
 */
void volkLoadInstance(VkInstance instance);

/**
 * Load global function pointers using application-created VkInstance; call this function after creating the Vulkan instance.
 * Skips loading device-based function pointers, requires usage of volkLoadDevice afterwards.
 */
void volkLoadInstanceOnly(VkInstance instance);

/**
 * Load global function pointers using application-created VkDevice; call this function after creating the Vulkan device.
 *
 * Note: this is not suitable for applications that want to use multiple VkDevice objects concurrently.
 */
void volkLoadDevice(VkDevice device);

/**
 * Return last VkInstance for which global function pointers have been loaded via volkLoadInstance(),
 * or VK_NULL_HANDLE if volkLoadInstance() has not been called.
 */
VkInstance volkGetLoadedInstance(void);

/**
 * Return last VkDevice for which global function pointers have been loaded via volkLoadDevice(),
 * or VK_NULL_HANDLE if volkLoadDevice() has not been called.
 */
VkDevice volkGetLoadedDevice(void);

/**
 * Load function pointers using application-created VkInstance into a table.
 * Application should use function pointers from that table instead of using global function pointers.
 */
void volkLoadInstanceTable(struct VolkInstanceTable* table, VkInstance instance);

/**
 * Load function pointers using application-created VkDevice into a table.
 * Application should use function pointers from that table instead of using global function pointers.
 */
void volkLoadDeviceTable(struct VolkDeviceTable* table, VkDevice device);

#ifdef __cplusplus
} // extern "C" / namespace volk
#endif

/* Instead of directly including vulkan.h, we include platform-specific parts of the SDK manually
 * This is necessary to avoid including platform headers in some cases (which vulkan.h does unconditionally)
 * and replace them with forward declarations, which makes build times faster and avoids macro conflicts.
 *
 * Note that we only replace platform-specific headers when the headers are known to be problematic: very large
 * or slow to compile (Windows), or introducing unprefixed macros which can cause conflicts (Windows, Xlib).
 */
#if !defined(VULKAN_H_) && !defined(VOLK_VULKAN_H_PATH)

#ifdef VK_USE_PLATFORM_ANDROID_KHR
#include <vulkan/vulkan_android.h>
#endif

#ifdef VK_USE_PLATFORM_FUCHSIA
#include <zircon/types.h>
#include <vulkan/vulkan_fuchsia.h>
#endif

#ifdef VK_USE_PLATFORM_IOS_MVK
#include <vulkan/vulkan_ios.h>
#endif

#ifdef VK_USE_PLATFORM_MACOS_MVK
#include <vulkan/vulkan_macos.h>
#endif

#ifdef VK_USE_PLATFORM_METAL_EXT
#include <vulkan/vulkan_metal.h>
#endif

#ifdef VK_USE_PLATFORM_VI_NN
#include <vulkan/vulkan_vi.h>
#endif

#ifdef VK_USE_PLATFORM_WAYLAND_KHR
#include <vulkan/vulkan_wayland.h>
#endif

#ifdef VK_USE_PLATFORM_WIN32_KHR
typedef unsigned long DWORD;
typedef const wchar_t* LPCWSTR;
typedef void* HANDLE;
typedef struct HINSTANCE__* HINSTANCE;
typedef struct HWND__* HWND;
typedef struct HMONITOR__* HMONITOR;
typedef struct _SECURITY_ATTRIBUTES SECURITY_ATTRIBUTES;
#include <vulkan/vulkan_win32.h>
#endif

#ifdef VK_USE_PLATFORM_XCB_KHR
#include <xcb/xcb.h>
#include <vulkan/vulkan_xcb.h>
#endif

#ifdef VK_USE_PLATFORM_XLIB_KHR
typedef struct _XDisplay Display;
typedef unsigned long Window;
typedef unsigned long VisualID;
#include <vulkan/vulkan_xlib.h>
#endif

#ifdef VK_USE_PLATFORM_DIRECTFB_EXT
#include <directfb.h>
#include <vulkan/vulkan_directfb.h>
#endif

#ifdef VK_USE_PLATFORM_XLIB_XRANDR_EXT
typedef struct _XDisplay Display;
typedef unsigned long RROutput;
#include <vulkan/vulkan_xlib_xrandr.h>
#endif

#ifdef VK_USE_PLATFORM_GGP
#include <ggp_c/vulkan_types.h>
#include <vulkan/vulkan_ggp.h>
#endif

#ifdef VK_USE_PLATFORM_SCREEN_QNX
#include <screen/screen.h>
#include <vulkan/vulkan_screen.h>
#endif

#ifdef VK_USE_PLATFORM_SCI
#include <nvscisync.h>
#include <nvscibuf.h>
#include <vulkan/vulkan_sci.h>
#endif

#ifdef VK_ENABLE_BETA_EXTENSIONS
#include <vulkan/vulkan_beta.h>
#endif

#endif

#ifdef __cplusplus
#ifdef VOLK_NAMESPACE
namespace volk {
#else
extern "C" {
#endif
#endif

/**
 * Instance-specific function pointer table
 */
struct VolkInstanceTable
{
	/* VOLK_GENERATE_INSTANCE_TABLE */
#if defined(VK_VERSION_1_0)
	PFN_vkCreateDevice vkCreateDevice;
	PFN_vkDestroyInstance vkDestroyInstance;
	PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
	PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
	PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
	PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
	PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
	PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
	PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
	PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
	PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
	PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
#else
	PFN_vkVoidFunction padding_f34b07f4[13];
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
	PFN_vkEnumeratePhysicalDeviceGroups vkEnumeratePhysicalDeviceGroups;
	PFN_vkGetPhysicalDeviceExternalBufferProperties vkGetPhysicalDeviceExternalBufferProperties;
	PFN_vkGetPhysicalDeviceExternalFenceProperties vkGetPhysicalDeviceExternalFenceProperties;
	PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties;
	PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2;
	PFN_vkGetPhysicalDeviceFormatProperties2 vkGetPhysicalDeviceFormatProperties2;
	PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2;
	PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2;
	PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2;
	PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 vkGetPhysicalDeviceSparseImageFormatProperties2;
#else
	PFN_vkVoidFunction padding_73de037b[11];
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_3)
	PFN_vkGetPhysicalDeviceToolProperties vkGetPhysicalDeviceToolProperties;
#else
	PFN_vkVoidFunction padding_60958868[1];
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_ARM_data_graph)
	PFN_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM;
	PFN_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM;
#else
	PFN_vkVoidFunction padding_15920a35[2];
#endif /* defined(VK_ARM_data_graph) */
#if defined(VK_ARM_performance_counters_by_region)
	PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM;
#else
	PFN_vkVoidFunction padding_4fd09193[1];
#endif /* defined(VK_ARM_performance_counters_by_region) */
#if defined(VK_ARM_tensors)
	PFN_vkGetPhysicalDeviceExternalTensorPropertiesARM vkGetPhysicalDeviceExternalTensorPropertiesARM;
#else
	PFN_vkVoidFunction padding_4622403f[1];
#endif /* defined(VK_ARM_tensors) */
#if defined(VK_EXT_acquire_drm_display)
	PFN_vkAcquireDrmDisplayEXT vkAcquireDrmDisplayEXT;
	PFN_vkGetDrmDisplayEXT vkGetDrmDisplayEXT;
#else
	PFN_vkVoidFunction padding_8e427d62[2];
#endif /* defined(VK_EXT_acquire_drm_display) */
#if defined(VK_EXT_acquire_xlib_display)
	PFN_vkAcquireXlibDisplayEXT vkAcquireXlibDisplayEXT;
	PFN_vkGetRandROutputDisplayEXT vkGetRandROutputDisplayEXT;
#else
	PFN_vkVoidFunction padding_6e6f0a05[2];
#endif /* defined(VK_EXT_acquire_xlib_display) */
#if defined(VK_EXT_calibrated_timestamps)
	PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;
#else
	PFN_vkVoidFunction padding_61710136[1];
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_debug_report)
	PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
	PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;
	PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
#else
	PFN_vkVoidFunction padding_250c28de[3];
#endif /* defined(VK_EXT_debug_report) */
#if defined(VK_EXT_debug_utils)
	PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
	PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
	PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
	PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
	PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
	PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT;
	PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT;
	PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT;
	PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
	PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT;
	PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
#else
	PFN_vkVoidFunction padding_3e2e81f7[11];
#endif /* defined(VK_EXT_debug_utils) */
#if defined(VK_EXT_direct_mode_display)
	PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT;
#else
	PFN_vkVoidFunction padding_899830c3[1];
#endif /* defined(VK_EXT_direct_mode_display) */
#if defined(VK_EXT_directfb_surface)
	PFN_vkCreateDirectFBSurfaceEXT vkCreateDirectFBSurfaceEXT;
	PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT vkGetPhysicalDeviceDirectFBPresentationSupportEXT;
#else
	PFN_vkVoidFunction padding_f7e0f7b1[2];
#endif /* defined(VK_EXT_directfb_surface) */
#if defined(VK_EXT_display_surface_counter)
	PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT;
#else
	PFN_vkVoidFunction padding_8bff43f7[1];
#endif /* defined(VK_EXT_display_surface_counter) */
#if defined(VK_EXT_full_screen_exclusive)
	PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT;
#else
	PFN_vkVoidFunction padding_ff6b086[1];
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_headless_surface)
	PFN_vkCreateHeadlessSurfaceEXT vkCreateHeadlessSurfaceEXT;
#else
	PFN_vkVoidFunction padding_f8cab9e0[1];
#endif /* defined(VK_EXT_headless_surface) */
#if defined(VK_EXT_metal_surface)
	PFN_vkCreateMetalSurfaceEXT vkCreateMetalSurfaceEXT;
#else
	PFN_vkVoidFunction padding_1c6d079a[1];
#endif /* defined(VK_EXT_metal_surface) */
#if defined(VK_EXT_sample_locations)
	PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT;
#else
	PFN_vkVoidFunction padding_fd7ffce7[1];
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_tooling_info)
	PFN_vkGetPhysicalDeviceToolPropertiesEXT vkGetPhysicalDeviceToolPropertiesEXT;
#else
	PFN_vkVoidFunction padding_99aa5ee9[1];
#endif /* defined(VK_EXT_tooling_info) */
#if defined(VK_FUCHSIA_imagepipe_surface)
	PFN_vkCreateImagePipeSurfaceFUCHSIA vkCreateImagePipeSurfaceFUCHSIA;
#else
	PFN_vkVoidFunction padding_6db35e8f[1];
#endif /* defined(VK_FUCHSIA_imagepipe_surface) */
#if defined(VK_GGP_stream_descriptor_surface)
	PFN_vkCreateStreamDescriptorSurfaceGGP vkCreateStreamDescriptorSurfaceGGP;
#else
	PFN_vkVoidFunction padding_cc96d0ec[1];
#endif /* defined(VK_GGP_stream_descriptor_surface) */
#if defined(VK_KHR_android_surface)
	PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
#else
	PFN_vkVoidFunction padding_ab4fe82c[1];
#endif /* defined(VK_KHR_android_surface) */
#if defined(VK_KHR_calibrated_timestamps)
	PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR vkGetPhysicalDeviceCalibrateableTimeDomainsKHR;
#else
	PFN_vkVoidFunction padding_663b2fa0[1];
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_cooperative_matrix)
	PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR;
#else
	PFN_vkVoidFunction padding_59e376cc[1];
#endif /* defined(VK_KHR_cooperative_matrix) */
#if defined(VK_KHR_device_group_creation)
	PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR;
#else
	PFN_vkVoidFunction padding_6db81211[1];
#endif /* defined(VK_KHR_device_group_creation) */
#if defined(VK_KHR_display)
	PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR;
	PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR;
	PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR;
	PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR;
	PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR;
	PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
	PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR;
#else
	PFN_vkVoidFunction padding_cce37eaf[7];
#endif /* defined(VK_KHR_display) */
#if defined(VK_KHR_external_fence_capabilities)
	PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR;
#else
	PFN_vkVoidFunction padding_b2076412[1];
#endif /* defined(VK_KHR_external_fence_capabilities) */
#if defined(VK_KHR_external_memory_capabilities)
	PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR;
#else
	PFN_vkVoidFunction padding_f167e378[1];
#endif /* defined(VK_KHR_external_memory_capabilities) */
#if defined(VK_KHR_external_semaphore_capabilities)
	PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR;
#else
	PFN_vkVoidFunction padding_acdaf099[1];
#endif /* defined(VK_KHR_external_semaphore_capabilities) */
#if defined(VK_KHR_fragment_shading_rate)
	PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR vkGetPhysicalDeviceFragmentShadingRatesKHR;
#else
	PFN_vkVoidFunction padding_d59cae82[1];
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_display_properties2)
	PFN_vkGetDisplayModeProperties2KHR vkGetDisplayModeProperties2KHR;
	PFN_vkGetDisplayPlaneCapabilities2KHR vkGetDisplayPlaneCapabilities2KHR;
	PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR;
	PFN_vkGetPhysicalDeviceDisplayProperties2KHR vkGetPhysicalDeviceDisplayProperties2KHR;
#else
	PFN_vkVoidFunction padding_46c0938b[4];
#endif /* defined(VK_KHR_get_display_properties2) */
#if defined(VK_KHR_get_physical_device_properties2)
	PFN_vkGetPhysicalDeviceFeatures2KHR vkGetPhysicalDeviceFeatures2KHR;
	PFN_vkGetPhysicalDeviceFormatProperties2KHR vkGetPhysicalDeviceFormatProperties2KHR;
	PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2KHR;
	PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR;
	PFN_vkGetPhysicalDeviceProperties2KHR vkGetPhysicalDeviceProperties2KHR;
	PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR vkGetPhysicalDeviceQueueFamilyProperties2KHR;
	PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR;
#else
	PFN_vkVoidFunction padding_5fac460e[7];
#endif /* defined(VK_KHR_get_physical_device_properties2) */
#if defined(VK_KHR_get_surface_capabilities2)
	PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
	PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR;
#else
	PFN_vkVoidFunction padding_3baff606[2];
#endif /* defined(VK_KHR_get_surface_capabilities2) */
#if defined(VK_KHR_performance_query)
	PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR;
	PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR;
#else
	PFN_vkVoidFunction padding_1b45ef8f[2];
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_surface)
	PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
	PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
	PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
	PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
	PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
#else
	PFN_vkVoidFunction padding_8f1ea665[5];
#endif /* defined(VK_KHR_surface) */
#if defined(VK_KHR_video_encode_queue)
	PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR;
#else
	PFN_vkVoidFunction padding_f0a3114[1];
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
	PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR vkGetPhysicalDeviceVideoCapabilitiesKHR;
	PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR vkGetPhysicalDeviceVideoFormatPropertiesKHR;
#else
	PFN_vkVoidFunction padding_12d937aa[2];
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_KHR_wayland_surface)
	PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR;
	PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR;
#else
	PFN_vkVoidFunction padding_92436324[2];
#endif /* defined(VK_KHR_wayland_surface) */
#if defined(VK_KHR_win32_surface)
	PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
	PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#else
	PFN_vkVoidFunction padding_b8dcaf56[2];
#endif /* defined(VK_KHR_win32_surface) */
#if defined(VK_KHR_xcb_surface)
	PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
	PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR;
#else
	PFN_vkVoidFunction padding_b6b79326[2];
#endif /* defined(VK_KHR_xcb_surface) */
#if defined(VK_KHR_xlib_surface)
	PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
	PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR;
#else
	PFN_vkVoidFunction padding_c5e2b5db[2];
#endif /* defined(VK_KHR_xlib_surface) */
#if defined(VK_MVK_ios_surface)
	PFN_vkCreateIOSSurfaceMVK vkCreateIOSSurfaceMVK;
#else
	PFN_vkVoidFunction padding_52f99096[1];
#endif /* defined(VK_MVK_ios_surface) */
#if defined(VK_MVK_macos_surface)
	PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK;
#else
	PFN_vkVoidFunction padding_1d7ced9a[1];
#endif /* defined(VK_MVK_macos_surface) */
#if defined(VK_NN_vi_surface)
	PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN;
#else
	PFN_vkVoidFunction padding_d9ec3901[1];
#endif /* defined(VK_NN_vi_surface) */
#if defined(VK_NV_acquire_winrt_display)
	PFN_vkAcquireWinrtDisplayNV vkAcquireWinrtDisplayNV;
	PFN_vkGetWinrtDisplayNV vkGetWinrtDisplayNV;
#else
	PFN_vkVoidFunction padding_41c66e6[2];
#endif /* defined(VK_NV_acquire_winrt_display) */
#if defined(VK_NV_cooperative_matrix)
	PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV vkGetPhysicalDeviceCooperativeMatrixPropertiesNV;
#else
	PFN_vkVoidFunction padding_ee7fcfc8[1];
#endif /* defined(VK_NV_cooperative_matrix) */
#if defined(VK_NV_cooperative_matrix2)
	PFN_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV;
#else
	PFN_vkVoidFunction padding_2ec091f4[1];
#endif /* defined(VK_NV_cooperative_matrix2) */
#if defined(VK_NV_cooperative_vector)
	PFN_vkGetPhysicalDeviceCooperativeVectorPropertiesNV vkGetPhysicalDeviceCooperativeVectorPropertiesNV;
#else
	PFN_vkVoidFunction padding_50d51145[1];
#endif /* defined(VK_NV_cooperative_vector) */
#if defined(VK_NV_coverage_reduction_mode)
	PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
#else
	PFN_vkVoidFunction padding_9a9f15ac[1];
#endif /* defined(VK_NV_coverage_reduction_mode) */
#if defined(VK_NV_external_memory_capabilities)
	PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV vkGetPhysicalDeviceExternalImageFormatPropertiesNV;
#else
	PFN_vkVoidFunction padding_988145[1];
#endif /* defined(VK_NV_external_memory_capabilities) */
#if defined(VK_NV_optical_flow)
	PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV vkGetPhysicalDeviceOpticalFlowImageFormatsNV;
#else
	PFN_vkVoidFunction padding_46a4b95[1];
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_OHOS_surface)
	PFN_vkCreateSurfaceOHOS vkCreateSurfaceOHOS;
#else
	PFN_vkVoidFunction padding_b94570ee[1];
#endif /* defined(VK_OHOS_surface) */
#if defined(VK_QNX_screen_surface)
	PFN_vkCreateScreenSurfaceQNX vkCreateScreenSurfaceQNX;
	PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX vkGetPhysicalDeviceScreenPresentationSupportQNX;
#else
	PFN_vkVoidFunction padding_9b43b57c[2];
#endif /* defined(VK_QNX_screen_surface) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR;
#else
	PFN_vkVoidFunction padding_a8092b55[1];
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
	/* VOLK_GENERATE_INSTANCE_TABLE */
};

/**
 * Device-specific function pointer table
 */
struct VolkDeviceTable
{
	/* VOLK_GENERATE_DEVICE_TABLE */
#if defined(VK_VERSION_1_0)
	PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
	PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
	PFN_vkAllocateMemory vkAllocateMemory;
	PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
	PFN_vkBindBufferMemory vkBindBufferMemory;
	PFN_vkBindImageMemory vkBindImageMemory;
	PFN_vkCmdBeginQuery vkCmdBeginQuery;
	PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
	PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
	PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
	PFN_vkCmdBindPipeline vkCmdBindPipeline;
	PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
	PFN_vkCmdBlitImage vkCmdBlitImage;
	PFN_vkCmdClearAttachments vkCmdClearAttachments;
	PFN_vkCmdClearColorImage vkCmdClearColorImage;
	PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
	PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
	PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
	PFN_vkCmdCopyImage vkCmdCopyImage;
	PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
	PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
	PFN_vkCmdDispatch vkCmdDispatch;
	PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
	PFN_vkCmdDraw vkCmdDraw;
	PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
	PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
	PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
	PFN_vkCmdEndQuery vkCmdEndQuery;
	PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
	PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
	PFN_vkCmdFillBuffer vkCmdFillBuffer;
	PFN_vkCmdNextSubpass vkCmdNextSubpass;
	PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
	PFN_vkCmdPushConstants vkCmdPushConstants;
	PFN_vkCmdResetEvent vkCmdResetEvent;
	PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
	PFN_vkCmdResolveImage vkCmdResolveImage;
	PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
	PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
	PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
	PFN_vkCmdSetEvent vkCmdSetEvent;
	PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
	PFN_vkCmdSetScissor vkCmdSetScissor;
	PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
	PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
	PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
	PFN_vkCmdSetViewport vkCmdSetViewport;
	PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
	PFN_vkCmdWaitEvents vkCmdWaitEvents;
	PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
	PFN_vkCreateBuffer vkCreateBuffer;
	PFN_vkCreateBufferView vkCreateBufferView;
	PFN_vkCreateCommandPool vkCreateCommandPool;
	PFN_vkCreateComputePipelines vkCreateComputePipelines;
	PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
	PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
	PFN_vkCreateEvent vkCreateEvent;
	PFN_vkCreateFence vkCreateFence;
	PFN_vkCreateFramebuffer vkCreateFramebuffer;
	PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
	PFN_vkCreateImage vkCreateImage;
	PFN_vkCreateImageView vkCreateImageView;
	PFN_vkCreatePipelineCache vkCreatePipelineCache;
	PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
	PFN_vkCreateQueryPool vkCreateQueryPool;
	PFN_vkCreateRenderPass vkCreateRenderPass;
	PFN_vkCreateSampler vkCreateSampler;
	PFN_vkCreateSemaphore vkCreateSemaphore;
	PFN_vkCreateShaderModule vkCreateShaderModule;
	PFN_vkDestroyBuffer vkDestroyBuffer;
	PFN_vkDestroyBufferView vkDestroyBufferView;
	PFN_vkDestroyCommandPool vkDestroyCommandPool;
	PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
	PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
	PFN_vkDestroyDevice vkDestroyDevice;
	PFN_vkDestroyEvent vkDestroyEvent;
	PFN_vkDestroyFence vkDestroyFence;
	PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
	PFN_vkDestroyImage vkDestroyImage;
	PFN_vkDestroyImageView vkDestroyImageView;
	PFN_vkDestroyPipeline vkDestroyPipeline;
	PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
	PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
	PFN_vkDestroyQueryPool vkDestroyQueryPool;
	PFN_vkDestroyRenderPass vkDestroyRenderPass;
	PFN_vkDestroySampler vkDestroySampler;
	PFN_vkDestroySemaphore vkDestroySemaphore;
	PFN_vkDestroyShaderModule vkDestroyShaderModule;
	PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
	PFN_vkEndCommandBuffer vkEndCommandBuffer;
	PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
	PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
	PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
	PFN_vkFreeMemory vkFreeMemory;
	PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
	PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
	PFN_vkGetDeviceQueue vkGetDeviceQueue;
	PFN_vkGetEventStatus vkGetEventStatus;
	PFN_vkGetFenceStatus vkGetFenceStatus;
	PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
	PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
	PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
	PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
	PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
	PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
	PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
	PFN_vkMapMemory vkMapMemory;
	PFN_vkMergePipelineCaches vkMergePipelineCaches;
	PFN_vkQueueBindSparse vkQueueBindSparse;
	PFN_vkQueueSubmit vkQueueSubmit;
	PFN_vkQueueWaitIdle vkQueueWaitIdle;
	PFN_vkResetCommandBuffer vkResetCommandBuffer;
	PFN_vkResetCommandPool vkResetCommandPool;
	PFN_vkResetDescriptorPool vkResetDescriptorPool;
	PFN_vkResetEvent vkResetEvent;
	PFN_vkResetFences vkResetFences;
	PFN_vkSetEvent vkSetEvent;
	PFN_vkUnmapMemory vkUnmapMemory;
	PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
	PFN_vkWaitForFences vkWaitForFences;
#else
	PFN_vkVoidFunction padding_6ce80d51[120];
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
	PFN_vkBindBufferMemory2 vkBindBufferMemory2;
	PFN_vkBindImageMemory2 vkBindImageMemory2;
	PFN_vkCmdDispatchBase vkCmdDispatchBase;
	PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask;
	PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate;
	PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
	PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate;
	PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;
	PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
	PFN_vkGetDescriptorSetLayoutSupport vkGetDescriptorSetLayoutSupport;
	PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures;
	PFN_vkGetDeviceQueue2 vkGetDeviceQueue2;
	PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
	PFN_vkGetImageSparseMemoryRequirements2 vkGetImageSparseMemoryRequirements2;
	PFN_vkTrimCommandPool vkTrimCommandPool;
	PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate;
#else
	PFN_vkVoidFunction padding_1ec56847[16];
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_2)
	PFN_vkCmdBeginRenderPass2 vkCmdBeginRenderPass2;
	PFN_vkCmdDrawIndexedIndirectCount vkCmdDrawIndexedIndirectCount;
	PFN_vkCmdDrawIndirectCount vkCmdDrawIndirectCount;
	PFN_vkCmdEndRenderPass2 vkCmdEndRenderPass2;
	PFN_vkCmdNextSubpass2 vkCmdNextSubpass2;
	PFN_vkCreateRenderPass2 vkCreateRenderPass2;
	PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
	PFN_vkGetBufferOpaqueCaptureAddress vkGetBufferOpaqueCaptureAddress;
	PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress;
	PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue;
	PFN_vkResetQueryPool vkResetQueryPool;
	PFN_vkSignalSemaphore vkSignalSemaphore;
	PFN_vkWaitSemaphores vkWaitSemaphores;
#else
	PFN_vkVoidFunction padding_a3e00662[13];
#endif /* defined(VK_VERSION_1_2) */
#if defined(VK_VERSION_1_3)
	PFN_vkCmdBeginRendering vkCmdBeginRendering;
	PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2;
	PFN_vkCmdBlitImage2 vkCmdBlitImage2;
	PFN_vkCmdCopyBuffer2 vkCmdCopyBuffer2;
	PFN_vkCmdCopyBufferToImage2 vkCmdCopyBufferToImage2;
	PFN_vkCmdCopyImage2 vkCmdCopyImage2;
	PFN_vkCmdCopyImageToBuffer2 vkCmdCopyImageToBuffer2;
	PFN_vkCmdEndRendering vkCmdEndRendering;
	PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2;
	PFN_vkCmdResetEvent2 vkCmdResetEvent2;
	PFN_vkCmdResolveImage2 vkCmdResolveImage2;
	PFN_vkCmdSetCullMode vkCmdSetCullMode;
	PFN_vkCmdSetDepthBiasEnable vkCmdSetDepthBiasEnable;
	PFN_vkCmdSetDepthBoundsTestEnable vkCmdSetDepthBoundsTestEnable;
	PFN_vkCmdSetDepthCompareOp vkCmdSetDepthCompareOp;
	PFN_vkCmdSetDepthTestEnable vkCmdSetDepthTestEnable;
	PFN_vkCmdSetDepthWriteEnable vkCmdSetDepthWriteEnable;
	PFN_vkCmdSetEvent2 vkCmdSetEvent2;
	PFN_vkCmdSetFrontFace vkCmdSetFrontFace;
	PFN_vkCmdSetPrimitiveRestartEnable vkCmdSetPrimitiveRestartEnable;
	PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology;
	PFN_vkCmdSetRasterizerDiscardEnable vkCmdSetRasterizerDiscardEnable;
	PFN_vkCmdSetScissorWithCount vkCmdSetScissorWithCount;
	PFN_vkCmdSetStencilOp vkCmdSetStencilOp;
	PFN_vkCmdSetStencilTestEnable vkCmdSetStencilTestEnable;
	PFN_vkCmdSetViewportWithCount vkCmdSetViewportWithCount;
	PFN_vkCmdWaitEvents2 vkCmdWaitEvents2;
	PFN_vkCmdWriteTimestamp2 vkCmdWriteTimestamp2;
	PFN_vkCreatePrivateDataSlot vkCreatePrivateDataSlot;
	PFN_vkDestroyPrivateDataSlot vkDestroyPrivateDataSlot;
	PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements;
	PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements;
	PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements;
	PFN_vkGetPrivateData vkGetPrivateData;
	PFN_vkQueueSubmit2 vkQueueSubmit2;
	PFN_vkSetPrivateData vkSetPrivateData;
#else
	PFN_vkVoidFunction padding_ee798a88[36];
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_VERSION_1_4)
	PFN_vkCmdBindDescriptorSets2 vkCmdBindDescriptorSets2;
	PFN_vkCmdBindIndexBuffer2 vkCmdBindIndexBuffer2;
	PFN_vkCmdPushConstants2 vkCmdPushConstants2;
	PFN_vkCmdPushDescriptorSet vkCmdPushDescriptorSet;
	PFN_vkCmdPushDescriptorSet2 vkCmdPushDescriptorSet2;
	PFN_vkCmdPushDescriptorSetWithTemplate vkCmdPushDescriptorSetWithTemplate;
	PFN_vkCmdPushDescriptorSetWithTemplate2 vkCmdPushDescriptorSetWithTemplate2;
	PFN_vkCmdSetLineStipple vkCmdSetLineStipple;
	PFN_vkCmdSetRenderingAttachmentLocations vkCmdSetRenderingAttachmentLocations;
	PFN_vkCmdSetRenderingInputAttachmentIndices vkCmdSetRenderingInputAttachmentIndices;
	PFN_vkCopyImageToImage vkCopyImageToImage;
	PFN_vkCopyImageToMemory vkCopyImageToMemory;
	PFN_vkCopyMemoryToImage vkCopyMemoryToImage;
	PFN_vkGetDeviceImageSubresourceLayout vkGetDeviceImageSubresourceLayout;
	PFN_vkGetImageSubresourceLayout2 vkGetImageSubresourceLayout2;
	PFN_vkGetRenderingAreaGranularity vkGetRenderingAreaGranularity;
	PFN_vkMapMemory2 vkMapMemory2;
	PFN_vkTransitionImageLayout vkTransitionImageLayout;
	PFN_vkUnmapMemory2 vkUnmapMemory2;
#else
	PFN_vkVoidFunction padding_82585fa3[19];
#endif /* defined(VK_VERSION_1_4) */
#if defined(VK_AMDX_shader_enqueue)
	PFN_vkCmdDispatchGraphAMDX vkCmdDispatchGraphAMDX;
	PFN_vkCmdDispatchGraphIndirectAMDX vkCmdDispatchGraphIndirectAMDX;
	PFN_vkCmdDispatchGraphIndirectCountAMDX vkCmdDispatchGraphIndirectCountAMDX;
	PFN_vkCmdInitializeGraphScratchMemoryAMDX vkCmdInitializeGraphScratchMemoryAMDX;
	PFN_vkCreateExecutionGraphPipelinesAMDX vkCreateExecutionGraphPipelinesAMDX;
	PFN_vkGetExecutionGraphPipelineNodeIndexAMDX vkGetExecutionGraphPipelineNodeIndexAMDX;
	PFN_vkGetExecutionGraphPipelineScratchSizeAMDX vkGetExecutionGraphPipelineScratchSizeAMDX;
#else
	PFN_vkVoidFunction padding_9d3e2bba[7];
#endif /* defined(VK_AMDX_shader_enqueue) */
#if defined(VK_AMD_anti_lag)
	PFN_vkAntiLagUpdateAMD vkAntiLagUpdateAMD;
#else
	PFN_vkVoidFunction padding_cf792fb4[1];
#endif /* defined(VK_AMD_anti_lag) */
#if defined(VK_AMD_buffer_marker)
	PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD;
#else
	PFN_vkVoidFunction padding_7836e92f[1];
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2))
	PFN_vkCmdWriteBufferMarker2AMD vkCmdWriteBufferMarker2AMD;
#else
	PFN_vkVoidFunction padding_bbf9b7bb[1];
#endif /* defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) */
#if defined(VK_AMD_display_native_hdr)
	PFN_vkSetLocalDimmingAMD vkSetLocalDimmingAMD;
#else
	PFN_vkVoidFunction padding_6b81b2fb[1];
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
	PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD;
	PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD;
#else
	PFN_vkVoidFunction padding_fbfa9964[2];
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
	PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD;
#else
	PFN_vkVoidFunction padding_bfb754b[1];
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
	PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
	PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID;
#else
	PFN_vkVoidFunction padding_c67b1beb[2];
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_ARM_data_graph)
	PFN_vkBindDataGraphPipelineSessionMemoryARM vkBindDataGraphPipelineSessionMemoryARM;
	PFN_vkCmdDispatchDataGraphARM vkCmdDispatchDataGraphARM;
	PFN_vkCreateDataGraphPipelineSessionARM vkCreateDataGraphPipelineSessionARM;
	PFN_vkCreateDataGraphPipelinesARM vkCreateDataGraphPipelinesARM;
	PFN_vkDestroyDataGraphPipelineSessionARM vkDestroyDataGraphPipelineSessionARM;
	PFN_vkGetDataGraphPipelineAvailablePropertiesARM vkGetDataGraphPipelineAvailablePropertiesARM;
	PFN_vkGetDataGraphPipelinePropertiesARM vkGetDataGraphPipelinePropertiesARM;
	PFN_vkGetDataGraphPipelineSessionBindPointRequirementsARM vkGetDataGraphPipelineSessionBindPointRequirementsARM;
	PFN_vkGetDataGraphPipelineSessionMemoryRequirementsARM vkGetDataGraphPipelineSessionMemoryRequirementsARM;
#else
	PFN_vkVoidFunction padding_894d85d8[9];
#endif /* defined(VK_ARM_data_graph) */
#if defined(VK_ARM_tensors)
	PFN_vkBindTensorMemoryARM vkBindTensorMemoryARM;
	PFN_vkCmdCopyTensorARM vkCmdCopyTensorARM;
	PFN_vkCreateTensorARM vkCreateTensorARM;
	PFN_vkCreateTensorViewARM vkCreateTensorViewARM;
	PFN_vkDestroyTensorARM vkDestroyTensorARM;
	PFN_vkDestroyTensorViewARM vkDestroyTensorViewARM;
	PFN_vkGetDeviceTensorMemoryRequirementsARM vkGetDeviceTensorMemoryRequirementsARM;
	PFN_vkGetTensorMemoryRequirementsARM vkGetTensorMemoryRequirementsARM;
#else
	PFN_vkVoidFunction padding_df67a729[8];
#endif /* defined(VK_ARM_tensors) */
#if defined(VK_ARM_tensors) && defined(VK_EXT_descriptor_buffer)
	PFN_vkGetTensorOpaqueCaptureDescriptorDataARM vkGetTensorOpaqueCaptureDescriptorDataARM;
	PFN_vkGetTensorViewOpaqueCaptureDescriptorDataARM vkGetTensorViewOpaqueCaptureDescriptorDataARM;
#else
	PFN_vkVoidFunction padding_9483bf7e[2];
#endif /* defined(VK_ARM_tensors) && defined(VK_EXT_descriptor_buffer) */
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state)
	PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT vkCmdSetAttachmentFeedbackLoopEnableEXT;
#else
	PFN_vkVoidFunction padding_760a41f5[1];
#endif /* defined(VK_EXT_attachment_feedback_loop_dynamic_state) */
#if defined(VK_EXT_buffer_device_address)
	PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressEXT;
#else
	PFN_vkVoidFunction padding_3b69d885[1];
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
	PFN_vkGetCalibratedTimestampsEXT vkGetCalibratedTimestampsEXT;
#else
	PFN_vkVoidFunction padding_d0981c89[1];
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
	PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT;
#else
	PFN_vkVoidFunction padding_d301ecc3[1];
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
	PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT;
	PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT;
#else
	PFN_vkVoidFunction padding_ab532c18[2];
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_custom_resolve) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))
	PFN_vkCmdBeginCustomResolveEXT vkCmdBeginCustomResolveEXT;
#else
	PFN_vkVoidFunction padding_962e418a[1];
#endif /* defined(VK_EXT_custom_resolve) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)) */
#if defined(VK_EXT_debug_marker)
	PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
	PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
	PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
	PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
	PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
#else
	PFN_vkVoidFunction padding_89986968[5];
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_depth_bias_control)
	PFN_vkCmdSetDepthBias2EXT vkCmdSetDepthBias2EXT;
#else
	PFN_vkVoidFunction padding_bcddab4d[1];
#endif /* defined(VK_EXT_depth_bias_control) */
#if defined(VK_EXT_descriptor_buffer)
	PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT vkCmdBindDescriptorBufferEmbeddedSamplersEXT;
	PFN_vkCmdBindDescriptorBuffersEXT vkCmdBindDescriptorBuffersEXT;
	PFN_vkCmdSetDescriptorBufferOffsetsEXT vkCmdSetDescriptorBufferOffsetsEXT;
	PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT vkGetBufferOpaqueCaptureDescriptorDataEXT;
	PFN_vkGetDescriptorEXT vkGetDescriptorEXT;
	PFN_vkGetDescriptorSetLayoutBindingOffsetEXT vkGetDescriptorSetLayoutBindingOffsetEXT;
	PFN_vkGetDescriptorSetLayoutSizeEXT vkGetDescriptorSetLayoutSizeEXT;
	PFN_vkGetImageOpaqueCaptureDescriptorDataEXT vkGetImageOpaqueCaptureDescriptorDataEXT;
	PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT vkGetImageViewOpaqueCaptureDescriptorDataEXT;
	PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT vkGetSamplerOpaqueCaptureDescriptorDataEXT;
#else
	PFN_vkVoidFunction padding_80aa973c[10];
#endif /* defined(VK_EXT_descriptor_buffer) */
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
	PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT;
#else
	PFN_vkVoidFunction padding_98d0fb33[1];
#endif /* defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)) */
#if defined(VK_EXT_device_fault)
	PFN_vkGetDeviceFaultInfoEXT vkGetDeviceFaultInfoEXT;
#else
	PFN_vkVoidFunction padding_55095419[1];
#endif /* defined(VK_EXT_device_fault) */
#if defined(VK_EXT_device_generated_commands)
	PFN_vkCmdExecuteGeneratedCommandsEXT vkCmdExecuteGeneratedCommandsEXT;
	PFN_vkCmdPreprocessGeneratedCommandsEXT vkCmdPreprocessGeneratedCommandsEXT;
	PFN_vkCreateIndirectCommandsLayoutEXT vkCreateIndirectCommandsLayoutEXT;
	PFN_vkCreateIndirectExecutionSetEXT vkCreateIndirectExecutionSetEXT;
	PFN_vkDestroyIndirectCommandsLayoutEXT vkDestroyIndirectCommandsLayoutEXT;
	PFN_vkDestroyIndirectExecutionSetEXT vkDestroyIndirectExecutionSetEXT;
	PFN_vkGetGeneratedCommandsMemoryRequirementsEXT vkGetGeneratedCommandsMemoryRequirementsEXT;
	PFN_vkUpdateIndirectExecutionSetPipelineEXT vkUpdateIndirectExecutionSetPipelineEXT;
	PFN_vkUpdateIndirectExecutionSetShaderEXT vkUpdateIndirectExecutionSetShaderEXT;
#else
	PFN_vkVoidFunction padding_7ba7ebaa[9];
#endif /* defined(VK_EXT_device_generated_commands) */
#if defined(VK_EXT_discard_rectangles)
	PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT;
#else
	PFN_vkVoidFunction padding_d6355c2[1];
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2
	PFN_vkCmdSetDiscardRectangleEnableEXT vkCmdSetDiscardRectangleEnableEXT;
	PFN_vkCmdSetDiscardRectangleModeEXT vkCmdSetDiscardRectangleModeEXT;
#else
	PFN_vkVoidFunction padding_7bb44f77[2];
#endif /* defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 */
#if defined(VK_EXT_display_control)
	PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT;
	PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT;
	PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT;
	PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT;
#else
	PFN_vkVoidFunction padding_d30dfaaf[4];
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_external_memory_host)
	PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT;
#else
	PFN_vkVoidFunction padding_357656e9[1];
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_external_memory_metal)
	PFN_vkGetMemoryMetalHandleEXT vkGetMemoryMetalHandleEXT;
	PFN_vkGetMemoryMetalHandlePropertiesEXT vkGetMemoryMetalHandlePropertiesEXT;
#else
	PFN_vkVoidFunction padding_37d43fb[2];
#endif /* defined(VK_EXT_external_memory_metal) */
#if defined(VK_EXT_fragment_density_map_offset)
	PFN_vkCmdEndRendering2EXT vkCmdEndRendering2EXT;
#else
	PFN_vkVoidFunction padding_9c90cf11[1];
#endif /* defined(VK_EXT_fragment_density_map_offset) */
#if defined(VK_EXT_full_screen_exclusive)
	PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT;
	PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT;
#else
	PFN_vkVoidFunction padding_3859df46[2];
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1))
	PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT;
#else
	PFN_vkVoidFunction padding_e5b48b5b[1];
#endif /* defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1)) */
#if defined(VK_EXT_hdr_metadata)
	PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT;
#else
	PFN_vkVoidFunction padding_ca6d733c[1];
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_host_image_copy)
	PFN_vkCopyImageToImageEXT vkCopyImageToImageEXT;
	PFN_vkCopyImageToMemoryEXT vkCopyImageToMemoryEXT;
	PFN_vkCopyMemoryToImageEXT vkCopyMemoryToImageEXT;
	PFN_vkTransitionImageLayoutEXT vkTransitionImageLayoutEXT;
#else
	PFN_vkVoidFunction padding_dd6d9b61[4];
#endif /* defined(VK_EXT_host_image_copy) */
#if defined(VK_EXT_host_query_reset)
	PFN_vkResetQueryPoolEXT vkResetQueryPoolEXT;
#else
	PFN_vkVoidFunction padding_34e58bd3[1];
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_drm_format_modifier)
	PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT;
#else
	PFN_vkVoidFunction padding_eb50dc14[1];
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
	PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT;
#else
	PFN_vkVoidFunction padding_8a212c37[1];
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_memory_decompression)
	PFN_vkCmdDecompressMemoryEXT vkCmdDecompressMemoryEXT;
	PFN_vkCmdDecompressMemoryIndirectCountEXT vkCmdDecompressMemoryIndirectCountEXT;
#else
	PFN_vkVoidFunction padding_c3b649ee[2];
#endif /* defined(VK_EXT_memory_decompression) */
#if defined(VK_EXT_mesh_shader)
	PFN_vkCmdDrawMeshTasksEXT vkCmdDrawMeshTasksEXT;
	PFN_vkCmdDrawMeshTasksIndirectEXT vkCmdDrawMeshTasksIndirectEXT;
#else
	PFN_vkVoidFunction padding_f65e838[2];
#endif /* defined(VK_EXT_mesh_shader) */
#if defined(VK_EXT_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count))
	PFN_vkCmdDrawMeshTasksIndirectCountEXT vkCmdDrawMeshTasksIndirectCountEXT;
#else
	PFN_vkVoidFunction padding_dcbaac2f[1];
#endif /* defined(VK_EXT_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) */
#if defined(VK_EXT_metal_objects)
	PFN_vkExportMetalObjectsEXT vkExportMetalObjectsEXT;
#else
	PFN_vkVoidFunction padding_df21f735[1];
#endif /* defined(VK_EXT_metal_objects) */
#if defined(VK_EXT_multi_draw)
	PFN_vkCmdDrawMultiEXT vkCmdDrawMultiEXT;
	PFN_vkCmdDrawMultiIndexedEXT vkCmdDrawMultiIndexedEXT;
#else
	PFN_vkVoidFunction padding_ce8b93b6[2];
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_opacity_micromap)
	PFN_vkBuildMicromapsEXT vkBuildMicromapsEXT;
	PFN_vkCmdBuildMicromapsEXT vkCmdBuildMicromapsEXT;
	PFN_vkCmdCopyMemoryToMicromapEXT vkCmdCopyMemoryToMicromapEXT;
	PFN_vkCmdCopyMicromapEXT vkCmdCopyMicromapEXT;
	PFN_vkCmdCopyMicromapToMemoryEXT vkCmdCopyMicromapToMemoryEXT;
	PFN_vkCmdWriteMicromapsPropertiesEXT vkCmdWriteMicromapsPropertiesEXT;
	PFN_vkCopyMemoryToMicromapEXT vkCopyMemoryToMicromapEXT;
	PFN_vkCopyMicromapEXT vkCopyMicromapEXT;
	PFN_vkCopyMicromapToMemoryEXT vkCopyMicromapToMemoryEXT;
	PFN_vkCreateMicromapEXT vkCreateMicromapEXT;
	PFN_vkDestroyMicromapEXT vkDestroyMicromapEXT;
	PFN_vkGetDeviceMicromapCompatibilityEXT vkGetDeviceMicromapCompatibilityEXT;
	PFN_vkGetMicromapBuildSizesEXT vkGetMicromapBuildSizesEXT;
	PFN_vkWriteMicromapsPropertiesEXT vkWriteMicromapsPropertiesEXT;
#else
	PFN_vkVoidFunction padding_fa41e53c[14];
#endif /* defined(VK_EXT_opacity_micromap) */
#if defined(VK_EXT_pageable_device_local_memory)
	PFN_vkSetDeviceMemoryPriorityEXT vkSetDeviceMemoryPriorityEXT;
#else
	PFN_vkVoidFunction padding_b2d2c2d7[1];
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_pipeline_properties)
	PFN_vkGetPipelinePropertiesEXT vkGetPipelinePropertiesEXT;
#else
	PFN_vkVoidFunction padding_11313020[1];
#endif /* defined(VK_EXT_pipeline_properties) */
#if defined(VK_EXT_present_timing)
	PFN_vkGetPastPresentationTimingEXT vkGetPastPresentationTimingEXT;
	PFN_vkGetSwapchainTimeDomainPropertiesEXT vkGetSwapchainTimeDomainPropertiesEXT;
	PFN_vkGetSwapchainTimingPropertiesEXT vkGetSwapchainTimingPropertiesEXT;
	PFN_vkSetSwapchainPresentTimingQueueSizeEXT vkSetSwapchainPresentTimingQueueSizeEXT;
#else
	PFN_vkVoidFunction padding_8751feb5[4];
#endif /* defined(VK_EXT_present_timing) */
#if defined(VK_EXT_private_data)
	PFN_vkCreatePrivateDataSlotEXT vkCreatePrivateDataSlotEXT;
	PFN_vkDestroyPrivateDataSlotEXT vkDestroyPrivateDataSlotEXT;
	PFN_vkGetPrivateDataEXT vkGetPrivateDataEXT;
	PFN_vkSetPrivateDataEXT vkSetPrivateDataEXT;
#else
	PFN_vkVoidFunction padding_108010f[4];
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
	PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT;
#else
	PFN_vkVoidFunction padding_26f9079f[1];
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_shader_module_identifier)
	PFN_vkGetShaderModuleCreateInfoIdentifierEXT vkGetShaderModuleCreateInfoIdentifierEXT;
	PFN_vkGetShaderModuleIdentifierEXT vkGetShaderModuleIdentifierEXT;
#else
	PFN_vkVoidFunction padding_e10c8f86[2];
#endif /* defined(VK_EXT_shader_module_identifier) */
#if defined(VK_EXT_shader_object)
	PFN_vkCmdBindShadersEXT vkCmdBindShadersEXT;
	PFN_vkCreateShadersEXT vkCreateShadersEXT;
	PFN_vkDestroyShaderEXT vkDestroyShaderEXT;
	PFN_vkGetShaderBinaryDataEXT vkGetShaderBinaryDataEXT;
#else
	PFN_vkVoidFunction padding_374f3e18[4];
#endif /* defined(VK_EXT_shader_object) */
#if defined(VK_EXT_swapchain_maintenance1)
	PFN_vkReleaseSwapchainImagesEXT vkReleaseSwapchainImagesEXT;
#else
	PFN_vkVoidFunction padding_ea55bf74[1];
#endif /* defined(VK_EXT_swapchain_maintenance1) */
#if defined(VK_EXT_transform_feedback)
	PFN_vkCmdBeginQueryIndexedEXT vkCmdBeginQueryIndexedEXT;
	PFN_vkCmdBeginTransformFeedbackEXT vkCmdBeginTransformFeedbackEXT;
	PFN_vkCmdBindTransformFeedbackBuffersEXT vkCmdBindTransformFeedbackBuffersEXT;
	PFN_vkCmdDrawIndirectByteCountEXT vkCmdDrawIndirectByteCountEXT;
	PFN_vkCmdEndQueryIndexedEXT vkCmdEndQueryIndexedEXT;
	PFN_vkCmdEndTransformFeedbackEXT vkCmdEndTransformFeedbackEXT;
#else
	PFN_vkVoidFunction padding_36980658[6];
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
	PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT;
	PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT;
	PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT;
	PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT;
#else
	PFN_vkVoidFunction padding_b4f2df29[4];
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_FUCHSIA_buffer_collection)
	PFN_vkCreateBufferCollectionFUCHSIA vkCreateBufferCollectionFUCHSIA;
	PFN_vkDestroyBufferCollectionFUCHSIA vkDestroyBufferCollectionFUCHSIA;
	PFN_vkGetBufferCollectionPropertiesFUCHSIA vkGetBufferCollectionPropertiesFUCHSIA;
	PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA vkSetBufferCollectionBufferConstraintsFUCHSIA;
	PFN_vkSetBufferCollectionImageConstraintsFUCHSIA vkSetBufferCollectionImageConstraintsFUCHSIA;
#else
	PFN_vkVoidFunction padding_8eaa27bc[5];
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
	PFN_vkGetMemoryZirconHandleFUCHSIA vkGetMemoryZirconHandleFUCHSIA;
	PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA vkGetMemoryZirconHandlePropertiesFUCHSIA;
#else
	PFN_vkVoidFunction padding_e3cb8a67[2];
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
	PFN_vkGetSemaphoreZirconHandleFUCHSIA vkGetSemaphoreZirconHandleFUCHSIA;
	PFN_vkImportSemaphoreZirconHandleFUCHSIA vkImportSemaphoreZirconHandleFUCHSIA;
#else
	PFN_vkVoidFunction padding_3df6f656[2];
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_GOOGLE_display_timing)
	PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE;
	PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE;
#else
	PFN_vkVoidFunction padding_2a6f50cd[2];
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_cluster_culling_shader)
	PFN_vkCmdDrawClusterHUAWEI vkCmdDrawClusterHUAWEI;
	PFN_vkCmdDrawClusterIndirectHUAWEI vkCmdDrawClusterIndirectHUAWEI;
#else
	PFN_vkVoidFunction padding_75b97be6[2];
#endif /* defined(VK_HUAWEI_cluster_culling_shader) */
#if defined(VK_HUAWEI_invocation_mask)
	PFN_vkCmdBindInvocationMaskHUAWEI vkCmdBindInvocationMaskHUAWEI;
#else
	PFN_vkVoidFunction padding_c3a4569f[1];
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading) && VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION >= 2
	PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
#else
	PFN_vkVoidFunction padding_2e923f32[1];
#endif /* defined(VK_HUAWEI_subpass_shading) && VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION >= 2 */
#if defined(VK_HUAWEI_subpass_shading)
	PFN_vkCmdSubpassShadingHUAWEI vkCmdSubpassShadingHUAWEI;
#else
	PFN_vkVoidFunction padding_f766fdf5[1];
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
	PFN_vkAcquirePerformanceConfigurationINTEL vkAcquirePerformanceConfigurationINTEL;
	PFN_vkCmdSetPerformanceMarkerINTEL vkCmdSetPerformanceMarkerINTEL;
	PFN_vkCmdSetPerformanceOverrideINTEL vkCmdSetPerformanceOverrideINTEL;
	PFN_vkCmdSetPerformanceStreamMarkerINTEL vkCmdSetPerformanceStreamMarkerINTEL;
	PFN_vkGetPerformanceParameterINTEL vkGetPerformanceParameterINTEL;
	PFN_vkInitializePerformanceApiINTEL vkInitializePerformanceApiINTEL;
	PFN_vkQueueSetPerformanceConfigurationINTEL vkQueueSetPerformanceConfigurationINTEL;
	PFN_vkReleasePerformanceConfigurationINTEL vkReleasePerformanceConfigurationINTEL;
	PFN_vkUninitializePerformanceApiINTEL vkUninitializePerformanceApiINTEL;
#else
	PFN_vkVoidFunction padding_495a0a0b[9];
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
	PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR;
	PFN_vkCmdBuildAccelerationStructuresIndirectKHR vkCmdBuildAccelerationStructuresIndirectKHR;
	PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
	PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR;
	PFN_vkCmdCopyAccelerationStructureToMemoryKHR vkCmdCopyAccelerationStructureToMemoryKHR;
	PFN_vkCmdCopyMemoryToAccelerationStructureKHR vkCmdCopyMemoryToAccelerationStructureKHR;
	PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR;
	PFN_vkCopyAccelerationStructureKHR vkCopyAccelerationStructureKHR;
	PFN_vkCopyAccelerationStructureToMemoryKHR vkCopyAccelerationStructureToMemoryKHR;
	PFN_vkCopyMemoryToAccelerationStructureKHR vkCopyMemoryToAccelerationStructureKHR;
	PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
	PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
	PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
	PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
	PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR;
	PFN_vkWriteAccelerationStructuresPropertiesKHR vkWriteAccelerationStructuresPropertiesKHR;
#else
	PFN_vkVoidFunction padding_5a999b78[16];
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_bind_memory2)
	PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR;
	PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;
#else
	PFN_vkVoidFunction padding_ed8481f5[2];
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
	PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR;
	PFN_vkGetBufferOpaqueCaptureAddressKHR vkGetBufferOpaqueCaptureAddressKHR;
	PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR vkGetDeviceMemoryOpaqueCaptureAddressKHR;
#else
	PFN_vkVoidFunction padding_178fdf81[3];
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_calibrated_timestamps)
	PFN_vkGetCalibratedTimestampsKHR vkGetCalibratedTimestampsKHR;
#else
	PFN_vkVoidFunction padding_8fd6f40d[1];
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_copy_commands2)
	PFN_vkCmdBlitImage2KHR vkCmdBlitImage2KHR;
	PFN_vkCmdCopyBuffer2KHR vkCmdCopyBuffer2KHR;
	PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR;
	PFN_vkCmdCopyImage2KHR vkCmdCopyImage2KHR;
	PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR;
	PFN_vkCmdResolveImage2KHR vkCmdResolveImage2KHR;
#else
	PFN_vkVoidFunction padding_4c841ff2[6];
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_copy_memory_indirect)
	PFN_vkCmdCopyMemoryIndirectKHR vkCmdCopyMemoryIndirectKHR;
	PFN_vkCmdCopyMemoryToImageIndirectKHR vkCmdCopyMemoryToImageIndirectKHR;
#else
	PFN_vkVoidFunction padding_95995957[2];
#endif /* defined(VK_KHR_copy_memory_indirect) */
#if defined(VK_KHR_create_renderpass2)
	PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR;
	PFN_vkCmdEndRenderPass2KHR vkCmdEndRenderPass2KHR;
	PFN_vkCmdNextSubpass2KHR vkCmdNextSubpass2KHR;
	PFN_vkCreateRenderPass2KHR vkCreateRenderPass2KHR;
#else
	PFN_vkVoidFunction padding_2a0a8727[4];
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
	PFN_vkCreateDeferredOperationKHR vkCreateDeferredOperationKHR;
	PFN_vkDeferredOperationJoinKHR vkDeferredOperationJoinKHR;
	PFN_vkDestroyDeferredOperationKHR vkDestroyDeferredOperationKHR;
	PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR;
	PFN_vkGetDeferredOperationResultKHR vkGetDeferredOperationResultKHR;
#else
	PFN_vkVoidFunction padding_346287bb[5];
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
	PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR;
	PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR;
	PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR;
#else
	PFN_vkVoidFunction padding_3d63aec0[3];
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
	PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR;
	PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR;
	PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR;
#else
	PFN_vkVoidFunction padding_5ebe16bd[3];
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_display_swapchain)
	PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
#else
	PFN_vkVoidFunction padding_12099367[1];
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
	PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR;
	PFN_vkCmdDrawIndirectCountKHR vkCmdDrawIndirectCountKHR;
#else
	PFN_vkVoidFunction padding_7b5bc4c1[2];
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
	PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR;
	PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR;
#else
	PFN_vkVoidFunction padding_b80f75a5[2];
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_dynamic_rendering_local_read)
	PFN_vkCmdSetRenderingAttachmentLocationsKHR vkCmdSetRenderingAttachmentLocationsKHR;
	PFN_vkCmdSetRenderingInputAttachmentIndicesKHR vkCmdSetRenderingInputAttachmentIndicesKHR;
#else
	PFN_vkVoidFunction padding_b1510532[2];
#endif /* defined(VK_KHR_dynamic_rendering_local_read) */
#if defined(VK_KHR_external_fence_fd)
	PFN_vkGetFenceFdKHR vkGetFenceFdKHR;
	PFN_vkImportFenceFdKHR vkImportFenceFdKHR;
#else
	PFN_vkVoidFunction padding_a2c787d5[2];
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
	PFN_vkGetFenceWin32HandleKHR vkGetFenceWin32HandleKHR;
	PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR;
#else
	PFN_vkVoidFunction padding_55d8e6a9[2];
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_fd)
	PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR;
	PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR;
#else
	PFN_vkVoidFunction padding_982d9e19[2];
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
	PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR;
	PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR;
#else
	PFN_vkVoidFunction padding_4af9e25a[2];
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_fd)
	PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR;
	PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR;
#else
	PFN_vkVoidFunction padding_2237b7cf[2];
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
	PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR;
	PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR;
#else
	PFN_vkVoidFunction padding_c18dea52[2];
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
	PFN_vkCmdSetFragmentShadingRateKHR vkCmdSetFragmentShadingRateKHR;
#else
	PFN_vkVoidFunction padding_f91b0a90[1];
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_memory_requirements2)
	PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR;
	PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;
	PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR;
#else
	PFN_vkVoidFunction padding_79d9c5c4[3];
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_line_rasterization)
	PFN_vkCmdSetLineStippleKHR vkCmdSetLineStippleKHR;
#else
	PFN_vkVoidFunction padding_83c2939[1];
#endif /* defined(VK_KHR_line_rasterization) */
#if defined(VK_KHR_maintenance1)
	PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR;
#else
	PFN_vkVoidFunction padding_4b372c56[1];
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance10)
	PFN_vkCmdEndRendering2KHR vkCmdEndRendering2KHR;
#else
	PFN_vkVoidFunction padding_c866e6ce[1];
#endif /* defined(VK_KHR_maintenance10) */
#if defined(VK_KHR_maintenance3)
	PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR;
#else
	PFN_vkVoidFunction padding_5ea7858d[1];
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
	PFN_vkGetDeviceBufferMemoryRequirementsKHR vkGetDeviceBufferMemoryRequirementsKHR;
	PFN_vkGetDeviceImageMemoryRequirementsKHR vkGetDeviceImageMemoryRequirementsKHR;
	PFN_vkGetDeviceImageSparseMemoryRequirementsKHR vkGetDeviceImageSparseMemoryRequirementsKHR;
#else
	PFN_vkVoidFunction padding_8e2d4198[3];
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_maintenance5)
	PFN_vkCmdBindIndexBuffer2KHR vkCmdBindIndexBuffer2KHR;
	PFN_vkGetDeviceImageSubresourceLayoutKHR vkGetDeviceImageSubresourceLayoutKHR;
	PFN_vkGetImageSubresourceLayout2KHR vkGetImageSubresourceLayout2KHR;
	PFN_vkGetRenderingAreaGranularityKHR vkGetRenderingAreaGranularityKHR;
#else
	PFN_vkVoidFunction padding_37040339[4];
#endif /* defined(VK_KHR_maintenance5) */
#if defined(VK_KHR_maintenance6)
	PFN_vkCmdBindDescriptorSets2KHR vkCmdBindDescriptorSets2KHR;
	PFN_vkCmdPushConstants2KHR vkCmdPushConstants2KHR;
#else
	PFN_vkVoidFunction padding_442955d8[2];
#endif /* defined(VK_KHR_maintenance6) */
#if defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor)
	PFN_vkCmdPushDescriptorSet2KHR vkCmdPushDescriptorSet2KHR;
	PFN_vkCmdPushDescriptorSetWithTemplate2KHR vkCmdPushDescriptorSetWithTemplate2KHR;
#else
	PFN_vkVoidFunction padding_80e8513f[2];
#endif /* defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer)
	PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT vkCmdBindDescriptorBufferEmbeddedSamplers2EXT;
	PFN_vkCmdSetDescriptorBufferOffsets2EXT vkCmdSetDescriptorBufferOffsets2EXT;
#else
	PFN_vkVoidFunction padding_2816b9cd[2];
#endif /* defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer) */
#if defined(VK_KHR_map_memory2)
	PFN_vkMapMemory2KHR vkMapMemory2KHR;
	PFN_vkUnmapMemory2KHR vkUnmapMemory2KHR;
#else
	PFN_vkVoidFunction padding_5a6d8986[2];
#endif /* defined(VK_KHR_map_memory2) */
#if defined(VK_KHR_performance_query)
	PFN_vkAcquireProfilingLockKHR vkAcquireProfilingLockKHR;
	PFN_vkReleaseProfilingLockKHR vkReleaseProfilingLockKHR;
#else
	PFN_vkVoidFunction padding_76f2673b[2];
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_binary)
	PFN_vkCreatePipelineBinariesKHR vkCreatePipelineBinariesKHR;
	PFN_vkDestroyPipelineBinaryKHR vkDestroyPipelineBinaryKHR;
	PFN_vkGetPipelineBinaryDataKHR vkGetPipelineBinaryDataKHR;
	PFN_vkGetPipelineKeyKHR vkGetPipelineKeyKHR;
	PFN_vkReleaseCapturedPipelineDataKHR vkReleaseCapturedPipelineDataKHR;
#else
	PFN_vkVoidFunction padding_65232810[5];
#endif /* defined(VK_KHR_pipeline_binary) */
#if defined(VK_KHR_pipeline_executable_properties)
	PFN_vkGetPipelineExecutableInternalRepresentationsKHR vkGetPipelineExecutableInternalRepresentationsKHR;
	PFN_vkGetPipelineExecutablePropertiesKHR vkGetPipelineExecutablePropertiesKHR;
	PFN_vkGetPipelineExecutableStatisticsKHR vkGetPipelineExecutableStatisticsKHR;
#else
	PFN_vkVoidFunction padding_f7629b1e[3];
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
	PFN_vkWaitForPresentKHR vkWaitForPresentKHR;
#else
	PFN_vkVoidFunction padding_b16cbe03[1];
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_present_wait2)
	PFN_vkWaitForPresent2KHR vkWaitForPresent2KHR;
#else
	PFN_vkVoidFunction padding_7401483a[1];
#endif /* defined(VK_KHR_present_wait2) */
#if defined(VK_KHR_push_descriptor)
	PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
#else
	PFN_vkVoidFunction padding_8f7712ad[1];
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
	PFN_vkCmdTraceRaysIndirect2KHR vkCmdTraceRaysIndirect2KHR;
#else
	PFN_vkVoidFunction padding_dd5f9b4a[1];
#endif /* defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_ray_tracing_pipeline)
	PFN_vkCmdSetRayTracingPipelineStackSizeKHR vkCmdSetRayTracingPipelineStackSizeKHR;
	PFN_vkCmdTraceRaysIndirectKHR vkCmdTraceRaysIndirectKHR;
	PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
	PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;
	PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR;
	PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
	PFN_vkGetRayTracingShaderGroupStackSizeKHR vkGetRayTracingShaderGroupStackSizeKHR;
#else
	PFN_vkVoidFunction padding_af99aedc[7];
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
	PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR;
	PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR;
#else
	PFN_vkVoidFunction padding_88e61b30[2];
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
	PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR;
#else
	PFN_vkVoidFunction padding_1ff3379[1];
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_swapchain)
	PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
	PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
	PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
	PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
	PFN_vkQueuePresentKHR vkQueuePresentKHR;
#else
	PFN_vkVoidFunction padding_a1de893b[5];
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_swapchain_maintenance1)
	PFN_vkReleaseSwapchainImagesKHR vkReleaseSwapchainImagesKHR;
#else
	PFN_vkVoidFunction padding_e032d5c4[1];
#endif /* defined(VK_KHR_swapchain_maintenance1) */
#if defined(VK_KHR_synchronization2)
	PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR;
	PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR;
	PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR;
	PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR;
	PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR;
	PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR;
#else
	PFN_vkVoidFunction padding_e85bf128[6];
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_timeline_semaphore)
	PFN_vkGetSemaphoreCounterValueKHR vkGetSemaphoreCounterValueKHR;
	PFN_vkSignalSemaphoreKHR vkSignalSemaphoreKHR;
	PFN_vkWaitSemaphoresKHR vkWaitSemaphoresKHR;
#else
	PFN_vkVoidFunction padding_c799d931[3];
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
	PFN_vkCmdDecodeVideoKHR vkCmdDecodeVideoKHR;
#else
	PFN_vkVoidFunction padding_7a7cc7ad[1];
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
	PFN_vkCmdEncodeVideoKHR vkCmdEncodeVideoKHR;
	PFN_vkGetEncodedVideoSessionParametersKHR vkGetEncodedVideoSessionParametersKHR;
#else
	PFN_vkVoidFunction padding_f2997fb4[2];
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
	PFN_vkBindVideoSessionMemoryKHR vkBindVideoSessionMemoryKHR;
	PFN_vkCmdBeginVideoCodingKHR vkCmdBeginVideoCodingKHR;
	PFN_vkCmdControlVideoCodingKHR vkCmdControlVideoCodingKHR;
	PFN_vkCmdEndVideoCodingKHR vkCmdEndVideoCodingKHR;
	PFN_vkCreateVideoSessionKHR vkCreateVideoSessionKHR;
	PFN_vkCreateVideoSessionParametersKHR vkCreateVideoSessionParametersKHR;
	PFN_vkDestroyVideoSessionKHR vkDestroyVideoSessionKHR;
	PFN_vkDestroyVideoSessionParametersKHR vkDestroyVideoSessionParametersKHR;
	PFN_vkGetVideoSessionMemoryRequirementsKHR vkGetVideoSessionMemoryRequirementsKHR;
	PFN_vkUpdateVideoSessionParametersKHR vkUpdateVideoSessionParametersKHR;
#else
	PFN_vkVoidFunction padding_98fb7016[10];
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_NVX_binary_import)
	PFN_vkCmdCuLaunchKernelNVX vkCmdCuLaunchKernelNVX;
	PFN_vkCreateCuFunctionNVX vkCreateCuFunctionNVX;
	PFN_vkCreateCuModuleNVX vkCreateCuModuleNVX;
	PFN_vkDestroyCuFunctionNVX vkDestroyCuFunctionNVX;
	PFN_vkDestroyCuModuleNVX vkDestroyCuModuleNVX;
#else
	PFN_vkVoidFunction padding_eb54309b[5];
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
	PFN_vkGetImageViewHandleNVX vkGetImageViewHandleNVX;
#else
	PFN_vkVoidFunction padding_887f6736[1];
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 3
	PFN_vkGetImageViewHandle64NVX vkGetImageViewHandle64NVX;
#else
	PFN_vkVoidFunction padding_64ad40e2[1];
#endif /* defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 3 */
#if defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 2
	PFN_vkGetImageViewAddressNVX vkGetImageViewAddressNVX;
#else
	PFN_vkVoidFunction padding_d290479a[1];
#endif /* defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 2 */
#if defined(VK_NV_clip_space_w_scaling)
	PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV;
#else
	PFN_vkVoidFunction padding_88d7eb2e[1];
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_cluster_acceleration_structure)
	PFN_vkCmdBuildClusterAccelerationStructureIndirectNV vkCmdBuildClusterAccelerationStructureIndirectNV;
	PFN_vkGetClusterAccelerationStructureBuildSizesNV vkGetClusterAccelerationStructureBuildSizesNV;
#else
	PFN_vkVoidFunction padding_60e35395[2];
#endif /* defined(VK_NV_cluster_acceleration_structure) */
#if defined(VK_NV_cooperative_vector)
	PFN_vkCmdConvertCooperativeVectorMatrixNV vkCmdConvertCooperativeVectorMatrixNV;
	PFN_vkConvertCooperativeVectorMatrixNV vkConvertCooperativeVectorMatrixNV;
#else
	PFN_vkVoidFunction padding_f4a887d0[2];
#endif /* defined(VK_NV_cooperative_vector) */
#if defined(VK_NV_copy_memory_indirect)
	PFN_vkCmdCopyMemoryIndirectNV vkCmdCopyMemoryIndirectNV;
	PFN_vkCmdCopyMemoryToImageIndirectNV vkCmdCopyMemoryToImageIndirectNV;
#else
	PFN_vkVoidFunction padding_9536230e[2];
#endif /* defined(VK_NV_copy_memory_indirect) */
#if defined(VK_NV_cuda_kernel_launch)
	PFN_vkCmdCudaLaunchKernelNV vkCmdCudaLaunchKernelNV;
	PFN_vkCreateCudaFunctionNV vkCreateCudaFunctionNV;
	PFN_vkCreateCudaModuleNV vkCreateCudaModuleNV;
	PFN_vkDestroyCudaFunctionNV vkDestroyCudaFunctionNV;
	PFN_vkDestroyCudaModuleNV vkDestroyCudaModuleNV;
	PFN_vkGetCudaModuleCacheNV vkGetCudaModuleCacheNV;
#else
	PFN_vkVoidFunction padding_2eabdf3b[6];
#endif /* defined(VK_NV_cuda_kernel_launch) */
#if defined(VK_NV_device_diagnostic_checkpoints)
	PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV;
	PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV;
#else
	PFN_vkVoidFunction padding_adaa5a21[2];
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2))
	PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV;
#else
	PFN_vkVoidFunction padding_c776633d[1];
#endif /* defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) */
#if defined(VK_NV_device_generated_commands)
	PFN_vkCmdBindPipelineShaderGroupNV vkCmdBindPipelineShaderGroupNV;
	PFN_vkCmdExecuteGeneratedCommandsNV vkCmdExecuteGeneratedCommandsNV;
	PFN_vkCmdPreprocessGeneratedCommandsNV vkCmdPreprocessGeneratedCommandsNV;
	PFN_vkCreateIndirectCommandsLayoutNV vkCreateIndirectCommandsLayoutNV;
	PFN_vkDestroyIndirectCommandsLayoutNV vkDestroyIndirectCommandsLayoutNV;
	PFN_vkGetGeneratedCommandsMemoryRequirementsNV vkGetGeneratedCommandsMemoryRequirementsNV;
#else
	PFN_vkVoidFunction padding_4c7e4395[6];
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_device_generated_commands_compute)
	PFN_vkCmdUpdatePipelineIndirectBufferNV vkCmdUpdatePipelineIndirectBufferNV;
	PFN_vkGetPipelineIndirectDeviceAddressNV vkGetPipelineIndirectDeviceAddressNV;
	PFN_vkGetPipelineIndirectMemoryRequirementsNV vkGetPipelineIndirectMemoryRequirementsNV;
#else
	PFN_vkVoidFunction padding_5195094c[3];
#endif /* defined(VK_NV_device_generated_commands_compute) */
#if defined(VK_NV_external_compute_queue)
	PFN_vkCreateExternalComputeQueueNV vkCreateExternalComputeQueueNV;
	PFN_vkDestroyExternalComputeQueueNV vkDestroyExternalComputeQueueNV;
	PFN_vkGetExternalComputeQueueDataNV vkGetExternalComputeQueueDataNV;
#else
	PFN_vkVoidFunction padding_4f947e0b[3];
#endif /* defined(VK_NV_external_compute_queue) */
#if defined(VK_NV_external_memory_rdma)
	PFN_vkGetMemoryRemoteAddressNV vkGetMemoryRemoteAddressNV;
#else
	PFN_vkVoidFunction padding_920e405[1];
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
	PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV;
#else
	PFN_vkVoidFunction padding_c13d6f3a[1];
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
	PFN_vkCmdSetFragmentShadingRateEnumNV vkCmdSetFragmentShadingRateEnumNV;
#else
	PFN_vkVoidFunction padding_4979ca14[1];
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_low_latency2)
	PFN_vkGetLatencyTimingsNV vkGetLatencyTimingsNV;
	PFN_vkLatencySleepNV vkLatencySleepNV;
	PFN_vkQueueNotifyOutOfBandNV vkQueueNotifyOutOfBandNV;
	PFN_vkSetLatencyMarkerNV vkSetLatencyMarkerNV;
	PFN_vkSetLatencySleepModeNV vkSetLatencySleepModeNV;
#else
	PFN_vkVoidFunction padding_fabf8b19[5];
#endif /* defined(VK_NV_low_latency2) */
#if defined(VK_NV_memory_decompression)
	PFN_vkCmdDecompressMemoryIndirectCountNV vkCmdDecompressMemoryIndirectCountNV;
	PFN_vkCmdDecompressMemoryNV vkCmdDecompressMemoryNV;
#else
	PFN_vkVoidFunction padding_706009[2];
#endif /* defined(VK_NV_memory_decompression) */
#if defined(VK_NV_mesh_shader)
	PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV;
	PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV;
#else
	PFN_vkVoidFunction padding_ac232758[2];
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count))
	PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV;
#else
	PFN_vkVoidFunction padding_53495be7[1];
#endif /* defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) */
#if defined(VK_NV_optical_flow)
	PFN_vkBindOpticalFlowSessionImageNV vkBindOpticalFlowSessionImageNV;
	PFN_vkCmdOpticalFlowExecuteNV vkCmdOpticalFlowExecuteNV;
	PFN_vkCreateOpticalFlowSessionNV vkCreateOpticalFlowSessionNV;
	PFN_vkDestroyOpticalFlowSessionNV vkDestroyOpticalFlowSessionNV;
#else
	PFN_vkVoidFunction padding_f67571eb[4];
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_NV_partitioned_acceleration_structure)
	PFN_vkCmdBuildPartitionedAccelerationStructuresNV vkCmdBuildPartitionedAccelerationStructuresNV;
	PFN_vkGetPartitionedAccelerationStructuresBuildSizesNV vkGetPartitionedAccelerationStructuresBuildSizesNV;
#else
	PFN_vkVoidFunction padding_d27c8c6d[2];
#endif /* defined(VK_NV_partitioned_acceleration_structure) */
#if defined(VK_NV_ray_tracing)
	PFN_vkBindAccelerationStructureMemoryNV vkBindAccelerationStructureMemoryNV;
	PFN_vkCmdBuildAccelerationStructureNV vkCmdBuildAccelerationStructureNV;
	PFN_vkCmdCopyAccelerationStructureNV vkCmdCopyAccelerationStructureNV;
	PFN_vkCmdTraceRaysNV vkCmdTraceRaysNV;
	PFN_vkCmdWriteAccelerationStructuresPropertiesNV vkCmdWriteAccelerationStructuresPropertiesNV;
	PFN_vkCompileDeferredNV vkCompileDeferredNV;
	PFN_vkCreateAccelerationStructureNV vkCreateAccelerationStructureNV;
	PFN_vkCreateRayTracingPipelinesNV vkCreateRayTracingPipelinesNV;
	PFN_vkDestroyAccelerationStructureNV vkDestroyAccelerationStructureNV;
	PFN_vkGetAccelerationStructureHandleNV vkGetAccelerationStructureHandleNV;
	PFN_vkGetAccelerationStructureMemoryRequirementsNV vkGetAccelerationStructureMemoryRequirementsNV;
	PFN_vkGetRayTracingShaderGroupHandlesNV vkGetRayTracingShaderGroupHandlesNV;
#else
	PFN_vkVoidFunction padding_feefbeac[12];
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2
	PFN_vkCmdSetExclusiveScissorEnableNV vkCmdSetExclusiveScissorEnableNV;
#else
	PFN_vkVoidFunction padding_e3c24f80[1];
#endif /* defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 */
#if defined(VK_NV_scissor_exclusive)
	PFN_vkCmdSetExclusiveScissorNV vkCmdSetExclusiveScissorNV;
#else
	PFN_vkVoidFunction padding_8e88d86c[1];
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
	PFN_vkCmdBindShadingRateImageNV vkCmdBindShadingRateImageNV;
	PFN_vkCmdSetCoarseSampleOrderNV vkCmdSetCoarseSampleOrderNV;
	PFN_vkCmdSetViewportShadingRatePaletteNV vkCmdSetViewportShadingRatePaletteNV;
#else
	PFN_vkVoidFunction padding_92a0767f[3];
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_OHOS_external_memory)
	PFN_vkGetMemoryNativeBufferOHOS vkGetMemoryNativeBufferOHOS;
	PFN_vkGetNativeBufferPropertiesOHOS vkGetNativeBufferPropertiesOHOS;
#else
	PFN_vkVoidFunction padding_9c703846[2];
#endif /* defined(VK_OHOS_external_memory) */
#if defined(VK_OHOS_native_buffer)
	PFN_vkAcquireImageOHOS vkAcquireImageOHOS;
	PFN_vkGetSwapchainGrallocUsageOHOS vkGetSwapchainGrallocUsageOHOS;
	PFN_vkQueueSignalReleaseImageOHOS vkQueueSignalReleaseImageOHOS;
#else
	PFN_vkVoidFunction padding_a26c9a3d[3];
#endif /* defined(VK_OHOS_native_buffer) */
#if defined(VK_QCOM_tile_memory_heap)
	PFN_vkCmdBindTileMemoryQCOM vkCmdBindTileMemoryQCOM;
#else
	PFN_vkVoidFunction padding_e2d55d04[1];
#endif /* defined(VK_QCOM_tile_memory_heap) */
#if defined(VK_QCOM_tile_properties)
	PFN_vkGetDynamicRenderingTilePropertiesQCOM vkGetDynamicRenderingTilePropertiesQCOM;
	PFN_vkGetFramebufferTilePropertiesQCOM vkGetFramebufferTilePropertiesQCOM;
#else
	PFN_vkVoidFunction padding_be12e32[2];
#endif /* defined(VK_QCOM_tile_properties) */
#if defined(VK_QCOM_tile_shading)
	PFN_vkCmdBeginPerTileExecutionQCOM vkCmdBeginPerTileExecutionQCOM;
	PFN_vkCmdDispatchTileQCOM vkCmdDispatchTileQCOM;
	PFN_vkCmdEndPerTileExecutionQCOM vkCmdEndPerTileExecutionQCOM;
#else
	PFN_vkVoidFunction padding_fcd9e1df[3];
#endif /* defined(VK_QCOM_tile_shading) */
#if defined(VK_QNX_external_memory_screen_buffer)
	PFN_vkGetScreenBufferPropertiesQNX vkGetScreenBufferPropertiesQNX;
#else
	PFN_vkVoidFunction padding_1c27735d[1];
#endif /* defined(VK_QNX_external_memory_screen_buffer) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
	PFN_vkGetDescriptorSetHostMappingVALVE vkGetDescriptorSetHostMappingVALVE;
	PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE vkGetDescriptorSetLayoutHostMappingInfoVALVE;
#else
	PFN_vkVoidFunction padding_fd71e4c6[2];
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control))
	PFN_vkCmdSetDepthClampRangeEXT vkCmdSetDepthClampRangeEXT;
#else
	PFN_vkVoidFunction padding_faa18a61[1];
#endif /* (defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control)) */
#if (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object))
	PFN_vkCmdBindVertexBuffers2EXT vkCmdBindVertexBuffers2EXT;
	PFN_vkCmdSetCullModeEXT vkCmdSetCullModeEXT;
	PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT;
	PFN_vkCmdSetDepthCompareOpEXT vkCmdSetDepthCompareOpEXT;
	PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnableEXT;
	PFN_vkCmdSetDepthWriteEnableEXT vkCmdSetDepthWriteEnableEXT;
	PFN_vkCmdSetFrontFaceEXT vkCmdSetFrontFaceEXT;
	PFN_vkCmdSetPrimitiveTopologyEXT vkCmdSetPrimitiveTopologyEXT;
	PFN_vkCmdSetScissorWithCountEXT vkCmdSetScissorWithCountEXT;
	PFN_vkCmdSetStencilOpEXT vkCmdSetStencilOpEXT;
	PFN_vkCmdSetStencilTestEnableEXT vkCmdSetStencilTestEnableEXT;
	PFN_vkCmdSetViewportWithCountEXT vkCmdSetViewportWithCountEXT;
#else
	PFN_vkVoidFunction padding_3e8c720f[12];
#endif /* (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object))
	PFN_vkCmdSetDepthBiasEnableEXT vkCmdSetDepthBiasEnableEXT;
	PFN_vkCmdSetLogicOpEXT vkCmdSetLogicOpEXT;
	PFN_vkCmdSetPatchControlPointsEXT vkCmdSetPatchControlPointsEXT;
	PFN_vkCmdSetPrimitiveRestartEnableEXT vkCmdSetPrimitiveRestartEnableEXT;
	PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT;
#else
	PFN_vkVoidFunction padding_b93e02a6[5];
#endif /* (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object))
	PFN_vkCmdSetAlphaToCoverageEnableEXT vkCmdSetAlphaToCoverageEnableEXT;
	PFN_vkCmdSetAlphaToOneEnableEXT vkCmdSetAlphaToOneEnableEXT;
	PFN_vkCmdSetColorBlendEnableEXT vkCmdSetColorBlendEnableEXT;
	PFN_vkCmdSetColorBlendEquationEXT vkCmdSetColorBlendEquationEXT;
	PFN_vkCmdSetColorWriteMaskEXT vkCmdSetColorWriteMaskEXT;
	PFN_vkCmdSetDepthClampEnableEXT vkCmdSetDepthClampEnableEXT;
	PFN_vkCmdSetLogicOpEnableEXT vkCmdSetLogicOpEnableEXT;
	PFN_vkCmdSetPolygonModeEXT vkCmdSetPolygonModeEXT;
	PFN_vkCmdSetRasterizationSamplesEXT vkCmdSetRasterizationSamplesEXT;
	PFN_vkCmdSetSampleMaskEXT vkCmdSetSampleMaskEXT;
#else
	PFN_vkVoidFunction padding_ab566e7e[10];
#endif /* (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object))
	PFN_vkCmdSetTessellationDomainOriginEXT vkCmdSetTessellationDomainOriginEXT;
#else
	PFN_vkVoidFunction padding_6730ed0c[1];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback))
	PFN_vkCmdSetRasterizationStreamEXT vkCmdSetRasterizationStreamEXT;
#else
	PFN_vkVoidFunction padding_d3ebb335[1];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization))
	PFN_vkCmdSetConservativeRasterizationModeEXT vkCmdSetConservativeRasterizationModeEXT;
	PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT vkCmdSetExtraPrimitiveOverestimationSizeEXT;
#else
	PFN_vkVoidFunction padding_a21758f4[2];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable))
	PFN_vkCmdSetDepthClipEnableEXT vkCmdSetDepthClipEnableEXT;
#else
	PFN_vkVoidFunction padding_a498a838[1];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations))
	PFN_vkCmdSetSampleLocationsEnableEXT vkCmdSetSampleLocationsEnableEXT;
#else
	PFN_vkVoidFunction padding_67db38de[1];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced))
	PFN_vkCmdSetColorBlendAdvancedEXT vkCmdSetColorBlendAdvancedEXT;
#else
	PFN_vkVoidFunction padding_fbea7481[1];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex))
	PFN_vkCmdSetProvokingVertexModeEXT vkCmdSetProvokingVertexModeEXT;
#else
	PFN_vkVoidFunction padding_3a8ec90e[1];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization))
	PFN_vkCmdSetLineRasterizationModeEXT vkCmdSetLineRasterizationModeEXT;
	PFN_vkCmdSetLineStippleEnableEXT vkCmdSetLineStippleEnableEXT;
#else
	PFN_vkVoidFunction padding_29cdb756[2];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control))
	PFN_vkCmdSetDepthClipNegativeOneToOneEXT vkCmdSetDepthClipNegativeOneToOneEXT;
#else
	PFN_vkVoidFunction padding_815a7240[1];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling))
	PFN_vkCmdSetViewportWScalingEnableNV vkCmdSetViewportWScalingEnableNV;
#else
	PFN_vkVoidFunction padding_d1f00511[1];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle))
	PFN_vkCmdSetViewportSwizzleNV vkCmdSetViewportSwizzleNV;
#else
	PFN_vkVoidFunction padding_7a73d553[1];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color))
	PFN_vkCmdSetCoverageToColorEnableNV vkCmdSetCoverageToColorEnableNV;
	PFN_vkCmdSetCoverageToColorLocationNV vkCmdSetCoverageToColorLocationNV;
#else
	PFN_vkVoidFunction padding_6045fb8c[2];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples))
	PFN_vkCmdSetCoverageModulationModeNV vkCmdSetCoverageModulationModeNV;
	PFN_vkCmdSetCoverageModulationTableEnableNV vkCmdSetCoverageModulationTableEnableNV;
	PFN_vkCmdSetCoverageModulationTableNV vkCmdSetCoverageModulationTableNV;
#else
	PFN_vkVoidFunction padding_bdc35c80[3];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image))
	PFN_vkCmdSetShadingRateImageEnableNV vkCmdSetShadingRateImageEnableNV;
#else
	PFN_vkVoidFunction padding_9a5cd6e8[1];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test))
	PFN_vkCmdSetRepresentativeFragmentTestEnableNV vkCmdSetRepresentativeFragmentTestEnableNV;
#else
	PFN_vkVoidFunction padding_3ee17e96[1];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode))
	PFN_vkCmdSetCoverageReductionModeNV vkCmdSetCoverageReductionModeNV;
#else
	PFN_vkVoidFunction padding_263d525a[1];
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode)) */
#if (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control))
	PFN_vkGetImageSubresourceLayout2EXT vkGetImageSubresourceLayout2EXT;
#else
	PFN_vkVoidFunction padding_ecddace1[1];
#endif /* (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control)) */
#if (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state))
	PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT;
#else
	PFN_vkVoidFunction padding_d83e1de1[1];
#endif /* (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && (defined(VK_VERSION_1_1) || defined(VK_KHR_descriptor_update_template)))
	PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR;
#else
	PFN_vkVoidFunction padding_60f8358a[1];
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && (defined(VK_VERSION_1_1) || defined(VK_KHR_descriptor_update_template))) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR;
	PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR;
#else
	PFN_vkVoidFunction padding_460290c6[2];
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
	PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR;
#else
	PFN_vkVoidFunction padding_cffc198[1];
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
	/* VOLK_GENERATE_DEVICE_TABLE */
};

/* VOLK_GENERATE_PROTOTYPES_H */
#if defined(VK_VERSION_1_0)
VKAPI PFN_vkCreateDevice vkCreateDevice;
VKAPI PFN_vkCreateInstance vkCreateInstance;
VKAPI PFN_vkDestroyInstance vkDestroyInstance;
VKAPI PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
VKAPI PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
VKAPI PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
VKAPI PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
VKAPI PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
VKAPI PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
VKAPI PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
VKAPI PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
VKAPI PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
VKAPI PFN_vkGetPhysicalDeviceImageFormatProperties vkGetPhysicalDeviceImageFormatProperties;
VKAPI PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
VKAPI PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
VKAPI PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
VKAPI PFN_vkGetPhysicalDeviceSparseImageFormatProperties vkGetPhysicalDeviceSparseImageFormatProperties;
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
VKAPI PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
VKAPI PFN_vkEnumeratePhysicalDeviceGroups vkEnumeratePhysicalDeviceGroups;
VKAPI PFN_vkGetPhysicalDeviceExternalBufferProperties vkGetPhysicalDeviceExternalBufferProperties;
VKAPI PFN_vkGetPhysicalDeviceExternalFenceProperties vkGetPhysicalDeviceExternalFenceProperties;
VKAPI PFN_vkGetPhysicalDeviceExternalSemaphoreProperties vkGetPhysicalDeviceExternalSemaphoreProperties;
VKAPI PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2;
VKAPI PFN_vkGetPhysicalDeviceFormatProperties2 vkGetPhysicalDeviceFormatProperties2;
VKAPI PFN_vkGetPhysicalDeviceImageFormatProperties2 vkGetPhysicalDeviceImageFormatProperties2;
VKAPI PFN_vkGetPhysicalDeviceMemoryProperties2 vkGetPhysicalDeviceMemoryProperties2;
VKAPI PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
VKAPI PFN_vkGetPhysicalDeviceQueueFamilyProperties2 vkGetPhysicalDeviceQueueFamilyProperties2;
VKAPI PFN_vkGetPhysicalDeviceSparseImageFormatProperties2 vkGetPhysicalDeviceSparseImageFormatProperties2;
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_3)
VKAPI PFN_vkGetPhysicalDeviceToolProperties vkGetPhysicalDeviceToolProperties;
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_ARM_data_graph)
VKAPI PFN_vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM vkGetPhysicalDeviceQueueFamilyDataGraphProcessingEnginePropertiesARM;
VKAPI PFN_vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM vkGetPhysicalDeviceQueueFamilyDataGraphPropertiesARM;
#endif /* defined(VK_ARM_data_graph) */
#if defined(VK_ARM_performance_counters_by_region)
VKAPI PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM vkEnumeratePhysicalDeviceQueueFamilyPerformanceCountersByRegionARM;
#endif /* defined(VK_ARM_performance_counters_by_region) */
#if defined(VK_ARM_tensors)
VKAPI PFN_vkGetPhysicalDeviceExternalTensorPropertiesARM vkGetPhysicalDeviceExternalTensorPropertiesARM;
#endif /* defined(VK_ARM_tensors) */
#if defined(VK_EXT_acquire_drm_display)
VKAPI PFN_vkAcquireDrmDisplayEXT vkAcquireDrmDisplayEXT;
VKAPI PFN_vkGetDrmDisplayEXT vkGetDrmDisplayEXT;
#endif /* defined(VK_EXT_acquire_drm_display) */
#if defined(VK_EXT_acquire_xlib_display)
VKAPI PFN_vkAcquireXlibDisplayEXT vkAcquireXlibDisplayEXT;
VKAPI PFN_vkGetRandROutputDisplayEXT vkGetRandROutputDisplayEXT;
#endif /* defined(VK_EXT_acquire_xlib_display) */
#if defined(VK_EXT_calibrated_timestamps)
VKAPI PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsEXT vkGetPhysicalDeviceCalibrateableTimeDomainsEXT;
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_debug_report)
VKAPI PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT;
VKAPI PFN_vkDebugReportMessageEXT vkDebugReportMessageEXT;
VKAPI PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallbackEXT;
#endif /* defined(VK_EXT_debug_report) */
#if defined(VK_EXT_debug_utils)
VKAPI PFN_vkCmdBeginDebugUtilsLabelEXT vkCmdBeginDebugUtilsLabelEXT;
VKAPI PFN_vkCmdEndDebugUtilsLabelEXT vkCmdEndDebugUtilsLabelEXT;
VKAPI PFN_vkCmdInsertDebugUtilsLabelEXT vkCmdInsertDebugUtilsLabelEXT;
VKAPI PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;
VKAPI PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
VKAPI PFN_vkQueueBeginDebugUtilsLabelEXT vkQueueBeginDebugUtilsLabelEXT;
VKAPI PFN_vkQueueEndDebugUtilsLabelEXT vkQueueEndDebugUtilsLabelEXT;
VKAPI PFN_vkQueueInsertDebugUtilsLabelEXT vkQueueInsertDebugUtilsLabelEXT;
VKAPI PFN_vkSetDebugUtilsObjectNameEXT vkSetDebugUtilsObjectNameEXT;
VKAPI PFN_vkSetDebugUtilsObjectTagEXT vkSetDebugUtilsObjectTagEXT;
VKAPI PFN_vkSubmitDebugUtilsMessageEXT vkSubmitDebugUtilsMessageEXT;
#endif /* defined(VK_EXT_debug_utils) */
#if defined(VK_EXT_direct_mode_display)
VKAPI PFN_vkReleaseDisplayEXT vkReleaseDisplayEXT;
#endif /* defined(VK_EXT_direct_mode_display) */
#if defined(VK_EXT_directfb_surface)
VKAPI PFN_vkCreateDirectFBSurfaceEXT vkCreateDirectFBSurfaceEXT;
VKAPI PFN_vkGetPhysicalDeviceDirectFBPresentationSupportEXT vkGetPhysicalDeviceDirectFBPresentationSupportEXT;
#endif /* defined(VK_EXT_directfb_surface) */
#if defined(VK_EXT_display_surface_counter)
VKAPI PFN_vkGetPhysicalDeviceSurfaceCapabilities2EXT vkGetPhysicalDeviceSurfaceCapabilities2EXT;
#endif /* defined(VK_EXT_display_surface_counter) */
#if defined(VK_EXT_full_screen_exclusive)
VKAPI PFN_vkGetPhysicalDeviceSurfacePresentModes2EXT vkGetPhysicalDeviceSurfacePresentModes2EXT;
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_headless_surface)
VKAPI PFN_vkCreateHeadlessSurfaceEXT vkCreateHeadlessSurfaceEXT;
#endif /* defined(VK_EXT_headless_surface) */
#if defined(VK_EXT_metal_surface)
VKAPI PFN_vkCreateMetalSurfaceEXT vkCreateMetalSurfaceEXT;
#endif /* defined(VK_EXT_metal_surface) */
#if defined(VK_EXT_sample_locations)
VKAPI PFN_vkGetPhysicalDeviceMultisamplePropertiesEXT vkGetPhysicalDeviceMultisamplePropertiesEXT;
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_tooling_info)
VKAPI PFN_vkGetPhysicalDeviceToolPropertiesEXT vkGetPhysicalDeviceToolPropertiesEXT;
#endif /* defined(VK_EXT_tooling_info) */
#if defined(VK_FUCHSIA_imagepipe_surface)
VKAPI PFN_vkCreateImagePipeSurfaceFUCHSIA vkCreateImagePipeSurfaceFUCHSIA;
#endif /* defined(VK_FUCHSIA_imagepipe_surface) */
#if defined(VK_GGP_stream_descriptor_surface)
VKAPI PFN_vkCreateStreamDescriptorSurfaceGGP vkCreateStreamDescriptorSurfaceGGP;
#endif /* defined(VK_GGP_stream_descriptor_surface) */
#if defined(VK_KHR_android_surface)
VKAPI PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
#endif /* defined(VK_KHR_android_surface) */
#if defined(VK_KHR_calibrated_timestamps)
VKAPI PFN_vkGetPhysicalDeviceCalibrateableTimeDomainsKHR vkGetPhysicalDeviceCalibrateableTimeDomainsKHR;
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_cooperative_matrix)
VKAPI PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR vkGetPhysicalDeviceCooperativeMatrixPropertiesKHR;
#endif /* defined(VK_KHR_cooperative_matrix) */
#if defined(VK_KHR_device_group_creation)
VKAPI PFN_vkEnumeratePhysicalDeviceGroupsKHR vkEnumeratePhysicalDeviceGroupsKHR;
#endif /* defined(VK_KHR_device_group_creation) */
#if defined(VK_KHR_display)
VKAPI PFN_vkCreateDisplayModeKHR vkCreateDisplayModeKHR;
VKAPI PFN_vkCreateDisplayPlaneSurfaceKHR vkCreateDisplayPlaneSurfaceKHR;
VKAPI PFN_vkGetDisplayModePropertiesKHR vkGetDisplayModePropertiesKHR;
VKAPI PFN_vkGetDisplayPlaneCapabilitiesKHR vkGetDisplayPlaneCapabilitiesKHR;
VKAPI PFN_vkGetDisplayPlaneSupportedDisplaysKHR vkGetDisplayPlaneSupportedDisplaysKHR;
VKAPI PFN_vkGetPhysicalDeviceDisplayPlanePropertiesKHR vkGetPhysicalDeviceDisplayPlanePropertiesKHR;
VKAPI PFN_vkGetPhysicalDeviceDisplayPropertiesKHR vkGetPhysicalDeviceDisplayPropertiesKHR;
#endif /* defined(VK_KHR_display) */
#if defined(VK_KHR_external_fence_capabilities)
VKAPI PFN_vkGetPhysicalDeviceExternalFencePropertiesKHR vkGetPhysicalDeviceExternalFencePropertiesKHR;
#endif /* defined(VK_KHR_external_fence_capabilities) */
#if defined(VK_KHR_external_memory_capabilities)
VKAPI PFN_vkGetPhysicalDeviceExternalBufferPropertiesKHR vkGetPhysicalDeviceExternalBufferPropertiesKHR;
#endif /* defined(VK_KHR_external_memory_capabilities) */
#if defined(VK_KHR_external_semaphore_capabilities)
VKAPI PFN_vkGetPhysicalDeviceExternalSemaphorePropertiesKHR vkGetPhysicalDeviceExternalSemaphorePropertiesKHR;
#endif /* defined(VK_KHR_external_semaphore_capabilities) */
#if defined(VK_KHR_fragment_shading_rate)
VKAPI PFN_vkGetPhysicalDeviceFragmentShadingRatesKHR vkGetPhysicalDeviceFragmentShadingRatesKHR;
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_display_properties2)
VKAPI PFN_vkGetDisplayModeProperties2KHR vkGetDisplayModeProperties2KHR;
VKAPI PFN_vkGetDisplayPlaneCapabilities2KHR vkGetDisplayPlaneCapabilities2KHR;
VKAPI PFN_vkGetPhysicalDeviceDisplayPlaneProperties2KHR vkGetPhysicalDeviceDisplayPlaneProperties2KHR;
VKAPI PFN_vkGetPhysicalDeviceDisplayProperties2KHR vkGetPhysicalDeviceDisplayProperties2KHR;
#endif /* defined(VK_KHR_get_display_properties2) */
#if defined(VK_KHR_get_physical_device_properties2)
VKAPI PFN_vkGetPhysicalDeviceFeatures2KHR vkGetPhysicalDeviceFeatures2KHR;
VKAPI PFN_vkGetPhysicalDeviceFormatProperties2KHR vkGetPhysicalDeviceFormatProperties2KHR;
VKAPI PFN_vkGetPhysicalDeviceImageFormatProperties2KHR vkGetPhysicalDeviceImageFormatProperties2KHR;
VKAPI PFN_vkGetPhysicalDeviceMemoryProperties2KHR vkGetPhysicalDeviceMemoryProperties2KHR;
VKAPI PFN_vkGetPhysicalDeviceProperties2KHR vkGetPhysicalDeviceProperties2KHR;
VKAPI PFN_vkGetPhysicalDeviceQueueFamilyProperties2KHR vkGetPhysicalDeviceQueueFamilyProperties2KHR;
VKAPI PFN_vkGetPhysicalDeviceSparseImageFormatProperties2KHR vkGetPhysicalDeviceSparseImageFormatProperties2KHR;
#endif /* defined(VK_KHR_get_physical_device_properties2) */
#if defined(VK_KHR_get_surface_capabilities2)
VKAPI PFN_vkGetPhysicalDeviceSurfaceCapabilities2KHR vkGetPhysicalDeviceSurfaceCapabilities2KHR;
VKAPI PFN_vkGetPhysicalDeviceSurfaceFormats2KHR vkGetPhysicalDeviceSurfaceFormats2KHR;
#endif /* defined(VK_KHR_get_surface_capabilities2) */
#if defined(VK_KHR_performance_query)
VKAPI PFN_vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR vkEnumeratePhysicalDeviceQueueFamilyPerformanceQueryCountersKHR;
VKAPI PFN_vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR vkGetPhysicalDeviceQueueFamilyPerformanceQueryPassesKHR;
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_surface)
VKAPI PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
VKAPI PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
VKAPI PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
VKAPI PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
VKAPI PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
#endif /* defined(VK_KHR_surface) */
#if defined(VK_KHR_video_encode_queue)
VKAPI PFN_vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR vkGetPhysicalDeviceVideoEncodeQualityLevelPropertiesKHR;
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
VKAPI PFN_vkGetPhysicalDeviceVideoCapabilitiesKHR vkGetPhysicalDeviceVideoCapabilitiesKHR;
VKAPI PFN_vkGetPhysicalDeviceVideoFormatPropertiesKHR vkGetPhysicalDeviceVideoFormatPropertiesKHR;
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_KHR_wayland_surface)
VKAPI PFN_vkCreateWaylandSurfaceKHR vkCreateWaylandSurfaceKHR;
VKAPI PFN_vkGetPhysicalDeviceWaylandPresentationSupportKHR vkGetPhysicalDeviceWaylandPresentationSupportKHR;
#endif /* defined(VK_KHR_wayland_surface) */
#if defined(VK_KHR_win32_surface)
VKAPI PFN_vkCreateWin32SurfaceKHR vkCreateWin32SurfaceKHR;
VKAPI PFN_vkGetPhysicalDeviceWin32PresentationSupportKHR vkGetPhysicalDeviceWin32PresentationSupportKHR;
#endif /* defined(VK_KHR_win32_surface) */
#if defined(VK_KHR_xcb_surface)
VKAPI PFN_vkCreateXcbSurfaceKHR vkCreateXcbSurfaceKHR;
VKAPI PFN_vkGetPhysicalDeviceXcbPresentationSupportKHR vkGetPhysicalDeviceXcbPresentationSupportKHR;
#endif /* defined(VK_KHR_xcb_surface) */
#if defined(VK_KHR_xlib_surface)
VKAPI PFN_vkCreateXlibSurfaceKHR vkCreateXlibSurfaceKHR;
VKAPI PFN_vkGetPhysicalDeviceXlibPresentationSupportKHR vkGetPhysicalDeviceXlibPresentationSupportKHR;
#endif /* defined(VK_KHR_xlib_surface) */
#if defined(VK_MVK_ios_surface)
VKAPI PFN_vkCreateIOSSurfaceMVK vkCreateIOSSurfaceMVK;
#endif /* defined(VK_MVK_ios_surface) */
#if defined(VK_MVK_macos_surface)
VKAPI PFN_vkCreateMacOSSurfaceMVK vkCreateMacOSSurfaceMVK;
#endif /* defined(VK_MVK_macos_surface) */
#if defined(VK_NN_vi_surface)
VKAPI PFN_vkCreateViSurfaceNN vkCreateViSurfaceNN;
#endif /* defined(VK_NN_vi_surface) */
#if defined(VK_NV_acquire_winrt_display)
VKAPI PFN_vkAcquireWinrtDisplayNV vkAcquireWinrtDisplayNV;
VKAPI PFN_vkGetWinrtDisplayNV vkGetWinrtDisplayNV;
#endif /* defined(VK_NV_acquire_winrt_display) */
#if defined(VK_NV_cooperative_matrix)
VKAPI PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV vkGetPhysicalDeviceCooperativeMatrixPropertiesNV;
#endif /* defined(VK_NV_cooperative_matrix) */
#if defined(VK_NV_cooperative_matrix2)
VKAPI PFN_vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV vkGetPhysicalDeviceCooperativeMatrixFlexibleDimensionsPropertiesNV;
#endif /* defined(VK_NV_cooperative_matrix2) */
#if defined(VK_NV_cooperative_vector)
VKAPI PFN_vkGetPhysicalDeviceCooperativeVectorPropertiesNV vkGetPhysicalDeviceCooperativeVectorPropertiesNV;
#endif /* defined(VK_NV_cooperative_vector) */
#if defined(VK_NV_coverage_reduction_mode)
VKAPI PFN_vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV vkGetPhysicalDeviceSupportedFramebufferMixedSamplesCombinationsNV;
#endif /* defined(VK_NV_coverage_reduction_mode) */
#if defined(VK_NV_external_memory_capabilities)
VKAPI PFN_vkGetPhysicalDeviceExternalImageFormatPropertiesNV vkGetPhysicalDeviceExternalImageFormatPropertiesNV;
#endif /* defined(VK_NV_external_memory_capabilities) */
#if defined(VK_NV_optical_flow)
VKAPI PFN_vkGetPhysicalDeviceOpticalFlowImageFormatsNV vkGetPhysicalDeviceOpticalFlowImageFormatsNV;
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_OHOS_surface)
VKAPI PFN_vkCreateSurfaceOHOS vkCreateSurfaceOHOS;
#endif /* defined(VK_OHOS_surface) */
#if defined(VK_QNX_screen_surface)
VKAPI PFN_vkCreateScreenSurfaceQNX vkCreateScreenSurfaceQNX;
VKAPI PFN_vkGetPhysicalDeviceScreenPresentationSupportQNX vkGetPhysicalDeviceScreenPresentationSupportQNX;
#endif /* defined(VK_QNX_screen_surface) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
VKAPI PFN_vkGetPhysicalDevicePresentRectanglesKHR vkGetPhysicalDevicePresentRectanglesKHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
/* VOLK_GENERATE_PROTOTYPES_H */

#ifndef VOLK_NO_DEVICE_PROTOTYPES
/* VOLK_GENERATE_PROTOTYPES_H_DEVICE */
#if defined(VK_VERSION_1_0)
VKAPI PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
VKAPI PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
VKAPI PFN_vkAllocateMemory vkAllocateMemory;
VKAPI PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
VKAPI PFN_vkBindBufferMemory vkBindBufferMemory;
VKAPI PFN_vkBindImageMemory vkBindImageMemory;
VKAPI PFN_vkCmdBeginQuery vkCmdBeginQuery;
VKAPI PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
VKAPI PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
VKAPI PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
VKAPI PFN_vkCmdBindPipeline vkCmdBindPipeline;
VKAPI PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
VKAPI PFN_vkCmdBlitImage vkCmdBlitImage;
VKAPI PFN_vkCmdClearAttachments vkCmdClearAttachments;
VKAPI PFN_vkCmdClearColorImage vkCmdClearColorImage;
VKAPI PFN_vkCmdClearDepthStencilImage vkCmdClearDepthStencilImage;
VKAPI PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
VKAPI PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
VKAPI PFN_vkCmdCopyImage vkCmdCopyImage;
VKAPI PFN_vkCmdCopyImageToBuffer vkCmdCopyImageToBuffer;
VKAPI PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;
VKAPI PFN_vkCmdDispatch vkCmdDispatch;
VKAPI PFN_vkCmdDispatchIndirect vkCmdDispatchIndirect;
VKAPI PFN_vkCmdDraw vkCmdDraw;
VKAPI PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
VKAPI PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
VKAPI PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
VKAPI PFN_vkCmdEndQuery vkCmdEndQuery;
VKAPI PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
VKAPI PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
VKAPI PFN_vkCmdFillBuffer vkCmdFillBuffer;
VKAPI PFN_vkCmdNextSubpass vkCmdNextSubpass;
VKAPI PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
VKAPI PFN_vkCmdPushConstants vkCmdPushConstants;
VKAPI PFN_vkCmdResetEvent vkCmdResetEvent;
VKAPI PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
VKAPI PFN_vkCmdResolveImage vkCmdResolveImage;
VKAPI PFN_vkCmdSetBlendConstants vkCmdSetBlendConstants;
VKAPI PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
VKAPI PFN_vkCmdSetDepthBounds vkCmdSetDepthBounds;
VKAPI PFN_vkCmdSetEvent vkCmdSetEvent;
VKAPI PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
VKAPI PFN_vkCmdSetScissor vkCmdSetScissor;
VKAPI PFN_vkCmdSetStencilCompareMask vkCmdSetStencilCompareMask;
VKAPI PFN_vkCmdSetStencilReference vkCmdSetStencilReference;
VKAPI PFN_vkCmdSetStencilWriteMask vkCmdSetStencilWriteMask;
VKAPI PFN_vkCmdSetViewport vkCmdSetViewport;
VKAPI PFN_vkCmdUpdateBuffer vkCmdUpdateBuffer;
VKAPI PFN_vkCmdWaitEvents vkCmdWaitEvents;
VKAPI PFN_vkCmdWriteTimestamp vkCmdWriteTimestamp;
VKAPI PFN_vkCreateBuffer vkCreateBuffer;
VKAPI PFN_vkCreateBufferView vkCreateBufferView;
VKAPI PFN_vkCreateCommandPool vkCreateCommandPool;
VKAPI PFN_vkCreateComputePipelines vkCreateComputePipelines;
VKAPI PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
VKAPI PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
VKAPI PFN_vkCreateEvent vkCreateEvent;
VKAPI PFN_vkCreateFence vkCreateFence;
VKAPI PFN_vkCreateFramebuffer vkCreateFramebuffer;
VKAPI PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
VKAPI PFN_vkCreateImage vkCreateImage;
VKAPI PFN_vkCreateImageView vkCreateImageView;
VKAPI PFN_vkCreatePipelineCache vkCreatePipelineCache;
VKAPI PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
VKAPI PFN_vkCreateQueryPool vkCreateQueryPool;
VKAPI PFN_vkCreateRenderPass vkCreateRenderPass;
VKAPI PFN_vkCreateSampler vkCreateSampler;
VKAPI PFN_vkCreateSemaphore vkCreateSemaphore;
VKAPI PFN_vkCreateShaderModule vkCreateShaderModule;
VKAPI PFN_vkDestroyBuffer vkDestroyBuffer;
VKAPI PFN_vkDestroyBufferView vkDestroyBufferView;
VKAPI PFN_vkDestroyCommandPool vkDestroyCommandPool;
VKAPI PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
VKAPI PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
VKAPI PFN_vkDestroyDevice vkDestroyDevice;
VKAPI PFN_vkDestroyEvent vkDestroyEvent;
VKAPI PFN_vkDestroyFence vkDestroyFence;
VKAPI PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
VKAPI PFN_vkDestroyImage vkDestroyImage;
VKAPI PFN_vkDestroyImageView vkDestroyImageView;
VKAPI PFN_vkDestroyPipeline vkDestroyPipeline;
VKAPI PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
VKAPI PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
VKAPI PFN_vkDestroyQueryPool vkDestroyQueryPool;
VKAPI PFN_vkDestroyRenderPass vkDestroyRenderPass;
VKAPI PFN_vkDestroySampler vkDestroySampler;
VKAPI PFN_vkDestroySemaphore vkDestroySemaphore;
VKAPI PFN_vkDestroyShaderModule vkDestroyShaderModule;
VKAPI PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
VKAPI PFN_vkEndCommandBuffer vkEndCommandBuffer;
VKAPI PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
VKAPI PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
VKAPI PFN_vkFreeDescriptorSets vkFreeDescriptorSets;
VKAPI PFN_vkFreeMemory vkFreeMemory;
VKAPI PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
VKAPI PFN_vkGetDeviceMemoryCommitment vkGetDeviceMemoryCommitment;
VKAPI PFN_vkGetDeviceQueue vkGetDeviceQueue;
VKAPI PFN_vkGetEventStatus vkGetEventStatus;
VKAPI PFN_vkGetFenceStatus vkGetFenceStatus;
VKAPI PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
VKAPI PFN_vkGetImageSparseMemoryRequirements vkGetImageSparseMemoryRequirements;
VKAPI PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
VKAPI PFN_vkGetPipelineCacheData vkGetPipelineCacheData;
VKAPI PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
VKAPI PFN_vkGetRenderAreaGranularity vkGetRenderAreaGranularity;
VKAPI PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
VKAPI PFN_vkMapMemory vkMapMemory;
VKAPI PFN_vkMergePipelineCaches vkMergePipelineCaches;
VKAPI PFN_vkQueueBindSparse vkQueueBindSparse;
VKAPI PFN_vkQueueSubmit vkQueueSubmit;
VKAPI PFN_vkQueueWaitIdle vkQueueWaitIdle;
VKAPI PFN_vkResetCommandBuffer vkResetCommandBuffer;
VKAPI PFN_vkResetCommandPool vkResetCommandPool;
VKAPI PFN_vkResetDescriptorPool vkResetDescriptorPool;
VKAPI PFN_vkResetEvent vkResetEvent;
VKAPI PFN_vkResetFences vkResetFences;
VKAPI PFN_vkSetEvent vkSetEvent;
VKAPI PFN_vkUnmapMemory vkUnmapMemory;
VKAPI PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
VKAPI PFN_vkWaitForFences vkWaitForFences;
#endif /* defined(VK_VERSION_1_0) */
#if defined(VK_VERSION_1_1)
VKAPI PFN_vkBindBufferMemory2 vkBindBufferMemory2;
VKAPI PFN_vkBindImageMemory2 vkBindImageMemory2;
VKAPI PFN_vkCmdDispatchBase vkCmdDispatchBase;
VKAPI PFN_vkCmdSetDeviceMask vkCmdSetDeviceMask;
VKAPI PFN_vkCreateDescriptorUpdateTemplate vkCreateDescriptorUpdateTemplate;
VKAPI PFN_vkCreateSamplerYcbcrConversion vkCreateSamplerYcbcrConversion;
VKAPI PFN_vkDestroyDescriptorUpdateTemplate vkDestroyDescriptorUpdateTemplate;
VKAPI PFN_vkDestroySamplerYcbcrConversion vkDestroySamplerYcbcrConversion;
VKAPI PFN_vkGetBufferMemoryRequirements2 vkGetBufferMemoryRequirements2;
VKAPI PFN_vkGetDescriptorSetLayoutSupport vkGetDescriptorSetLayoutSupport;
VKAPI PFN_vkGetDeviceGroupPeerMemoryFeatures vkGetDeviceGroupPeerMemoryFeatures;
VKAPI PFN_vkGetDeviceQueue2 vkGetDeviceQueue2;
VKAPI PFN_vkGetImageMemoryRequirements2 vkGetImageMemoryRequirements2;
VKAPI PFN_vkGetImageSparseMemoryRequirements2 vkGetImageSparseMemoryRequirements2;
VKAPI PFN_vkTrimCommandPool vkTrimCommandPool;
VKAPI PFN_vkUpdateDescriptorSetWithTemplate vkUpdateDescriptorSetWithTemplate;
#endif /* defined(VK_VERSION_1_1) */
#if defined(VK_VERSION_1_2)
VKAPI PFN_vkCmdBeginRenderPass2 vkCmdBeginRenderPass2;
VKAPI PFN_vkCmdDrawIndexedIndirectCount vkCmdDrawIndexedIndirectCount;
VKAPI PFN_vkCmdDrawIndirectCount vkCmdDrawIndirectCount;
VKAPI PFN_vkCmdEndRenderPass2 vkCmdEndRenderPass2;
VKAPI PFN_vkCmdNextSubpass2 vkCmdNextSubpass2;
VKAPI PFN_vkCreateRenderPass2 vkCreateRenderPass2;
VKAPI PFN_vkGetBufferDeviceAddress vkGetBufferDeviceAddress;
VKAPI PFN_vkGetBufferOpaqueCaptureAddress vkGetBufferOpaqueCaptureAddress;
VKAPI PFN_vkGetDeviceMemoryOpaqueCaptureAddress vkGetDeviceMemoryOpaqueCaptureAddress;
VKAPI PFN_vkGetSemaphoreCounterValue vkGetSemaphoreCounterValue;
VKAPI PFN_vkResetQueryPool vkResetQueryPool;
VKAPI PFN_vkSignalSemaphore vkSignalSemaphore;
VKAPI PFN_vkWaitSemaphores vkWaitSemaphores;
#endif /* defined(VK_VERSION_1_2) */
#if defined(VK_VERSION_1_3)
VKAPI PFN_vkCmdBeginRendering vkCmdBeginRendering;
VKAPI PFN_vkCmdBindVertexBuffers2 vkCmdBindVertexBuffers2;
VKAPI PFN_vkCmdBlitImage2 vkCmdBlitImage2;
VKAPI PFN_vkCmdCopyBuffer2 vkCmdCopyBuffer2;
VKAPI PFN_vkCmdCopyBufferToImage2 vkCmdCopyBufferToImage2;
VKAPI PFN_vkCmdCopyImage2 vkCmdCopyImage2;
VKAPI PFN_vkCmdCopyImageToBuffer2 vkCmdCopyImageToBuffer2;
VKAPI PFN_vkCmdEndRendering vkCmdEndRendering;
VKAPI PFN_vkCmdPipelineBarrier2 vkCmdPipelineBarrier2;
VKAPI PFN_vkCmdResetEvent2 vkCmdResetEvent2;
VKAPI PFN_vkCmdResolveImage2 vkCmdResolveImage2;
VKAPI PFN_vkCmdSetCullMode vkCmdSetCullMode;
VKAPI PFN_vkCmdSetDepthBiasEnable vkCmdSetDepthBiasEnable;
VKAPI PFN_vkCmdSetDepthBoundsTestEnable vkCmdSetDepthBoundsTestEnable;
VKAPI PFN_vkCmdSetDepthCompareOp vkCmdSetDepthCompareOp;
VKAPI PFN_vkCmdSetDepthTestEnable vkCmdSetDepthTestEnable;
VKAPI PFN_vkCmdSetDepthWriteEnable vkCmdSetDepthWriteEnable;
VKAPI PFN_vkCmdSetEvent2 vkCmdSetEvent2;
VKAPI PFN_vkCmdSetFrontFace vkCmdSetFrontFace;
VKAPI PFN_vkCmdSetPrimitiveRestartEnable vkCmdSetPrimitiveRestartEnable;
VKAPI PFN_vkCmdSetPrimitiveTopology vkCmdSetPrimitiveTopology;
VKAPI PFN_vkCmdSetRasterizerDiscardEnable vkCmdSetRasterizerDiscardEnable;
VKAPI PFN_vkCmdSetScissorWithCount vkCmdSetScissorWithCount;
VKAPI PFN_vkCmdSetStencilOp vkCmdSetStencilOp;
VKAPI PFN_vkCmdSetStencilTestEnable vkCmdSetStencilTestEnable;
VKAPI PFN_vkCmdSetViewportWithCount vkCmdSetViewportWithCount;
VKAPI PFN_vkCmdWaitEvents2 vkCmdWaitEvents2;
VKAPI PFN_vkCmdWriteTimestamp2 vkCmdWriteTimestamp2;
VKAPI PFN_vkCreatePrivateDataSlot vkCreatePrivateDataSlot;
VKAPI PFN_vkDestroyPrivateDataSlot vkDestroyPrivateDataSlot;
VKAPI PFN_vkGetDeviceBufferMemoryRequirements vkGetDeviceBufferMemoryRequirements;
VKAPI PFN_vkGetDeviceImageMemoryRequirements vkGetDeviceImageMemoryRequirements;
VKAPI PFN_vkGetDeviceImageSparseMemoryRequirements vkGetDeviceImageSparseMemoryRequirements;
VKAPI PFN_vkGetPrivateData vkGetPrivateData;
VKAPI PFN_vkQueueSubmit2 vkQueueSubmit2;
VKAPI PFN_vkSetPrivateData vkSetPrivateData;
#endif /* defined(VK_VERSION_1_3) */
#if defined(VK_VERSION_1_4)
VKAPI PFN_vkCmdBindDescriptorSets2 vkCmdBindDescriptorSets2;
VKAPI PFN_vkCmdBindIndexBuffer2 vkCmdBindIndexBuffer2;
VKAPI PFN_vkCmdPushConstants2 vkCmdPushConstants2;
VKAPI PFN_vkCmdPushDescriptorSet vkCmdPushDescriptorSet;
VKAPI PFN_vkCmdPushDescriptorSet2 vkCmdPushDescriptorSet2;
VKAPI PFN_vkCmdPushDescriptorSetWithTemplate vkCmdPushDescriptorSetWithTemplate;
VKAPI PFN_vkCmdPushDescriptorSetWithTemplate2 vkCmdPushDescriptorSetWithTemplate2;
VKAPI PFN_vkCmdSetLineStipple vkCmdSetLineStipple;
VKAPI PFN_vkCmdSetRenderingAttachmentLocations vkCmdSetRenderingAttachmentLocations;
VKAPI PFN_vkCmdSetRenderingInputAttachmentIndices vkCmdSetRenderingInputAttachmentIndices;
VKAPI PFN_vkCopyImageToImage vkCopyImageToImage;
VKAPI PFN_vkCopyImageToMemory vkCopyImageToMemory;
VKAPI PFN_vkCopyMemoryToImage vkCopyMemoryToImage;
VKAPI PFN_vkGetDeviceImageSubresourceLayout vkGetDeviceImageSubresourceLayout;
VKAPI PFN_vkGetImageSubresourceLayout2 vkGetImageSubresourceLayout2;
VKAPI PFN_vkGetRenderingAreaGranularity vkGetRenderingAreaGranularity;
VKAPI PFN_vkMapMemory2 vkMapMemory2;
VKAPI PFN_vkTransitionImageLayout vkTransitionImageLayout;
VKAPI PFN_vkUnmapMemory2 vkUnmapMemory2;
#endif /* defined(VK_VERSION_1_4) */
#if defined(VK_AMDX_shader_enqueue)
VKAPI PFN_vkCmdDispatchGraphAMDX vkCmdDispatchGraphAMDX;
VKAPI PFN_vkCmdDispatchGraphIndirectAMDX vkCmdDispatchGraphIndirectAMDX;
VKAPI PFN_vkCmdDispatchGraphIndirectCountAMDX vkCmdDispatchGraphIndirectCountAMDX;
VKAPI PFN_vkCmdInitializeGraphScratchMemoryAMDX vkCmdInitializeGraphScratchMemoryAMDX;
VKAPI PFN_vkCreateExecutionGraphPipelinesAMDX vkCreateExecutionGraphPipelinesAMDX;
VKAPI PFN_vkGetExecutionGraphPipelineNodeIndexAMDX vkGetExecutionGraphPipelineNodeIndexAMDX;
VKAPI PFN_vkGetExecutionGraphPipelineScratchSizeAMDX vkGetExecutionGraphPipelineScratchSizeAMDX;
#endif /* defined(VK_AMDX_shader_enqueue) */
#if defined(VK_AMD_anti_lag)
VKAPI PFN_vkAntiLagUpdateAMD vkAntiLagUpdateAMD;
#endif /* defined(VK_AMD_anti_lag) */
#if defined(VK_AMD_buffer_marker)
VKAPI PFN_vkCmdWriteBufferMarkerAMD vkCmdWriteBufferMarkerAMD;
#endif /* defined(VK_AMD_buffer_marker) */
#if defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2))
VKAPI PFN_vkCmdWriteBufferMarker2AMD vkCmdWriteBufferMarker2AMD;
#endif /* defined(VK_AMD_buffer_marker) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) */
#if defined(VK_AMD_display_native_hdr)
VKAPI PFN_vkSetLocalDimmingAMD vkSetLocalDimmingAMD;
#endif /* defined(VK_AMD_display_native_hdr) */
#if defined(VK_AMD_draw_indirect_count)
VKAPI PFN_vkCmdDrawIndexedIndirectCountAMD vkCmdDrawIndexedIndirectCountAMD;
VKAPI PFN_vkCmdDrawIndirectCountAMD vkCmdDrawIndirectCountAMD;
#endif /* defined(VK_AMD_draw_indirect_count) */
#if defined(VK_AMD_shader_info)
VKAPI PFN_vkGetShaderInfoAMD vkGetShaderInfoAMD;
#endif /* defined(VK_AMD_shader_info) */
#if defined(VK_ANDROID_external_memory_android_hardware_buffer)
VKAPI PFN_vkGetAndroidHardwareBufferPropertiesANDROID vkGetAndroidHardwareBufferPropertiesANDROID;
VKAPI PFN_vkGetMemoryAndroidHardwareBufferANDROID vkGetMemoryAndroidHardwareBufferANDROID;
#endif /* defined(VK_ANDROID_external_memory_android_hardware_buffer) */
#if defined(VK_ARM_data_graph)
VKAPI PFN_vkBindDataGraphPipelineSessionMemoryARM vkBindDataGraphPipelineSessionMemoryARM;
VKAPI PFN_vkCmdDispatchDataGraphARM vkCmdDispatchDataGraphARM;
VKAPI PFN_vkCreateDataGraphPipelineSessionARM vkCreateDataGraphPipelineSessionARM;
VKAPI PFN_vkCreateDataGraphPipelinesARM vkCreateDataGraphPipelinesARM;
VKAPI PFN_vkDestroyDataGraphPipelineSessionARM vkDestroyDataGraphPipelineSessionARM;
VKAPI PFN_vkGetDataGraphPipelineAvailablePropertiesARM vkGetDataGraphPipelineAvailablePropertiesARM;
VKAPI PFN_vkGetDataGraphPipelinePropertiesARM vkGetDataGraphPipelinePropertiesARM;
VKAPI PFN_vkGetDataGraphPipelineSessionBindPointRequirementsARM vkGetDataGraphPipelineSessionBindPointRequirementsARM;
VKAPI PFN_vkGetDataGraphPipelineSessionMemoryRequirementsARM vkGetDataGraphPipelineSessionMemoryRequirementsARM;
#endif /* defined(VK_ARM_data_graph) */
#if defined(VK_ARM_tensors)
VKAPI PFN_vkBindTensorMemoryARM vkBindTensorMemoryARM;
VKAPI PFN_vkCmdCopyTensorARM vkCmdCopyTensorARM;
VKAPI PFN_vkCreateTensorARM vkCreateTensorARM;
VKAPI PFN_vkCreateTensorViewARM vkCreateTensorViewARM;
VKAPI PFN_vkDestroyTensorARM vkDestroyTensorARM;
VKAPI PFN_vkDestroyTensorViewARM vkDestroyTensorViewARM;
VKAPI PFN_vkGetDeviceTensorMemoryRequirementsARM vkGetDeviceTensorMemoryRequirementsARM;
VKAPI PFN_vkGetTensorMemoryRequirementsARM vkGetTensorMemoryRequirementsARM;
#endif /* defined(VK_ARM_tensors) */
#if defined(VK_ARM_tensors) && defined(VK_EXT_descriptor_buffer)
VKAPI PFN_vkGetTensorOpaqueCaptureDescriptorDataARM vkGetTensorOpaqueCaptureDescriptorDataARM;
VKAPI PFN_vkGetTensorViewOpaqueCaptureDescriptorDataARM vkGetTensorViewOpaqueCaptureDescriptorDataARM;
#endif /* defined(VK_ARM_tensors) && defined(VK_EXT_descriptor_buffer) */
#if defined(VK_EXT_attachment_feedback_loop_dynamic_state)
VKAPI PFN_vkCmdSetAttachmentFeedbackLoopEnableEXT vkCmdSetAttachmentFeedbackLoopEnableEXT;
#endif /* defined(VK_EXT_attachment_feedback_loop_dynamic_state) */
#if defined(VK_EXT_buffer_device_address)
VKAPI PFN_vkGetBufferDeviceAddressEXT vkGetBufferDeviceAddressEXT;
#endif /* defined(VK_EXT_buffer_device_address) */
#if defined(VK_EXT_calibrated_timestamps)
VKAPI PFN_vkGetCalibratedTimestampsEXT vkGetCalibratedTimestampsEXT;
#endif /* defined(VK_EXT_calibrated_timestamps) */
#if defined(VK_EXT_color_write_enable)
VKAPI PFN_vkCmdSetColorWriteEnableEXT vkCmdSetColorWriteEnableEXT;
#endif /* defined(VK_EXT_color_write_enable) */
#if defined(VK_EXT_conditional_rendering)
VKAPI PFN_vkCmdBeginConditionalRenderingEXT vkCmdBeginConditionalRenderingEXT;
VKAPI PFN_vkCmdEndConditionalRenderingEXT vkCmdEndConditionalRenderingEXT;
#endif /* defined(VK_EXT_conditional_rendering) */
#if defined(VK_EXT_custom_resolve) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3))
VKAPI PFN_vkCmdBeginCustomResolveEXT vkCmdBeginCustomResolveEXT;
#endif /* defined(VK_EXT_custom_resolve) && (defined(VK_KHR_dynamic_rendering) || defined(VK_VERSION_1_3)) */
#if defined(VK_EXT_debug_marker)
VKAPI PFN_vkCmdDebugMarkerBeginEXT vkCmdDebugMarkerBeginEXT;
VKAPI PFN_vkCmdDebugMarkerEndEXT vkCmdDebugMarkerEndEXT;
VKAPI PFN_vkCmdDebugMarkerInsertEXT vkCmdDebugMarkerInsertEXT;
VKAPI PFN_vkDebugMarkerSetObjectNameEXT vkDebugMarkerSetObjectNameEXT;
VKAPI PFN_vkDebugMarkerSetObjectTagEXT vkDebugMarkerSetObjectTagEXT;
#endif /* defined(VK_EXT_debug_marker) */
#if defined(VK_EXT_depth_bias_control)
VKAPI PFN_vkCmdSetDepthBias2EXT vkCmdSetDepthBias2EXT;
#endif /* defined(VK_EXT_depth_bias_control) */
#if defined(VK_EXT_descriptor_buffer)
VKAPI PFN_vkCmdBindDescriptorBufferEmbeddedSamplersEXT vkCmdBindDescriptorBufferEmbeddedSamplersEXT;
VKAPI PFN_vkCmdBindDescriptorBuffersEXT vkCmdBindDescriptorBuffersEXT;
VKAPI PFN_vkCmdSetDescriptorBufferOffsetsEXT vkCmdSetDescriptorBufferOffsetsEXT;
VKAPI PFN_vkGetBufferOpaqueCaptureDescriptorDataEXT vkGetBufferOpaqueCaptureDescriptorDataEXT;
VKAPI PFN_vkGetDescriptorEXT vkGetDescriptorEXT;
VKAPI PFN_vkGetDescriptorSetLayoutBindingOffsetEXT vkGetDescriptorSetLayoutBindingOffsetEXT;
VKAPI PFN_vkGetDescriptorSetLayoutSizeEXT vkGetDescriptorSetLayoutSizeEXT;
VKAPI PFN_vkGetImageOpaqueCaptureDescriptorDataEXT vkGetImageOpaqueCaptureDescriptorDataEXT;
VKAPI PFN_vkGetImageViewOpaqueCaptureDescriptorDataEXT vkGetImageViewOpaqueCaptureDescriptorDataEXT;
VKAPI PFN_vkGetSamplerOpaqueCaptureDescriptorDataEXT vkGetSamplerOpaqueCaptureDescriptorDataEXT;
#endif /* defined(VK_EXT_descriptor_buffer) */
#if defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing))
VKAPI PFN_vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT vkGetAccelerationStructureOpaqueCaptureDescriptorDataEXT;
#endif /* defined(VK_EXT_descriptor_buffer) && (defined(VK_KHR_acceleration_structure) || defined(VK_NV_ray_tracing)) */
#if defined(VK_EXT_device_fault)
VKAPI PFN_vkGetDeviceFaultInfoEXT vkGetDeviceFaultInfoEXT;
#endif /* defined(VK_EXT_device_fault) */
#if defined(VK_EXT_device_generated_commands)
VKAPI PFN_vkCmdExecuteGeneratedCommandsEXT vkCmdExecuteGeneratedCommandsEXT;
VKAPI PFN_vkCmdPreprocessGeneratedCommandsEXT vkCmdPreprocessGeneratedCommandsEXT;
VKAPI PFN_vkCreateIndirectCommandsLayoutEXT vkCreateIndirectCommandsLayoutEXT;
VKAPI PFN_vkCreateIndirectExecutionSetEXT vkCreateIndirectExecutionSetEXT;
VKAPI PFN_vkDestroyIndirectCommandsLayoutEXT vkDestroyIndirectCommandsLayoutEXT;
VKAPI PFN_vkDestroyIndirectExecutionSetEXT vkDestroyIndirectExecutionSetEXT;
VKAPI PFN_vkGetGeneratedCommandsMemoryRequirementsEXT vkGetGeneratedCommandsMemoryRequirementsEXT;
VKAPI PFN_vkUpdateIndirectExecutionSetPipelineEXT vkUpdateIndirectExecutionSetPipelineEXT;
VKAPI PFN_vkUpdateIndirectExecutionSetShaderEXT vkUpdateIndirectExecutionSetShaderEXT;
#endif /* defined(VK_EXT_device_generated_commands) */
#if defined(VK_EXT_discard_rectangles)
VKAPI PFN_vkCmdSetDiscardRectangleEXT vkCmdSetDiscardRectangleEXT;
#endif /* defined(VK_EXT_discard_rectangles) */
#if defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2
VKAPI PFN_vkCmdSetDiscardRectangleEnableEXT vkCmdSetDiscardRectangleEnableEXT;
VKAPI PFN_vkCmdSetDiscardRectangleModeEXT vkCmdSetDiscardRectangleModeEXT;
#endif /* defined(VK_EXT_discard_rectangles) && VK_EXT_DISCARD_RECTANGLES_SPEC_VERSION >= 2 */
#if defined(VK_EXT_display_control)
VKAPI PFN_vkDisplayPowerControlEXT vkDisplayPowerControlEXT;
VKAPI PFN_vkGetSwapchainCounterEXT vkGetSwapchainCounterEXT;
VKAPI PFN_vkRegisterDeviceEventEXT vkRegisterDeviceEventEXT;
VKAPI PFN_vkRegisterDisplayEventEXT vkRegisterDisplayEventEXT;
#endif /* defined(VK_EXT_display_control) */
#if defined(VK_EXT_external_memory_host)
VKAPI PFN_vkGetMemoryHostPointerPropertiesEXT vkGetMemoryHostPointerPropertiesEXT;
#endif /* defined(VK_EXT_external_memory_host) */
#if defined(VK_EXT_external_memory_metal)
VKAPI PFN_vkGetMemoryMetalHandleEXT vkGetMemoryMetalHandleEXT;
VKAPI PFN_vkGetMemoryMetalHandlePropertiesEXT vkGetMemoryMetalHandlePropertiesEXT;
#endif /* defined(VK_EXT_external_memory_metal) */
#if defined(VK_EXT_fragment_density_map_offset)
VKAPI PFN_vkCmdEndRendering2EXT vkCmdEndRendering2EXT;
#endif /* defined(VK_EXT_fragment_density_map_offset) */
#if defined(VK_EXT_full_screen_exclusive)
VKAPI PFN_vkAcquireFullScreenExclusiveModeEXT vkAcquireFullScreenExclusiveModeEXT;
VKAPI PFN_vkReleaseFullScreenExclusiveModeEXT vkReleaseFullScreenExclusiveModeEXT;
#endif /* defined(VK_EXT_full_screen_exclusive) */
#if defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1))
VKAPI PFN_vkGetDeviceGroupSurfacePresentModes2EXT vkGetDeviceGroupSurfacePresentModes2EXT;
#endif /* defined(VK_EXT_full_screen_exclusive) && (defined(VK_KHR_device_group) || defined(VK_VERSION_1_1)) */
#if defined(VK_EXT_hdr_metadata)
VKAPI PFN_vkSetHdrMetadataEXT vkSetHdrMetadataEXT;
#endif /* defined(VK_EXT_hdr_metadata) */
#if defined(VK_EXT_host_image_copy)
VKAPI PFN_vkCopyImageToImageEXT vkCopyImageToImageEXT;
VKAPI PFN_vkCopyImageToMemoryEXT vkCopyImageToMemoryEXT;
VKAPI PFN_vkCopyMemoryToImageEXT vkCopyMemoryToImageEXT;
VKAPI PFN_vkTransitionImageLayoutEXT vkTransitionImageLayoutEXT;
#endif /* defined(VK_EXT_host_image_copy) */
#if defined(VK_EXT_host_query_reset)
VKAPI PFN_vkResetQueryPoolEXT vkResetQueryPoolEXT;
#endif /* defined(VK_EXT_host_query_reset) */
#if defined(VK_EXT_image_drm_format_modifier)
VKAPI PFN_vkGetImageDrmFormatModifierPropertiesEXT vkGetImageDrmFormatModifierPropertiesEXT;
#endif /* defined(VK_EXT_image_drm_format_modifier) */
#if defined(VK_EXT_line_rasterization)
VKAPI PFN_vkCmdSetLineStippleEXT vkCmdSetLineStippleEXT;
#endif /* defined(VK_EXT_line_rasterization) */
#if defined(VK_EXT_memory_decompression)
VKAPI PFN_vkCmdDecompressMemoryEXT vkCmdDecompressMemoryEXT;
VKAPI PFN_vkCmdDecompressMemoryIndirectCountEXT vkCmdDecompressMemoryIndirectCountEXT;
#endif /* defined(VK_EXT_memory_decompression) */
#if defined(VK_EXT_mesh_shader)
VKAPI PFN_vkCmdDrawMeshTasksEXT vkCmdDrawMeshTasksEXT;
VKAPI PFN_vkCmdDrawMeshTasksIndirectEXT vkCmdDrawMeshTasksIndirectEXT;
#endif /* defined(VK_EXT_mesh_shader) */
#if defined(VK_EXT_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count))
VKAPI PFN_vkCmdDrawMeshTasksIndirectCountEXT vkCmdDrawMeshTasksIndirectCountEXT;
#endif /* defined(VK_EXT_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) */
#if defined(VK_EXT_metal_objects)
VKAPI PFN_vkExportMetalObjectsEXT vkExportMetalObjectsEXT;
#endif /* defined(VK_EXT_metal_objects) */
#if defined(VK_EXT_multi_draw)
VKAPI PFN_vkCmdDrawMultiEXT vkCmdDrawMultiEXT;
VKAPI PFN_vkCmdDrawMultiIndexedEXT vkCmdDrawMultiIndexedEXT;
#endif /* defined(VK_EXT_multi_draw) */
#if defined(VK_EXT_opacity_micromap)
VKAPI PFN_vkBuildMicromapsEXT vkBuildMicromapsEXT;
VKAPI PFN_vkCmdBuildMicromapsEXT vkCmdBuildMicromapsEXT;
VKAPI PFN_vkCmdCopyMemoryToMicromapEXT vkCmdCopyMemoryToMicromapEXT;
VKAPI PFN_vkCmdCopyMicromapEXT vkCmdCopyMicromapEXT;
VKAPI PFN_vkCmdCopyMicromapToMemoryEXT vkCmdCopyMicromapToMemoryEXT;
VKAPI PFN_vkCmdWriteMicromapsPropertiesEXT vkCmdWriteMicromapsPropertiesEXT;
VKAPI PFN_vkCopyMemoryToMicromapEXT vkCopyMemoryToMicromapEXT;
VKAPI PFN_vkCopyMicromapEXT vkCopyMicromapEXT;
VKAPI PFN_vkCopyMicromapToMemoryEXT vkCopyMicromapToMemoryEXT;
VKAPI PFN_vkCreateMicromapEXT vkCreateMicromapEXT;
VKAPI PFN_vkDestroyMicromapEXT vkDestroyMicromapEXT;
VKAPI PFN_vkGetDeviceMicromapCompatibilityEXT vkGetDeviceMicromapCompatibilityEXT;
VKAPI PFN_vkGetMicromapBuildSizesEXT vkGetMicromapBuildSizesEXT;
VKAPI PFN_vkWriteMicromapsPropertiesEXT vkWriteMicromapsPropertiesEXT;
#endif /* defined(VK_EXT_opacity_micromap) */
#if defined(VK_EXT_pageable_device_local_memory)
VKAPI PFN_vkSetDeviceMemoryPriorityEXT vkSetDeviceMemoryPriorityEXT;
#endif /* defined(VK_EXT_pageable_device_local_memory) */
#if defined(VK_EXT_pipeline_properties)
VKAPI PFN_vkGetPipelinePropertiesEXT vkGetPipelinePropertiesEXT;
#endif /* defined(VK_EXT_pipeline_properties) */
#if defined(VK_EXT_present_timing)
VKAPI PFN_vkGetPastPresentationTimingEXT vkGetPastPresentationTimingEXT;
VKAPI PFN_vkGetSwapchainTimeDomainPropertiesEXT vkGetSwapchainTimeDomainPropertiesEXT;
VKAPI PFN_vkGetSwapchainTimingPropertiesEXT vkGetSwapchainTimingPropertiesEXT;
VKAPI PFN_vkSetSwapchainPresentTimingQueueSizeEXT vkSetSwapchainPresentTimingQueueSizeEXT;
#endif /* defined(VK_EXT_present_timing) */
#if defined(VK_EXT_private_data)
VKAPI PFN_vkCreatePrivateDataSlotEXT vkCreatePrivateDataSlotEXT;
VKAPI PFN_vkDestroyPrivateDataSlotEXT vkDestroyPrivateDataSlotEXT;
VKAPI PFN_vkGetPrivateDataEXT vkGetPrivateDataEXT;
VKAPI PFN_vkSetPrivateDataEXT vkSetPrivateDataEXT;
#endif /* defined(VK_EXT_private_data) */
#if defined(VK_EXT_sample_locations)
VKAPI PFN_vkCmdSetSampleLocationsEXT vkCmdSetSampleLocationsEXT;
#endif /* defined(VK_EXT_sample_locations) */
#if defined(VK_EXT_shader_module_identifier)
VKAPI PFN_vkGetShaderModuleCreateInfoIdentifierEXT vkGetShaderModuleCreateInfoIdentifierEXT;
VKAPI PFN_vkGetShaderModuleIdentifierEXT vkGetShaderModuleIdentifierEXT;
#endif /* defined(VK_EXT_shader_module_identifier) */
#if defined(VK_EXT_shader_object)
VKAPI PFN_vkCmdBindShadersEXT vkCmdBindShadersEXT;
VKAPI PFN_vkCreateShadersEXT vkCreateShadersEXT;
VKAPI PFN_vkDestroyShaderEXT vkDestroyShaderEXT;
VKAPI PFN_vkGetShaderBinaryDataEXT vkGetShaderBinaryDataEXT;
#endif /* defined(VK_EXT_shader_object) */
#if defined(VK_EXT_swapchain_maintenance1)
VKAPI PFN_vkReleaseSwapchainImagesEXT vkReleaseSwapchainImagesEXT;
#endif /* defined(VK_EXT_swapchain_maintenance1) */
#if defined(VK_EXT_transform_feedback)
VKAPI PFN_vkCmdBeginQueryIndexedEXT vkCmdBeginQueryIndexedEXT;
VKAPI PFN_vkCmdBeginTransformFeedbackEXT vkCmdBeginTransformFeedbackEXT;
VKAPI PFN_vkCmdBindTransformFeedbackBuffersEXT vkCmdBindTransformFeedbackBuffersEXT;
VKAPI PFN_vkCmdDrawIndirectByteCountEXT vkCmdDrawIndirectByteCountEXT;
VKAPI PFN_vkCmdEndQueryIndexedEXT vkCmdEndQueryIndexedEXT;
VKAPI PFN_vkCmdEndTransformFeedbackEXT vkCmdEndTransformFeedbackEXT;
#endif /* defined(VK_EXT_transform_feedback) */
#if defined(VK_EXT_validation_cache)
VKAPI PFN_vkCreateValidationCacheEXT vkCreateValidationCacheEXT;
VKAPI PFN_vkDestroyValidationCacheEXT vkDestroyValidationCacheEXT;
VKAPI PFN_vkGetValidationCacheDataEXT vkGetValidationCacheDataEXT;
VKAPI PFN_vkMergeValidationCachesEXT vkMergeValidationCachesEXT;
#endif /* defined(VK_EXT_validation_cache) */
#if defined(VK_FUCHSIA_buffer_collection)
VKAPI PFN_vkCreateBufferCollectionFUCHSIA vkCreateBufferCollectionFUCHSIA;
VKAPI PFN_vkDestroyBufferCollectionFUCHSIA vkDestroyBufferCollectionFUCHSIA;
VKAPI PFN_vkGetBufferCollectionPropertiesFUCHSIA vkGetBufferCollectionPropertiesFUCHSIA;
VKAPI PFN_vkSetBufferCollectionBufferConstraintsFUCHSIA vkSetBufferCollectionBufferConstraintsFUCHSIA;
VKAPI PFN_vkSetBufferCollectionImageConstraintsFUCHSIA vkSetBufferCollectionImageConstraintsFUCHSIA;
#endif /* defined(VK_FUCHSIA_buffer_collection) */
#if defined(VK_FUCHSIA_external_memory)
VKAPI PFN_vkGetMemoryZirconHandleFUCHSIA vkGetMemoryZirconHandleFUCHSIA;
VKAPI PFN_vkGetMemoryZirconHandlePropertiesFUCHSIA vkGetMemoryZirconHandlePropertiesFUCHSIA;
#endif /* defined(VK_FUCHSIA_external_memory) */
#if defined(VK_FUCHSIA_external_semaphore)
VKAPI PFN_vkGetSemaphoreZirconHandleFUCHSIA vkGetSemaphoreZirconHandleFUCHSIA;
VKAPI PFN_vkImportSemaphoreZirconHandleFUCHSIA vkImportSemaphoreZirconHandleFUCHSIA;
#endif /* defined(VK_FUCHSIA_external_semaphore) */
#if defined(VK_GOOGLE_display_timing)
VKAPI PFN_vkGetPastPresentationTimingGOOGLE vkGetPastPresentationTimingGOOGLE;
VKAPI PFN_vkGetRefreshCycleDurationGOOGLE vkGetRefreshCycleDurationGOOGLE;
#endif /* defined(VK_GOOGLE_display_timing) */
#if defined(VK_HUAWEI_cluster_culling_shader)
VKAPI PFN_vkCmdDrawClusterHUAWEI vkCmdDrawClusterHUAWEI;
VKAPI PFN_vkCmdDrawClusterIndirectHUAWEI vkCmdDrawClusterIndirectHUAWEI;
#endif /* defined(VK_HUAWEI_cluster_culling_shader) */
#if defined(VK_HUAWEI_invocation_mask)
VKAPI PFN_vkCmdBindInvocationMaskHUAWEI vkCmdBindInvocationMaskHUAWEI;
#endif /* defined(VK_HUAWEI_invocation_mask) */
#if defined(VK_HUAWEI_subpass_shading) && VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION >= 2
VKAPI PFN_vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI vkGetDeviceSubpassShadingMaxWorkgroupSizeHUAWEI;
#endif /* defined(VK_HUAWEI_subpass_shading) && VK_HUAWEI_SUBPASS_SHADING_SPEC_VERSION >= 2 */
#if defined(VK_HUAWEI_subpass_shading)
VKAPI PFN_vkCmdSubpassShadingHUAWEI vkCmdSubpassShadingHUAWEI;
#endif /* defined(VK_HUAWEI_subpass_shading) */
#if defined(VK_INTEL_performance_query)
VKAPI PFN_vkAcquirePerformanceConfigurationINTEL vkAcquirePerformanceConfigurationINTEL;
VKAPI PFN_vkCmdSetPerformanceMarkerINTEL vkCmdSetPerformanceMarkerINTEL;
VKAPI PFN_vkCmdSetPerformanceOverrideINTEL vkCmdSetPerformanceOverrideINTEL;
VKAPI PFN_vkCmdSetPerformanceStreamMarkerINTEL vkCmdSetPerformanceStreamMarkerINTEL;
VKAPI PFN_vkGetPerformanceParameterINTEL vkGetPerformanceParameterINTEL;
VKAPI PFN_vkInitializePerformanceApiINTEL vkInitializePerformanceApiINTEL;
VKAPI PFN_vkQueueSetPerformanceConfigurationINTEL vkQueueSetPerformanceConfigurationINTEL;
VKAPI PFN_vkReleasePerformanceConfigurationINTEL vkReleasePerformanceConfigurationINTEL;
VKAPI PFN_vkUninitializePerformanceApiINTEL vkUninitializePerformanceApiINTEL;
#endif /* defined(VK_INTEL_performance_query) */
#if defined(VK_KHR_acceleration_structure)
VKAPI PFN_vkBuildAccelerationStructuresKHR vkBuildAccelerationStructuresKHR;
VKAPI PFN_vkCmdBuildAccelerationStructuresIndirectKHR vkCmdBuildAccelerationStructuresIndirectKHR;
VKAPI PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
VKAPI PFN_vkCmdCopyAccelerationStructureKHR vkCmdCopyAccelerationStructureKHR;
VKAPI PFN_vkCmdCopyAccelerationStructureToMemoryKHR vkCmdCopyAccelerationStructureToMemoryKHR;
VKAPI PFN_vkCmdCopyMemoryToAccelerationStructureKHR vkCmdCopyMemoryToAccelerationStructureKHR;
VKAPI PFN_vkCmdWriteAccelerationStructuresPropertiesKHR vkCmdWriteAccelerationStructuresPropertiesKHR;
VKAPI PFN_vkCopyAccelerationStructureKHR vkCopyAccelerationStructureKHR;
VKAPI PFN_vkCopyAccelerationStructureToMemoryKHR vkCopyAccelerationStructureToMemoryKHR;
VKAPI PFN_vkCopyMemoryToAccelerationStructureKHR vkCopyMemoryToAccelerationStructureKHR;
VKAPI PFN_vkCreateAccelerationStructureKHR vkCreateAccelerationStructureKHR;
VKAPI PFN_vkDestroyAccelerationStructureKHR vkDestroyAccelerationStructureKHR;
VKAPI PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
VKAPI PFN_vkGetAccelerationStructureDeviceAddressKHR vkGetAccelerationStructureDeviceAddressKHR;
VKAPI PFN_vkGetDeviceAccelerationStructureCompatibilityKHR vkGetDeviceAccelerationStructureCompatibilityKHR;
VKAPI PFN_vkWriteAccelerationStructuresPropertiesKHR vkWriteAccelerationStructuresPropertiesKHR;
#endif /* defined(VK_KHR_acceleration_structure) */
#if defined(VK_KHR_bind_memory2)
VKAPI PFN_vkBindBufferMemory2KHR vkBindBufferMemory2KHR;
VKAPI PFN_vkBindImageMemory2KHR vkBindImageMemory2KHR;
#endif /* defined(VK_KHR_bind_memory2) */
#if defined(VK_KHR_buffer_device_address)
VKAPI PFN_vkGetBufferDeviceAddressKHR vkGetBufferDeviceAddressKHR;
VKAPI PFN_vkGetBufferOpaqueCaptureAddressKHR vkGetBufferOpaqueCaptureAddressKHR;
VKAPI PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR vkGetDeviceMemoryOpaqueCaptureAddressKHR;
#endif /* defined(VK_KHR_buffer_device_address) */
#if defined(VK_KHR_calibrated_timestamps)
VKAPI PFN_vkGetCalibratedTimestampsKHR vkGetCalibratedTimestampsKHR;
#endif /* defined(VK_KHR_calibrated_timestamps) */
#if defined(VK_KHR_copy_commands2)
VKAPI PFN_vkCmdBlitImage2KHR vkCmdBlitImage2KHR;
VKAPI PFN_vkCmdCopyBuffer2KHR vkCmdCopyBuffer2KHR;
VKAPI PFN_vkCmdCopyBufferToImage2KHR vkCmdCopyBufferToImage2KHR;
VKAPI PFN_vkCmdCopyImage2KHR vkCmdCopyImage2KHR;
VKAPI PFN_vkCmdCopyImageToBuffer2KHR vkCmdCopyImageToBuffer2KHR;
VKAPI PFN_vkCmdResolveImage2KHR vkCmdResolveImage2KHR;
#endif /* defined(VK_KHR_copy_commands2) */
#if defined(VK_KHR_copy_memory_indirect)
VKAPI PFN_vkCmdCopyMemoryIndirectKHR vkCmdCopyMemoryIndirectKHR;
VKAPI PFN_vkCmdCopyMemoryToImageIndirectKHR vkCmdCopyMemoryToImageIndirectKHR;
#endif /* defined(VK_KHR_copy_memory_indirect) */
#if defined(VK_KHR_create_renderpass2)
VKAPI PFN_vkCmdBeginRenderPass2KHR vkCmdBeginRenderPass2KHR;
VKAPI PFN_vkCmdEndRenderPass2KHR vkCmdEndRenderPass2KHR;
VKAPI PFN_vkCmdNextSubpass2KHR vkCmdNextSubpass2KHR;
VKAPI PFN_vkCreateRenderPass2KHR vkCreateRenderPass2KHR;
#endif /* defined(VK_KHR_create_renderpass2) */
#if defined(VK_KHR_deferred_host_operations)
VKAPI PFN_vkCreateDeferredOperationKHR vkCreateDeferredOperationKHR;
VKAPI PFN_vkDeferredOperationJoinKHR vkDeferredOperationJoinKHR;
VKAPI PFN_vkDestroyDeferredOperationKHR vkDestroyDeferredOperationKHR;
VKAPI PFN_vkGetDeferredOperationMaxConcurrencyKHR vkGetDeferredOperationMaxConcurrencyKHR;
VKAPI PFN_vkGetDeferredOperationResultKHR vkGetDeferredOperationResultKHR;
#endif /* defined(VK_KHR_deferred_host_operations) */
#if defined(VK_KHR_descriptor_update_template)
VKAPI PFN_vkCreateDescriptorUpdateTemplateKHR vkCreateDescriptorUpdateTemplateKHR;
VKAPI PFN_vkDestroyDescriptorUpdateTemplateKHR vkDestroyDescriptorUpdateTemplateKHR;
VKAPI PFN_vkUpdateDescriptorSetWithTemplateKHR vkUpdateDescriptorSetWithTemplateKHR;
#endif /* defined(VK_KHR_descriptor_update_template) */
#if defined(VK_KHR_device_group)
VKAPI PFN_vkCmdDispatchBaseKHR vkCmdDispatchBaseKHR;
VKAPI PFN_vkCmdSetDeviceMaskKHR vkCmdSetDeviceMaskKHR;
VKAPI PFN_vkGetDeviceGroupPeerMemoryFeaturesKHR vkGetDeviceGroupPeerMemoryFeaturesKHR;
#endif /* defined(VK_KHR_device_group) */
#if defined(VK_KHR_display_swapchain)
VKAPI PFN_vkCreateSharedSwapchainsKHR vkCreateSharedSwapchainsKHR;
#endif /* defined(VK_KHR_display_swapchain) */
#if defined(VK_KHR_draw_indirect_count)
VKAPI PFN_vkCmdDrawIndexedIndirectCountKHR vkCmdDrawIndexedIndirectCountKHR;
VKAPI PFN_vkCmdDrawIndirectCountKHR vkCmdDrawIndirectCountKHR;
#endif /* defined(VK_KHR_draw_indirect_count) */
#if defined(VK_KHR_dynamic_rendering)
VKAPI PFN_vkCmdBeginRenderingKHR vkCmdBeginRenderingKHR;
VKAPI PFN_vkCmdEndRenderingKHR vkCmdEndRenderingKHR;
#endif /* defined(VK_KHR_dynamic_rendering) */
#if defined(VK_KHR_dynamic_rendering_local_read)
VKAPI PFN_vkCmdSetRenderingAttachmentLocationsKHR vkCmdSetRenderingAttachmentLocationsKHR;
VKAPI PFN_vkCmdSetRenderingInputAttachmentIndicesKHR vkCmdSetRenderingInputAttachmentIndicesKHR;
#endif /* defined(VK_KHR_dynamic_rendering_local_read) */
#if defined(VK_KHR_external_fence_fd)
VKAPI PFN_vkGetFenceFdKHR vkGetFenceFdKHR;
VKAPI PFN_vkImportFenceFdKHR vkImportFenceFdKHR;
#endif /* defined(VK_KHR_external_fence_fd) */
#if defined(VK_KHR_external_fence_win32)
VKAPI PFN_vkGetFenceWin32HandleKHR vkGetFenceWin32HandleKHR;
VKAPI PFN_vkImportFenceWin32HandleKHR vkImportFenceWin32HandleKHR;
#endif /* defined(VK_KHR_external_fence_win32) */
#if defined(VK_KHR_external_memory_fd)
VKAPI PFN_vkGetMemoryFdKHR vkGetMemoryFdKHR;
VKAPI PFN_vkGetMemoryFdPropertiesKHR vkGetMemoryFdPropertiesKHR;
#endif /* defined(VK_KHR_external_memory_fd) */
#if defined(VK_KHR_external_memory_win32)
VKAPI PFN_vkGetMemoryWin32HandleKHR vkGetMemoryWin32HandleKHR;
VKAPI PFN_vkGetMemoryWin32HandlePropertiesKHR vkGetMemoryWin32HandlePropertiesKHR;
#endif /* defined(VK_KHR_external_memory_win32) */
#if defined(VK_KHR_external_semaphore_fd)
VKAPI PFN_vkGetSemaphoreFdKHR vkGetSemaphoreFdKHR;
VKAPI PFN_vkImportSemaphoreFdKHR vkImportSemaphoreFdKHR;
#endif /* defined(VK_KHR_external_semaphore_fd) */
#if defined(VK_KHR_external_semaphore_win32)
VKAPI PFN_vkGetSemaphoreWin32HandleKHR vkGetSemaphoreWin32HandleKHR;
VKAPI PFN_vkImportSemaphoreWin32HandleKHR vkImportSemaphoreWin32HandleKHR;
#endif /* defined(VK_KHR_external_semaphore_win32) */
#if defined(VK_KHR_fragment_shading_rate)
VKAPI PFN_vkCmdSetFragmentShadingRateKHR vkCmdSetFragmentShadingRateKHR;
#endif /* defined(VK_KHR_fragment_shading_rate) */
#if defined(VK_KHR_get_memory_requirements2)
VKAPI PFN_vkGetBufferMemoryRequirements2KHR vkGetBufferMemoryRequirements2KHR;
VKAPI PFN_vkGetImageMemoryRequirements2KHR vkGetImageMemoryRequirements2KHR;
VKAPI PFN_vkGetImageSparseMemoryRequirements2KHR vkGetImageSparseMemoryRequirements2KHR;
#endif /* defined(VK_KHR_get_memory_requirements2) */
#if defined(VK_KHR_line_rasterization)
VKAPI PFN_vkCmdSetLineStippleKHR vkCmdSetLineStippleKHR;
#endif /* defined(VK_KHR_line_rasterization) */
#if defined(VK_KHR_maintenance1)
VKAPI PFN_vkTrimCommandPoolKHR vkTrimCommandPoolKHR;
#endif /* defined(VK_KHR_maintenance1) */
#if defined(VK_KHR_maintenance10)
VKAPI PFN_vkCmdEndRendering2KHR vkCmdEndRendering2KHR;
#endif /* defined(VK_KHR_maintenance10) */
#if defined(VK_KHR_maintenance3)
VKAPI PFN_vkGetDescriptorSetLayoutSupportKHR vkGetDescriptorSetLayoutSupportKHR;
#endif /* defined(VK_KHR_maintenance3) */
#if defined(VK_KHR_maintenance4)
VKAPI PFN_vkGetDeviceBufferMemoryRequirementsKHR vkGetDeviceBufferMemoryRequirementsKHR;
VKAPI PFN_vkGetDeviceImageMemoryRequirementsKHR vkGetDeviceImageMemoryRequirementsKHR;
VKAPI PFN_vkGetDeviceImageSparseMemoryRequirementsKHR vkGetDeviceImageSparseMemoryRequirementsKHR;
#endif /* defined(VK_KHR_maintenance4) */
#if defined(VK_KHR_maintenance5)
VKAPI PFN_vkCmdBindIndexBuffer2KHR vkCmdBindIndexBuffer2KHR;
VKAPI PFN_vkGetDeviceImageSubresourceLayoutKHR vkGetDeviceImageSubresourceLayoutKHR;
VKAPI PFN_vkGetImageSubresourceLayout2KHR vkGetImageSubresourceLayout2KHR;
VKAPI PFN_vkGetRenderingAreaGranularityKHR vkGetRenderingAreaGranularityKHR;
#endif /* defined(VK_KHR_maintenance5) */
#if defined(VK_KHR_maintenance6)
VKAPI PFN_vkCmdBindDescriptorSets2KHR vkCmdBindDescriptorSets2KHR;
VKAPI PFN_vkCmdPushConstants2KHR vkCmdPushConstants2KHR;
#endif /* defined(VK_KHR_maintenance6) */
#if defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor)
VKAPI PFN_vkCmdPushDescriptorSet2KHR vkCmdPushDescriptorSet2KHR;
VKAPI PFN_vkCmdPushDescriptorSetWithTemplate2KHR vkCmdPushDescriptorSetWithTemplate2KHR;
#endif /* defined(VK_KHR_maintenance6) && defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer)
VKAPI PFN_vkCmdBindDescriptorBufferEmbeddedSamplers2EXT vkCmdBindDescriptorBufferEmbeddedSamplers2EXT;
VKAPI PFN_vkCmdSetDescriptorBufferOffsets2EXT vkCmdSetDescriptorBufferOffsets2EXT;
#endif /* defined(VK_KHR_maintenance6) && defined(VK_EXT_descriptor_buffer) */
#if defined(VK_KHR_map_memory2)
VKAPI PFN_vkMapMemory2KHR vkMapMemory2KHR;
VKAPI PFN_vkUnmapMemory2KHR vkUnmapMemory2KHR;
#endif /* defined(VK_KHR_map_memory2) */
#if defined(VK_KHR_performance_query)
VKAPI PFN_vkAcquireProfilingLockKHR vkAcquireProfilingLockKHR;
VKAPI PFN_vkReleaseProfilingLockKHR vkReleaseProfilingLockKHR;
#endif /* defined(VK_KHR_performance_query) */
#if defined(VK_KHR_pipeline_binary)
VKAPI PFN_vkCreatePipelineBinariesKHR vkCreatePipelineBinariesKHR;
VKAPI PFN_vkDestroyPipelineBinaryKHR vkDestroyPipelineBinaryKHR;
VKAPI PFN_vkGetPipelineBinaryDataKHR vkGetPipelineBinaryDataKHR;
VKAPI PFN_vkGetPipelineKeyKHR vkGetPipelineKeyKHR;
VKAPI PFN_vkReleaseCapturedPipelineDataKHR vkReleaseCapturedPipelineDataKHR;
#endif /* defined(VK_KHR_pipeline_binary) */
#if defined(VK_KHR_pipeline_executable_properties)
VKAPI PFN_vkGetPipelineExecutableInternalRepresentationsKHR vkGetPipelineExecutableInternalRepresentationsKHR;
VKAPI PFN_vkGetPipelineExecutablePropertiesKHR vkGetPipelineExecutablePropertiesKHR;
VKAPI PFN_vkGetPipelineExecutableStatisticsKHR vkGetPipelineExecutableStatisticsKHR;
#endif /* defined(VK_KHR_pipeline_executable_properties) */
#if defined(VK_KHR_present_wait)
VKAPI PFN_vkWaitForPresentKHR vkWaitForPresentKHR;
#endif /* defined(VK_KHR_present_wait) */
#if defined(VK_KHR_present_wait2)
VKAPI PFN_vkWaitForPresent2KHR vkWaitForPresent2KHR;
#endif /* defined(VK_KHR_present_wait2) */
#if defined(VK_KHR_push_descriptor)
VKAPI PFN_vkCmdPushDescriptorSetKHR vkCmdPushDescriptorSetKHR;
#endif /* defined(VK_KHR_push_descriptor) */
#if defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline)
VKAPI PFN_vkCmdTraceRaysIndirect2KHR vkCmdTraceRaysIndirect2KHR;
#endif /* defined(VK_KHR_ray_tracing_maintenance1) && defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_ray_tracing_pipeline)
VKAPI PFN_vkCmdSetRayTracingPipelineStackSizeKHR vkCmdSetRayTracingPipelineStackSizeKHR;
VKAPI PFN_vkCmdTraceRaysIndirectKHR vkCmdTraceRaysIndirectKHR;
VKAPI PFN_vkCmdTraceRaysKHR vkCmdTraceRaysKHR;
VKAPI PFN_vkCreateRayTracingPipelinesKHR vkCreateRayTracingPipelinesKHR;
VKAPI PFN_vkGetRayTracingCaptureReplayShaderGroupHandlesKHR vkGetRayTracingCaptureReplayShaderGroupHandlesKHR;
VKAPI PFN_vkGetRayTracingShaderGroupHandlesKHR vkGetRayTracingShaderGroupHandlesKHR;
VKAPI PFN_vkGetRayTracingShaderGroupStackSizeKHR vkGetRayTracingShaderGroupStackSizeKHR;
#endif /* defined(VK_KHR_ray_tracing_pipeline) */
#if defined(VK_KHR_sampler_ycbcr_conversion)
VKAPI PFN_vkCreateSamplerYcbcrConversionKHR vkCreateSamplerYcbcrConversionKHR;
VKAPI PFN_vkDestroySamplerYcbcrConversionKHR vkDestroySamplerYcbcrConversionKHR;
#endif /* defined(VK_KHR_sampler_ycbcr_conversion) */
#if defined(VK_KHR_shared_presentable_image)
VKAPI PFN_vkGetSwapchainStatusKHR vkGetSwapchainStatusKHR;
#endif /* defined(VK_KHR_shared_presentable_image) */
#if defined(VK_KHR_swapchain)
VKAPI PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
VKAPI PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
VKAPI PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
VKAPI PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
VKAPI PFN_vkQueuePresentKHR vkQueuePresentKHR;
#endif /* defined(VK_KHR_swapchain) */
#if defined(VK_KHR_swapchain_maintenance1)
VKAPI PFN_vkReleaseSwapchainImagesKHR vkReleaseSwapchainImagesKHR;
#endif /* defined(VK_KHR_swapchain_maintenance1) */
#if defined(VK_KHR_synchronization2)
VKAPI PFN_vkCmdPipelineBarrier2KHR vkCmdPipelineBarrier2KHR;
VKAPI PFN_vkCmdResetEvent2KHR vkCmdResetEvent2KHR;
VKAPI PFN_vkCmdSetEvent2KHR vkCmdSetEvent2KHR;
VKAPI PFN_vkCmdWaitEvents2KHR vkCmdWaitEvents2KHR;
VKAPI PFN_vkCmdWriteTimestamp2KHR vkCmdWriteTimestamp2KHR;
VKAPI PFN_vkQueueSubmit2KHR vkQueueSubmit2KHR;
#endif /* defined(VK_KHR_synchronization2) */
#if defined(VK_KHR_timeline_semaphore)
VKAPI PFN_vkGetSemaphoreCounterValueKHR vkGetSemaphoreCounterValueKHR;
VKAPI PFN_vkSignalSemaphoreKHR vkSignalSemaphoreKHR;
VKAPI PFN_vkWaitSemaphoresKHR vkWaitSemaphoresKHR;
#endif /* defined(VK_KHR_timeline_semaphore) */
#if defined(VK_KHR_video_decode_queue)
VKAPI PFN_vkCmdDecodeVideoKHR vkCmdDecodeVideoKHR;
#endif /* defined(VK_KHR_video_decode_queue) */
#if defined(VK_KHR_video_encode_queue)
VKAPI PFN_vkCmdEncodeVideoKHR vkCmdEncodeVideoKHR;
VKAPI PFN_vkGetEncodedVideoSessionParametersKHR vkGetEncodedVideoSessionParametersKHR;
#endif /* defined(VK_KHR_video_encode_queue) */
#if defined(VK_KHR_video_queue)
VKAPI PFN_vkBindVideoSessionMemoryKHR vkBindVideoSessionMemoryKHR;
VKAPI PFN_vkCmdBeginVideoCodingKHR vkCmdBeginVideoCodingKHR;
VKAPI PFN_vkCmdControlVideoCodingKHR vkCmdControlVideoCodingKHR;
VKAPI PFN_vkCmdEndVideoCodingKHR vkCmdEndVideoCodingKHR;
VKAPI PFN_vkCreateVideoSessionKHR vkCreateVideoSessionKHR;
VKAPI PFN_vkCreateVideoSessionParametersKHR vkCreateVideoSessionParametersKHR;
VKAPI PFN_vkDestroyVideoSessionKHR vkDestroyVideoSessionKHR;
VKAPI PFN_vkDestroyVideoSessionParametersKHR vkDestroyVideoSessionParametersKHR;
VKAPI PFN_vkGetVideoSessionMemoryRequirementsKHR vkGetVideoSessionMemoryRequirementsKHR;
VKAPI PFN_vkUpdateVideoSessionParametersKHR vkUpdateVideoSessionParametersKHR;
#endif /* defined(VK_KHR_video_queue) */
#if defined(VK_NVX_binary_import)
VKAPI PFN_vkCmdCuLaunchKernelNVX vkCmdCuLaunchKernelNVX;
VKAPI PFN_vkCreateCuFunctionNVX vkCreateCuFunctionNVX;
VKAPI PFN_vkCreateCuModuleNVX vkCreateCuModuleNVX;
VKAPI PFN_vkDestroyCuFunctionNVX vkDestroyCuFunctionNVX;
VKAPI PFN_vkDestroyCuModuleNVX vkDestroyCuModuleNVX;
#endif /* defined(VK_NVX_binary_import) */
#if defined(VK_NVX_image_view_handle)
VKAPI PFN_vkGetImageViewHandleNVX vkGetImageViewHandleNVX;
#endif /* defined(VK_NVX_image_view_handle) */
#if defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 3
VKAPI PFN_vkGetImageViewHandle64NVX vkGetImageViewHandle64NVX;
#endif /* defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 3 */
#if defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 2
VKAPI PFN_vkGetImageViewAddressNVX vkGetImageViewAddressNVX;
#endif /* defined(VK_NVX_image_view_handle) && VK_NVX_IMAGE_VIEW_HANDLE_SPEC_VERSION >= 2 */
#if defined(VK_NV_clip_space_w_scaling)
VKAPI PFN_vkCmdSetViewportWScalingNV vkCmdSetViewportWScalingNV;
#endif /* defined(VK_NV_clip_space_w_scaling) */
#if defined(VK_NV_cluster_acceleration_structure)
VKAPI PFN_vkCmdBuildClusterAccelerationStructureIndirectNV vkCmdBuildClusterAccelerationStructureIndirectNV;
VKAPI PFN_vkGetClusterAccelerationStructureBuildSizesNV vkGetClusterAccelerationStructureBuildSizesNV;
#endif /* defined(VK_NV_cluster_acceleration_structure) */
#if defined(VK_NV_cooperative_vector)
VKAPI PFN_vkCmdConvertCooperativeVectorMatrixNV vkCmdConvertCooperativeVectorMatrixNV;
VKAPI PFN_vkConvertCooperativeVectorMatrixNV vkConvertCooperativeVectorMatrixNV;
#endif /* defined(VK_NV_cooperative_vector) */
#if defined(VK_NV_copy_memory_indirect)
VKAPI PFN_vkCmdCopyMemoryIndirectNV vkCmdCopyMemoryIndirectNV;
VKAPI PFN_vkCmdCopyMemoryToImageIndirectNV vkCmdCopyMemoryToImageIndirectNV;
#endif /* defined(VK_NV_copy_memory_indirect) */
#if defined(VK_NV_cuda_kernel_launch)
VKAPI PFN_vkCmdCudaLaunchKernelNV vkCmdCudaLaunchKernelNV;
VKAPI PFN_vkCreateCudaFunctionNV vkCreateCudaFunctionNV;
VKAPI PFN_vkCreateCudaModuleNV vkCreateCudaModuleNV;
VKAPI PFN_vkDestroyCudaFunctionNV vkDestroyCudaFunctionNV;
VKAPI PFN_vkDestroyCudaModuleNV vkDestroyCudaModuleNV;
VKAPI PFN_vkGetCudaModuleCacheNV vkGetCudaModuleCacheNV;
#endif /* defined(VK_NV_cuda_kernel_launch) */
#if defined(VK_NV_device_diagnostic_checkpoints)
VKAPI PFN_vkCmdSetCheckpointNV vkCmdSetCheckpointNV;
VKAPI PFN_vkGetQueueCheckpointDataNV vkGetQueueCheckpointDataNV;
#endif /* defined(VK_NV_device_diagnostic_checkpoints) */
#if defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2))
VKAPI PFN_vkGetQueueCheckpointData2NV vkGetQueueCheckpointData2NV;
#endif /* defined(VK_NV_device_diagnostic_checkpoints) && (defined(VK_VERSION_1_3) || defined(VK_KHR_synchronization2)) */
#if defined(VK_NV_device_generated_commands)
VKAPI PFN_vkCmdBindPipelineShaderGroupNV vkCmdBindPipelineShaderGroupNV;
VKAPI PFN_vkCmdExecuteGeneratedCommandsNV vkCmdExecuteGeneratedCommandsNV;
VKAPI PFN_vkCmdPreprocessGeneratedCommandsNV vkCmdPreprocessGeneratedCommandsNV;
VKAPI PFN_vkCreateIndirectCommandsLayoutNV vkCreateIndirectCommandsLayoutNV;
VKAPI PFN_vkDestroyIndirectCommandsLayoutNV vkDestroyIndirectCommandsLayoutNV;
VKAPI PFN_vkGetGeneratedCommandsMemoryRequirementsNV vkGetGeneratedCommandsMemoryRequirementsNV;
#endif /* defined(VK_NV_device_generated_commands) */
#if defined(VK_NV_device_generated_commands_compute)
VKAPI PFN_vkCmdUpdatePipelineIndirectBufferNV vkCmdUpdatePipelineIndirectBufferNV;
VKAPI PFN_vkGetPipelineIndirectDeviceAddressNV vkGetPipelineIndirectDeviceAddressNV;
VKAPI PFN_vkGetPipelineIndirectMemoryRequirementsNV vkGetPipelineIndirectMemoryRequirementsNV;
#endif /* defined(VK_NV_device_generated_commands_compute) */
#if defined(VK_NV_external_compute_queue)
VKAPI PFN_vkCreateExternalComputeQueueNV vkCreateExternalComputeQueueNV;
VKAPI PFN_vkDestroyExternalComputeQueueNV vkDestroyExternalComputeQueueNV;
VKAPI PFN_vkGetExternalComputeQueueDataNV vkGetExternalComputeQueueDataNV;
#endif /* defined(VK_NV_external_compute_queue) */
#if defined(VK_NV_external_memory_rdma)
VKAPI PFN_vkGetMemoryRemoteAddressNV vkGetMemoryRemoteAddressNV;
#endif /* defined(VK_NV_external_memory_rdma) */
#if defined(VK_NV_external_memory_win32)
VKAPI PFN_vkGetMemoryWin32HandleNV vkGetMemoryWin32HandleNV;
#endif /* defined(VK_NV_external_memory_win32) */
#if defined(VK_NV_fragment_shading_rate_enums)
VKAPI PFN_vkCmdSetFragmentShadingRateEnumNV vkCmdSetFragmentShadingRateEnumNV;
#endif /* defined(VK_NV_fragment_shading_rate_enums) */
#if defined(VK_NV_low_latency2)
VKAPI PFN_vkGetLatencyTimingsNV vkGetLatencyTimingsNV;
VKAPI PFN_vkLatencySleepNV vkLatencySleepNV;
VKAPI PFN_vkQueueNotifyOutOfBandNV vkQueueNotifyOutOfBandNV;
VKAPI PFN_vkSetLatencyMarkerNV vkSetLatencyMarkerNV;
VKAPI PFN_vkSetLatencySleepModeNV vkSetLatencySleepModeNV;
#endif /* defined(VK_NV_low_latency2) */
#if defined(VK_NV_memory_decompression)
VKAPI PFN_vkCmdDecompressMemoryIndirectCountNV vkCmdDecompressMemoryIndirectCountNV;
VKAPI PFN_vkCmdDecompressMemoryNV vkCmdDecompressMemoryNV;
#endif /* defined(VK_NV_memory_decompression) */
#if defined(VK_NV_mesh_shader)
VKAPI PFN_vkCmdDrawMeshTasksIndirectNV vkCmdDrawMeshTasksIndirectNV;
VKAPI PFN_vkCmdDrawMeshTasksNV vkCmdDrawMeshTasksNV;
#endif /* defined(VK_NV_mesh_shader) */
#if defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count))
VKAPI PFN_vkCmdDrawMeshTasksIndirectCountNV vkCmdDrawMeshTasksIndirectCountNV;
#endif /* defined(VK_NV_mesh_shader) && (defined(VK_VERSION_1_2) || defined(VK_KHR_draw_indirect_count) || defined(VK_AMD_draw_indirect_count)) */
#if defined(VK_NV_optical_flow)
VKAPI PFN_vkBindOpticalFlowSessionImageNV vkBindOpticalFlowSessionImageNV;
VKAPI PFN_vkCmdOpticalFlowExecuteNV vkCmdOpticalFlowExecuteNV;
VKAPI PFN_vkCreateOpticalFlowSessionNV vkCreateOpticalFlowSessionNV;
VKAPI PFN_vkDestroyOpticalFlowSessionNV vkDestroyOpticalFlowSessionNV;
#endif /* defined(VK_NV_optical_flow) */
#if defined(VK_NV_partitioned_acceleration_structure)
VKAPI PFN_vkCmdBuildPartitionedAccelerationStructuresNV vkCmdBuildPartitionedAccelerationStructuresNV;
VKAPI PFN_vkGetPartitionedAccelerationStructuresBuildSizesNV vkGetPartitionedAccelerationStructuresBuildSizesNV;
#endif /* defined(VK_NV_partitioned_acceleration_structure) */
#if defined(VK_NV_ray_tracing)
VKAPI PFN_vkBindAccelerationStructureMemoryNV vkBindAccelerationStructureMemoryNV;
VKAPI PFN_vkCmdBuildAccelerationStructureNV vkCmdBuildAccelerationStructureNV;
VKAPI PFN_vkCmdCopyAccelerationStructureNV vkCmdCopyAccelerationStructureNV;
VKAPI PFN_vkCmdTraceRaysNV vkCmdTraceRaysNV;
VKAPI PFN_vkCmdWriteAccelerationStructuresPropertiesNV vkCmdWriteAccelerationStructuresPropertiesNV;
VKAPI PFN_vkCompileDeferredNV vkCompileDeferredNV;
VKAPI PFN_vkCreateAccelerationStructureNV vkCreateAccelerationStructureNV;
VKAPI PFN_vkCreateRayTracingPipelinesNV vkCreateRayTracingPipelinesNV;
VKAPI PFN_vkDestroyAccelerationStructureNV vkDestroyAccelerationStructureNV;
VKAPI PFN_vkGetAccelerationStructureHandleNV vkGetAccelerationStructureHandleNV;
VKAPI PFN_vkGetAccelerationStructureMemoryRequirementsNV vkGetAccelerationStructureMemoryRequirementsNV;
VKAPI PFN_vkGetRayTracingShaderGroupHandlesNV vkGetRayTracingShaderGroupHandlesNV;
#endif /* defined(VK_NV_ray_tracing) */
#if defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2
VKAPI PFN_vkCmdSetExclusiveScissorEnableNV vkCmdSetExclusiveScissorEnableNV;
#endif /* defined(VK_NV_scissor_exclusive) && VK_NV_SCISSOR_EXCLUSIVE_SPEC_VERSION >= 2 */
#if defined(VK_NV_scissor_exclusive)
VKAPI PFN_vkCmdSetExclusiveScissorNV vkCmdSetExclusiveScissorNV;
#endif /* defined(VK_NV_scissor_exclusive) */
#if defined(VK_NV_shading_rate_image)
VKAPI PFN_vkCmdBindShadingRateImageNV vkCmdBindShadingRateImageNV;
VKAPI PFN_vkCmdSetCoarseSampleOrderNV vkCmdSetCoarseSampleOrderNV;
VKAPI PFN_vkCmdSetViewportShadingRatePaletteNV vkCmdSetViewportShadingRatePaletteNV;
#endif /* defined(VK_NV_shading_rate_image) */
#if defined(VK_OHOS_external_memory)
VKAPI PFN_vkGetMemoryNativeBufferOHOS vkGetMemoryNativeBufferOHOS;
VKAPI PFN_vkGetNativeBufferPropertiesOHOS vkGetNativeBufferPropertiesOHOS;
#endif /* defined(VK_OHOS_external_memory) */
#if defined(VK_OHOS_native_buffer)
VKAPI PFN_vkAcquireImageOHOS vkAcquireImageOHOS;
VKAPI PFN_vkGetSwapchainGrallocUsageOHOS vkGetSwapchainGrallocUsageOHOS;
VKAPI PFN_vkQueueSignalReleaseImageOHOS vkQueueSignalReleaseImageOHOS;
#endif /* defined(VK_OHOS_native_buffer) */
#if defined(VK_QCOM_tile_memory_heap)
VKAPI PFN_vkCmdBindTileMemoryQCOM vkCmdBindTileMemoryQCOM;
#endif /* defined(VK_QCOM_tile_memory_heap) */
#if defined(VK_QCOM_tile_properties)
VKAPI PFN_vkGetDynamicRenderingTilePropertiesQCOM vkGetDynamicRenderingTilePropertiesQCOM;
VKAPI PFN_vkGetFramebufferTilePropertiesQCOM vkGetFramebufferTilePropertiesQCOM;
#endif /* defined(VK_QCOM_tile_properties) */
#if defined(VK_QCOM_tile_shading)
VKAPI PFN_vkCmdBeginPerTileExecutionQCOM vkCmdBeginPerTileExecutionQCOM;
VKAPI PFN_vkCmdDispatchTileQCOM vkCmdDispatchTileQCOM;
VKAPI PFN_vkCmdEndPerTileExecutionQCOM vkCmdEndPerTileExecutionQCOM;
#endif /* defined(VK_QCOM_tile_shading) */
#if defined(VK_QNX_external_memory_screen_buffer)
VKAPI PFN_vkGetScreenBufferPropertiesQNX vkGetScreenBufferPropertiesQNX;
#endif /* defined(VK_QNX_external_memory_screen_buffer) */
#if defined(VK_VALVE_descriptor_set_host_mapping)
VKAPI PFN_vkGetDescriptorSetHostMappingVALVE vkGetDescriptorSetHostMappingVALVE;
VKAPI PFN_vkGetDescriptorSetLayoutHostMappingInfoVALVE vkGetDescriptorSetLayoutHostMappingInfoVALVE;
#endif /* defined(VK_VALVE_descriptor_set_host_mapping) */
#if (defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control))
VKAPI PFN_vkCmdSetDepthClampRangeEXT vkCmdSetDepthClampRangeEXT;
#endif /* (defined(VK_EXT_depth_clamp_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clamp_control)) */
#if (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object))
VKAPI PFN_vkCmdBindVertexBuffers2EXT vkCmdBindVertexBuffers2EXT;
VKAPI PFN_vkCmdSetCullModeEXT vkCmdSetCullModeEXT;
VKAPI PFN_vkCmdSetDepthBoundsTestEnableEXT vkCmdSetDepthBoundsTestEnableEXT;
VKAPI PFN_vkCmdSetDepthCompareOpEXT vkCmdSetDepthCompareOpEXT;
VKAPI PFN_vkCmdSetDepthTestEnableEXT vkCmdSetDepthTestEnableEXT;
VKAPI PFN_vkCmdSetDepthWriteEnableEXT vkCmdSetDepthWriteEnableEXT;
VKAPI PFN_vkCmdSetFrontFaceEXT vkCmdSetFrontFaceEXT;
VKAPI PFN_vkCmdSetPrimitiveTopologyEXT vkCmdSetPrimitiveTopologyEXT;
VKAPI PFN_vkCmdSetScissorWithCountEXT vkCmdSetScissorWithCountEXT;
VKAPI PFN_vkCmdSetStencilOpEXT vkCmdSetStencilOpEXT;
VKAPI PFN_vkCmdSetStencilTestEnableEXT vkCmdSetStencilTestEnableEXT;
VKAPI PFN_vkCmdSetViewportWithCountEXT vkCmdSetViewportWithCountEXT;
#endif /* (defined(VK_EXT_extended_dynamic_state)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object))
VKAPI PFN_vkCmdSetDepthBiasEnableEXT vkCmdSetDepthBiasEnableEXT;
VKAPI PFN_vkCmdSetLogicOpEXT vkCmdSetLogicOpEXT;
VKAPI PFN_vkCmdSetPatchControlPointsEXT vkCmdSetPatchControlPointsEXT;
VKAPI PFN_vkCmdSetPrimitiveRestartEnableEXT vkCmdSetPrimitiveRestartEnableEXT;
VKAPI PFN_vkCmdSetRasterizerDiscardEnableEXT vkCmdSetRasterizerDiscardEnableEXT;
#endif /* (defined(VK_EXT_extended_dynamic_state2)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object))
VKAPI PFN_vkCmdSetAlphaToCoverageEnableEXT vkCmdSetAlphaToCoverageEnableEXT;
VKAPI PFN_vkCmdSetAlphaToOneEnableEXT vkCmdSetAlphaToOneEnableEXT;
VKAPI PFN_vkCmdSetColorBlendEnableEXT vkCmdSetColorBlendEnableEXT;
VKAPI PFN_vkCmdSetColorBlendEquationEXT vkCmdSetColorBlendEquationEXT;
VKAPI PFN_vkCmdSetColorWriteMaskEXT vkCmdSetColorWriteMaskEXT;
VKAPI PFN_vkCmdSetDepthClampEnableEXT vkCmdSetDepthClampEnableEXT;
VKAPI PFN_vkCmdSetLogicOpEnableEXT vkCmdSetLogicOpEnableEXT;
VKAPI PFN_vkCmdSetPolygonModeEXT vkCmdSetPolygonModeEXT;
VKAPI PFN_vkCmdSetRasterizationSamplesEXT vkCmdSetRasterizationSamplesEXT;
VKAPI PFN_vkCmdSetSampleMaskEXT vkCmdSetSampleMaskEXT;
#endif /* (defined(VK_EXT_extended_dynamic_state3)) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object))
VKAPI PFN_vkCmdSetTessellationDomainOriginEXT vkCmdSetTessellationDomainOriginEXT;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && (defined(VK_KHR_maintenance2) || defined(VK_VERSION_1_1))) || (defined(VK_EXT_shader_object)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback))
VKAPI PFN_vkCmdSetRasterizationStreamEXT vkCmdSetRasterizationStreamEXT;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_transform_feedback)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_transform_feedback)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization))
VKAPI PFN_vkCmdSetConservativeRasterizationModeEXT vkCmdSetConservativeRasterizationModeEXT;
VKAPI PFN_vkCmdSetExtraPrimitiveOverestimationSizeEXT vkCmdSetExtraPrimitiveOverestimationSizeEXT;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_conservative_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_conservative_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable))
VKAPI PFN_vkCmdSetDepthClipEnableEXT vkCmdSetDepthClipEnableEXT;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_enable)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_enable)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations))
VKAPI PFN_vkCmdSetSampleLocationsEnableEXT vkCmdSetSampleLocationsEnableEXT;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_sample_locations)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_sample_locations)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced))
VKAPI PFN_vkCmdSetColorBlendAdvancedEXT vkCmdSetColorBlendAdvancedEXT;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_blend_operation_advanced)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_blend_operation_advanced)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex))
VKAPI PFN_vkCmdSetProvokingVertexModeEXT vkCmdSetProvokingVertexModeEXT;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_provoking_vertex)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_provoking_vertex)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization))
VKAPI PFN_vkCmdSetLineRasterizationModeEXT vkCmdSetLineRasterizationModeEXT;
VKAPI PFN_vkCmdSetLineStippleEnableEXT vkCmdSetLineStippleEnableEXT;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_line_rasterization)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_line_rasterization)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control))
VKAPI PFN_vkCmdSetDepthClipNegativeOneToOneEXT vkCmdSetDepthClipNegativeOneToOneEXT;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_EXT_depth_clip_control)) || (defined(VK_EXT_shader_object) && defined(VK_EXT_depth_clip_control)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling))
VKAPI PFN_vkCmdSetViewportWScalingEnableNV vkCmdSetViewportWScalingEnableNV;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_clip_space_w_scaling)) || (defined(VK_EXT_shader_object) && defined(VK_NV_clip_space_w_scaling)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle))
VKAPI PFN_vkCmdSetViewportSwizzleNV vkCmdSetViewportSwizzleNV;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_viewport_swizzle)) || (defined(VK_EXT_shader_object) && defined(VK_NV_viewport_swizzle)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color))
VKAPI PFN_vkCmdSetCoverageToColorEnableNV vkCmdSetCoverageToColorEnableNV;
VKAPI PFN_vkCmdSetCoverageToColorLocationNV vkCmdSetCoverageToColorLocationNV;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_fragment_coverage_to_color)) || (defined(VK_EXT_shader_object) && defined(VK_NV_fragment_coverage_to_color)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples))
VKAPI PFN_vkCmdSetCoverageModulationModeNV vkCmdSetCoverageModulationModeNV;
VKAPI PFN_vkCmdSetCoverageModulationTableEnableNV vkCmdSetCoverageModulationTableEnableNV;
VKAPI PFN_vkCmdSetCoverageModulationTableNV vkCmdSetCoverageModulationTableNV;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_framebuffer_mixed_samples)) || (defined(VK_EXT_shader_object) && defined(VK_NV_framebuffer_mixed_samples)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image))
VKAPI PFN_vkCmdSetShadingRateImageEnableNV vkCmdSetShadingRateImageEnableNV;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_shading_rate_image)) || (defined(VK_EXT_shader_object) && defined(VK_NV_shading_rate_image)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test))
VKAPI PFN_vkCmdSetRepresentativeFragmentTestEnableNV vkCmdSetRepresentativeFragmentTestEnableNV;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_representative_fragment_test)) || (defined(VK_EXT_shader_object) && defined(VK_NV_representative_fragment_test)) */
#if (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode))
VKAPI PFN_vkCmdSetCoverageReductionModeNV vkCmdSetCoverageReductionModeNV;
#endif /* (defined(VK_EXT_extended_dynamic_state3) && defined(VK_NV_coverage_reduction_mode)) || (defined(VK_EXT_shader_object) && defined(VK_NV_coverage_reduction_mode)) */
#if (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control))
VKAPI PFN_vkGetImageSubresourceLayout2EXT vkGetImageSubresourceLayout2EXT;
#endif /* (defined(VK_EXT_host_image_copy)) || (defined(VK_EXT_image_compression_control)) */
#if (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state))
VKAPI PFN_vkCmdSetVertexInputEXT vkCmdSetVertexInputEXT;
#endif /* (defined(VK_EXT_shader_object)) || (defined(VK_EXT_vertex_input_dynamic_state)) */
#if (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && (defined(VK_VERSION_1_1) || defined(VK_KHR_descriptor_update_template)))
VKAPI PFN_vkCmdPushDescriptorSetWithTemplateKHR vkCmdPushDescriptorSetWithTemplateKHR;
#endif /* (defined(VK_KHR_descriptor_update_template) && defined(VK_KHR_push_descriptor)) || (defined(VK_KHR_push_descriptor) && (defined(VK_VERSION_1_1) || defined(VK_KHR_descriptor_update_template))) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
VKAPI PFN_vkGetDeviceGroupPresentCapabilitiesKHR vkGetDeviceGroupPresentCapabilitiesKHR;
VKAPI PFN_vkGetDeviceGroupSurfacePresentModesKHR vkGetDeviceGroupSurfacePresentModesKHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_surface)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
#if (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1))
VKAPI PFN_vkAcquireNextImage2KHR vkAcquireNextImage2KHR;
#endif /* (defined(VK_KHR_device_group) && defined(VK_KHR_swapchain)) || (defined(VK_KHR_swapchain) && defined(VK_VERSION_1_1)) */
/* VOLK_GENERATE_PROTOTYPES_H_DEVICE */
#endif

#ifdef __cplusplus
} // extern "C" / namespace volk
#endif

#ifdef VOLK_NAMESPACE
using namespace volk;
#endif

#endif // VOLK_H

#ifdef VOLK_IMPLEMENTATION
#undef VOLK_IMPLEMENTATION
/* Prevent tools like dependency checkers from detecting a cyclic dependency */
#define VOLK_SOURCE "volk.c"
#include VOLK_SOURCE
#endif

/**
 * Copyright (c) 2018-2025 Arseny Kapoulkine
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/
/* clang-format on */
