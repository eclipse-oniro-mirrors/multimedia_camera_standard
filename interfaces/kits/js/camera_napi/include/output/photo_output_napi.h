/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef PHOTO_OUTPUT_NAPI_H_
#define PHOTO_OUTPUT_NAPI_H_

#include <securec.h>

#include "media_log.h"
#include "napi/native_api.h"
#include "napi/native_node_api.h"

#include "input/camera_manager.h"
#include "input/camera_info.h"
#include "output/photo_output.h"

#include "hilog/log.h"
#include "camera_napi_utils.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>

namespace OHOS {
namespace CameraStandard {
static const std::string CAMERA_PHOTO_OUTPUT_NAPI_CLASS_NAME = "PhotoOutput";

class PhotoSurfaceListener : public IBufferConsumerListener {
public:
    void OnBufferAvailable() override;
    int32_t SaveData(const char *buffer, int32_t size);
    void SetConsumerSurface(sptr<Surface> captureSurface);

private:
    sptr<Surface> captureSurface_;
    std::string photoPath;
};

class PhotoOutputNapi {
public:
    static napi_value Init(napi_env env, napi_value exports);
    static napi_value CreatePhotoOutput(napi_env env, long surfaceId);
    static bool IsPhotoOutput(napi_env env, napi_value obj);
    PhotoOutputNapi();
    ~PhotoOutputNapi();

    sptr<CaptureOutput> GetPhotoOutput();
    long GetSurfaceId();

private:
    static void PhotoOutputNapiDestructor(napi_env env, void* nativeObject, void* finalize_hint);
    static napi_value PhotoOutputNapiConstructor(napi_env env, napi_callback_info info);

    static napi_value Capture(napi_env env, napi_callback_info info);
    static napi_value Release(napi_env env, napi_callback_info info);
    static napi_value On(napi_env env, napi_callback_info info);

    static napi_ref sConstructor_;
    static long sSurfaceId_;
    static sptr<CaptureOutput> sPhotoOutput_;
    static sptr<PhotoSurfaceListener> listener;

    std::vector<std::string> callbackList_;
    void RegisterCallback(napi_env env, napi_ref callbackRef);

    napi_env env_;
    napi_ref wrapper_;
    long surfaceId_;
    sptr<CaptureOutput> photoOutput_;
};

struct PhotoOutputAsyncContext {
    napi_env env;
    napi_async_work work;
    napi_deferred deferred;
    napi_ref callbackRef;
    long surfaceId;
    int32_t quality = -1;
    int32_t mirror = -1;
    double latitude;
    double longitude;
    int32_t rotaion = -1;
    PhotoOutputNapi* objectInfo;
    bool status;
    bool hasPhotoSettings;
};
} // namespace CameraStandard
} // namespace OHOS
#endif /* PHOTO_OUTPUT_NAPI_H_ */
