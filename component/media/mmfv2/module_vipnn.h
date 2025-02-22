#ifndef _MODULE_RTKNN_H
#define _MODULE_RTKNN_H

#include <stdint.h>
#include <vip_lite.h>

#define CMD_VIPNN_SET_MODEL_FILE   	MM_MODULE_CMD(0x00)  // set parameter
#define CMD_VIPNN_SET_MODEL_MEM    	MM_MODULE_CMD(0x01)  // set parameter
#define CMD_VIPNN_SET_MODEL_MEM_SZ 	MM_MODULE_CMD(0x02)  // set parameter
#define CMD_VIPNN_SET_MODEL		 	MM_MODULE_CMD(0x03)  // set parameter

#define CMD_VIPNN_SET_PARAMS     	MM_MODULE_CMD(0x04)  // set parameter
#define CMD_VIPNN_SET_WIDTH     	MM_MODULE_CMD(0x05)
#define CMD_VIPNN_SET_HEIGHT     	MM_MODULE_CMD(0x06)
#define CMD_VIPNN_SET_PREPROC     	MM_MODULE_CMD(0x07)
#define CMD_VIPNN_SET_POSTPROC     	MM_MODULE_CMD(0x08)
#define CMD_VIPNN_SET_FPS			MM_MODULE_CMD(0x09)
#define CMD_VIPNN_SET_IN_PARAMS		MM_MODULE_CMD(0x0A)
#define CMD_VIPNN_SET_DISPPOST		MM_MODULE_CMD(0x0B)
#define CMD_VIPNN_SET_SETOBJECT		MM_MODULE_CMD(0x0C)

#define CMD_VIPNN_SET_MODEL_ID		MM_MODULE_CMD(0x0D)

#define CMD_VIPNN_GET_PARAMS     	MM_MODULE_CMD(0x10)  // get parameter
#define CMD_VIPNN_GET_HWVER     	MM_MODULE_CMD(0x11)

#define CMD_VIPNN_SET_OUTPUT     	MM_MODULE_CMD(0x15)

#define CMD_VIPNN_APPLY				MM_MODULE_CMD(0x20)  // for hardware module

#define VIPNN_MODEL_FILE 0
#define VIPNN_MODEL_MEM  1

typedef struct rect_s {
	int xmin;
	int ymin;
	int xmax;
	int ymax;
} rect_t;

//----------------------------------------------------------------------------
typedef struct nn_tensor_format_s {
	int type;
	float scale;
	int zero_point;		//asymmatric	uint8
	int fix_point_pos;	//symmatric		int16
} nn_tensor_format_t;

typedef struct nn_tensor_dim_s {
	int num;
	uint32_t size[6];
} nn_tensor_dim_t;

typedef struct nn_tensor_param_s {
	int count;

	nn_tensor_format_t format[8];
	nn_tensor_dim_t dim[8];
} nn_tensor_param_t;

typedef struct nn_data_param_s {
	union {
		struct nn_img_params_s {
			// img width height order
			int width, height, rgb;
			// roi
			rect_t roi;
		} img;
		struct nn_aud_params_s {
			int dummy;
		} aud;
	};
} nn_data_param_t;


typedef void (*disp_postprcess_t)(void *, void *);
typedef int (*nn_preprocess_t)(void *data_in, nn_data_param_t *data_param, void *tensor_in, nn_tensor_param_t *tensor_param);
typedef void *(*nn_postprocess_t)(void *tensor_out, nn_tensor_param_t *param);
typedef void *(*nn_get_nb_t)(void);
typedef int (*nn_get_nb_size_t)(void);

typedef struct nnmodel_s {
	// user implement
	//void *network_binary;
	nn_get_nb_t 		nb;
	nn_get_nb_size_t 	nb_size;
	nn_preprocess_t 	preprocess;
	nn_postprocess_t 	postprocess;

	// setup by nn modoule
	nn_tensor_param_t input_param;
	nn_tensor_param_t output_param;
} nnmodel_t;
//------------------------------------------------------------------------------

typedef struct vipnn_param_s {
	int model_type;
	char model_file[64];
	uint8_t *model_mem;
	uint32_t model_size;

	int fps;

	int in_width, in_height;
	rect_t roi;

	int m_width, m_height;		// should read from model, not user setting

	nn_data_param_t *in_param;
	nnmodel_t *model;
} vipnn_params_t;

typedef enum {
	VIPNN_INITED = 0,
	VIPNN_APPLIED = 1,
	VIPNN_DEINITED = 2
} vipnn_status_t;

typedef int (*vipnn_preproc_t)(void *, void *, void *, void *, uint32_t);
typedef int (*vipnn_postproc_t)(void *, void *, void *);

#define MAX_IO_NUM              20

typedef struct vipnn_ctx_s {
	void *parent;

	vip_network network;
	vip_buffer_create_params_t vip_param_in[MAX_IO_NUM];
	vip_buffer_create_params_t vip_param_out[MAX_IO_NUM];

	vip_buffer input_buffers[MAX_IO_NUM];
	vip_buffer output_buffers[MAX_IO_NUM];

	vipnn_params_t params;

	vipnn_status_t status;

	int input_count;
	int output_count;

	vipnn_preproc_t pre_process;
	vipnn_postproc_t post_process;


	disp_postprcess_t disp_postproc;

	bool module_out_en;

} vipnn_ctx_t;

#define MAX_DETECT_OBJ_NUM 30
typedef struct objdetect_res_s {
	float result[MAX_DETECT_OBJ_NUM * 6];
	int obj_num;
} objdetect_res_t;

typedef struct vipnn_res_s {
	union {
		objdetect_res_t od_res;
	};
} vipnn_res_t;

typedef struct {
	uint32_t    vipnn_out_tensor_num;
	void        *vipnn_out_tensor[6];
	uint32_t    vipnn_out_tensor_size[6];
	vipnn_res_t vipnn_res;
	uint32_t    quant_format[6];
	union {
		struct {
			vip_int32_t fixed_point_pos;
		} dfp;
		struct {
			vip_float_t        scale;
			vip_int32_t        zeroPoint;
		} affine;
	}
	quant_data[6];
} VIPNN_OUT_BUFFER;

extern mm_module_t vipnn_module;

#endif
