/*###############################################################################
#
# Copyright 2020 NVIDIA Corporation
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
###############################################################################*/
#ifndef NvAR_DEFS_H
#define NvAR_DEFS_H

#include <stddef.h>
#include <stdint.h>
#include <nvCVImage.h>
#include <nvCVStatus.h>

#ifdef _WIN32
  #ifdef NVAR_API_EXPORT
    #define NvAR_API __declspec(dllexport) __cdecl
  #else
    #define NvAR_API
  #endif
#else
  #ifdef NVAR_API_EXPORT
    #define NvAR_API __attribute__((visibility("default")))
  #else
    #define NvAR_API
  #endif
#endif // OS dependencies

typedef struct NvAR_Vector3f
{
  float vec[3];
} NvAR_Vector3f;

typedef struct NvAR_Vector3u16 {
  unsigned short vec[3];
} NvAR_Vector3u16;

typedef struct NvAR_Rect {
  float x, y, width, height;
} NvAR_Rect;

typedef struct NvAR_BBoxes {
  NvAR_Rect *boxes;
  uint8_t num_boxes;
  uint8_t max_boxes;
} NvAR_BBoxes;

typedef struct NvAR_TrackingBBox {
  NvAR_Rect bbox;
  uint16_t tracking_id;
} NvAR_TrackingBBox;

typedef struct NvAR_TrackingBBoxes {
  NvAR_TrackingBBox *boxes;
  uint8_t num_boxes;
  uint8_t max_boxes;
} NvAR_TrackingBBoxes;

typedef struct NvAR_FaceMesh {
  NvAR_Vector3f *vertices;  ///< Mesh 3D vertex positions.
  size_t num_vertices;
  NvAR_Vector3u16 *tvi;     ///< Mesh triangle's vertex indices
  size_t num_triangles;     ///< The number of triangles (previously num_tri_idx)
} NvAR_FaceMesh;
#define num_tri_idx num_triangles  ///< num_tri_idx is confusing and deprecated

typedef struct NvAR_Frustum {
  float left;
  float right;
  float bottom;
  float top;
} NvAR_Frustum;

typedef struct NvAR_Quaternion {
  float x, y, z, w;
} NvAR_Quaternion;

typedef struct NvAR_Point2f {
  float x, y;
} NvAR_Point2f;

typedef struct NvAR_Point3f {
  float x, y, z;
} NvAR_Point3f;

typedef struct NvAR_Vector2f {
  float x, y;
} NvAR_Vector2f;

typedef struct NvAR_RenderingParams {
  NvAR_Frustum frustum;
  NvAR_Quaternion rotation;
  NvAR_Vector3f translation;
} NvAR_RenderingParams;

// Parameters provided by client application
typedef const char* NvAR_FeatureID;

#define NvAR_Feature_FaceBoxDetection     "FaceBoxDetection"       // 
#define NvAR_Feature_FaceDetection        "FaceDetection"          //        // deprecated in favor of FaceBox
#define NvAR_Feature_LandmarkDetection    "LandmarkDetection"      // 
#define NvAR_Feature_Face3DReconstruction "Face3DReconstruction"   // 
#define NvAR_Feature_BodyDetection        "BodyDetection"          // 
#define NvAR_Feature_BodyPoseEstimation   "BodyPoseEstimation"     // 
#define NvAR_Feature_GazeRedirection      "GazeRedirection"        // 
#define NvAR_Feature_FaceExpressions      "FaceExpressions"        // 
#define NvAR_Feature_VolumetricEncoding   "VolumetricEncoding"     // 
#define NvAR_Feature_AIDubbing            "AIDubbing"              // !AIDubbing!

#define NvAR_Parameter_Input(Name) "NvAR_Parameter_Input_" #Name
#define NvAR_Parameter_Output(Name) "NvAR_Parameter_Output_" #Name
#define NvAR_Parameter_Config(Name) "NvAR_Parameter_Config_" #Name
#define NvAR_Parameter_InOut(Name) "NvAR_Parameter_InOut_" #Name

#define NVAR_TEMPORAL_FILTER_FACE_BOX                 (1U << 0)  // 0x001
#define NVAR_TEMPORAL_FILTER_FACIAL_LANDMARKS         (1U << 1)  // 0x002
#define NVAR_TEMPORAL_FILTER_FACE_ROTATIONAL_POSE     (1U << 2)  // 0x004
#define NVAR_TEMPORAL_FILTER_FACIAL_EXPRESSIONS       (1U << 4)  // 0x010
#define NVAR_TEMPORAL_FILTER_FACIAL_GAZE              (1U << 5)  // 0x020
#define NVAR_TEMPORAL_FILTER_ENHANCE_EXPRESSIONS      (1U << 8)  // 0x100


#define NVAR_VOLUMETRIC_ENCODING_QUANTIZATION_FILTER_NONE 0u                // 
#define NVAR_VOLUMETRIC_ENCODING_QUANTIZATION_FILTER_INTERVAL_1 (1U << 0)   // 
#define NVAR_VOLUMETRIC_ENCODING_QUANTIZATION_FILTER_INTERVAL_2 (1U << 1)   // 
#define NVAR_VOLUMETRIC_ENCODING_QUANTIZATION_FILTER_INTERVAL_4 (1U << 2)   // 
#define NVAR_VOLUMETRIC_ENCODING_QUANTIZATION_FILTER_INTERVAL_8 (1U << 3)   // 
#define NVAR_VOLUMETRIC_ENCODING_QUANTIZATION_FILTER_INTERVAL_16 (1U << 4)  // 
#define NVAR_VOLUMETRIC_ENCODING_QUANTIZATION_FILTER_INTERVAL_32 (1U << 5)  // 
#define NVAR_VOLUMETRIC_ENCODING_QUANTIZATION_FILTER_INTERVAL_64 (1U << 6)  // 

#ifndef   NVCV_LOG_FATAL
  #define NVCV_LOG_FATAL    0   //!< Message to be printed right before aborting due to an unrecoverable error.
  #define NVCV_LOG_ERROR    1   //!< An operation has failed, but it is not fatal.
  #define NVCV_LOG_WARNING  2   //!< Something was not quite right, but we fixed it up, perhaps at a loss in performance.
  #define NVCV_LOG_INFO     3   //!< Nothing is wrong, but this information might be of interest.
#endif // NVCV_LOG_FATAL

/*
Parameters supported by each NvAR_FeatureID

*******NvAR_Feature_FaceDetection*******                                // 
Config:                                                                 // 
NvAR_Parameter_Config(FeatureDescription)                               // 
NvAR_Parameter_Config(CUDAStream)                                       // 
NvAR_Parameter_Config(TRTModelDir)                                      // 
NvAR_Parameter_Config(Temporal)                                         // 
                                                                        // 
Input:                                                                  // 
NvAR_Parameter_Input(Image)                                             // 
                                                                        // 
Output:                                                                 // 
NvAR_Parameter_Output(BoundingBoxes)                                    // 
NvAR_Parameter_Output(BoundingBoxesConfidence) - OPTIONAL               // 
                                                                        // 
*******NvAR_Feature_LandmarkDetection*******                            // 
Config:                                                                 // 
NvAR_Parameter_Config(FeatureDescription)                               // 
NvAR_Parameter_Config(CUDAStream)                                       // 
NvAR_Parameter_Config(ModelDir)                                         // 
NvAR_Parameter_Config(Landmarks_Size)                                   // 
NvAR_Parameter_Config(LandmarksConfidence_Size)                         // 
NvAR_Parameter_Config(Temporal)                                         // 
                                                                        // 
Input:                                                                  // 
NvAR_Parameter_Input(Image)                                             // 
NvAR_Parameter_Input(BoundingBoxes) - OPTIONAL                          // 
                                                                        // 
Output:                                                                 // 
NvAR_Parameter_Output(BoundingBoxes) - OPTIONAL                         // 
NvAR_Parameter_Output(Landmarks)                                        // 
NvAR_Parameter_Output(Pose) - OPTIONAL                                  // 
NvAR_Parameter_Output(LandmarksConfidence) - OPTIONAL                   // 
                                                                        // 
*******NvAR_Feature_Face3DReconstruction*******                         // 
Config:                                                                 // 
NvAR_Parameter_Config(FeatureDescription)                               // 
NvAR_Parameter_Config(ModelDir)                                         // 
NvAR_Parameter_Config(Landmarks_Size)                                   // 
NvAR_Parameter_Config(CUDAStream) - OPTIONAL                            // 
NvAR_Parameter_Config(Temporal) - OPTIONAL                              // 
NvAR_Parameter_Config(GazeMode) - OPTIONAL                              // 
NvAR_Parameter_Config(ModelName) - OPTIONAL                             // 
NvAR_Parameter_Config(Mode) - OPTIONAL                                  // 
NvAR_Parameter_Config(GPU) - OPTIONAL                                   // 
NvAR_Parameter_Config(VertexCount) - QUERY                              // 
NvAR_Parameter_Config(TriangleCount) - QUERY                            // 
NvAR_Parameter_Config(ExpressionCount) - QUERY                          // 
NvAR_Parameter_Config(ShapeEigenValueCount) - QUERY                     // 
                                                                        // 
Input:                                                                  // 
NvAR_Parameter_Input(Width)                                             // 
NvAR_Parameter_Input(Height)                                            // 
NvAR_Parameter_Input(Image) - OPTIONAL                                  // 
NvAR_Parameter_Input(Landmarks) - OPTIONAL                              // 
                                                                        // 
Output:                                                                 // 
NvAR_Parameter_Output(FaceMesh)                                         // 
NvAR_Parameter_Output(RenderingParams)                                  // 
NvAR_Parameter_Output(BoundingBoxes) - OPTIONAL                         // 
NvAR_Parameter_Output(BoundingBoxesConfidence) - OPTIONAL               // 
NvAR_Parameter_Output(Landmarks) - OPTIONAL                             // 
NvAR_Parameter_Output(Pose) - OPTIONAL                                  // 
NvAR_Parameter_Output(LandmarksConfidence) - OPTIONAL                   // 
NvAR_Parameter_Output(ExpressionCoefficients) - OPTIONAL                // 
NvAR_Parameter_Output(ShapeEigenValues) - OPTIONAL                      // 
                                                                        // 
*******NvAR_Feature_BodyDetection*******                                // 
Config:                                                                 // 
NvAR_Parameter_Config(FeatureDescription)                               // 
NvAR_Parameter_Config(CUDAStream)                                       // 
NvAR_Parameter_Config(TRTModelDir)                                      // 
NvAR_Parameter_Config(Temporal)                                         // 
NvAR_Parameter_Config(FullBodyOnly)                                     // 
                                                                        // 
Input:                                                                  // 
NvAR_Parameter_Input(Image)                                             // 
                                                                        // 
Output:                                                                 // 
NvAR_Parameter_Output(BoundingBoxes)                                    // 
NvAR_Parameter_Output(BoundingBoxesConfidence) - OPTIONAL               // 
                                                                        // 
*******NvAR_Feature_BodyPoseEstimation*******                           // 
Config:                                                                 // 
NvAR_Parameter_Config(FeatureDescription)                               // 
NvAR_Parameter_Config(CUDAStream)                                       // 
NvAR_Parameter_Config(ModelDir)                                         // 
NvAR_Parameter_Config(BatchSize)                                        // 
NvAR_Parameter_Config(Mode)                                             // 
NvAR_Parameter_Config(NumKeyPoints)                                     // 
NvAR_Parameter_Config(ReferencePose)                                    // 
NvAR_Parameter_Config(Temporal)                                         // 
NvAR_Parameter_Config(UseCudaGraph)                                     // 
NvAR_Parameter_Config(FocalLength)                                      // 
NvAR_Parameter_Config(TrackPeople)                                      // 
NvAR_Parameter_Config(ShadowTrackingAge)                                // 
NvAR_Parameter_Config(MaxTargetsTracked)                                // 
NvAR_Parameter_Config(FullBodyOnly)                                     // 
NvAR_Parameter_Config(PostprocessJointAngle)                            // 
NvAR_Parameter_Config(TargetSeatedPoseForInterpolation)                 // 
NvAR_Parameter_Config(TargetStandPoseForInterpolation)                  // 
                                                                        // 
Input:                                                                  // 
NvAR_Parameter_Input(Image)                                             // 
NvAR_Parameter_Input(BoundingBoxes) - OPTIONAL                          // 
                                                                        // 
Output:                                                                 // 
NvAR_Parameter_Output(BoundingBoxes) - OPTIONAL                         // 
NvAR_Parameter_Output(TrackingBoundingBoxes) - OPTIONAL                 // 
NvAR_Parameter_Output(BoundingBoxesConfidence) - OPTIONAL               // 
NvAR_Parameter_Output(KeyPoints)                                        // 
NvAR_Parameter_Output(KeyPoints3D)                                      // 
NvAR_Parameter_Output(JointAngles)                                      // 
NvAR_Parameter_Output(KeyPointsConfidence) - OPTIONAL                   // 
                                                                        // 
*******NvAR_Feature_GazeRedirection*******                              // 
Config:                                                                 // 
NvAR_Parameter_Config(FeatureDescription)                               // 
NvAR_Parameter_Config(CUDAStream)                                       // 
NvAR_Parameter_Config(ModelDir)                                         // 
NvAR_Parameter_Config(Landmarks_Size)                                   // 
NvAR_Parameter_Config(GazeRedirect)                                     // 
NvAR_Parameter_Config(Temporal)                                         // 
NvAR_Parameter_Config(DetectClosure) - OPTIONAL                         // 
                                                                        // 
Input:                                                                  // 
NvAR_Parameter_Input(Image)                                             // 
NvAR_Parameter_Input(Width)                                             // 
NvAR_Parameter_Input(Height)                                            // 
NvAR_Parameter_Input(Landmarks) - OPTIONAL                              // 
NvAR_Parameter_Input(LandmarksConfidence) - OPTIONAL                    // 
                                                                        // 
Output:                                                                 // 
NvAR_Parameter_Output(OutputGazeVector)                                 // 
NvAR_Parameter_Output(OutputHeadTranslation)                            // 
NvAR_Parameter_Output(HeadPose)                                         // 
NvAR_Parameter_Output(EyeCenters3D)                                     // 
NvAR_Parameter_Output(Image) - OPTIONAL                                 // 
NvAR_Parameter_Output(BoundingBoxes) - OPTIONAL                         // 
NvAR_Parameter_Output(Landmarks) - OPTIONAL                             // 
NvAR_Parameter_Output(Pose) - OPTIONAL                                  // 
NvAR_Parameter_Output(LandmarksConfidence) - OPTIONAL                   // 
                                                                        // 
*******NvAR_Feature_FaceExpressions*******                              // 
Config:                                                                 // 
NvAR_Parameter_Config(FeatureDescription)                               // 
NvAR_Parameter_Config(CUDAStream) - OPTIONAL                            // 
NvAR_Parameter_Config(ModelDir) - OPTIONAL                              // 
NvAR_Parameter_Config(BatchSize) - OPTIONAL                             // 
NvAR_Parameter_Config(Temporal) - OPTIONAL                              // 
NvAR_Parameter_Config(Landmarks_Size) - QUERY                           // 
NvAR_Parameter_Config(ExpressionCount) - QUERY                          // 
                                                                        // 
Input:                                                                  // 
NvAR_Parameter_Input(Image)                                             // 
NvAR_Parameter_Input(Landmarks) - OPTIONAL                              // 
NvAR_Parameter_Input(PoseMode) - OPTIONAL                               // 
NvAR_Parameter_Input(CameraIntrinsicParams) - OPTIONAL                  // 
                                                                        // 
Output:                                                                 // 
NvAR_Parameter_Output(ExpressionCoefficients)                           // 
NvAR_Parameter_Output(Landmarks) - OPTIONAL                             // 
NvAR_Parameter_Output(LandmarksConfidence) - OPTIONAL                   // 
NvAR_Parameter_Output(Pose) - OPTIONAL                                  // 
NvAR_Parameter_Output(PoseTranslation) - OPTIONAL                       // 
NvAR_Parameter_Output(BoundingBoxes) - OPTIONAL                         // 
NvAR_Parameter_Output(BoundingBoxesConfidence) - OPTIONAL               // 





*/
#endif  // NvAR_DEFS_H
