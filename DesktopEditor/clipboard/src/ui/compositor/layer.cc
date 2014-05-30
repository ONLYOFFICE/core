// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "ui/compositor/layer.h"

#include <algorithm>

#include "base/command_line.h"
#include "base/debug/trace_event.h"
#include "base/logging.h"
#include "base/memory/scoped_ptr.h"
#include "cc/base/scoped_ptr_algorithm.h"
#include "cc/layers/content_layer.h"
#include "cc/layers/delegated_renderer_layer.h"
#include "cc/layers/solid_color_layer.h"
#include "cc/layers/texture_layer.h"
#include "cc/output/delegated_frame_data.h"
#include "cc/resources/transferable_resource.h"
#include "third_party/WebKit/Source/Platform/chromium/public/WebFilterOperation.h"
#include "third_party/WebKit/Source/Platform/chromium/public/WebFilterOperations.h"
#include "ui/base/animation/animation.h"
#include "ui/compositor/compositor_switches.h"
#include "ui/compositor/dip_util.h"
#include "ui/compositor/layer_animator.h"
#include "ui/gfx/canvas.h"
#include "ui/gfx/display.h"
#include "ui/gfx/interpolated_transform.h"
#include "ui/gfx/point3_f.h"
#include "ui/gfx/point_conversions.h"
#include "ui/gfx/size_conversions.h"

namespace {

const ui::Layer* GetRoot(const ui::Layer* layer) {
  while (layer->parent())
    layer = layer->parent();
  return layer;
}

}  // namespace

namespace ui {

Layer::Layer()
    : type_(LAYER_TEXTURED),
      compositor_(NULL),
      parent_(NULL),
      visible_(true),
      is_drawn_(true),
      force_render_surface_(false),
      fills_bounds_opaquely_(true),
      layer_updated_externally_(false),
      background_blur_radius_(0),
      layer_saturation_(0.0f),
      layer_brightness_(0.0f),
      layer_grayscale_(0.0f),
      layer_inverted_(false),
      layer_mask_(NULL),
      layer_mask_back_link_(NULL),
      zoom_(1),
      zoom_inset_(0),
      delegate_(NULL),
      cc_layer_(NULL),
      scale_content_(true),
      device_scale_factor_(1.0f) {
  CreateWebLayer();
}

Layer::Layer(LayerType type)
    : type_(type),
      compositor_(NULL),
      parent_(NULL),
      visible_(true),
      is_drawn_(true),
      force_render_surface_(false),
      fills_bounds_opaquely_(true),
      layer_updated_externally_(false),
      background_blur_radius_(0),
      layer_saturation_(0.0f),
      layer_brightness_(0.0f),
      layer_grayscale_(0.0f),
      layer_inverted_(false),
      layer_mask_(NULL),
      layer_mask_back_link_(NULL),
      zoom_(1),
      zoom_inset_(0),
      delegate_(NULL),
      scale_content_(true),
      device_scale_factor_(1.0f) {
  CreateWebLayer();
}

Layer::~Layer() {
  // Destroying the animator may cause observers to use the layer (and
  // indirectly the WebLayer). Destroy the animator first so that the WebLayer
  // is still around.
  if (animator_)
    animator_->SetDelegate(NULL);
  animator_ = NULL;
  if (compositor_)
    compositor_->SetRootLayer(NULL);
  if (parent_)
    parent_->Remove(this);
  if (layer_mask_)
    SetMaskLayer(NULL);
  if (layer_mask_back_link_)
    layer_mask_back_link_->SetMaskLayer(NULL);
  for (size_t i = 0; i < children_.size(); ++i)
    children_[i]->parent_ = NULL;
  cc_layer_->RemoveLayerAnimationEventObserver(this);
  cc_layer_->RemoveFromParent();
}

Compositor* Layer::GetCompositor() {
  return GetRoot(this)->compositor_;
}

float Layer::opacity() const {
  return cc_layer_->opacity();
}

void Layer::SetCompositor(Compositor* compositor) {
  // This function must only be called to set the compositor on the root layer,
  // or to reset it.
  DCHECK(!compositor || !compositor_);
  DCHECK(!compositor || compositor->root_layer() == this);
  DCHECK(!parent_);
  compositor_ = compositor;
  if (compositor) {
    OnDeviceScaleFactorChanged(compositor->device_scale_factor());
    SendPendingThreadedAnimations();
  }
}

void Layer::Add(Layer* child) {
  DCHECK(!child->compositor_);
  if (child->parent_)
    child->parent_->Remove(child);
  child->parent_ = this;
  children_.push_back(child);
  cc_layer_->AddChild(child->cc_layer_);
  child->OnDeviceScaleFactorChanged(device_scale_factor_);
  child->UpdateIsDrawn();
  if (GetCompositor())
    child->SendPendingThreadedAnimations();
}

void Layer::Remove(Layer* child) {
  std::vector<Layer*>::iterator i =
      std::find(children_.begin(), children_.end(), child);
  DCHECK(i != children_.end());
  children_.erase(i);
  child->parent_ = NULL;
  child->cc_layer_->RemoveFromParent();
}

void Layer::StackAtTop(Layer* child) {
  if (children_.size() <= 1 || child == children_.back())
    return;  // Already in front.
  StackAbove(child, children_.back());
}

void Layer::StackAbove(Layer* child, Layer* other) {
  StackRelativeTo(child, other, true);
}

void Layer::StackAtBottom(Layer* child) {
  if (children_.size() <= 1 || child == children_.front())
    return;  // Already on bottom.
  StackBelow(child, children_.front());
}

void Layer::StackBelow(Layer* child, Layer* other) {
  StackRelativeTo(child, other, false);
}

bool Layer::Contains(const Layer* other) const {
  for (const Layer* parent = other; parent; parent = parent->parent()) {
    if (parent == this)
      return true;
  }
  return false;
}

void Layer::SetAnimator(LayerAnimator* animator) {
  if (animator)
    animator->SetDelegate(this);
  animator_ = animator;
}

LayerAnimator* Layer::GetAnimator() {
  if (!animator_.get())
    SetAnimator(LayerAnimator::CreateDefaultAnimator());
  return animator_.get();
}

void Layer::SetTransform(const gfx::Transform& transform) {
  GetAnimator()->SetTransform(transform);
}

gfx::Transform Layer::GetTargetTransform() const {
  if (animator_.get() && animator_->IsAnimatingProperty(
      LayerAnimationElement::TRANSFORM)) {
    return animator_->GetTargetTransform();
  }
  return transform();
}

void Layer::SetBounds(const gfx::Rect& bounds) {
  GetAnimator()->SetBounds(bounds);
}

gfx::Rect Layer::GetTargetBounds() const {
  if (animator_.get() && animator_->IsAnimatingProperty(
      LayerAnimationElement::BOUNDS)) {
    return animator_->GetTargetBounds();
  }
  return bounds_;
}

void Layer::SetMasksToBounds(bool masks_to_bounds) {
  cc_layer_->SetMasksToBounds(masks_to_bounds);
}

bool Layer::GetMasksToBounds() const {
  return cc_layer_->masks_to_bounds();
}

void Layer::SetOpacity(float opacity) {
  GetAnimator()->SetOpacity(opacity);
}

float Layer::GetCombinedOpacity() const {
  float opacity = this->opacity();
  Layer* current = this->parent_;
  while (current) {
    opacity *= current->opacity();
    current = current->parent_;
  }
  return opacity;
}

void Layer::SetBackgroundBlur(int blur_radius) {
  background_blur_radius_ = blur_radius;

  SetLayerBackgroundFilters();
}

void Layer::SetLayerSaturation(float saturation) {
  layer_saturation_ = saturation;
  SetLayerFilters();
}

void Layer::SetLayerBrightness(float brightness) {
  GetAnimator()->SetBrightness(brightness);
}

float Layer::GetTargetBrightness() const {
  if (animator_.get() && animator_->IsAnimatingProperty(
      LayerAnimationElement::BRIGHTNESS)) {
    return animator_->GetTargetBrightness();
  }
  return layer_brightness();
}

void Layer::SetLayerGrayscale(float grayscale) {
  GetAnimator()->SetGrayscale(grayscale);
}

float Layer::GetTargetGrayscale() const {
  if (animator_.get() && animator_->IsAnimatingProperty(
      LayerAnimationElement::GRAYSCALE)) {
    return animator_->GetTargetGrayscale();
  }
  return layer_grayscale();
}

void Layer::SetLayerInverted(bool inverted) {
  layer_inverted_ = inverted;
  SetLayerFilters();
}

void Layer::SetMaskLayer(Layer* layer_mask) {
  // The provided mask should not have a layer mask itself.
  DCHECK(!layer_mask ||
         (!layer_mask->layer_mask_layer() &&
          layer_mask->children().empty() &&
          !layer_mask->layer_mask_back_link_));
  DCHECK(!layer_mask_back_link_);
  if (layer_mask_ == layer_mask)
    return;
  // We need to de-reference the currently linked object so that no problem
  // arises if the mask layer gets deleted before this object.
  if (layer_mask_)
    layer_mask_->layer_mask_back_link_ = NULL;
  layer_mask_ = layer_mask;
  cc_layer_->SetMaskLayer(
      layer_mask ? layer_mask->cc_layer() : NULL);
  // We need to reference the linked object so that it can properly break the
  // link to us when it gets deleted.
  if (layer_mask) {
    layer_mask->layer_mask_back_link_ = this;
    layer_mask->OnDeviceScaleFactorChanged(device_scale_factor_);
  }
}

void Layer::SetBackgroundZoom(float zoom, int inset) {
  zoom_ = zoom;
  zoom_inset_ = inset;

  SetLayerBackgroundFilters();
}

void Layer::SetLayerFilters() {
  WebKit::WebFilterOperations filters;
  if (layer_saturation_) {
    filters.append(WebKit::WebFilterOperation::createSaturateFilter(
        layer_saturation_));
  }
  if (layer_grayscale_) {
    filters.append(WebKit::WebFilterOperation::createGrayscaleFilter(
        layer_grayscale_));
  }
  if (layer_inverted_)
    filters.append(WebKit::WebFilterOperation::createInvertFilter(1.0));
  // Brightness goes last, because the resulting colors neeed clamping, which
  // cause further color matrix filters to be applied separately. In this order,
  // they all can be combined in a single pass.
  if (layer_brightness_) {
    filters.append(WebKit::WebFilterOperation::createSaturatingBrightnessFilter(
        layer_brightness_));
  }

  cc_layer_->SetFilters(filters);
}

void Layer::SetLayerBackgroundFilters() {
  WebKit::WebFilterOperations filters;
  if (zoom_ != 1) {
    filters.append(WebKit::WebFilterOperation::createZoomFilter(zoom_,
                                                                zoom_inset_));
  }

  if (background_blur_radius_) {
    filters.append(WebKit::WebFilterOperation::createBlurFilter(
        background_blur_radius_));
  }

  cc_layer_->SetBackgroundFilters(filters);
}

float Layer::GetTargetOpacity() const {
  if (animator_.get() && animator_->IsAnimatingProperty(
      LayerAnimationElement::OPACITY))
    return animator_->GetTargetOpacity();
  return opacity();
}

void Layer::SetVisible(bool visible) {
  GetAnimator()->SetVisibility(visible);
}

bool Layer::GetTargetVisibility() const {
  if (animator_.get() && animator_->IsAnimatingProperty(
      LayerAnimationElement::VISIBILITY))
    return animator_->GetTargetVisibility();
  return visible_;
}

bool Layer::IsDrawn() const {
  return is_drawn_;
}

void Layer::UpdateIsDrawn() {
  bool updated_is_drawn = visible_ && (!parent_ || parent_->IsDrawn());

  if (updated_is_drawn == is_drawn_)
    return;

  is_drawn_ = updated_is_drawn;
  cc_layer_->SetIsDrawable(is_drawn_ && type_ != LAYER_NOT_DRAWN);

  for (size_t i = 0; i < children_.size(); ++i) {
    children_[i]->UpdateIsDrawn();
  }
}

bool Layer::ShouldDraw() const {
  return type_ != LAYER_NOT_DRAWN && GetCombinedOpacity() > 0.0f;
}

// static
void Layer::ConvertPointToLayer(const Layer* source,
                                const Layer* target,
                                gfx::Point* point) {
  if (source == target)
    return;

  const Layer* root_layer = GetRoot(source);
  CHECK_EQ(root_layer, GetRoot(target));

  if (source != root_layer)
    source->ConvertPointForAncestor(root_layer, point);
  if (target != root_layer)
    target->ConvertPointFromAncestor(root_layer, point);
}

bool Layer::GetTargetTransformRelativeTo(const Layer* ancestor,
                                         gfx::Transform* transform) const {
  const Layer* p = this;
  for (; p && p != ancestor; p = p->parent()) {
    gfx::Transform translation;
    translation.Translate(static_cast<float>(p->bounds().x()),
                          static_cast<float>(p->bounds().y()));
    // Use target transform so that result will be correct once animation is
    // finished.
    if (!p->GetTargetTransform().IsIdentity())
      transform->ConcatTransform(p->GetTargetTransform());
    transform->ConcatTransform(translation);
  }
  return p == ancestor;
}

// static
gfx::Transform Layer::ConvertTransformToCCTransform(
    const gfx::Transform& transform,
    float device_scale_factor) {
  gfx::Transform cc_transform;
  cc_transform.Scale(device_scale_factor, device_scale_factor);
  cc_transform.PreconcatTransform(transform);
  cc_transform.Scale(1.0f / device_scale_factor, 1.0f / device_scale_factor);
  return cc_transform;
}

void Layer::SetFillsBoundsOpaquely(bool fills_bounds_opaquely) {
  if (fills_bounds_opaquely_ == fills_bounds_opaquely)
    return;

  fills_bounds_opaquely_ = fills_bounds_opaquely;

  cc_layer_->SetContentsOpaque(fills_bounds_opaquely);
}

void Layer::SwitchToLayer(scoped_refptr<cc::Layer> new_layer) {
  // Finish animations being handled by cc_layer_.
  if (animator_) {
    animator_->StopAnimatingProperty(LayerAnimationElement::TRANSFORM);
    animator_->StopAnimatingProperty(LayerAnimationElement::OPACITY);
  }

  if (texture_layer_.get())
    texture_layer_->WillModifyTexture();
  // TODO(piman): delegated_renderer_layer_ cleanup.

  cc_layer_->RemoveAllChildren();
  if (parent_) {
    DCHECK(parent_->cc_layer_);
    parent_->cc_layer_->ReplaceChild(cc_layer_, new_layer);
  }
  cc_layer_->RemoveLayerAnimationEventObserver(this);
  new_layer->SetOpacity(cc_layer_->opacity());
  new_layer->SetTransform(cc_layer_->transform());
  new_layer->SetPosition(cc_layer_->position());

  cc_layer_= new_layer;
  content_layer_ = NULL;
  solid_color_layer_ = NULL;
  texture_layer_ = NULL;
  delegated_renderer_layer_ = NULL;

  cc_layer_->AddLayerAnimationEventObserver(this);
  for (size_t i = 0; i < children_.size(); ++i) {
    DCHECK(children_[i]->cc_layer_);
    cc_layer_->AddChild(children_[i]->cc_layer_);
  }
  cc_layer_->SetAnchorPoint(gfx::PointF());
  cc_layer_->SetContentsOpaque(fills_bounds_opaquely_);
  cc_layer_->SetForceRenderSurface(force_render_surface_);
  cc_layer_->SetIsDrawable(IsDrawn());
}

void Layer::SwitchCCLayerForTest() {
  scoped_refptr<cc::ContentLayer> new_layer = cc::ContentLayer::Create(this);
  SwitchToLayer(new_layer);
  content_layer_ = new_layer;
}

void Layer::SetExternalTexture(Texture* texture) {
  // Hold a ref to the old |Texture| until we have updated all
  // compositor references to the texture id that it holds.
  scoped_refptr<ui::Texture> old_texture = texture_;

  DCHECK_EQ(type_, LAYER_TEXTURED);
  DCHECK(!solid_color_layer_);
  bool has_texture = !!texture;
  layer_updated_externally_ = has_texture;
  texture_ = texture;
  if (!!texture_layer_ != has_texture) {
    // Switch to a different type of layer.
    if (has_texture) {
      scoped_refptr<cc::TextureLayer> new_layer =
          cc::TextureLayer::Create(this);
      new_layer->SetFlipped(texture_->flipped());
      SwitchToLayer(new_layer);
      texture_layer_ = new_layer;
    } else {
      scoped_refptr<cc::ContentLayer> new_layer =
          cc::ContentLayer::Create(this);
      SwitchToLayer(new_layer);
      content_layer_ = new_layer;
    }
  }
  RecomputeDrawsContentAndUVRect();
}

void Layer::SetDelegatedFrame(scoped_ptr<cc::DelegatedFrameData> frame,
                              gfx::Size frame_size_in_dip) {
  DCHECK_EQ(type_, LAYER_TEXTURED);
  bool has_frame = frame.get() && !frame->render_pass_list.empty();
  layer_updated_externally_ = has_frame;
  delegated_frame_size_in_dip_ = frame_size_in_dip;
  if (!!delegated_renderer_layer_ != has_frame) {
    if (has_frame) {
      scoped_refptr<cc::DelegatedRendererLayer> new_layer =
          cc::DelegatedRendererLayer::Create();
      SwitchToLayer(new_layer);
      delegated_renderer_layer_ = new_layer;
    } else {
      scoped_refptr<cc::ContentLayer> new_layer =
          cc::ContentLayer::Create(this);
      SwitchToLayer(new_layer);
      content_layer_ = new_layer;
    }
  }
  if (has_frame)
    delegated_renderer_layer_->SetFrameData(frame.Pass());
  RecomputeDrawsContentAndUVRect();
}

void Layer::TakeUnusedResourcesForChildCompositor(
    cc::TransferableResourceArray* list) {
  if (delegated_renderer_layer_)
    delegated_renderer_layer_->TakeUnusedResourcesForChildCompositor(list);
}

void Layer::SetColor(SkColor color) {
  GetAnimator()->SetColor(color);
}

bool Layer::SchedulePaint(const gfx::Rect& invalid_rect) {
  if (type_ == LAYER_SOLID_COLOR || (!delegate_ && !texture_))
    return false;

  damaged_region_.op(invalid_rect.x(),
                     invalid_rect.y(),
                     invalid_rect.right(),
                     invalid_rect.bottom(),
                     SkRegion::kUnion_Op);
  ScheduleDraw();
  return true;
}

void Layer::ScheduleDraw() {
  Compositor* compositor = GetCompositor();
  if (compositor)
    compositor->ScheduleDraw();
}

void Layer::SendDamagedRects() {
  if ((delegate_ || texture_) && !damaged_region_.isEmpty()) {
    for (SkRegion::Iterator iter(damaged_region_);
         !iter.done(); iter.next()) {
      const SkIRect& sk_damaged = iter.rect();
      gfx::Rect damaged(
          sk_damaged.x(),
          sk_damaged.y(),
          sk_damaged.width(),
          sk_damaged.height());

      gfx::Rect damaged_in_pixel = ConvertRectToPixel(this, damaged);
      cc_layer_->SetNeedsDisplayRect(damaged_in_pixel);
    }
    damaged_region_.setEmpty();
  }
  for (size_t i = 0; i < children_.size(); ++i)
    children_[i]->SendDamagedRects();
}

void Layer::SuppressPaint() {
  if (!delegate_)
    return;
  delegate_ = NULL;
  for (size_t i = 0; i < children_.size(); ++i)
    children_[i]->SuppressPaint();
}

void Layer::OnDeviceScaleFactorChanged(float device_scale_factor) {
  if (device_scale_factor_ == device_scale_factor)
    return;
  if (animator_)
    animator_->StopAnimatingProperty(LayerAnimationElement::TRANSFORM);
  gfx::Transform transform = this->transform();
  device_scale_factor_ = device_scale_factor;
  RecomputeCCTransformFromTransform(transform);
  RecomputeDrawsContentAndUVRect();
  RecomputePosition();
  SchedulePaint(gfx::Rect(bounds_.size()));
  if (delegate_)
    delegate_->OnDeviceScaleFactorChanged(device_scale_factor);
  for (size_t i = 0; i < children_.size(); ++i)
    children_[i]->OnDeviceScaleFactorChanged(device_scale_factor);
  if (layer_mask_)
    layer_mask_->OnDeviceScaleFactorChanged(device_scale_factor);
}

void Layer::PaintContents(SkCanvas* sk_canvas,
                          gfx::Rect clip,
                          gfx::RectF* opaque) {
  TRACE_EVENT0("ui", "Layer::PaintContents");
  scoped_ptr<gfx::Canvas> canvas(gfx::Canvas::CreateCanvasWithoutScaling(
      sk_canvas, ui::GetScaleFactorFromScale(device_scale_factor_)));

  bool scale_content = scale_content_;
  if (scale_content) {
    canvas->Save();
    canvas->sk_canvas()->scale(SkFloatToScalar(device_scale_factor_),
                               SkFloatToScalar(device_scale_factor_));
  }

  if (delegate_)
    delegate_->OnPaintLayer(canvas.get());
  if (scale_content)
    canvas->Restore();
}

unsigned Layer::PrepareTexture(cc::ResourceUpdateQueue* queue) {
  DCHECK(texture_layer_);
  return texture_->PrepareTexture();
}

WebKit::WebGraphicsContext3D* Layer::Context3d() {
  DCHECK(texture_layer_);
  return texture_->HostContext3D();
}

bool Layer::PrepareTextureMailbox(cc::TextureMailbox* mailbox) {
  return false;
}

void Layer::SetForceRenderSurface(bool force) {
  if (force_render_surface_ == force)
    return;

  force_render_surface_ = force;
  cc_layer_->SetForceRenderSurface(force_render_surface_);
}

void Layer::OnAnimationStarted(const cc::AnimationEvent& event) {
  if (animator_)
    animator_->OnThreadedAnimationStarted(event);
}

void Layer::StackRelativeTo(Layer* child, Layer* other, bool above) {
  DCHECK_NE(child, other);
  DCHECK_EQ(this, child->parent());
  DCHECK_EQ(this, other->parent());

  const size_t child_i =
      std::find(children_.begin(), children_.end(), child) - children_.begin();
  const size_t other_i =
      std::find(children_.begin(), children_.end(), other) - children_.begin();
  if ((above && child_i == other_i + 1) || (!above && child_i + 1 == other_i))
    return;

  const size_t dest_i =
      above ?
      (child_i < other_i ? other_i : other_i + 1) :
      (child_i < other_i ? other_i - 1 : other_i);
  children_.erase(children_.begin() + child_i);
  children_.insert(children_.begin() + dest_i, child);

  child->cc_layer_->RemoveFromParent();
  cc_layer_->InsertChild(child->cc_layer_, dest_i);
}

bool Layer::ConvertPointForAncestor(const Layer* ancestor,
                                    gfx::Point* point) const {
  gfx::Transform transform;
  bool result = GetTargetTransformRelativeTo(ancestor, &transform);
  gfx::Point3F p(*point);
  transform.TransformPoint(p);
  *point = gfx::ToFlooredPoint(p.AsPointF());
  return result;
}

bool Layer::ConvertPointFromAncestor(const Layer* ancestor,
                                     gfx::Point* point) const {
  gfx::Transform transform;
  bool result = GetTargetTransformRelativeTo(ancestor, &transform);
  gfx::Point3F p(*point);
  transform.TransformPointReverse(p);
  *point = gfx::ToFlooredPoint(p.AsPointF());
  return result;
}

void Layer::SetBoundsImmediately(const gfx::Rect& bounds) {
  if (bounds == bounds_)
    return;

  base::Closure closure;
  if (delegate_)
    closure = delegate_->PrepareForLayerBoundsChange();
  bool was_move = bounds_.size() == bounds.size();
  bounds_ = bounds;

  RecomputeDrawsContentAndUVRect();
  RecomputePosition();

  if (!closure.is_null())
    closure.Run();

  if (was_move) {
    // Don't schedule a draw if we're invisible. We'll schedule one
    // automatically when we get visible.
    if (IsDrawn())
      ScheduleDraw();
  } else {
    // Always schedule a paint, even if we're invisible.
    SchedulePaint(gfx::Rect(bounds.size()));
  }
}

void Layer::SetTransformImmediately(const gfx::Transform& transform) {
  RecomputeCCTransformFromTransform(transform);
}

void Layer::SetOpacityImmediately(float opacity) {
  cc_layer_->SetOpacity(opacity);
  ScheduleDraw();
}

void Layer::SetVisibilityImmediately(bool visible) {
  if (visible_ == visible)
    return;

  visible_ = visible;
  UpdateIsDrawn();
}

void Layer::SetBrightnessImmediately(float brightness) {
  layer_brightness_ = brightness;
  SetLayerFilters();
}

void Layer::SetGrayscaleImmediately(float grayscale) {
  layer_grayscale_ = grayscale;
  SetLayerFilters();
}

void Layer::SetColorImmediately(SkColor color) {
  DCHECK_EQ(type_, LAYER_SOLID_COLOR);
  // WebColor is equivalent to SkColor, per WebColor.h.
  solid_color_layer_->SetBackgroundColor(static_cast<WebKit::WebColor>(color));
  SetFillsBoundsOpaquely(SkColorGetA(color) == 0xFF);
}

void Layer::SetBoundsFromAnimation(const gfx::Rect& bounds) {
  SetBoundsImmediately(bounds);
}

void Layer::SetTransformFromAnimation(const gfx::Transform& transform) {
  SetTransformImmediately(transform);
}

void Layer::SetOpacityFromAnimation(float opacity) {
  SetOpacityImmediately(opacity);
}

void Layer::SetVisibilityFromAnimation(bool visibility) {
  SetVisibilityImmediately(visibility);
}

void Layer::SetBrightnessFromAnimation(float brightness) {
  SetBrightnessImmediately(brightness);
}

void Layer::SetGrayscaleFromAnimation(float grayscale) {
  SetGrayscaleImmediately(grayscale);
}

void Layer::SetColorFromAnimation(SkColor color) {
  SetColorImmediately(color);
}

void Layer::ScheduleDrawForAnimation() {
  ScheduleDraw();
}

const gfx::Rect& Layer::GetBoundsForAnimation() const {
  return bounds();
}

gfx::Transform Layer::GetTransformForAnimation() const {
  return transform();
}

float Layer::GetOpacityForAnimation() const {
  return opacity();
}

bool Layer::GetVisibilityForAnimation() const {
  return visible();
}

float Layer::GetBrightnessForAnimation() const {
  return layer_brightness();
}

float Layer::GetGrayscaleForAnimation() const {
  return layer_grayscale();
}

SkColor Layer::GetColorForAnimation() const {
  // WebColor is equivalent to SkColor, per WebColor.h.
  // The NULL check is here since this is invoked regardless of whether we have
  // been configured as LAYER_SOLID_COLOR.
  return solid_color_layer_.get() ?
      solid_color_layer_->background_color() : SK_ColorBLACK;
}

float Layer::GetDeviceScaleFactor() const {
  return device_scale_factor_;
}

void Layer::AddThreadedAnimation(scoped_ptr<cc::Animation> animation) {
  DCHECK(cc_layer_);
  // Until this layer has a compositor (and hence cc_layer_ has a
  // LayerTreeHost), addAnimation will fail.
  if (GetCompositor())
    cc_layer_->AddAnimation(animation.Pass());
  else
    pending_threaded_animations_.push_back(animation.Pass());
}

namespace{

struct HasAnimationId {
  HasAnimationId(int id): id_(id) {
  }

  bool operator()(cc::Animation* animation) const {
    return animation->id() == id_;
  }

 private:
  int id_;
};

}

void Layer::RemoveThreadedAnimation(int animation_id) {
  DCHECK(cc_layer_);
  if (pending_threaded_animations_.size() == 0) {
    cc_layer_->RemoveAnimation(animation_id);
    return;
  }

  pending_threaded_animations_.erase(
      cc::remove_if(&pending_threaded_animations_,
                    pending_threaded_animations_.begin(),
                    pending_threaded_animations_.end(),
                    HasAnimationId(animation_id)),
      pending_threaded_animations_.end());
}

void Layer::SendPendingThreadedAnimations() {
  for (cc::ScopedPtrVector<cc::Animation>::iterator it =
           pending_threaded_animations_.begin();
       it != pending_threaded_animations_.end();
       ++it)
    cc_layer_->AddAnimation(pending_threaded_animations_.take(it));

  pending_threaded_animations_.clear();

  for (size_t i = 0; i < children_.size(); ++i)
    children_[i]->SendPendingThreadedAnimations();
}

void Layer::CreateWebLayer() {
  if (type_ == LAYER_SOLID_COLOR) {
    solid_color_layer_ = cc::SolidColorLayer::Create();
    cc_layer_ = solid_color_layer_.get();
  } else {
    content_layer_ = cc::ContentLayer::Create(this);
    cc_layer_ = content_layer_.get();
  }
  cc_layer_->SetAnchorPoint(gfx::PointF());
  cc_layer_->SetContentsOpaque(true);
  cc_layer_->SetIsDrawable(type_ != LAYER_NOT_DRAWN);
  cc_layer_->AddLayerAnimationEventObserver(this);
  RecomputePosition();
}

void Layer::RecomputeCCTransformFromTransform(const gfx::Transform& transform) {
  cc_layer_->SetTransform(ConvertTransformToCCTransform(transform,
                                                        device_scale_factor_));
}

gfx::Transform Layer::transform() const {
  gfx::Transform transform;
  transform.Scale(1.0f / device_scale_factor_, 1.0f / device_scale_factor_);
  transform.PreconcatTransform(cc_layer_->transform());
  transform.Scale(device_scale_factor_, device_scale_factor_);
  return transform;
}

void Layer::RecomputeDrawsContentAndUVRect() {
  DCHECK(cc_layer_);
  gfx::Size size(bounds_.size());
  if (texture_layer_.get()) {
    DCHECK(texture_);

    float texture_scale_factor = 1.0f / texture_->device_scale_factor();
    gfx::Size texture_size = gfx::ToFlooredSize(
        gfx::ScaleSize(texture_->size(), texture_scale_factor));
    size.ClampToMax(texture_size);

    gfx::PointF uv_top_left(0.f, 0.f);
    gfx::PointF uv_bottom_right(
        static_cast<float>(size.width())/texture_size.width(),
        static_cast<float>(size.height())/texture_size.height());
    texture_layer_->SetUV(uv_top_left, uv_bottom_right);
  } else if (delegated_renderer_layer_.get()) {
    delegated_renderer_layer_->SetDisplaySize(
        ConvertSizeToPixel(this, delegated_frame_size_in_dip_));
    size.ClampToMax(delegated_frame_size_in_dip_);
  }
  cc_layer_->SetBounds(ConvertSizeToPixel(this, size));
}

void Layer::RecomputePosition() {
  cc_layer_->SetPosition(gfx::ScalePoint(
        gfx::PointF(bounds_.x(), bounds_.y()),
        device_scale_factor_));
}

}  // namespace ui
