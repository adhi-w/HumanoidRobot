#define TOTAL_FEATURE_NUM 17	//total feature num of the matlab's data structure,not include denoisy

#define KERNEL 0		//0:linear kernel(actually no kernel); 1:Gaussian(Radial base)
#define MODEL_NUM_FEA 12	//how many input feature in model when training(not include predict feature)

class SSVR
{
public:
  SSVR();
  float predictSSVR(int label_feature, float inst[MODEL_NUM_FEA]);
  void normalization(float in_row_data[TOTAL_FEATURE_NUM] ,float out_row_data[MODEL_NUM_FEA]);

  float row_data[TOTAL_FEATURE_NUM];
  float normal_row_data[MODEL_NUM_FEA];
  float predict;
};




