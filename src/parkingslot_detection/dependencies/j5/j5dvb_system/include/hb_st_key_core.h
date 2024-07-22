#ifndef HB_ST_KEY_CORE_H
#define HB_ST_KEY_CORE_H

#include <stdio.h>

#define HB_ST_HEAD_MAGIC 0x6b646174	/*kdat*/

typedef enum {
/* 从tee读出私钥：私钥经过加密, hb_st_skey_read时使用 */
HB_READ_OP_SKEY        	= 0xaa,
/* 从tee读出公钥：公钥未经过加密 hb_st_pubkey_read时使用*/
HB_READ_OP_PUBKEY      	= 0xbb,
/* 直接(非从安全存储区)导入私钥进tee :直接导入key明文，暂时未实现,预留 */
HB_WR_ST_KEY        	= 0xc0,
 /* 从安全存储区读出客户key数据后导入tee ：hb_st_key_import时使用 */
HB_WR_ST_KDATA        	= 0xc1,
/* 从安全存储区读取数据，导入tee解密且验证完整性：hb_st_key_info_read 时使用*/
HB_RD_ST_DATA_HEAD   	= 0xc2,
/* 向安全存储区写入数据，导入tee摘要且加密 ，hb_st_key_store时使用 */
HB_WR_ST_DATA           = 0xc3,
} hb_st_cmd;


typedef struct {
unsigned int magic; //0x kdat
/* 见:hb_st_cmd */
unsigned int cmd;
unsigned int key_type;
/* 导入客户kek功能专用:客户索引默认0：索引客户kek */
unsigned int user_index;
/* 当前默认值：0. 
  bit0=0:tee需要设备kek加解密payload;
  bit0=1:tee不需要设备kek加解密payload; 
*/
unsigned int kek_op;
/* hb_st_key_import接口调用时:保存hb_key_meta_data类型入参;
hb_st_key_store接口调用时:保存从emmc上读取得key;
hb_st_key_info_read接口调用时，保存从emmc上读取得key;
注意tee返回更新后的数据也保存在此 */
unsigned char *payload;
/* payload长度，注意更新后数据长度也保存在此 */
unsigned int payload_len;
/* tee处理后，需要返回到用户空间的地址 */
unsigned char *outdata;
/* tee处理后，需要返回到用户空间的数据长度 
1 hb_st_key_store调用时：
out_len= hb_key_meta_data->key_len+ hb_key_meta_data->digest_len+sizeof(hb_key_meta_data)+32（sha256）+32(对齐pad);
2 hb_st_key_info_read调用时：
out_len= sizeof(hb_key_meta_data);
3 hb_st_key_import调用时：
out_len= 0;
*/
unsigned int out_len;
unsigned int reserved[8];
} hb_st_key_data;

#endif  //HB_ST_KEY_CORE_H

