// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_COMPOSITOR_LAYER_H_
#define UI_COMPOSITOR_LAYER_H_

#include <string>
#include <vector>

#include "base/compiler_specific.h"
#include "base/memory/ref_counted.h"
#include "base/memory/scoped_ptr.h"
#include "base/message_loop.h"
#include "cc/animation/animation_events.h"
#include "cc/animation/layer_animation_event_observer.h"
#include "cc/base/scoped_ptr_vector.h"
#include "cc/layers/content_layer_client.h"
#include "cc/layers/texture_layer_client.h"
#include "third_party/skia/include/core/SkColor.h"
#include "third_party/skia/include/core/SkRegion.h"
#include "ui/compositor/compositor.h"
#include "ui/compositor/layer_animation_delegate.h"
#include "ui/compositor/layer_delegate.h"
#include "ui/compositor/layer_type.h"
#include "ui/gfx/rect.h"
#include "ui/gfx/transform.h"

class SkCanvas;

namespace cc {
class ContentLayer;
class DelegatedFrameData;
class DelegatedRendererLayer;
class Layer;
class ResourceUpdateQueue;
class SolidColorLayer;
class TextureLayer;
struct TransferableResource;
typedef std::vector<TransferableResource> TransferableResourceArray;
}

namespace ui {

class Compositor;
class LayerAnimator;
class Texture;

// Layer manages a texture, transform and a set of child Layers. Any View that
// has enabled layers ends up creating a Layer to manage the texture.
// A Layer can also be created without a texture, in which case it renders
// nothing and is simply used as a node in a hierarchy of layers.
// Coordinate system used in layers is DIP (Density Independent Pixel)
// coordinates unless explicitly mentioned as pixel coordinates.
//
// NOTE: unlike Views, each Layer does *not* own its children views. If you
// delete a Layer and it has children, the parent of each child layer is set to
// NULL, but the children are not deleted.
class COMPOSITOR_EXPORT Layer
    : public LayerAnimationDelegate,
      NON_EXPORTED_BASE(public cc::ContentLayerClient),
      NON_EXPORTED_BASE(public cc::TextureLayerClient),
      NON_EXPORTED_BASE(public cc::LayerAnimationEventObserver) {
 public:
  Layer();
  explicit Layer(LayerType type);
  virtual ~Layer();

  // Retrieves the Layer's compositor. The Layer will walk up its parent chain
  // to locate it. Returns NULL if the Layer is not attached to a compositor.
  Compositor* GetCompositor();

  // Called by the compositor when the Layer is set as its root Layer. This can
  // only ever be called on the root layer.
  void SetCompositor(Compositor* compositor);

  LayerDelegate* delegate() { return delegate_; }
  void set_delegate(LayerDelegate* delegate) { delegate_ = delegate; }

  // Adds a new Layer to this Layer.
  void Add(Layer* child);

  // Removes a Layer from this Layer.
  void Remove(Layer* child);

  // Stacks |child| above all other children.
  void StackAtTop(Layer* child);

  // Stacks |child| directly above |other|.  Both must be children of this
  // layer.  Note that if |child| is initially stacked even higher, calling this
  // method will result in |child| being lowered in the stacking order.
  void StackAbove(Layer* child, Layer* other);

  // Stacks |child| below all other children.
  void StackAtBottom(Layer* child);

  // Stacks |child| directly below |other|.  Both must be children of this
  // layer.
  void StackBelow(Layer* child, Layer* other);

  // Returns the child Layers.
  const std::vector<Layer*>& children() const { return children_; }

  // The parent.
  const Layer* parent() const { return parent_; }
  Layer* parent() { return parent_; }

  LayerType type() const { return type_; }

  // Returns true if this Layer contains |other| somewhere in its children.
  bool Contains(const Layer* other) const;

  // The layer's animator is responsible for causing automatic animations when
  // properties are set. It also manages a queue of pending animations and
  // handles blending of animations. The layer takes ownership of the animator.
  void SetAnimator(LayerAnimator* animator);

  // Returns the layer's animator. Creates a default animator of one has not
  // been set. Will not return NULL.
  LayerAnimator* GetAnimator();

  // The transform, relative to the parent.
  void SetTransform(const gfx::Transform& transform);
  gfx::Transform transform() const;

  // Return the target transform if animator is running, or the current
  // transform otherwise.
  gfx::Transform GetTargetTransform() const;

  // The bounds, relative to the parent.
  void SetBounds(const gfx::Rect& bounds);
  const gfx::Rect& bounds() const { return bounds_; }

  // Return the target bounds if animator is running, or the current bounds
  // otherwise.
  gfx::Rect GetTargetBounds() const;

  // Sets/gets whether or not drawing of child layers should be clipped to the
  // bounds of this layer.
  void SetMasksToBounds(bool masks_to_bounds);
  bool GetMasksToBounds() const;

  // The opacity of the layer. The opacity is applied to each pixel of the
  // texture (resulting alpha = opacity * alpha).
  float opacity() const;
  void SetOpacity(float opacity);

  // Returns the actual opacity, which the opacity of this layer multipled by
  // the combined opacity of the parent.
  float GetCombinedOpacity() const;

  // Blur pixels by this amount in anything below the layer and visible through
  // the layer.
  int background_blur() const { return background_blur_radius_; }
  void SetBackgroundBlur(int blur_radius);

  // Saturate all pixels of this layer by this amount.
  // This effect will get "combined" with the inverted,
  // brightness and grayscale setting.
  float layer_saturation() const { return layer_saturation_; }
  void SetLayerSaturation(float saturation);

  // Change the brightness of all pixels from this layer by this amount.
  // This effect will get "combined" with the inverted, saturate
  // and grayscale setting.
  float layer_brightness() const { return layer_brightness_; }
  void SetLayerBrightness(float brightness);

  // Return the target brightness if animator is running, or the current
  // brightness otherwise.
  float GetTargetBrightness() const;

  // Change the grayscale of all pixels from this layer by this amount.
  // This effect will get "combined" with the inverted, saturate
  // and brightness setting.
  float layer_grayscale() const { return layer_grayscale_; }
  void SetLayerGrayscale(float grayscale);

  // Return the target grayscale if animator is running, or the current
  // grayscale otherwise.
  float GetTargetGrayscale() const;

  // Zoom the background by a factor of |zoom|. The effect is blended along the
  // edge across |inset| pixels.
  void SetBackgroundZoom(float zoom, int inset);

  // Invert the layer.
  bool layer_inverted() const { return layer_inverted_; }
  void SetLayerInverted(bool inverted);

  // Return the target opacity if animator is running, or the current opacity
  // otherwise.
  float GetTargetOpacity() const;

  // Set a layer mask for a layer.
  // Note the provided layer mask can neither have a layer mask itself nor can
  // it have any children. The ownership of |layer_mask| will not be
  // transferred with this call.
  // Furthermore: A mask layer can only be set to one layer.
  void SetMaskLayer(Layer* layer_mask);
  Layer* layer_mask_layer() { return layer_mask_; }

  // Sets the visibility of the Layer. A Layer may be visible but not
  // drawn. This happens if any ancestor of a Layer is not visible.
  void SetVisible(bool visible);
  bool visible() const { return visible_; }

  // Returns the target visibility if the animator is running. Otherwise, it
  // returns the current visibility.
  bool GetTargetVisibility() const;

  // Returns true if this Layer is drawn. A Layer is drawn only if all ancestors
  // are visible.
  bool IsDrawn() const;

  // Returns true if this layer can have a texture (has_texture_ is true)
  // and is not completely obscured by a child.
  bool ShouldDraw() const;

  // Converts a point from the coordinates of |source| to the coordinates of
  // |target|. Necessarily, |source| and |target| must inhabit the same Layer
  // tree.
  static void ConvertPointToLayer(const Layer* source,
                                  const Layer* target,
                                  gfx::Point* point);

  // Converts a transform to be relative to the given |ancestor|. Returns
  // whether success (that is, whether the given ancestor was really an
  // ancestor of this layer).
  bool GetTargetTransformRelativeTo(const Layer* ancestor,
                                    gfx::Transform* transform) const;

  // Converts a ui::Layer's transform to the transform on the corresponding
  // cc::Layer.
  static gfx::Transform ConvertTransformToCCTransform(
      const gfx::Transform& transform,
      float device_scale_factor);

  // See description in View for details
  void SetFillsBoundsOpaquely(bool fills_bounds_opaquely);
  bool fills_bounds_opaquely() const { return fills_bounds_opaquely_; }

  const std::string& name() const { return name_; }
  void set_name(const std::string& name) { name_ = name; }

  const ui::Texture* texture() const { return texture_.get(); }

  // Assigns a new external texture.  |texture| can be NULL to disable external
  // updates.
  void SetExternalTexture(ui::Texture* texture);
  ui::Texture* external_texture() { return texture_.get(); }

  // Sets a delegated frame, coming from a child compositor.
  void SetDelegatedFrame(scoped_ptr<cc::DelegatedFrameData> frame,
                         gfx::Size frame_size_in_dip);

  // Gets unused resources to recycle to the child compositor.
  void TakeUnusedResourcesForChildCompositor(
      cc::TransferableResourceArray* array);

  // Sets the layer's fill color.  May only be called for LAYER_SOLID_COLOR.
  void SetColor(SkColor color);

  // Adds |invalid_rect| to the Layer's pending invalid rect and calls
  // ScheduleDraw(). Returns false if the paint request is ignored.
  bool SchedulePaint(const gfx::Rect& invalid_rect);

  // Schedules a redraw of the layer tree at the compositor.
  // Note that this _does not_ invalidate any region of this layer; use
  // SchedulePaint() for that.
  void ScheduleDraw();

  // Sends damaged rectangles recorded in |damaged_region_| to
  // |compostior_| to repaint the content.
  void SendDamagedRects();

  // Suppresses painting the content by disgarding damaged region and ignoring
  // new paint requests.
  void SuppressPaint();

  // Notifies the layer that the device scale factor has changed.
  void OnDeviceScaleFactorChanged(float device_scale_factor);

  // Sets whether the layer should scale its content. If true, the canvas will
  // be scaled in software rendering mode before it is passed to
  // |LayerDelegate::OnPaint|.
  // Set to false if the delegate handles scaling.
  // NOTE: if this is called during |LayerDelegate::OnPaint|, the new value will
  // not apply to the canvas passed to the pending draw.
  void set_scale_content(bool scale_content) { scale_content_ = scale_content; }

  // Returns true if the layer scales its content.
  bool scale_content() const { return scale_content_; }

  // Sometimes the Layer is being updated by something other than SetCanvas
  // (e.g. the GPU process on UI_COMPOSITOR_IMAGE_TRANSPORT).
  bool layer_updated_externally() const { return layer_updated_externally_; }

  // ContentLayerClient
  virtual void PaintContents(
      SkCanvas* canvas, gfx::Rect clip, gfx::RectF* opaque) OVERRIDE;
  virtual void DidChangeLayerCanUseLCDText() OVERRIDE {}

  cc::Layer* cc_layer() { return cc_layer_; }

  // TextureLayerClient
  virtual unsigned PrepareTexture(cc::ResourceUpdateQueue* queue) OVERRIDE;
  virtual WebKit::WebGraphicsContext3D* Context3d() OVERRIDE;
  virtual bool PrepareTextureMailbox(cc::TextureMailbox* mailbox) OVERRIDE;

  float device_scale_factor() const { return device_scale_factor_; }

  // Forces a render surface to be used on this layer. This has no positive
  // impact, and is only used for benchmarking/testing purpose.
  void SetForceRenderSurface(bool force);
  bool force_render_surface() const { return force_render_surface_; }

  // LayerAnimationEventObserver
  virtual void OnAnimationStarted(const cc::AnimationEvent& event) OVERRIDE;

  // Whether this layer has animations waiting to get sent to its cc::Layer.
  bool HasPendingThreadedAnimations() {
    return pending_threaded_animations_.size() != 0;
  }

  // Triggers a call to SwitchToLayer.
  void SwitchCCLayerForTest();

 private:
  // Stacks |child| above or below |other|.  Helper method for StackAbove() and
  // StackBelow().
  void StackRelativeTo(Layer* child, Layer* other, bool above);

  bool ConvertPointForAncestor(const Layer* ancestor, gfx::Point* point) const;
  bool ConvertPointFromAncestor(const Layer* ancestor, gfx::Point* point) const;

  // Following are invoked from the animation or if no animation exists to
  // update the values immediately.
  void SetBoundsImmediately(const gfx::Rect& bounds);
  void SetTransformImmediately(const gfx::Transform& transform);
  void SetOpacityImmediately(float opacity);
  void SetVisibilityImmediately(bool visibility);
  void SetBrightnessImmediately(float brightness);
  void SetGrayscaleImmediately(float grayscale);
  void SetColorImmediately(SkColor color);

  // Implementation of LayerAnimatorDelegate
  virtual void SetBoundsFromAnimation(const gfx::Rect& bounds) OVERRIDE;
  virtual void SetTransformFromAnimation(
      const gfx::Transform& transform) OVERRIDE;
  virtual void SetOpacityFromAnimation(float opacity) OVERRIDE;
  virtual void SetVisibilityFromAnimation(bool visibility) OVERRIDE;
  virtual void SetBrightnessFromAnimation(float brightness) OVERRIDE;
  virtual void SetGrayscaleFromAnimation(float grayscale) OVERRIDE;
  virtual void SetColorFromAnimation(SkColor color) OVERRIDE;
  virtual void ScheduleDrawForAnimation() OVERRIDE;
  virtual const gfx::Rect& GetBoundsForAnimation() const OVERRIDE;
  virtual gfx::Transform GetTransformForAnimation() const OVERRIDE;
  virtual float GetOpacityForAnimation() const OVERRIDE;
  virtual bool GetVisibilityForAnimation() const OVERRIDE;
  virtual float GetBrightnessForAnimation() const OVERRIDE;
  virtual float GetGrayscaleForAnimation() const OVERRIDE;
  virtual SkColor GetColorForAnimation() const OVERRIDE;
  virtual float GetDeviceScaleFactor() const OVERRIDE;
  virtual void AddThreadedAnimation(
      scoped_ptr<cc::Animation> animation) OVERRIDE;
  virtual void RemoveThreadedAnimation(int animation_id) OVERRIDE;

  void CreateWebLayer();
  void RecomputeCCTransformFromTransform(const gfx::Transform& transform);
  void RecomputeDrawsContentAndUVRect();
  void RecomputePosition();

  // Set all filters which got applied to the layer.
  void SetLayerFilters();

  // Set all filters which got applied to the layer background.
  void SetLayerBackgroundFilters();

  void UpdateIsDrawn();

  void SwitchToLayer(scoped_refptr<cc::Layer> new_layer);

  // We cannot send animations to our cc_layer_ until we have been added to a
  // layer tree. Instead, we hold on to these animations in
  // pending_threaded_animations_, and expect SendPendingThreadedAnimations to
  // be called once we have been added to a tree.
  void SendPendingThreadedAnimations();

  const LayerType type_;

  Compositor* compositor_;

  scoped_refptr<ui::Texture> texture_;

  Layer* parent_;

  // This layer's children, in bottom-to-top stacking order.
  std::vector<Layer*> children_;

  gfx::Rect bounds_;

  // Visibility of this layer. See SetVisible/IsDrawn for more details.
  bool visible_;

  // Computed based on the visibility of this layer and its ancestors.
  bool is_drawn_;

  bool force_render_surface_;

  bool fills_bounds_opaquely_;

  // If true the layer is always up to date.
  bool layer_updated_externally_;

  // Union of damaged rects, in pixel coordinates, to be used when
  // compositor is ready to paint the content.
  SkRegion damaged_region_;

  int background_blur_radius_;

  // Several variables which will change the visible representation of
  // the layer.
  float layer_saturation_;
  float layer_brightness_;
  float layer_grayscale_;
  bool layer_inverted_;

  // The associated mask layer with this layer.
  Layer* layer_mask_;
  // The back link from the mask layer to it's associated masked layer.
  // We keep this reference for the case that if the mask layer gets deleted
  // while attached to the main layer before the main layer is deleted.
  Layer* layer_mask_back_link_;

  // The zoom factor to scale the layer by.  Zooming is disabled when this is
  // set to 1.
  float zoom_;

  // Width of the border in pixels, where the scaling is blended.
  int zoom_inset_;

  std::string name_;

  LayerDelegate* delegate_;

  scoped_refptr<LayerAnimator> animator_;

  // Animations that are passed to AddThreadedAnimation before this layer is
  // added to a tree.
  cc::ScopedPtrVector<cc::Animation> pending_threaded_animations_;

  // Ownership of the layer is held through one of the strongly typed layer
  // pointers, depending on which sort of layer this is.
  scoped_refptr<cc::ContentLayer> content_layer_;
  scoped_refptr<cc::TextureLayer> texture_layer_;
  scoped_refptr<cc::SolidColorLayer> solid_color_layer_;
  scoped_refptr<cc::DelegatedRendererLayer> delegated_renderer_layer_;
  cc::Layer* cc_layer_;

  // If true, the layer scales the canvas and the texture with the device scale
  // factor as appropriate. When true, the texture size is in DIP.
  bool scale_content_;

  // A cached copy of |Compositor::device_scale_factor()|.
  float device_scale_factor_;

  // The size of the delegated frame in DIP, set when SetDelegatedFrame was
  // called.
  gfx::Size delegated_frame_size_in_dip_;

  DISALLOW_COPY_AND_ASSIGN(Layer);
};

}  // namespace ui

#endif  // UI_COMPOSITOR_LAYER_H_
