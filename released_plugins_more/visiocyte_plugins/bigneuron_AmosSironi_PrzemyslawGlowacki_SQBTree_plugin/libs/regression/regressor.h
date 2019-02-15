#ifndef _REGRESSOR_H
#define _REGRESSOR_H


//using namespace Eigen;
typedef Eigen::VectorXf VectorTypeFloat;
typedef Eigen::VectorXd VectorTypeDouble;
typedef Eigen::MatrixXd MatrixTypeDouble;
typedef Eigen::MatrixXf MatrixTypeFloat;


typedef SQB::TreeBoosterWeightsType  WeightsType;
typedef float   FeatsType;
typedef Eigen::Array<FeatsType, Eigen::Dynamic, Eigen::Dynamic>     gFeatArrayType;
typedef Eigen::Array<WeightsType, Eigen::Dynamic, 1>                gResponseArrayType;
typedef Eigen::Array<WeightsType, Eigen::Dynamic, 1>                gWeightsArrayType;

typedef itk::Image<float, 4>  ITK4DDistImageType;
typedef itk::Image<float, 3>  ITKFloatImageType;
typedef itk::Image<unsigned char, 3>  ITKBinaryImageType;


typedef SQB::MatrixFeatureIndexList<gFeatArrayType>          MatrixFeatureIndexListType;
typedef SQB::MatrixSampleIndexList<gFeatArrayType>           MatrixSampleIndexListType;
typedef SQB::MatrixFeatureValueObject<gFeatArrayType>        MatrixFeatureValueObjectType;
typedef SQB::MatrixSingleResponseValueObject<gResponseArrayType>   MatrixClassifResponseValueObjectType;

typedef SQB::TreeBooster<
            MatrixSampleIndexListType,
            MatrixFeatureIndexListType,
            MatrixFeatureValueObjectType,
            MatrixClassifResponseValueObjectType >      TreeBoosterType;

void trainRegressor(const gFeatArrayType &all_samples_features,const gResponseArrayType &all_samples_gt,char *regressor_output_file,char * loss_type = "squaredloss",const unsigned int max_boost_iters = 200,const unsigned int max_depth_wl_tree = 0,const double shrink_factor = 0.1,unsigned int m_try =0);

void predictRegressor(const char * regressor_output_file, const gFeatArrayType &all_samples_features, TreeBoosterType::ResponseArrayType &newScores);

template<typename ImageType>
ITKFloatImageType::Pointer binaryGt2ExpDistGt(typename ImageType::Pointer train_gt_radial_ITK,float thresh_distance);

template<typename ImageType>
ITK4DDistImageType::Pointer radialGt2ExpDistGt(typename ImageType::Pointer train_gt_radial_ITK,float thresh_distance,VectorTypeFloat scales, float scale_toll);

template<typename DistImageType>
typename DistImageType::Pointer transformDistGt(typename DistImageType::Pointer dist_gt_image,float thresh_distance);



#endif
