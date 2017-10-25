/*************************************************
  Copyright (C), 1988-1999,  Tech. Co., Ltd.
  File name:      
  Author:       Version:        Date:
  Description:   
  Others:         
  Function List:  
    1. ....
  History:         
                   
    1. Date:
       Author:
       Modification:
    2. ...
*************************************************/
#ifndef _FTP_H
#define _FTP_H
#include <stm32f10x.h>





int ftp_size(uint8_t *cmd, uint8_t *file);
bool ftp_download_app(uint8_t *file_name, uint8_t *check_sum);
bool ftp_download_ini(uint8_t *file_name);

#endif
