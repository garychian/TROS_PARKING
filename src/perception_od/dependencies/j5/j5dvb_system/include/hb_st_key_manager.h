#ifndef HB_ST_KEY_MANAGER_H
#define HB_ST_KEY_MANAGER_H


#include <sys/types.h>

#include "hb_keymaster.h"

#define LOG_TAG "st_log"

#define  HBST_OK						0	/* No error */

#define  HBST_ERR_PARAM					-1	/* 入参错误 */
#define  HBST_ERR_KEYPATH				-2	/* 文件路径非法 */
#define  HBST_ERR_MALLOC				-3	/* 内存申请失败 */

#define  HBST_ERR_FILEOPEN				-4	/* 文件打开或者创建失败 */
#define  HBST_ERR_DELFILE				-5	/* 删除文件失败 */
#define  HBST_ERR_FILEWR				-6	/* 写文件失败 */
#define  HBST_ERR_FILERD				-7	/* 读文件失败 */
#define HBST_ERR_FILENULL				-8	/* 空文件 */

#define  HBST_ERR_OLDFILE				-9	/* 存储文件时，存储失败，介质中文件恢复为上次的文件 */
#define  HBST_ERR_BADFILE				-10	/* 存储文件时，存储失败，介质中的文件不可用 */
#define  HBST_ERR_ENCRYPT				-11	/* 数据加密失败 */
#define  HBST_ERR_VERIFY				-12	/* 完整验证失败 */

#define  HBST_ERR_CREATDIR				-13	/* 创建目录失败 */
#define  HBST_ERR_MOUNT					-14	/* mount失败 */
#define  HBST_ERR_UMOUNT				-15	/* umount失败 */
#define  HBST_ERR_FILELCK				-16
#define  HBST_ERR_FILEUNLCK				-17


#define  HBST_STORE_ERR_BASE				-10000
#define  HBST_LOAD_ERR_BASE					-10100
#define  HBST_IMPORT_ERR_BASE				-10200
#define  HBST_DELETE_ERR_BASE				-10300
#define  HBST_INIT_ERR_BASE					-10400
#define  HBST_DEINIT_ERR_BASE				-10500

/*
以下函数直接调用libhbkm.so接口，错误码参考hb_keymaster.h
hb_st_key_add
hb_st_skey_read
hb_st_pubkey_read
hb_st_key_invalidate
*/

/*hb_st_key_store*/
#define  HBST_STORE_ERR_GENERIC			HBST_STORE_ERR_BASE	/* General error */
#define  HBST_STORE_ERR_PARAM			HBST_STORE_ERR_BASE + HBST_ERR_PARAM
#define  HBST_STORE_ERR_KEYPATH			HBST_STORE_ERR_BASE + HBST_ERR_KEYPATH
#define  HBST_STORE_ERR_MALLOC			HBST_STORE_ERR_BASE + HBST_ERR_MALLOC
#define  HBST_STORE_ERR_FILEOPEN 		HBST_STORE_ERR_BASE + HBST_ERR_FILEOPEN
#define  HBST_STORE_ERR_DELFILE			HBST_STORE_ERR_BASE + HBST_ERR_DELFILE
#define  HBST_STORE_ERR_FILEWR			HBST_STORE_ERR_BASE + HBST_ERR_FILEWR
#define  HBST_STORE_ERR_FILERD			HBST_STORE_ERR_BASE + HBST_ERR_FILERD
#define  HBST_STORE_ERR_FILENULL		HBST_STORE_ERR_BASE + HBST_ERR_FILENULL

#define  HBST_STORE_ERR_OLDFILE			HBST_STORE_ERR_BASE + HBST_ERR_OLDFILE
#define  HBST_STORE_ERR_BADFILE			HBST_STORE_ERR_BASE + HBST_ERR_BADFILE
#define  HBST_STORE_ERR_VERIFY			HBST_STORE_ERR_BASE + HBST_ERR_VERIFY
#define  HBST_STORE_ERR_ENCRYPT			HBST_STORE_ERR_BASE + HBST_ERR_ENCRYPT
#define  HBST_STORE_ERR_FILELCK			HBST_STORE_ERR_BASE + HBST_ERR_FILELCK
#define  HBST_STORE_ERR_FILEUNLCK		HBST_STORE_ERR_BASE + HBST_ERR_FILEUNLCK

/*hb_st_key_info_read*/
#define  HBST_LOAD_ERR_GENERIC			HBST_LOAD_ERR_BASE	/* General error */
#define  HBST_LOAD_ERR_PARAM			HBST_LOAD_ERR_BASE + HBST_ERR_PARAM
#define  HBST_LOAD_ERR_KEYPATH			HBST_LOAD_ERR_BASE + HBST_ERR_KEYPATH
#define  HBST_LOAD_ERR_MALLOC			HBST_LOAD_ERR_BASE + HBST_ERR_MALLOC
#define  HBST_LOAD_ERR_FILEOPEN 		HBST_LOAD_ERR_BASE + HBST_ERR_FILEOPEN
#define  HBST_LOAD_ERR_DELFILE			HBST_LOAD_ERR_BASE + HBST_ERR_DELFILE
#define  HBST_LOAD_ERR_FILEWR			HBST_LOAD_ERR_BASE + HBST_ERR_FILEWR
#define  HBST_LOAD_ERR_FILERD			HBST_LOAD_ERR_BASE + HBST_ERR_FILERD
#define  HBST_LOAD_ERR_FILENULL			HBST_LOAD_ERR_BASE + HBST_ERR_FILENULL

#define  HBST_LOAD_ERR_OLDFILE			HBST_LOAD_ERR_BASE + HBST_ERR_OLDFILE
#define  HBST_LOAD_ERR_BADFILE			HBST_LOAD_ERR_BASE + HBST_ERR_BADFILE
#define  HBST_LOAD_ERR_VERIFY			HBST_LOAD_ERR_BASE + HBST_ERR_VERIFY
#define  HBST_LOAD_ERR_ENCRYPT			HBST_LOAD_ERR_BASE + HBST_ERR_ENCRYPT
#define  HBST_LOAD_ERR_FILELCK			HBST_LOAD_ERR_BASE + HBST_ERR_FILELCK
#define  HBST_LOAD_ERR_FILEUNLCK		HBST_LOAD_ERR_BASE + HBST_ERR_FILEUNLCK

/* hb_st_key_import */
#define  HBST_IMPORT_ERR_GENERIC		HBST_IMPORT_ERR_BASE	/* General error */
#define  HBST_IMPORT_ERR_PARAM			HBST_IMPORT_ERR_BASE + HBST_ERR_PARAM
#define  HBST_IMPORT_ERR_KEYPATH		HBST_IMPORT_ERR_BASE + HBST_ERR_KEYPATH
#define  HBST_IMPORT_ERR_MALLOC			HBST_IMPORT_ERR_BASE + HBST_ERR_MALLOC
#define  HBST_IMPORT_ERR_FILEOPEN 		HBST_IMPORT_ERR_BASE + HBST_ERR_FILEOPEN
#define  HBST_IMPORT_ERR_DELFILE		HBST_IMPORT_ERR_BASE + HBST_ERR_DELFILE
#define  HBST_IMPORT_ERR_FILEWR			HBST_IMPORT_ERR_BASE + HBST_ERR_FILEWR
#define  HBST_IMPORT_ERR_FILERD			HBST_IMPORT_ERR_BASE + HBST_ERR_FILERD
#define  HBST_IMPORT_ERR_FILENULL			HBST_IMPORT_ERR_BASE + HBST_ERR_FILENULL

#define  HBST_IMPORT_ERR_OLDFILE		HBST_IMPORT_ERR_BASE + HBST_ERR_OLDFILE
#define  HBST_IMPORT_ERR_BADFILE		HBST_IMPORT_ERR_BASE + HBST_ERR_BADFILE
#define  HBST_IMPORT_ERR_VERIFY			HBST_IMPORT_ERR_BASE + HBST_ERR_VERIFY
#define  HBST_IMPORT_ERR_ENCRYPT		HBST_IMPORT_ERR_BASE + HBST_ERR_ENCRYPT
#define  HBST_IMPORT_ERR_FILELCK		HBST_IMPORT_ERR_BASE + HBST_ERR_FILELCK
#define  HBST_IMPORT_ERR_FILEUNLCK		HBST_IMPORT_ERR_BASE + HBST_ERR_FILEUNLCK

/*hb_st_key_delete*/
#define  HBST_DELETE_ERR_GENERIC		HBST_DELETE_ERR_BASE	/* General error */
#define  HBST_DELETE_ERR_PARAM			HBST_DELETE_ERR_BASE + HBST_ERR_PARAM
#define  HBST_DELETE_ERR_KEYPATH		HBST_DELETE_ERR_BASE + HBST_ERR_KEYPATH
#define  HBST_DELETE_ERR_MALLOC			HBST_DELETE_ERR_BASE + HBST_ERR_MALLOC
#define  HBST_DELETE_ERR_FILEOPEN 		HBST_DELETE_ERR_BASE + HBST_ERR_FILEOPEN
#define  HBST_DELETE_ERR_DELFILE		HBST_DELETE_ERR_BASE + HBST_ERR_DELFILE
#define  HBST_DELETE_ERR_FILEWR			HBST_DELETE_ERR_BASE + HBST_ERR_FILEWR
#define  HBST_DELETE_ERR_FILERD			HBST_DELETE_ERR_BASE + HBST_ERR_FILERD
#define  HBST_DELETE_ERR_OLDFILE		HBST_DELETE_ERR_BASE + HBST_ERR_OLDFILE
#define  HBST_DELETE_ERR_BADFILE		HBST_DELETE_ERR_BASE + HBST_ERR_BADFILE
#define  HBST_DELETE_ERR_VERIFY			HBST_DELETE_ERR_BASE + HBST_ERR_VERIFY
#define  HBST_DELETE_ERR_ENCRYPT		HBST_DELETE_ERR_BASE + HBST_ERR_ENCRYPT
/*hb_st_key_init*/
#define HBST_INIT_ERR_GENERIC			HBST_INIT_ERR_BASE
#define HBST_INIT_ERR_PARAM				HBST_INIT_ERR_BASE + HBST_ERR_PARAM
#define HBST_INIT_ERR_CREATDIR			HBST_INIT_ERR_BASE + HBST_ERR_CREATDIR
#define HBST_INIT_ERR_MOUNT				HBST_INIT_ERR_BASE + HBST_ERR_MOUNT
/*hb_st_key_deinit*/
#define HBST_DEINIT_ERR_GENERIC			HBST_DEINIT_ERR_BASE
#define HBST_DEINIT_ERR_CREATDIR		HBST_DEINIT_ERR_BASE + HBST_ERR_CREATDIR
#define HBST_DEINIT_ERR_UMOUNT			HBST_DEINIT_ERR_BASE + HBST_ERR_UMOUNT

#define HB_ST_READ_OP_SPEC_KEY  0xcc
typedef enum {
TEE_TYPE_AES                = 0xA0000010, //CBC/ECB nopad
TEE_TYPE_RSA_PUBLIC_KEY     = 0xA0000030, //RSA2048  pkcs1.5
TEE_TYPE_RSA_KEYPAIR    	= 0xA1000030,
TEE_TYPE_ECDSA_PUBLIC_KEY   = 0xA0000041,
TEE_TYPE_ECDSA_KEYPAIR		= 0xA1000041,
TEE_TYPE_ED25519_PUBLIC_KEY = 0xA0000043,
TEE_TYPE_ED25519_KEYPAIR	= 0xA1000043,
TEE_TYPE_X25519_PUBLIC_KEY	= 0xA0000044,
TEE_TYPE_X25519_KEYPAIR 	= 0xA1000044,
TEE_TYPE_STDATA             = 0xA00000C0,
TEE_TYPE_CMAC_AES           = 0xA00000C1,
} hb_key_type;


typedef struct {
unsigned int uid;
unsigned int gid;
} hb_user_id;

/* 注意该结构体在tee里也使用，指针指向的地址需要在kernel里拷贝到共享内存区 */
typedef struct {
unsigned int version;
/* 暂未使用 */
unsigned int index;
hb_user_id   uergroup;
/* 
用于判断key的来源：
key_src=0表示客户原生的key(非在设备上新生成).
key_src=1表示客户新生成的key(在设备上新生成).
 */
unsigned int key_src;/* 接口都不管 */
/*  
bit0=1:导出时使用cloud_pub_key加密
bit0=0:导出时不使用cloud_pub_key加密.
举例：只有Reset_AES_key 置1；
*/
unsigned int export_type; /*import接口关注，其他不管*/
/* 密钥存储的类型：
bit0=1:key被设备kek加密过, 适用于tee新生成的私钥.
bit0=0:key未被设备kek加密过;适用于tee新生成的公钥.
bit1=1:key被一级密钥(rsa)加密过，适用于二级密钥(aes). 使用security_pri_key(rsa2048私钥)解密.
bit1=0:key未被一级密钥加密过，适用于tee新生成公私钥和客户原生且非二级密钥.
bit2=1:key被二级密钥(aes)加密过,适用于客户原生密钥.使用二级kek AesKey解密.
bit2=0:key未被二级密钥加密过，适用于tee新生公私钥.
bit3=1:使用客户公钥servicePub验证签名.
bit3=0:不使用客户公钥servicePub验证签名.
bit4=1:使用客户根公钥root_pub_key验证签名.
bit4=0:不使用客户根公钥root_pub_key验证签名.


举例：
1 从emmc读出后，注入二级公钥明文servicePub及签名servicePubSign进入tee : key_st_type=0x10=0b10000，需要使用根公钥验证明文签名.
2 从emmc读出后，注入二级kek密文encryptedAesKey及二级kek明文签名aesKeySign进入tee :key_st_type= 0xa = 0b1010，需要使用security_pri_key(rsa2048私钥)解密，使用客户公钥验证明文签名.
3 从emmc读出后，注入CMAC_AES_Key:key_st_type=0xc=0b1100，需要tee使用二级 KEK解密，使用客户公钥验证明文签名.
4 从emmc读出后，注入IOT密钥：key_st_type=0xc=0b1100，需要tee使用二级 KEK解密，使用客户公钥验证明文签名.
5 从emmc读出后，注入TEE新生密钥Encrypt_AES_Key：key_st_type==0x1=0b0001，需要tee使用J5 KEK解密
6 从emmc读出后，注入TEE新生密钥Reset_AES_Key：key_st_type==0x1=0b0001,需要tee使用J5 KEK解密
*/
unsigned int key_st_type;
/* 密钥类型:见hobot_key_type  */
unsigned int key_type;
unsigned int reserved[8];

/* 密钥keysize大小，单位bit,如rsa2048：keysize=2048 */
unsigned int keysize;
/* keydata长度 */
unsigned int keydata_len;

/* 摘要数据长度：如果没有摘要数据则为0 */
unsigned long digest_len;
/* 密钥内容+摘要数据内容：emmc上数据形如: --hb_key_meta_data | keydata | digest； 传二进制数据；
   非对称密钥格式：真正实现时决定ned 、pem or DER？*/
unsigned char payload[0];
} hb_key_meta_data;

key_serial_t hb_st_key_add(const char *type,
	const char *description,
	const void *payload,
	size_t plen,
	key_serial_t ringid);

long hb_st_skey_read(key_serial_t id, char *buffer, size_t buflen);

long hb_st_pubkey_read(key_serial_t id, char *buffer, size_t buflen);

long hb_st_key_spec_read(key_serial_t id, char *buffer, size_t buflen, unsigned char cmd);

long hb_st_key_invalidate(key_serial_t id);

long hb_st_key_store(const char *key_path, hb_key_meta_data *key_data);

long hb_st_key_info_read(const char *key_path, hb_key_meta_data *key_data);

key_serial_t hb_st_key_import(const char *key_path, hb_key_meta_data *key_data);

long hb_st_key_delete(const char *key_path, hb_key_meta_data *key_data);

long hb_st_key_init(const char *dir, const char *dir_bak);

long hb_st_key_deinit(void);

#endif  //HB_ST_KEY_MANAGER_H

