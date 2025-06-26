#pragma once

#include "stm32f10x.h"
#include "bsp_usart.h"
#include <string.h>
#include "config.h"



typedef enum
{
		Reset                             = 0xAA,                 /*串口通信起始码，单发此指令为重置串口缓存*/
		CheckPlayState                    = 0x01,                 /*查询播报状态*/
    
    Play                            = 0x02,                 /*播放*/
    pause                           = 0x03,                 /*暂停*/
    Stop                            = 0x04,                 /*停止*/
    LastSong                        = 0x05,                 /*上一曲*/
    NextSong                        = 0x06,                 /*下一曲*/
    EndPlay                         = 0x10,                 /*播放完当前曲目后停止*/

    CheckOnelineDisksign            = 0x09,                 /*查询当前在线盘符*/
    CheckCurrentDisksign            = 0X0A,                 /*查询当前播放盘符*/
    CheckTotalTrack                 = 0x0C,                 /*查询总曲目*/
    CurrentTrack                    = 0x0D,                 /*当前曲目*/

    LastFloder                      = 0x0E,                 /*上一个文件夹目录*/
    NextFloder                      = 0x0F,                 /*下一个文件夹目录*/
    

    CheckFloderFirstTrack           = 0x11,                 /*查询文件目录首曲目*/
    CheckFloderAllTrack             = 0x12,                 /*查询文件目录总曲目*/
    
    AddVolume                       = 0x14,                 /*音量加*/
    DecVolume                       = 0x15,                 /*音量减*/
    
    EndZHPlay                       = 0X1C,                 /*结束组合播报*/
    CheckSongShortName              = 0x1E,                 /*查询歌曲短文件名*/
    EndLoop                         = 0x21,                 /*结束复读*/
    
    GetTotalSongTime                = 0x24,                 /*获取当前曲目总时间*/
    OpenPlayTime                    = 0x25,                 /*播放时间开发送*/
    ClosePlayTime                   = 0x26,                 /*关闭播放时间发送*/
} 
UartCommand;                                                //后面无附加数据的指令，
                                                            //指令发送格式为 起始码-指令类型-数据长度-校验和

typedef enum
{
    AppointTrack                    = 0x07,                 /*指定曲目播放*/
  /*在后面输入要播放的曲目编号即可播放，
   *文件编号顺序按照写入存储介质先后顺序排列，
   *例如先复制03.MP3后复制01.mp3，选择01号曲目会播放03.mp3*/
    SetCycleCount                   = 0x19,                 /*设置循环次数*/
  /*在后面输入循环次数，循环模式下循环次数为无限次*/
    SetEQ                           = 0X1A,                 /*EQ设置*/
  /*更改音频输出的均匀器效果，后面可接“EQModeSelect”其中一个指令*/
    SelectTrackNoPlay               = 0x1F,                 /*选曲不播放*/
  /*在后面输入要播放的曲目编号之后下一首播放*/
    GoToDisksign                    = 0X0B,                 /*切换播放存储介质*/
  /*请从后续“JQ8X00_Symbol”选择你需要切换的存储介质*/
    SetVolume                       = 0x13,                 /*音量设置*/
  /*可设置0~30点音量，开机状况下默认音量30*/
    SetLoopMode                     = 0x18,                 /*设置循环模式*/
  /*更改音乐播放的循环模式，后面可接“LoopModeSelect”其中一个指令*/
    SetChannel                      = 0x1D,                 /*设置通道*/
  /*0为MP3，1为AUX，2为MP3和AUX混音*/
    AppointTimeBack                 = 0x22,                 /*指定时间快退*/
    AppointTimeFast                 = 0x23,                 /*指定时间快退*/
  /*后面参数单位均为秒*/
}
UartCommandData;                                            //包含附加数据的指令，
                                                            //指令发送格式为 起始码-指令类型-数据长度-数据1-数据2-...-数据N-校验和

typedef enum {
    Normal                          = 0X00,                 /*标准    均匀器预设*/
    Pop                             = 0x01,                 /*流行音乐  均匀器预设*/
    Rock                            = 0x02,                 /*摇滚音乐  均匀器预设*/
    Jazz                            = 0X03,                 /*爵士音乐  均匀器预设*/
    Classic                         = 0X04,                 /*古典音乐  均匀器预设*/
}
EQModeSelect;                                               //均匀器预设选择   对应“SetEQ”

typedef enum {
    OverallCycle                    = 0X00,                 /*全盘循环 按顺序播放全盘曲目,播放完后循环播放*/
    SingleCycle                     = 0x01,                 /*单曲循环 一直循环播放当前曲目*/
    SingleStop                      = 0x02,                 /*单曲停止 播放完当前曲目一次停止*/
    OverallRandom                   = 0X03,                 /*全盘随机 随机播放盘符内曲目*/
    CatalogCycle                    = 0X04,                 /*目录循环 按顺序播放当前文件夹内曲目,播放完后循环播放，目录不包含子目录*/
    CatalogRandom                   = 0x05,                 /*目录随机 在当前目录内随机播放，目录不包含子目录*/
    CatalogTurnPlay                 = 0x06,                 /*目录顺序播放 按顺序播放当前文件夹内曲目,播放完后停止，目录不包含子目录*/
    OverallTurnPlay                 = 0x07,                 /*全盘顺序播放 按顺序播放全盘曲目,播放完后停止*/ 
}
LoopModeSelect;                                             //循环模式选择      对应“SetLoopMode”

typedef enum{
    JQ8X00_USB                      = 0X00,                 /*选择存储介质为 U盘*/
    JQ8X00_SD                       = 0x01,                 /*选择存储介质为 SD*/
    JQ8X00_FLASH                    = 0x02,                 /*选择存储介质为 与芯片联通的eMMC存储模块*/
}
JQ8X00_Symbol;                                              //切换播放存储介质编号  对应“GoToDisksign”



void JQ8900_Config(void);
void JQ8x00_Command(UartCommand Command);
void JQ8x00_Command_Data(UartCommandData Command, uint8_t DATA);
void JQ8x00_AppointPlay(JQ8X00_Symbol MODE, char *DATA);
void JQ8x00_ZuHeBoFang(unsigned char *DATA, unsigned char Len);
void JQ8900_SndStr(unsigned char * str,unsigned char num);
void JQ8900setup(void);


