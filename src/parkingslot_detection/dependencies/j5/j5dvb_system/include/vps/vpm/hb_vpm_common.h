/***************************************************************************
 * COPYRIGHT NOTICE
 * Copyright 2019 Horizon Robotics, Inc.
 * All rights reserved.
 ***************************************************************************/

#ifndef J5_VPM_INC_HB_VPM_COMMON_H_
#define J5_VPM_INC_HB_VPM_COMMON_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define HB_VIO_NO_ERR                   0

//VIO ERR INFO
#define HB_VIO_PAESER_FAIL              (301)
#define HB_VIO_OPEN_CFG_FAIL            (302)
#define HB_VIO_INIT_FAIL                (303)
#define HB_VIO_START_FAIL               (304)
#define HB_VIO_STOP_FAIL                (305)
#define HB_VIO_PIPELINE_ID_INVALID      (306)
#define HB_VIO_NULL_POINTER             (307)
#define HB_VIO_INVALID_CONFIG           (309)
#define HB_VIO_INVALID_OPERATION        (310)
#define HB_VIO_NOT_SUPPORT              (312)
#define HB_VIO_CFG_FILE_NULL            (313)
#define HB_VIO_NO_MEMORY_FAIL           (314)
#define HB_VIO_PIPELINE_NULL_POINTER    (315)
#define HB_VIO_INVALID_DATATYPE         (316)
#define HB_VIO_SET_INFO_NULL            (317)
#define HB_VIO_GET_INFO_NULL            (318)
#define HB_VIO_INVALID_INFO_TYPE        (319)
#define HB_VIO_JSON_CFG_PIPELINE_NULL   (320)
#define HB_VIO_PIPE_ENTITY_NULL_POINTER (321)
#define HB_VIO_BUF_NULL_POINTER         (322)
#define HB_VIO_JSON_ROOT_NULL_POINTER   (323)


//CIM ERR INFO
#define HB_VIO_CIM_OPEN_DEV_FAIL       (100)
#define HB_VIO_CIM_INIT_FAIL           (101)
#define HB_VIO_CIM_DEINIT_FAIL         (102)
#define HB_VIO_CIM_UPDATE_FAIL         (103)
#define HB_VIO_CIM_STOP_FAIL           (104)
#define HB_VIO_CIM_START_FAIL          (105)
#define HB_VIO_CIM_PARSER_FAIL         (106)
#define HB_VIO_CIM_EPOLL_CREATE_FAIL   (107)
#define HB_VIO_CIM_EPOLL_CTL_FAIL      (108)
#define HB_VIO_CIM_EPOLL_WAIT_FAIL     (109)
#define HB_VIO_CIM_STOP_WORKING        (110)
#define HB_VIO_CIM_BAD_VALUE           (111)
#define HB_VIO_CIM_TIME_OUT            (112)
#define HB_VIO_CIM_INVALID_OPERATION   (113)
#define HB_VIO_CIM_INVALID_CONFIG      (114)
#define HB_VIO_CIM_NULL_POINT          (115)
#define HB_VIO_CIM_BUF_MGR_FAIL        (116)

//DIS ERR INFO
#define HB_VIO_DIS_OPEN_DEV_FAIL       (200)
#define HB_VIO_DIS_INIT_FAIL           (201)
#define HB_VIO_DIS_DEINIT_FAIL         (202)
#define HB_VIO_DIS_UPDATE_FAIL         (203)
#define HB_VIO_DIS_STOP_FAIL           (204)
#define HB_VIO_DIS_START_FAIL          (205)
#define HB_VIO_DIS_PARSER_FAIL         (206)
#define HB_VIO_DIS_EPOLL_CREATE_FAIL   (207)
#define HB_VIO_DIS_EPOLL_CTL_FAIL      (208)
#define HB_VIO_DIS_EPOLL_WAIT_FAIL     (209)
#define HB_VIO_DIS_STOP_WORKING        (210)
#define HB_VIO_DIS_BAD_VALUE           (211)
#define HB_VIO_DIS_TIME_OUT            (212)
#define HB_VIO_DIS_INVALID_OPERATION   (213)
#define HB_VIO_DIS_INVALID_CONFIG      (214)

//ISP ERR INFO
#define HB_VIO_ISP_OPEN_DEV_FAIL                           (5000)
#define HB_VIO_ISP_INVALID_DATATYPE                        (5001)
#define HB_VIO_ISP_FREE_DATA_INPUT_BUF_NULL                (5002)    //temple unused
#define HB_VIO_ISP_GET_DATA_INPUT_BUF_NULL                 (5003)    //temple unused
#define HB_VIO_ISP_FEEDBACK_BUF_NULL                       (5004)    //temple unused
#define HB_VIO_ISP_GET_BUF_FAIL                            (5006)
#define HB_VIO_ISP_FREE_BUF_FAIL                           (5007)
#define HB_VIO_ISP_BAD_HW_ID                               (5008)
#define HB_VIO_ISP_BAD_CTX_ID                              (5009)
#define HB_VIO_ISP_BAD_SCHED_MODE                          (5010)
#define HB_VIO_ISP_FRAME_DROP                              (5011)
#define HB_VIO_ISP_DEINIT_FAIL                             (5012)
#define HB_VIO_ISP_INIT_FAIL                               (5013)
#define HB_VIO_ISP_INVALID_CONFIG                          (5014)
#define HB_VIO_ISP_STOP_FAIL                               (5015)
#define HB_VIO_ISP_START_FAIL                              (5016)
#define HB_VIO_ISP_STOP_WORKING                            (5017)
#define HB_VIO_ISP_UPDATE_FAIL                             (5018)
#define HB_VIO_ISP_NULL_POINTER                            (5019)
#define HB_VIO_ISP_ILLEGAL_PARAM                           (5020)
#define HB_VIO_ISP_NOT_SUPPORT                             (5021)
#define HB_VIO_ISP_NOT_INIT                                (5022)
#define HB_VIO_ISP_SNS_UNREGISTER                          (5023)
#define HB_VIO_ISP_INVALID_ADDR                            (5024)
#define HB_VIO_ISP_NOMEM                                   (5025)
#define HB_VIO_ISP_INVAILD_VALUE                           (5026)
#define HB_VIO_ISP_TIMEOUT                                 (5027)
#define HB_VIO_ISP_SET_OUTPUT_ATTR_FAIL                    (5028)
#define HB_VIO_ISP_SET_PUB_ATTR_FAIL                       (5029)
#define HB_VIO_ISP_PIPE_INIT_FAIL                          (5030)
#define HB_VIO_ISP_GET_PUB_ATTR_FAIL                       (5031)
#define HB_VIO_ISP_CHECK_PUB_ATTR_FAIL                     (5033)
#define HB_VIO_ISP_CALIB_NULL_POINTER                      (5034)
#define HB_VIO_ISP_JSON_ROOT_NULL_POINTER                  (5035)
#define HB_VIO_ISP_CFG_NULL_POINTER                        (5036)
#define HB_VIO_ISP_PIEP_STOP_FAIL                          (5037)
#define HB_VIO_ISP_ALGO_STOP_FAIL                          (5038)
#define HB_VIO_ISP_IMAGE_INFO_NULL                         (5039)
#define HB_VIO_ISP_ALGO_START_FAIL                         (5040)
#define HB_VIO_ISP_BUF_NULL                                (5042)
#define HB_VIO_ISP_IMAGE_FORMAT_INVALID                    (5043)

#define HB_VIO_ISP_LOOP_BACK                               (5300)


//PYM ERR INFO
#define  HB_VIO_PYM_OPEN_DEV_FAIL                          (6000)
#define  HB_VIO_PYM_PARSER_FAIL                            (6006)
#define  HB_VIO_PYM_TIME_OUT                               (6013)
#define  HB_VIO_PYM_INVALID_CONFIG                         (6015)
#define  HB_VIO_PYM_NULL_POINTER                           (6016)
#define  HB_VIO_PYM_BUF_MGR_NULL                           (6019)
#define  HB_VIO_PYM_ALLOC_BUF_FAIL                         (6020)
#define  HB_VIO_PYM_BUF_MAP_FAIL                           (6021)
#define  HB_VIO_PYM_RUN_INVALID_BUF                        (6025)
#define  HB_VIO_PYM_GET_BUF_FAIL                           (6028)
#define  HB_VIO_PYM_FREE_BUF_FAIL                          (6029)
#define  HB_VIO_PYM_SET_PARAMETER_FAIL                     (6030)
#define  HB_VIO_PYM_FLUSH_BUF_FAIL                         (6031)
#define  HB_VIO_PYM_INVALID_CACHE_FAIL                     (6032)
#define  HB_VIO_PYM_INVALID_DATATYPE                       (6033)
#define  HB_VIO_PYM_NO_MEMORY_FAIL                         (6034)
#define  HB_VIO_PYM_BUF_MGR_INIT_FAIL                      (6035)
#define  HB_VIO_PYM_BUF_UNMAP_FAIL                         (6036)
#define  HB_VIO_PYM_START_STREAM_SRC_FAIL                  (6037)
#define  HB_VIO_PYM_START_STREAM_CAP_FAIL                  (6038)
#define  HB_VIO_PYM_STOP_STREAM_SRC_FAIL                   (6039)
#define  HB_VIO_PYM_STOP_STREAM_CAP_FAIL                   (6040)
#define  HB_VIO_PYM_CFG_NULL                               (6041)
#define  HB_VIO_PYM_BUF_INDEX_INVALID                      (6042)
#define  HB_VIO_PYM_BUF_INFO_NULL                          (6043)
#define  HB_VIO_PYM_IO_FLUSH_FRAME_FAIL                    (6044)
#define  HB_VIO_PYM_FD_INVALID                             (6045)
#define  HB_VIO_PYM_BUF_NULL                               (6046)
#define  HB_VIO_PYM_CFG_JSON_ROOT_NULL                     (6047)
#define  HB_VIO_PYM_IMG_INFO_NULL                          (6048)
#define  HB_VIO_PYM_SRC_BIND_GROUP_FAIL                    (6049)
#define  HB_VIO_PYM_CAP_BIND_GROUP_FAIL                    (6050)
#define  HB_VIO_PYM_SRC_IOC_INIT_FAIL                      (6051)
#define  HB_VIO_PYM_CAP_IOC_INIT_FAIL                      (6052)




//DEV IOTCL ERR INFO
#define  HB_VIO_BUF_INFO_NULL                              (6500)
#define  HB_VIO_FRAME_INFO_NULL                            (6501)
#define  HB_VIO_FD_INVALID                                 (6502)
#define  HB_VIO_POLL_ERROR                                 (6503)
#define  HB_VIO_POLL_TIMEOUT                               (6504)
#define  HB_VIO_POLL_FRAME_DROP                            (6505)
#define  HB_VIO_PLANE_COUNT_INVALID                        (6506)
#define  HB_VIO_PYM_PULL_HUP                               (6507)
#define  HB_VIO_MANAGER_NULL                               (6508)
#define  HB_VIO_BUF_NULL                                   (6509)
#define  HB_VIO_BUF_TYPE_INVALID                           (6510)
#define  HB_VIO_BUF_NO_MARK_IN_USER_QUEUE                  (6511)
#define  HB_VIO_GET_PEEK_BUF_NULL                          (6512)


//GDC ERR INFO
#define  HB_VIO_GDC_OPEN_DEV_FAIL            		       (7000)
#define  HB_VIO_GDC_UPDATE_FAIL              		       (7003)
#define  HB_VIO_GDC_BAD_VALUE                		       (7012)
#define  HB_VIO_GDC_NULL_POINTER               		       (7013)
#define  HB_VIO_GDC_PROCESS_FAIL             		       (7014)
#define  HB_VIO_GDC_INVALID_CONFIG           		       (7016)
#define  HB_VIO_GDC_SET_CFG_NULL             		       (7020)
#define  HB_VIO_GDC_SET_CFG_SIZE_INVALID     		       (7021)
#define  HB_VIO_GDC_RUN_SRC_BUF_NULL         		       (7022)
#define  HB_VIO_GDC_RUN_INVALID_DEGREE       		       (7024)
#define  HB_VIO_GDC_RUN_INVALID_BUF          		       (7025)
#define  HB_VIO_GDC_RUN_ADV_CFG_NULL         		       (7026)
#define  HB_VIO_GDC_GEN_BAD_VALUE            		       (7027)
#define  HB_VIO_GDC_GEN_NULL_POINTER           		       (7028)
#define  HB_VIO_GDC_GEN_INIT_FAIL            		       (7029)
#define  HB_VIO_GDC_ALLOC_BUF_FAIL           		       (7030)
#define  HB_VIO_GDC_INVALID_DATATYPE         		       (7032)
#define  HB_VIO_GDC_GET_BUF_FAIL    		               (7033)
#define  HB_VIO_GDC_FREE_BUF_FAIL            		       (7034)
#define  HB_VIO_GDC_ALLOC_BIN_FAIL           		       (7035)
#define  HB_VIO_GDC_CFG_BIN_NULL             		       (7036)
#define  HB_VIO_GDC_INVALID_HW_ID            		       (7037)
#define  HB_VIO_GDC_RUN_DST_BUF_NULL         		       (7038)
#define  HB_VIO_GDC_GEN_WIN_NUM_ZERO         		       (7039)
#define  HB_VIO_GDC_GEN_PARAM_NULL           		       (7040)
#define  HB_VIO_GDC_GEN_WINDOW_NULL          		       (7041)
#define  HB_VIO_GDC_GEN_CFG_NULL             		       (7042)
#define  HB_VIO_GDC_GEN_CFG_SIZE_NULL        		       (7043)
#define  HB_VIO_GDC_GEN_INVALID_PARAMETER    		       (7044)
#define  HB_VIO_GDC_FORCE_STOP_FAIL                        (7047)
#define  HB_VIO_GDC_HANDLE_NULL_POINTER                    (7048)
#define  HB_VIO_GDC_BUF_MANAGER_NULL_POINTER               (7049)
#define  HB_VIO_GDC_GET_OUTPUT_BUF_FAIL                    (7050)
#define  HB_VIO_GDC_BUF_NULL                               (7051)
#define  HB_VIO_GDC_BUF_PLANE_COUNT_INVALID                (7052)
#define  HB_VIO_GDC_JSON_ROOT_NULL_POINTER                 (7054)
#define  HB_VIO_GDC_CFG_NULL_POINTER               		   (7055)
#define  HB_VIO_GDC_ALLOC_BINARY_FAIL           		   (7056)
#define  HB_VIO_GDC_ALLOC_FEEDBACK_BUF_FAIL           	   (7057)
#define  HB_VIO_GDC_INVALID_USEER_MODE           	       (7059)
#define  HB_VIO_GDC_SRC_BIND_GROUP_FAIL                    (7060)


//temple unused , to be removed
#define  HB_VIO_TIME_OUT                                   (308)    //temple unused , to be removed
#define  HB_VIO_BUF_MGR_FAIL                               (311)    //temple unused , to be removed
#define  HB_VIO_BAD_VALUE                                  (321)    //temple unused , to be removed
#define  HB_VIO_PYM_INIT_FAIL                              (6001)   //temple unused , to be removed
#define  HB_VIO_PYM_DEINIT_FAIL                            (6002)   //temple unused , to be removed
#define  HB_VIO_PYM_UPDATE_FAIL                            (6003)   //temple unused , to be removed
#define  HB_VIO_PYM_STOP_FAIL                              (6004)   //temple unused , to be removed
#define  HB_VIO_PYM_START_FAIL                             (6005)   //temple unused , to be removed
#define  HB_VIO_PYM_IS_BUSY                                (6007)   //temple unused , to be removed
#define  HB_VIO_PYM_EPOLL_CREATE_FAIL                      (6008)   //temple unused , to be removed
#define  HB_VIO_PYM_EPOLL_CTL_FAIL                         (6009)   //temple unused , to be removed
#define  HB_VIO_PYM_EPOLL_WAIT_FAIL                        (6010)   //temple unused , to be removed
#define  HB_VIO_PYM_STOP_WORKING                           (6011)   //temple unused , to be removed
#define  HB_VIO_PYM_BAD_VALUE                              (6012)   //temple unused , to be removed
#define  HB_VIO_PYM_INVALID_OPERATION                      (6014)   //temple unused , to be removed
#define  HB_VIO_PYM_BUF_MGR_FAIL                           (6017)   //temple unused , to be removed
#define  HB_VIO_PYM_FREE_DATA_INPUT_BUF_NULL               (6022)   //temple unused , to be removed
#define  HB_VIO_PYM_GET_DATA_INPUT_BUF_NULL                (6023)   //temple unused , to be removed
#define  HB_VIO_PYM_RUN_BUF_NULL                           (6024)   //temple unused , to be removed
#define  HB_VIO_PYM_FRAME_DROP                             (6027)   //temple unused , to be removed
#define  HB_VIO_PYM_RUN_WRONGSTATE                         (6026)   //temple unused , to be removed
#define  HB_VIO_GDC_INIT_FAIL                		       (7001)   //temple unused , to be removed
#define  HB_VIO_GDC_DEINIT_FAIL              		       (7002)   //temple unused , to be removed
#define  HB_VIO_GDC_STOP_FAIL                		       (7004)   //temple unused , to be removed
#define  HB_VIO_GDC_START_FAIL               		       (7005)   //temple unused , to be removed
#define  HB_VIO_GDC_PARSER_FAIL              		       (7006)   //temple unused , to be removed
#define  HB_VIO_GDC_EPOLL_CREATE_FAIL        		       (7007)   //temple unused , to be removed
#define  HB_VIO_GDC_EPOLL_CTL_FAIL           		       (7008)   //temple unused , to be removed
#define  HB_VIO_GDC_EPOLL_WAIT_FAIL          		       (7009)   //temple unused , to be removed
#define  HB_VIO_GDC_STOP_WORKING             		       (7010)   //temple unused , to be removed
#define  HB_VIO_GDC_BUF_MGR_FAIL             		       (7011)   //temple unused , to be removed
#define  HB_VIO_GDC_GET_STATUS_FAIL          		       (7015)   //temple unused , to be removed
#define  HB_VIO_GDC_FREE_DATA_INPUT_BUF_NULL 		       (7017)   //temple unused , to be removed
#define  HB_VIO_GDC_GET_FEEDBACK_BUF_INPUT_BUF_NULL        (7018)   //temple unused , to be removed
#define  HB_VIO_GDC_WRONGSTATE_SETCFG        		       (7019)   //temple unused , to be removed
#define  HB_VIO_GDC_GET_DATA_INPUT_BUF_NULL  		       (7045)   //temple unused , to be removed
#define  HB_VIO_GDC_FRAME_DROP                             (7046)   //temple unused , to be removed
#define  HB_VIO_GDC_GET_FEEDBACK_BUF_FAIL    		       (7047)   //temple unused , to be removed
#define  HB_VIO_GDC_OPEN_NODE_FAIL           		       (7031)   //temple unused , to be removed

//LDC ERR INFO
#define  HB_VIO_LDC_OPEN_DEV_FAIL      (600)
#define  HB_VIO_LDC_INIT_FAIL          (601)
#define  HB_VIO_LDC_DEINIT_FAIL        (602)
#define  HB_VIO_LDC_UPDATE_FAIL        (603)
#define  HB_VIO_LDC_STOP_FAIL          (604)
#define  HB_VIO_LDC_START_FAIL         (605)
#define  HB_VIO_LDC_PARSER_FAIL        (606)
#define  HB_VIO_LDC_STOP_WORKING       (607)
#define  HB_VIO_LDC_BAD_VALUE          (608)
#define  HB_VIO_LDC_TIME_OUT           (609)
#define  HB_VIO_LDC_INVALID_OPERATION  (610)
#define  HB_VIO_LDC_INVALID_CONFIG     (611)

//SIF ERR INFO
#define  HB_VIO_SIF_OPEN_DEV_FAIL      (700)
#define  HB_VIO_SIF_INIT_FAIL          (701)
#define  HB_VIO_SIF_UPDATE_FAIL        (702)
#define  HB_VIO_SIF_STOP_FAIL          (703)
#define  HB_VIO_SIF_START_FAIL         (704)
#define  HB_VIO_SIF_PARSER_FAIL        (705)
#define  HB_VIO_SIF_EPOLL_CREATE_FAIL  (706)
#define  HB_VIO_SIF_EPOLL_CTL_FAIL     (707)
#define  HB_VIO_SIF_EPOLL_WAIT_FAIL    (708)
#define  HB_VIO_SIF_STOP_WORKING       (709)
#define  HB_VIO_SIF_DEINIT_FAIL        (710)
#define  HB_VIO_SIF_BAD_VALUE          (711)
#define  HB_VIO_SIF_TIME_OUT           (712)
#define  HB_VIO_SIF_INVALID_OPERATION  (713)
#define  HB_VIO_SIF_INVALID_CONFIG     (714)
#define  HB_VIO_SIF_NULL_POINT         (715)
#define  HB_VIO_SIF_BUF_MGR_FAIL       (716)

//IPU ERR INFO
#define  HB_VIO_IPU_INIT_FAIL          (800)
#define  HB_VIO_IPU_DEINIT_FAIL        (801)
#define  HB_VIO_IPU_START_FAIL         (802)
#define  HB_VIO_IPU_STOP_FAIL          (803)
#define  HB_VIO_IPU_PARSER_FAIL        (804)
#define  HB_VIO_IPU_EPOLL_CREATE_FAIL  (805)
#define  HB_VIO_IPU_EPOLL_CTL_FAIL     (806)
#define  HB_VIO_IPU_EPOLL_WAIT_FAIL    (807)
#define  HB_VIO_IPU_STOP_WORKING       (808)
#define  HB_VIO_IPU_OPEN_DEV_FAIL      (809)
#define  HB_VIO_IPU_UPDATE_FAIL        (810)
#define  HB_VIO_IPU_BAD_VALUE          (811)
#define  HB_VIO_IPU_TIME_OUT           (812)
#define  HB_VIO_IPU_INVALID_OPERATION  (813)
#define  HB_VIO_IPU_INVALID_CONFIG     (814)
#define  HB_VIO_IPU_NULL_POINT         (815)
#define  HB_VIO_IPU_CHN_NOT_ENABLE     (816)
#define  HB_VIO_IPU_OPERATE_OSD_FAIL   (817)
#define  HB_VIO_IPU_BUF_MGR_FAIL       (818)
#define  HB_VIO_IPU_OSD_OPEN_FILE_FAIL (819)
#define  HB_VIO_IPU_ERR_NOT_SUPPORT    (820)

//DWE ERR INFO
#define  HB_VIO_DWE_NULL_POINT         (900)
#define  HB_VIO_DWE_PARSER_FAIL        (901)
#define  HB_VIO_DWE_BAD_VALUE          (902)
#define  HB_VIO_DWE_OPEN_DEV_FAIL      (903)
#define  HB_VIO_DWE_INIT_FAIL          (904)
#define  HB_VIO_DWE_DEINIT_FAIL        (905)
#define  HB_VIO_DWE_START_FAIL         (906)
#define  HB_VIO_DWE_STOP_FAIL          (907)



#ifdef __cplusplus
}
#endif

#endif// J5_VPM_INC_HB_VPM_COMMON_H_
