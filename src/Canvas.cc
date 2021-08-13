#include <iostream>

#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"

#include "Canvas.h"
#include "CanvasRenderingContext2D.h"
#include "helpers.h"

Canvas::Canvas(const Napi::CallbackInfo& info) : Napi::ObjectWrap<Canvas>(info) {
    Napi::Number width = info[0].As<Napi::Number>();
    Napi::Number height = info[0].As<Napi::Number>();
    width_ = width.Int32Value();
    height_ = height.Int32Value();

    rasterSurface_ = SkSurface::MakeRasterN32Premul(width_, height_);
}

Napi::FunctionReference Canvas::constructor;

sk_sp<SkSurface> Canvas::getSurface() {
    return rasterSurface_;
}

int Canvas::getWidth() {
    return width_;
}

int Canvas::getHeight() {
    return height_;
}

// Binding 初始化层，定义如何向上暴露 API
Napi::Object Canvas::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "Canvas", {
        InstanceAccessor<&Canvas::GetWidth, &Canvas::SetWidth>("width"),
        InstanceAccessor<&Canvas::GetHeight, &Canvas::SetHeight>("height"),
        InstanceMethod<&Canvas::GetContext>("getContext"),
        InstanceMethod<&Canvas::ToBuffer>("toBuffer")
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("Canvas", func);

    return exports;
}

Napi::Value Canvas::GetWidth(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), this->width_);
}

void Canvas::SetWidth(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Number width = value.As<Napi::Number>();

    this->width_ = width.Int32Value();
    this->rasterSurface_ = SkSurface::MakeRasterN32Premul(this->width_, this->height_);
    this->inner_ctx->SetCanvas(this->rasterSurface_->getCanvas());
}

Napi::Value Canvas::GetHeight(const Napi::CallbackInfo& info) {
    return Napi::Number::New(info.Env(), this->height_);
}

void Canvas::SetHeight(const Napi::CallbackInfo& info, const Napi::Value& value) {
    Napi::Number height = value.As<Napi::Number>();

    this->height_ = height.Int32Value();
    this->rasterSurface_ = SkSurface::MakeRasterN32Premul(this->width_, this->height_);
    this->inner_ctx->SetCanvas(this->rasterSurface_->getCanvas());
}

Napi::Value Canvas::GetContext(const Napi::CallbackInfo& info) {
    napi_status status;
    std::string cc = info[0].As<Napi::String>().Utf8Value();

    napi_valuetype ctx_type;
    status = napi_typeof(info.Env(), this->ctx_, &ctx_type);

    // NOTE: 这里判断 == nullptr 会有问题，后续挖一下为什么
    if (ctx_type != napi_object) {
        status = CanvasRenderingContext2D::NewInstance(info.Env(), &this->ctx_);

        CanvasRenderingContext2D* inner_ctx;
        status = napi_unwrap(info.Env(), this->ctx_, reinterpret_cast<void**>(&inner_ctx));
        this->inner_ctx = inner_ctx;
        inner_ctx->SetCanvas(this->rasterSurface_->getCanvas());
    }
    
    return Napi::Value::From(info.Env(), this->ctx_);
}

Napi::Value Canvas::ToBuffer(const Napi::CallbackInfo& info) {
    // TODO: ouptut config argv[1]
    SkEncodedImageFormat sk_format = SkEncodedImageFormat::kPNG;

    if (info.Length() >= 1) {
        string mime_type_str = info[0].As<Napi::String>().Utf8Value();

        if (mime_type_str == "image/jpeg") {
            sk_format = SkEncodedImageFormat::kJPEG;
        } else if (mime_type_str == "image/png") {
            sk_format = SkEncodedImageFormat::kPNG;
        }
    }

    SkCanvas* c = this->rasterSurface_->getCanvas();
    sk_sp<SkImage> img(this->rasterSurface_->makeImageSnapshot());
    sk_sp<SkData> png(img->encodeToData(sk_format, 100));

    return Napi::Buffer<unsigned char>::Copy(info.Env(), (unsigned char *)png->data(), png->size());
}
