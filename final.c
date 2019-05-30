#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <string.h>

#define MAX_BUTTON 9
#define MAX_DIGIT 4
#define LINE_BUFF 16
#define MAX_BUFF 32
#define MOTOR_ATTRIBUTE_ERROR_RANGE 4
unsigned char quit=0;

char passwd[4]={0,};
char verify[4]={0,};
char in_passwd[4]={0,};

void user_signal1(int sig)

{
        quit=1;
}

int main(void)
{
        FILE* fp;
    int push_dev;
    int dip_dev;
    int buzzer_dev;
    int fnd_dev;
    int text_lcd_dev;
    int motor_dev;
    int result=0;
    int motor_action; //1이면 on 0이면 off
    int motor_direction; //1이면 오른쪽 회전 0이면 왼쪽 회전
    int motor_speed;// 0부터 250 까지 회전속도
    static int cnt=0;
    static int cnt2=0;
    int i=0;
    int cal=0;
    int str_size2;                      //lcd
//    char* pname="passwd.txt";
    char success[30]={"success"};
    char fail[30]={"fail"};
    unsigned char push_sw_buff[MAX_BUTTON];
    unsigned char dip_sw_buff=0;
    unsigned char state=0;
    unsigned char retval;
    unsigned char retval2; //fnd
    unsigned char data;
    unsigned char string[32];
    unsigned char motor_state[3];
    memset(motor_state,0,sizeof(motor_state));
    int buff_size=sizeof(push_sw_buff);

    push_dev=open("/dev/fpga_push_switch",O_RDWR);
    dip_dev=open("/dev/fpga_dip_switch",O_RDWR);
    buzzer_dev=open("/dev/fpga_buzzer",O_RDWR);
    fnd_dev=open("/dev/fpga_fnd",O_RDWR);
    text_lcd_dev=open("/dev/fpga_text_lcd",O_RDWR);
    motor_dev=open("/dev/fpga_step_motor",O_RDWR);

    if(push_dev<0){
        printf("Push_Switch Device Open Error\n");
        close(push_dev);
        return -1;
    }
    if(dip_dev<0){
        printf("Dip_Switch Device Open Error\n");
        close(dip_dev);
        return -1;
    }
    if(buzzer_dev<0){
        printf("Buzzer Device Open Error\n");
        close(buzzer_dev);
        return -1;
    }
    if(fnd_dev<0){
        printf("Fnd Device Open Error\n");
        close(fnd_dev);
        return -1;
    }
    if(text_lcd_dev<0){
        printf("Text_lcd Device Open Error\n");
        close(text_lcd_dev);
        return -1;
    }
    if(motor_dev<0){
        printf("Motor Device Open Error\n");
        close(motor_dev);
        return -1;
    }
    (void)signal(SIGINT, user_signal1);

    read(dip_dev,&dip_sw_buff,1);
    printf("====Door Lock System====\n");
    printf("Set Dip_Switch : 0 to set Password\n");
    printf("Set Dip_Switch : 1 to verify Password\n");
    printf("Press <Ctrl+c> to terminate\n");
    printf("\n\n\n");

    sleep(2);
    switch(dip_sw_buff){
                case 0:
            printf("Selected setting Password\n");
            printf("Input Password what you want\n");
            while(!quit){
                usleep(400000);
                    read(push_dev,&push_sw_buff, buff_size);
                    for(i=0;i<MAX_BUTTON;i++){
                        printf("[%d]", push_sw_buff[i]);
                    }
                if(cnt==4) quit=1;
                printf("\n");
                if(push_sw_buff[0] ==1){
                        passwd[cnt++]='1';
                    if(cnt==4) break;
                }
                if(push_sw_buff[1]==1){
                        passwd[cnt++]='2';
                        if(cnt==4) break;
                }
                if(push_sw_buff[2]==1){
                    passwd[cnt++]='3';
                    if(cnt==4) break;
                }
                if(push_sw_buff[3]==1){
                        passwd[cnt++]='4';
                        if(cnt==4) break;
                    }
                if(push_sw_buff[4]==1){
                        passwd[cnt++]='5';
                        if(cnt==4) break;
                    }
                if(push_sw_buff[5]==1){
                        passwd[cnt++]='6';
                        if(cnt==4) break;
                    }
                 if(push_sw_buff[6]==1){
                        passwd[cnt++]='7';
                        if(cnt==4) break;
                }
                if(push_sw_buff[7]==1){
                        passwd[cnt++]='8';
                        if(cnt==4) break;
                }
                if(push_sw_buff[8]==1){
                        passwd[cnt++]='9';
                        if(cnt==4) break;
                }
                    if(cnt==4) quit=1;
                }
                printf("Setting Password : %c%c%c%c\n", passwd[0],passwd[1],passwd[2],passwd[3]);
            if((fp=fopen("passwd.txt","wt"))==NULL)
            {
                printf("File open error!.");
                exit(1);
            }
            fprintf(fp,"%s",passwd);
            printf("Input Password : %s\n", passwd);
                break;
                case 1:
 	if((fp=fopen("passwd.txt","r"))==NULL)
           {
              fprintf(stdout, "File open error!");
                exit(1);
            }
            fread(in_passwd,4,1,fp);
            if(!strncmp(in_passwd,"",1)){
                       printf("Please set your password first.\n");
                       break;
             }
            printf("Selected verifying Password\n");
            printf("Start Verifying : Input your Password\n");
            while(!quit){
                usleep(400000);
                    read(push_dev,&push_sw_buff, buff_size);
                    for(i=0;i<MAX_BUTTON;i++){
                        printf("[%d]", push_sw_buff[i]);
                    }
                if(cnt==4) quit=1;
                printf("\n");
                if(push_sw_buff[0] ==1){
                        verify[cnt2++]='1';
                    if(cnt2==4) break;
                }
                if(push_sw_buff[1]==1){
                        verify[cnt2++]='2';
                        if(cnt2==4) break;
                }
                if(push_sw_buff[2]==1){
                    verify[cnt2++]='3';
                    if(cnt2==4) break;
                }
                if(push_sw_buff[3]==1){
                        verify[cnt2++]='4';
                        if(cnt2==4) break;
                    }
                    if(push_sw_buff[4]==1){
                        verify[cnt2++]='5';
                        if(cnt2==4) break;
                    }
                    if(push_sw_buff[5]==1){
                        verify[cnt2++]='6';
                        if(cnt2==4) break;
                    }
                    if(push_sw_buff[6]==1){
                        verify[cnt2++]='7';
                        if(cnt2==4) break;
                    }
                    if(push_sw_buff[7]==1){
                        verify[cnt2++]='8';
                        if(cnt2==4) break;
                    }
                    if(push_sw_buff[8]==1){
                        verify[cnt2++]='9';
                        if(cnt2==4) break;
                    }
                    if(cnt2==4) quit=1;
                }
        printf("%s\n",verify);
            retval2=write(fnd_dev,verify,4);
            if(retval2<0){
                printf("WriteError!\n");
                return -1;
            }
        sleep(1);

            if(!strncmp(in_passwd,verify,4))
            {
                printf("!!!! Congratulation !!!!\n");
                result=1;
                str_size2=strlen(success);
                strncat(string,success,str_size2);
                memset(string+str_size2,' ',LINE_BUFF-str_size2);
                str_size2=strlen(success);
                strncat(string,success,str_size2);
                memset(string+LINE_BUFF+str_size2,' ', LINE_BUFF-str_size2);
                write(text_lcd_dev,string,MAX_BUFF);
                motor_action = atoi("1");
                motor_direction = atoi("1");
                motor_speed = atoi("15");
                motor_state[0]=(unsigned char)motor_action;
                motor_state[1]=(unsigned char)motor_direction;
                motor_state[2]=(unsigned char)motor_speed;
                write(motor_dev,motor_state,3);
                sleep(1);
                motor_action = atoi("0");
                motor_state[0] =(unsigned char)motor_action;
                write(motor_dev,motor_state,3);
            }
            else{
                printf("Warning!!! Incorrect Password!!!\n");
                result=0;
                str_size2=strlen(fail);
                strncat(string,fail,str_size2);
                memset(string+str_size2,' ', LINE_BUFF-str_size2);
                str_size2=strlen(fail);
                strncat(string,fail,str_size2);
                memset(string+LINE_BUFF+str_size2,' ', LINE_BUFF-str_size2);

                write(text_lcd_dev,string,MAX_BUFF);

                if(result==0){
                        for(i=0;i<7;i++){
                        if(state!=0){
                                state=0;
                                data=1;
                                retval=write(buzzer_dev,&data,1);
                                if(retval<0){
                                        printf("Write Error!\n");
                                        return -1;
                                    }
                            }
                                                else{
                                state=1;
                                data=0;
                                retval=write(buzzer_dev,&data,1);
                                if(retval<0){
                                    printf("Write Error!\n");
                                    return -1;
                                }
                        }sleep(1);
                        }
                }
            }
        break;
        default:
                printf("default\n");
                break;
    }
    close(push_dev);
    close(dip_dev);
    close(buzzer_dev);
    close(fnd_dev);
    close(text_lcd_dev);
    close(motor_dev);

        return 0;

}




