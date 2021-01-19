/*
 * @Description: 代码框架
 * @Version: V1.0.0
 * @Author: caojq
 * @Date: 2020-10-26 16:11:54
 * @LastEditTime: 2020-10-28 16:16:35
 * @LastEditors:  
 * @Reference: 单插功能；
 */

#define __TUYA_DEVICE_GLOBAL
#include "tuya_device.h"

#include "tuya_gpio.h"
#include "uni_log.h"
#include "tuya_cloud_types.h"
#include "tuya_iot_com_api.h"
#include "tuya_cloud_wifi_defs.h"
#include "tuya_hal_system.h"
#include "tuya_iot_com_api.h"
#include "tuya_iot_wifi_api.h"

#include "tuya_hard_table.h"
#include "gw_intf.h"

#include "tuya_led.h"

#define  WIFI_CONFIG_MODE_SEL    GWCM_LOW_POWER      
 
 LED_HANDLE wifi_led;

STATIC VOID key_process(TY_GPIO_PORT_E port,PUSH_KEY_TYPE_E type,INT_T cnt);

/**
 * @Function: gpio_test
 * @Description: 模块引脚初始化测试
 * @Input: in:测试引脚序列
 * @Output: out:测试引脚
 * @Return: TRUE:测试通过；FALSE:测试失败
 * @Others: none
 */
BOOL_T gpio_test(IN CONST CHAR_T *in, OUT CHAR_T *out)
{
    return gpio_test_all(in,out);
}

/**
 * @Function: mf_user_callback
 * @Description: 授权回调函数
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 清空flash中存储的数据
 */
VOID mf_user_callback(VOID)
{
    //Clear all stored data in flash
//    #error ""
}

/**
 * @Function: pre_device_init
 * @Description: 设备信息(SDK信息、版本号、固件标识等)打印、重启原因和打印等级设置
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID pre_device_init(VOID)
{
    PR_DEBUG("%s",tuya_iot_get_sdk_info());
    PR_DEBUG("%s:%s",APP_BIN_NAME,DEV_SW_VERSION);
    PR_NOTICE("firmware compiled at %s %s",__DATE__,__TIME__);
    PR_NOTICE("system reset reason:[%s]",tuya_hal_system_get_rst_info());
    SetLogManageAttr(TY_LOG_LEVEL_DEBUG);                //打印等级设置
}

/**
 * @Function:tuya_device_config 
 * @Description: 设备硬件信息注册
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID tuya_device_config(VOID)
{
    DEVICE_PARAM_S dev_param;

#if 0
    /*开发者根据硬件信息的IO口进行设置*/
    dev_param.wifi_mode = GWCM_LOW_POWER;
    dev_param.wifi_led.io_pin = TY_GPIOA_9;
    dev_param.wifi_led.io_level = IO_LOW_LEVEL;
    dev_param.key_control.io_config.io_pin = TY_GPIOA_7;
    dev_param.key_control.io_config.io_level = IO_LOW_LEVEL;
    dev_param.key_control.press_time = 3000;
    dev_param.key_control.key_call_back = key_process;

    dev_param.relay.io_pin = TY_GPIOA_6;
    dev_param.relay.io_level = IO_HIGH_LEVEL;
    dev_param.dev_dp.switch_dp.is_exist = TRUE;
    dev_param.dev_dp.switch_dp.dpid_num = 1;
    dev_param.dev_dp.switch_cd_dp.is_exist = TRUE;
    dev_param.dev_dp.switch_cd_dp.dpid_num = 9;
    dev_param.dev_dp.app_relay_stat.is_exist = TRUE;
    dev_param.dev_dp.app_relay_stat.dpid_num = 38;
    dev_param.relay_led.io_pin = TY_GPIOA_8;
    dev_param.relay_led.io_level = IO_LOW_LEVEL;

    tuya_hard_init(&dev_param);
#endif

//OPERATE_RET tuya_create_led_handle_select(IN CONST TY_GPIO_PORT_E port,IN CONST BOOL_T high,OUT LED_HANDLE *handle)
#if 0
        tuya_set_led_light_type(led_param->io_handle,IO_ACTIVE_FLASH_EN(led_param->io_config.io_level),led_time,LED_TIMER_UNINIT);
        PR_DEBUG("led flash!!");
    }
    else if (led_type == LED_ON_OFF)
    {
        PR_DEBUG("led on off!");
        if(led_time == 0)
        {
            tuya_set_led_light_type(led_param->io_handle,IO_LED_TIME_ACTIVE_DIS(led_param->io_config.io_level),0,0);
            PR_DEBUG("led off!");
        }
        else
        {
            tuya_set_led_light_type(led_param->io_handle,IO_LED_TIME_ACTIVE_EN(led_param->io_config.io_level),0,0);
#endif
    OPERATE_RET op_ret = OPRT_OK;
    op_ret = tuya_create_led_handle_select(TY_GPIOA_9, FALSE, &wifi_led);
    if(OPRT_OK != op_ret) 
    {
        PR_ERR("tuya_create_led_handle_select fail,fail_num:%d",op_ret);
        return;
    }

    op_ret = key_init(NULL,0,20);
    if (OPRT_OK != op_ret)
    {
        PR_ERR("key_init fail,fail_num:%d",op_ret);
        return op_ret;
    }

}

/**
 * @Function: prod_test
 * @Description: 扫描到产测热点，进入回调函数，主要是按键、指示灯、继电器功能测试
 * @Input: flag:授权标识；rssi:信号强度
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID prod_test(BOOL_T flag, SCHAR_T rssi)
{

    if (flag == FALSE || rssi < -60) 
    {
        //Prod test error message
        PR_ERR("Prod test failed... flag:%d, rssi:%d", flag, rssi);
//      #error ""
        return;
    }
    //input product test mode 
    PR_NOTICE("flag:%d rssi:%d", flag, rssi);
//  #error ""
}

/**
 * @Function: app_init
 * @Description: 设备初始化，设置工作模式
 * @Input: none
 * @Output: none
 * @Return: none
 * @Others: 无
 */
VOID app_init(VOID)
{
    tuya_device_config();
    app_cfg_set(WIFI_CONFIG_MODE_SEL, prod_test);
    tuya_iot_wf_timeout_set(180);
}

/**
 * @Function: status_changed_cb
 * @Description: gw status changed callback
 * @Input: status: current status
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID gw_status_cb(IN CONST GW_STATUS_E status)
{
    PR_NOTICE("gw_status_cb status_type:%d",status);
    if (GW_NORMAL == status)
    {
        //synchronous device status, update all data 
//      #error ""

    }
}

/**
 * @Function: upgrade_notify_cb
 * @Description: firmware download finish result callback
 * @Input: fw: firmware info
 * @Input: download_result: 0 means download succes. other means fail
 * @Input: pri_data: private data
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID upgrade_notify_cb(IN CONST FW_UG_S *fw, IN CONST INT_T download_result, IN PVOID_T pri_data)
{
    PR_DEBUG("download  Finish");
    PR_DEBUG("download_result:%d", download_result);
}

/**
 * @Function: get_file_data_cb
 * @Description: firmware download content process callback
 * @Input: fw: firmware info
 * @Input: total_len: firmware total size
 * @Input: offset: offset of this download package
 * @Input: data && len: this download package
 * @Input: pri_data: private data
 * @Output: remain_len: the size left to process in next cb
 * @Return: OPRT_OK: success  Other: fail
 * @Others: none
 */
OPERATE_RET get_file_data_cb(IN CONST FW_UG_S *fw, IN CONST UINT_T total_len, IN CONST UINT_T offset,
                                     IN CONST BYTE_T *data, IN CONST UINT_T len, OUT UINT_T *remain_len, IN PVOID_T pri_data)
{
    PR_DEBUG("Rev File Data");
    PR_DEBUG("Total_len:%d ", total_len);
    PR_DEBUG("Offset:%d Len:%d", offset, len);

    return OPRT_OK;
}

/**
 * @Function: gw_ug_cb
 * @Description: gateway ota firmware available nofity callback
 * @Input: fw: firmware info
 * @Output: none
 * @Return: int:
 * @Others: 
 */
int gw_ug_cb(IN CONST FW_UG_S *fw)
{
    PR_DEBUG("Rev GW Upgrade Info");
    PR_DEBUG("fw->fw_url:%s", fw->fw_url);
    PR_DEBUG("fw->sw_ver:%s", fw->sw_ver);
    PR_DEBUG("fw->file_size:%d", fw->file_size);

    return tuya_iot_upgrade_gw(fw, get_file_data_cb, upgrade_notify_cb, NULL);
}

/**
 * @Function: gw_reset_cb
 * @Description: gateway restart callback
 * @Input: type:gateway reset type
 * @Output: none
 * @Return: none
 * @Others: reset factory clear flash data
 */
VOID gw_reset_cb(GW_RESET_TYPE_E type)
{
    if (GW_REMOTE_RESET_FACTORY != type)
    {
        return;
    }

    /*clear falsh data*/
//    tuya_hard_clear_flash_data();
//      #error ""
    return;
}


/**
 * @Function: dev_obj_dp_cb
 * @Description: obj dp info cmd callback
 * @Input: dp:obj dp info
 * @Output: none
 * @Return: none
 * @Others: app send data by dpid  control device stat
 */
VOID dev_obj_dp_cb(IN CONST TY_RECV_OBJ_DP_S *dp)
{
    PR_NOTICE("recv dp cmd_type:%d trans_type:%d cnt:%d", dp->cmd_tp, dp->dtt_tp, dp->dps_cnt);
    OPERATE_RET op_ret = OPRT_OK;

    UINT_T index = 0;
    for (index = 0; index < dp->dps_cnt; index ++)
    {
        if(PROP_STR == dp->dps[index].type)
            PR_NOTICE("dpid:%d type:%d value:%s, time_stamp:%x", dp->dps[index].dpid, dp->dps[index].type, dp->dps[index].value.dp_str, dp->dps[index].time_stamp);
        else
            PR_NOTICE("dpid:%d type:%d value:%04x, time_stamp:%x", dp->dps[index].dpid, dp->dps[index].type, dp->dps[index].value, dp->dps[index].time_stamp);
    }
    
    op_ret = dev_report_dp_json_async(get_gw_cntl()->gw_if.id, dp->dps, dp->dps_cnt);
    if (OPRT_OK != op_ret)
    {
        PR_ERR("dev_report_dp_json_async is fail,fail_num:%d", op_ret);
        return op_ret;
    }
}

/**
 * @Function: dev_raw_dp_cb
 * @Description: raw dp info cmd callback
 * @Input: dp: raw dp info
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID dev_raw_dp_cb(IN CONST TY_RECV_RAW_DP_S *dp)
{
    PR_DEBUG("raw data dpid:%d",dp->dpid);

    PR_DEBUG("recv len:%d",dp->len);
#if 1
    INT_T i = 0;
    for(i = 0;i < dp->len;i++) {
        PR_DEBUG_RAW("%02X ",dp->data[i]);
    }
#endif
    PR_DEBUG_RAW("\n");
    PR_DEBUG("end");
    return;
}

/**
 * @Function: dev_dp_query_cb
 * @Description: dp info query callback
 * @Input: dp_qry: query info
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID dev_dp_query_cb(IN CONST TY_DP_QUERY_S *dp_query)
{
    PR_DEBUG("Recv DP Query Cmd...query cnt:%d, dpid:%d", dp_query->cnt, dp_query->dpid[0]);
    OPERATE_RET op_ret = OPRT_OK;
//      #error ""

    op_ret = tuya_hard_upload_all_data();
    if (OPRT_OK != op_ret)
    {
        PR_ERR("tuya_hard_upload_all_data fail,fail_num: %d",op_ret);
    }
    return;
}


/**
 * @Function: tuya_hard_set_led
 * @Description: 
 * @Input: 
 * @Output: 
 * @Return:  
 * @Others: 
 */
VOID tuya_hard_set_led(IN const LED_MOUDLE_S *led_param,LED_TYPE_E led_type,UINT_T led_time)
{
    if (NULL == led_param || led_param->io_config.io_level == IO_INIT_LEVEL)
    {
        PR_ERR("%s param is null",__FUNCTION__);
        return;
    }
    PR_DEBUG("led_time:%d,led_io:%d!",led_time,IO_ACTIVE_FLASH_EN(led_param->io_config.io_level));
    if (led_type == LED_FLASH)
    {
        tuya_set_led_light_type(led_param->io_handle,IO_ACTIVE_FLASH_EN(led_param->io_config.io_level),led_time,LED_TIMER_UNINIT);
        PR_DEBUG("led flash!!");
    }
    else if (led_type == LED_ON_OFF)
    {
        PR_DEBUG("led on off!");
        if(led_time == 0)
        {
            tuya_set_led_light_type(led_param->io_handle,IO_LED_TIME_ACTIVE_DIS(led_param->io_config.io_level),0,0);
            PR_DEBUG("led off!");
        }
        else
        {
            tuya_set_led_light_type(led_param->io_handle,IO_LED_TIME_ACTIVE_EN(led_param->io_config.io_level),0,0);
            PR_DEBUG("led on!");
        }
    }
    else
    {
        PR_ERR("%s led_type is error");
    }
    
    return;
}

/**
 * @Function: tuya_hard_set_led_state
 * @Description: 指示灯状态显示，主要包括产测、wifi状态、状态指示；
 * @Input: led_type:指示灯类型
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID tuya_hard_set_led_state(LED_STAT_E led_type)
{
    switch (led_type)
    {
        case PROD_TEST_FAIL:
            tuya_hard_set_led(&g_dev_info.wifi_led,LED_FLASH,100);
        break;

        case ENTER_PROD_TEST:
        case EZ_LINKING:
            tuya_hard_set_led(&g_dev_info.wifi_led,LED_FLASH,250);
        break;

        case AP_LINKING:
            tuya_hard_set_led(&g_dev_info.wifi_led,LED_FLASH,1500);
        break;

        case NORMAL_ON:
            tuya_hard_set_led(&g_dev_info.wifi_led,LED_ON_OFF,1);
        break;

        case NORMAL_OFF:
            tuya_hard_set_led(&g_dev_info.wifi_led,LED_ON_OFF,0);
        break;

        default:
        break;
    }
}

/**
 * @Function: tuya_hard_wifi_status
 * @Description: wifi状态对应指示灯显示
 * @Input: cur_stat：当前wifi状态
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID tuya_hard_wifi_status(IN CONST GW_WIFI_NW_STAT_E cur_stat)
{
    switch (cur_stat)
    {
        case STAT_LOW_POWER:
            tuya_hard_set_led_state(NORMAL_OFF);
        break;
        case STAT_UNPROVISION:
            tuya_hard_set_led_state(EZ_LINKING);
        break;
        case STAT_AP_STA_UNCFG:
            tuya_hard_set_led_state(AP_LINKING);
        break;
        case STAT_AP_STA_DISC:
        case STAT_STA_DISC:
            tuya_hard_set_led_state(NORMAL_OFF);
        break;
        case STAT_CLOUD_CONN:
        case STAT_AP_CLOUD_CONN:
            tuya_hard_set_led_state(NORMAL_ON);
        break;
        default:
        break;
    }
}

/**
 * @Function: nw_stat_cb
 * @Description: tuya-sdk network check callback
 * @Input: stat: curr network status
 * @Output: none
 * @Return: none
 * @Others: none
 */
VOID nw_stat_cb(IN CONST GW_WIFI_NW_STAT_E stat)
{
    PR_NOTICE("cur_wifi_stat:%d",stat);
    OPERATE_RET op_ret = OPRT_OK;

    tuya_hard_wifi_status(stat);

    if (STAT_CLOUD_CONN == stat || STAT_AP_CLOUD_CONN == stat)
    {
//      #error ""
//        op_ret = tuya_hard_upload_all_data();
//        if (OPRT_OK != op_ret)
//        {
//            PR_ERR("tuya_hard_upload_all_data fail,fail_num:%d",op_ret);
//        }
    }
    return;
}

/**
 * @Function: device_init
 * @Description: 设备初始化处理
 * @Input: none
 * @Output: none
 * @Return: OPRT_OK: success  Other: fail
 * @Others: none
 */
OPERATE_RET device_init(VOID)
{
    OPERATE_RET op_ret = OPRT_OK;

    tuya_hard_prod_flag(FALSE);

    TY_IOT_CBS_S wf_cbs = {
        gw_status_cb,\
        gw_ug_cb,\
        gw_reset_cb,\
        dev_obj_dp_cb,\
        dev_raw_dp_cb,\
        dev_dp_query_cb,\
        NULL
    };

    op_ret = tuya_iot_wf_soc_dev_init_param(WIFI_CONFIG_MODE_SEL, WF_START_SMART_FIRST, &wf_cbs, NULL, PRODUCT_KEY, DEV_SW_VERSION);
    if (OPRT_OK != op_ret)
    {
        PR_ERR("tuya_iot_wf_soc_dev_init fail,fail_num:%d",op_ret);
        return op_ret;
    }

#if 0
    op_ret = tuya_hard_soft_timer_init();
    if(OPRT_OK != op_ret)
    {
        PR_ERR("tuya_hard_soft_timer_init fail,fail_num:%d",op_ret);
        return op_ret;
    }
#endif

    op_ret = tuya_iot_reg_get_wf_nw_stat_cb(nw_stat_cb);
    if(OPRT_OK != op_ret)
    {
        PR_ERR("tuya_iot_reg_get_wf_nw_stat_cb fail,fail_num:%d",op_ret);
        return op_ret;
    }

    PR_NOTICE("system free heap:%d",tuya_hal_system_getheapsize());

    return OPRT_OK;
}

/**
 * @Function: key_process
 * @Description: 按键回调函数
 * @Input: port：触发引脚,type：按键触发类型,cnt:触发次数
 * @Output: none
 * @Return: none
 * @Others: 长按触发配网模式，短按控制继电器
 */
STATIC VOID key_process(TY_GPIO_PORT_E port,PUSH_KEY_TYPE_E type,INT_T cnt)
{
    PR_DEBUG("port:%d,type:%d,cnt:%d",port,type,cnt);
    OPERATE_RET op_ret = OPRT_OK;

    if(tuya_hard_judge_key_pin(port)) {
        if(LONG_KEY == type)//press long enter linking network
        {
            //手动移除设备
            tuya_iot_wf_gw_unactive();
        }
        else if(NORMAL_KEY == type)
        {
            tuya_hard_key_control();
            op_ret = tuya_hard_channel_upload(DOUBLE_TYPE);
            if (OPRT_OK != op_ret)
            {
                PR_ERR("[%s] tuya_hard_channel_upload fail,fail_num:%d",__FUNCTION__,op_ret);
            }
            tuya_hard_channel_save();
        }
        else 
        {
            PR_NOTICE("key type is no deal");
        }
    }

    return;
}
