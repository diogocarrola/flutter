// Copyright 2013 The Flutter Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef FLUTTER_SHELL_PLATFORM_ANDROID_CONTEXT_ANDROID_CONTEXT_H_
#define FLUTTER_SHELL_PLATFORM_ANDROID_CONTEXT_ANDROID_CONTEXT_H_

#include "flutter/common/macros.h"
#include "flutter/fml/macros.h"
#include "flutter/impeller/base/flags.h"
#include "flutter/impeller/renderer/context.h"
#include "flutter/shell/platform/android/android_rendering_selector.h"

#if !SLIMPELLER
#include "third_party/skia/include/gpu/ganesh/GrDirectContext.h"
#else
#include "third_party/skia/include/core/SkRefCnt.h"
class GrDirectContext;
#endif  // !SLIMPELLER

namespace flutter {

//------------------------------------------------------------------------------
/// @brief      Holds state that is shared across Android surfaces.
///
class AndroidContext {
 public:
  explicit AndroidContext(AndroidRenderingAPI rendering_api);

  virtual ~AndroidContext();

  struct ContextSettings {
    bool enable_validation = false;
    bool enable_gpu_tracing = false;
    bool enable_surface_control = false;
    bool quiet = false;
    impeller::Flags impeller_flags;
  };

  virtual AndroidRenderingAPI RenderingApi() const;

  virtual bool IsDynamicSelection() const;

  virtual bool IsValid() const;

  //----------------------------------------------------------------------------
  /// @brief      Setter for the Skia context to be used by subsequent
  ///             AndroidSurfaces.
  /// @details    This is useful to reduce memory consumption when creating
  ///             multiple AndroidSurfaces for the same AndroidContext.
  ///
  ///             The first AndroidSurface should set this for the
  ///             AndroidContext if the AndroidContext does not yet have a
  ///             Skia context to share via GetMainSkiaContext.
  ///
  void SetMainSkiaContext(const sk_sp<GrDirectContext>& main_context);

  //----------------------------------------------------------------------------
  /// @brief      Accessor for the Skia context associated with AndroidSurfaces
  ///             and the raster thread.
  /// @details    This context is created lazily by the AndroidSurface based
  ///             on their respective rendering backend and set on this
  ///             AndroidContext to share via SetMainSkiaContext.
  /// @returns    `nullptr` when no Skia context has been set yet by its
  ///             AndroidSurface via SetMainSkiaContext.
  /// @attention  The software context doesn't have a Skia context, so this
  ///             value will be nullptr.
  ///
  sk_sp<GrDirectContext> GetMainSkiaContext() const;

  //----------------------------------------------------------------------------
  /// @brief      Accessor for the Impeller context associated with
  ///             AndroidSurfaces and the raster thread.
  ///
  virtual std::shared_ptr<impeller::Context> GetImpellerContext() const;

  //----------------------------------------------------------------------------
  /// @brief      Perform deferred setup for the impeller Context.
  ///
  virtual void SetupImpellerContext() {}

 protected:
  void SetImpellerContext(
      const std::shared_ptr<impeller::Context>& impeller_context);

 private:
  const AndroidRenderingAPI rendering_api_;

  // This is the Skia context used for on-screen rendering.
  NOT_SLIMPELLER(sk_sp<GrDirectContext> main_context_);
  std::shared_ptr<impeller::Context> impeller_context_;

  FML_DISALLOW_COPY_AND_ASSIGN(AndroidContext);
};

}  // namespace flutter

#endif  // FLUTTER_SHELL_PLATFORM_ANDROID_CONTEXT_ANDROID_CONTEXT_H_
